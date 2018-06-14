#pragma once
#include <atlbase.h>
#include <Xaudio2fx.h>
#include <XAudio2.h>
#include <XAPOFX.h>
#include "wav.h"

#define _USE_VOICECALLBACK_
#pragma comment(lib,"XAudio2.lib")

namespace EffectParameters
{
	struct Equalizer_DESC
	{
		float Bandwidth0;				//0.1f~2.0f
		float Bandwidth1;				//
		float Bandwidth2;				//
		float Bandwidth3;				//
		float FrequencyCenter0;		//20.0f~20000.0f
		float FrequencyCenter1;		//
		float FrequencyCenter2;		//
		float FrequencyCenter3;		//
		float Gain0;						//0.126f(-18dB) ~  7.94f(+18dB)
		float Gain1;						//
		float Gain2;						//
		float Gain3;						//

		Equalizer_DESC() :
			Bandwidth0(FXEQ_DEFAULT_BANDWIDTH),
			Bandwidth1(FXEQ_DEFAULT_BANDWIDTH),
			Bandwidth2(FXEQ_DEFAULT_BANDWIDTH),
			Bandwidth3(FXEQ_DEFAULT_BANDWIDTH),
			Gain0(FXEQ_DEFAULT_GAIN),
			Gain1(FXEQ_DEFAULT_GAIN),
			Gain2(FXEQ_DEFAULT_GAIN),
			Gain3(FXEQ_DEFAULT_GAIN),
			FrequencyCenter0(FXEQ_DEFAULT_FREQUENCY_CENTER_0),
			FrequencyCenter1(FXEQ_DEFAULT_FREQUENCY_CENTER_1),
			FrequencyCenter2(FXEQ_DEFAULT_FREQUENCY_CENTER_2),
			FrequencyCenter3(FXEQ_DEFAULT_FREQUENCY_CENTER_3)
		{}
	};

	struct SimpleReverb_DESC
	{
		float Diffusion;		//音の広がり(拡散量） 0.0f~1.0f 
		float	RoomSize;		//音が鳴っている施設の大きさ 0.0001f~1.0f

		SimpleReverb_DESC() :
			Diffusion(FXREVERB_DEFAULT_DIFFUSION),
			RoomSize(FXREVERB_DEFAULT_ROOMSIZE)
		{}
	};	

	struct Delay_DESC
	{
		float WetDryMix;		//原音にどれくらいエフェクトの音を混ぜるか 0.0f~1.0f
		float Feedback;			//跳ね返りの強さ 0.0f~1.0f
		float DelayTime;			//ディレイタイム(ミリ秒)1.0f~2000.0f

		Delay_DESC():
			WetDryMix(FXECHO_DEFAULT_WETDRYMIX),
			Feedback(FXECHO_DEFAULT_FEEDBACK),
			DelayTime(FXECHO_DEFAULT_DELAY)
		{}
	};

	struct Limiter_DESC
	{
		UINT32 Release;		//1~20
		UINT32 Loudness;		//1~1800

		Limiter_DESC():
			Release(FXMASTERINGLIMITER_DEFAULT_RELEASE),
			Loudness(FXMASTERINGLIMITER_DEFAULT_LOUDNESS)
		{}
	};
};

class XAudio2Callback : public IXAudio2VoiceCallback {
private:
	HANDLE handle;
public:
	XAudio2Callback() { }
	~XAudio2Callback() { }
	void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired) { }
	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd(void) { }
	void STDMETHODCALLTYPE OnStreamEnd(void) { SetEvent(handle); }
	void STDMETHODCALLTYPE OnBufferStart(void *pBufferContext) { SetEvent(handle); }
	void STDMETHODCALLTYPE OnBufferEnd(void *pBufferContext) { }
	void STDMETHODCALLTYPE OnLoopEnd(void *pBufferContext) { }
	void STDMETHODCALLTYPE OnVoiceError(void *pBufferContext, HRESULT Error) { }
};

class SoundSource
{
private:

	//音源格納用のバッファー
	XAUDIO2_BUFFER buf;
	//ソースヴォイス(ここに音源が格納される
	IXAudio2SourceVoice* pSource;
	//wavデータ格納用
	WAV					  wav;

	XAUDIO2_VOICE_STATE xstate;
	void GetState();
public:
	SoundSource();
	SoundSource(SoundSource& sound);
	~SoundSource();
	bool Load(const char* path);
	void PlayBGM(int loopNum = XAUDIO2_LOOP_INFINITE, float gain = 1.0f, float pitch = 1.0f);
	void PlaySE(float gain = 1.0f, float pitch = 1.0f);
	//一時停止
	void Pause();
	//停止
	void Stop();
	//ソース破棄
	void Destroy();

	/*ループ中にセットするとノイズが乗るので注意*/
	void SetEQ(EffectParameters::Equalizer_DESC& eq_desc);
	void SetSimpleReverb(EffectParameters::SimpleReverb_DESC& reverb_desc);
	void SetReverb();
	void SetDelay(EffectParameters::Delay_DESC& delay_desc);
	void SetLimiter(EffectParameters::Limiter_DESC& limiter_desc);
	//サンプル数で再生時間を返す
	int GetCurrentSampleTime();
	IXAudio2SourceVoice** GetSource();
	WAV GetWav();

};

//サウンド管理部
class SoundSystem 
{
private:
	XAudio2Callback voiceCallback;
	//インターフェース
	ATL::CComPtr<IXAudio2> pXAudio2;
	//マスターヴォイス
	IXAudio2MasteringVoice* pMaster;
	//サウンドシステムの初期化、一度だけ作ればよい
	bool Create();
	
public:
	SoundSystem();
	~SoundSystem();

	//ソースボイスの登録
	bool AddSource(SoundSource& source);
	
	bool AddSourceUseCallBack(SoundSource& source);

	//登録したサウンド全体の音量調整
	void SetMasterGain(float gain);
	void DestroySystem(SoundSource& source);
};

