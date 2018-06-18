#pragma once
#include "audio.h"
enum FileType
{
	WAVE,
	OGG
};
class SoundSource
{
private:
	IXAudio2SubmixVoice* subMixVoice;
	XAudio2Callback voiceCallBack;
	//X3DAudioCalculate�̌Ăяo�����ʂ��󂯎��B
	X3DAUDIO_DSP_SETTINGS dsp;
	//�����i�[�p�̃o�b�t�@�[
	XAUDIO2_BUFFER buf;
	//�\�[�X���H�C�X(�����ɉ������i�[�����
	IXAudio2SourceVoice* pSource;
	//wav�f�[�^�i�[�p
	Wav wav;
	//Ogg ogg;
	//�\�[�X�̏��i�[�p
	XAUDIO2_VOICE_STATE xstate;
	//3D�I�[�f�B�I�\�[�X�̒�`
	X3DAUDIO_EMITTER emitter;
	//���X�i�[�ʒu
	X3DAUDIO_LISTENER listener;


	//�t�@�C�����
	FileType fileType;

	void GetState();

	bool is3D;

public:
	SoundSource();
	SoundSource(const std::string path, bool is3DSound);
	SoundSource(SoundSource& sound);
	~SoundSource();
	bool Load(const std::string path, bool is3DSound);
	//�����̈ʒu�Ȃǂ̍X�V
	void UpData3DSound(Vec3&& pos, Vec3&& listenerPos);
	//BGM�Đ��ɓK�����Đ����s���܂�
	void PlayBGM(int loopNum = XAUDIO2_LOOP_INFINITE, float gain = 1.0f, float pitch = 1.0f);
	//���ʉ��Đ��ɓK�����Đ����s���܂�
	void PlaySE(float gain = 1.0f, float pitch = 1.0f);
	//�ꎞ��~
	void Pause();
	//��~
	void Stop() const;

	//���[�v�̏I�[�ɗ����烋�[�v���~����
	void ExitLoop() const;
	//�\�[�X�j��
	void Destroy();

	/*���������ɃZ�b�g���邱�Ƃ𐄏�(���t���[���ĂԂƃm�C�Y����邽��)*/
	//MEMO : �N���X���Č�
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
	//Ogg GetOgg();
	FileType GetFileType();
};
