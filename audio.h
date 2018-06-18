#pragma once
#pragma warning (disable : 4100)	//�R�[���o�b�N�֐�
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
//�T�E���h�Ǘ���
class SoundSystem
{
private:
	XAUDIO2_VOICE_DETAILS voiceDetails;
	//�C���^�[�t�F�[�X
	ATL::CComPtr<IXAudio2> pXAudio2;
	//3D�C���^�[�t�F�[�X
	X3DAUDIO_HANDLE x3DInstance;
	//�}�X�^�[���H�C�X
	IXAudio2MasteringVoice* pMaster;
	//�T�E���h�V�X�e���̏������A��x�������΂悢
	bool Create();

public:
	SoundSystem();
	~SoundSystem();

	IXAudio2* GetXAudio2() const;
	IXAudio2MasteringVoice* GetMaster() const;
	X3DAUDIO_HANDLE& Get3DInstance();
	XAUDIO2_VOICE_DETAILS& GetVoiceDetails();
	//�o�^�����T�E���h�S�̂̉��ʒ���
	void SetMasterGain(float gain);
	void DestroySystem(SoundSource& source);
};

