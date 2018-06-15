#pragma once
#include <atlbase.h>
#include <XAudio2.h>
#include "EffectParameter.hpp"
#include "wav.h"

#define _USE_VOICECALLBACK_
#pragma comment(lib,"XAudio2.lib")


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
	//ループの終端に来たらループを停止する
	void ExitLoop();
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

