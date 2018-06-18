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

class SoundSource;

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
//サウンド管理部
class SoundSystem
{
private:
	XAUDIO2_VOICE_DETAILS voiceDetails;
	//インターフェース
	ATL::CComPtr<IXAudio2> pXAudio2;
	//3Dインターフェース
	X3DAUDIO_HANDLE x3DInstance;
	//マスターヴォイス
	IXAudio2MasteringVoice* pMaster;
	//サウンドシステムの初期化、一度だけ作ればよい
	bool Create();

public:
	SoundSystem();
	~SoundSystem();

	IXAudio2* GetXAudio2() const;
	IXAudio2MasteringVoice* GetMaster() const;
	X3DAUDIO_HANDLE& Get3DInstance();
	XAUDIO2_VOICE_DETAILS& GetVoiceDetails();
	//登録したサウンド全体の音量調整
	void SetMasterGain(float gain);
	void DestroySystem(SoundSource& source);
};

