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
//MEMO:�t���̃T���v���ɏ����Ă������̂����p
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
	//X3DAudioCalculate�̌Ăяo�����ʂ��󂯎��B
	X3DAUDIO_DSP_SETTINGS dsp;
	//�����i�[�p�̃o�b�t�@�[
	XAUDIO2_BUFFER buf;
	//�\�[�X���H�C�X(�����ɉ������i�[�����
	IXAudio2SourceVoice* pSource;
	//wav�f�[�^�i�[�p
	Wav wav;
	Ogg ogg;
	//�\�[�X�̏��i�[�p
	XAUDIO2_VOICE_STATE xstate;
	//3D�I�[�f�B�I�\�[�X�̒�`
	X3DAUDIO_EMITTER emitter;
	//�t�@�C�����
	FileType fileType;
	void GetState();
	void Init();
	//3D�ʒu�Ȃǂ̍X�V
	void UpData();

public:
	SoundSource();
	SoundSource(const std::string path);
	SoundSource(SoundSource& sound);
	~SoundSource();
	bool Load(const std::string path);
	void PlayBGM(int loopNum = XAUDIO2_LOOP_INFINITE, float gain = 1.0f, float pitch = 1.0f);
	void PlaySE(float gain = 1.0f, float pitch = 1.0f);
	//�ꎞ��~
	void Pause();
	//��~
	void Stop() const;
	
	void UpDataPosition(Vec3&& pos);
	//���[�v�̏I�[�ɗ����烋�[�v���~����
	void ExitLoop() const;
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
	//3D�C���^�[�t�F�[�X
	X3DAUDIO_HANDLE x3DInstance;
	//3D�I�[�f�B�I�̎�M�ʒu
	X3DAUDIO_LISTENER listener;
	//3D�I�[�f�B�I�\�[�X�̌���
	//X3DAUDIO_CONE emitterCone;
	//�}�X�^�[���H�C�X
	IXAudio2MasteringVoice* pMaster;
	//�T�E���h�V�X�e���̏������A��x�������΂悢
	bool Create();

public:
	SoundSystem();
	~SoundSystem();
	IXAudio2MasteringVoice* GetMaster() const;
	X3DAUDIO_HANDLE& Get3DInstance();
	X3DAUDIO_LISTENER GetListener() const;
	//�\�[�X�{�C�X�̓o�^
	bool AddSource(SoundSource& source);
	//�`�����l�����𓾂�
	UINT GetNumChannels();
	UINT GetChannelMask() const;
	//�R�[���o�b�N�֐����g��
	bool AddSourceUseCallBack(SoundSource& source);
	//�o�^�����T�E���h�S�̂̉��ʒ���
	void SetMasterGain(float gain);
	void SetListenerPosition(float x, float y, float z);
	void DestroySystem(SoundSource& source);
};

