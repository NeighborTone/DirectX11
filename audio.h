#pragma once
#pragma warning (disable : 4100)	//コールバック関数
#include <atlbase.h>
#include <XAudio2.h>
#include <x3daudio.h>
#include "EffectParameter.hpp"
#include "Utility.hpp"
#include "wav.h"
#include "ogg.h"

#define _USE_VOICECALLBACK_
#pragma comment(lib,"XAudio2.lib")
//MEMO:付属のサンプルに書いてあったのを引用
static constexpr X3DAUDIO_DISTANCE_CURVE_POINT Emitter_LFE_CurvePoints[3] = { 0.0f, 1.0f, 0.25f, 0.0f, 1.0f, 0.0f };
static constexpr X3DAUDIO_DISTANCE_CURVE Emitter_LFE_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_LFE_CurvePoints[0], 3 };
static constexpr X3DAUDIO_DISTANCE_CURVE_POINT Emitter_Reverb_CurvePoints[3] = { 0.0f, 0.5f, 0.75f, 1.0f, 1.0f, 0.0f };
static constexpr X3DAUDIO_DISTANCE_CURVE Emitter_Reverb_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_Reverb_CurvePoints[0], 3 };
static constexpr int INPUTCHANNELS = 1;

enum FileType
{
	WAVE,
	OGG
};
class SoundSource
{
private:
	//X3DAudioCalculateの呼び出し結果を受け取る。
	X3DAUDIO_DSP_SETTINGS dsp;
	//音源格納用のバッファー
	XAUDIO2_BUFFER buf;
	//ソースヴォイス(ここに音源が格納される
	IXAudio2SourceVoice* pSource;
	//wavデータ格納用
	Wav wav;
	Ogg ogg;
	//ソースの情報格納用
	XAUDIO2_VOICE_STATE xstate;
	//3Dオーディオソースの定義
	X3DAUDIO_EMITTER emitter;
	//ファイル情報
	FileType fileType;
	void GetState();
	void Init();
	//3D位置などの更新
	void UpData();

public:
	SoundSource();
	SoundSource(const std::string path);
	SoundSource(SoundSource& sound);
	~SoundSource();
	bool Load(const std::string path);
	void PlayBGM(int loopNum = XAUDIO2_LOOP_INFINITE, float gain = 1.0f, float pitch = 1.0f);
	void PlaySE(float gain = 1.0f, float pitch = 1.0f);
	//一時停止
	void Pause();
	//停止
	void Stop() const;
	
	void UpDataPosition(Vec3&& pos);
	//ループの終端に来たらループを停止する
	void ExitLoop() const;
	//ソース破棄
	void Destroy();

	/*初期化時にセットすることを推奨*/
	void SetEQ(EffectParameters::Equalizer_DESC& eq_desc);
	void SetSimpleReverb(EffectParameters::SimpleReverb_DESC& reverb_desc);
	void SetReverb(EffectParameters::Reverb_DESC& reverb_desc);
	void SetDelay(EffectParameters::Delay_DESC& delay_desc);
	void SetLimiter(EffectParameters::Limiter_DESC& limiter_desc);
	void SetMultiEffecter(
		EffectParameters::Equalizer_DESC& eq_desc,
		EffectParameters::Reverb_DESC& reverb_desc,
		EffectParameters::Delay_DESC& delay_desc,
		EffectParameters::Limiter_DESC& limiter_desc
	);

	//サンプル数で再生時間を返す
	int GetCurrentSampleTime();
	IXAudio2SourceVoice** GetSource();
	Wav GetWav();
	Ogg GetOgg();
	FileType GetFileType();
};

//サウンド管理部
class SoundSystem
{
private:
	
	class XAudio2Callback : public IXAudio2VoiceCallback
	{
		
	private:
		HANDLE handle;
	public:
		XAudio2Callback() { }
		~XAudio2Callback() { }
		void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired) override
		{ }
		void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() override
		{ }
		void STDMETHODCALLTYPE OnStreamEnd() override
		{
			SetEvent(handle);
		}
		void STDMETHODCALLTYPE OnBufferStart(void* pBufferContext) override
		{
			SetEvent(handle);
		}
		void STDMETHODCALLTYPE OnBufferEnd(void* pBufferContext) override
		{ }
		void STDMETHODCALLTYPE OnLoopEnd(void* pBufferContext) override
		{ }
		void STDMETHODCALLTYPE OnVoiceError(void* pBufferContext, HRESULT Error) override
		{ }
	};

	XAudio2Callback voiceCallback;
	//インターフェース
	ATL::CComPtr<IXAudio2> pXAudio2;
	//3Dインターフェース
	X3DAUDIO_HANDLE x3DInstance;
	//3Dオーディオの受信位置
	X3DAUDIO_LISTENER listener;
	//3Dオーディオソースの向き
	//X3DAUDIO_CONE emitterCone;
	//マスターヴォイス
	IXAudio2MasteringVoice* pMaster;
	//サウンドシステムの初期化、一度だけ作ればよい
	bool Create();

public:
	SoundSystem();
	~SoundSystem();
	IXAudio2MasteringVoice* GetMaster() const;
	X3DAUDIO_HANDLE& Get3DInstance();
	X3DAUDIO_LISTENER GetListener() const;
	//ソースボイスの登録
	bool AddSource(SoundSource& source);
	//チャンネル数を得る
	UINT GetNumChannels();
	UINT GetChannelMask() const;
	//コールバック関数を使う
	bool AddSourceUseCallBack(SoundSource& source);
	//登録したサウンド全体の音量調整
	void SetMasterGain(float gain);
	void SetListenerPosition(float x, float y, float z);
	void DestroySystem(SoundSource& source);
};

