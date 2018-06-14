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
	struct EQ_DESC
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

		EQ_DESC() :
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

	struct REVERB_DESC
	{
		float Diffusion;		// ���̍L����(�g�U�ʁj 0.0f~1.0f 
		float	RoomSize;		// �������Ă���{�݂̑傫�������� 0.0001f~1.0f

		REVERB_DESC() :
			Diffusion(FXREVERB_DEFAULT_DIFFUSION),
			RoomSize(FXREVERB_DEFAULT_ROOMSIZE)
		{};
	};	

	struct DELAY_DESC
	{
		float WetDryMix;		//�����ɂǂꂭ�炢�����邩
		float Feedback;			//���˕Ԃ�̋���
		float Delay;				//�f�B���C�^�C��(�~���b)

		DELAY_DESC():
			WetDryMix(FXECHO_DEFAULT_WETDRYMIX),
			Feedback(FXECHO_DEFAULT_FEEDBACK),
			Delay(FXECHO_DEFAULT_DELAY)
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

	//�����i�[�p�̃o�b�t�@�[
	XAUDIO2_BUFFER buf;
	//�\�[�X���H�C�X(�����ɉ������i�[�����
	IXAudio2SourceVoice* pSource;
	//wav�f�[�^�i�[�p
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
	//�ꎞ��~
	void Pause();
	//��~
	void Stop();
	//�\�[�X�j��
	void Destroy();

	void SetEQ(EffectParameters::EQ_DESC& eq_desc);
	void SetReverb(EffectParameters::REVERB_DESC& reverb_desc);
	void SetDelay(EffectParameters::DELAY_DESC& delay_desc);
	//�T���v�����ōĐ����Ԃ�Ԃ�
	//������
	unsigned __int64 GetCurrentSampleTime();
	IXAudio2SourceVoice** GetSource();
	WAV GetWav();

};

//�T�E���h�Ǘ���
class SoundSystem 
{
private:
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

