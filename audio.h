#pragma once
#pragma warning (disable : 4100)	//�R�[���o�b�N�֐�
#include <atlbase.h>
#include <XAudio2.h>
#include "EffectParameter.hpp"
#include "wav.h"
#include "ogg.h"

#define _USE_VOICECALLBACK_
#pragma comment(lib,"XAudio2.lib")

enum FileType
{
	WAVE,
	OGG
};
class SoundSource
{
private:
	//�����i�[�p�̃o�b�t�@�[
	XAUDIO2_BUFFER buf;
	//�\�[�X���H�C�X(�����ɉ������i�[�����
	IXAudio2SourceVoice* pSource;
	//wav�f�[�^�i�[�p
	Wav wav;
	Ogg ogg;
	XAUDIO2_VOICE_STATE xstate;
	void GetState();
	
	FileType fileType;
public:
	SoundSource();
	SoundSource(SoundSource& sound);
	~SoundSource();
	bool Load(const std::string path);
	void PlayBGM(int loopNum = XAUDIO2_LOOP_INFINITE, float gain = 1.0f, float pitch = 1.0f);
	void PlaySE(float gain = 1.0f, float pitch = 1.0f);
	//�ꎞ��~
	void Pause();
	//��~
	void Stop();
	//���[�v�̏I�[�ɗ����烋�[�v���~����
	void ExitLoop();
	//�\�[�X�j��
	void Destroy();

	/*���������ɃZ�b�g���邱�Ƃ𐄏�*/
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

	//�T���v�����ōĐ����Ԃ�Ԃ�
	int GetCurrentSampleTime();
	IXAudio2SourceVoice** GetSource();
	Wav GetWav();
	Ogg GetOgg();
	FileType GetFileType();
};

//�T�E���h�Ǘ���
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
	//�C���^�[�t�F�[�X
	ATL::CComPtr<IXAudio2> pXAudio2;
	//�}�X�^�[���H�C�X
	IXAudio2MasteringVoice* pMaster;
	//�T�E���h�V�X�e���̏������A��x�������΂悢
	bool Create();
	
public:
	SoundSystem();
	~SoundSystem();

	//�\�[�X�{�C�X�̓o�^
	bool AddSource(SoundSource& source);
	
	bool AddSourceUseCallBack(SoundSource& source);

	//�o�^�����T�E���h�S�̂̉��ʒ���
	void SetMasterGain(float gain);
	void DestroySystem(SoundSource& source);
};

