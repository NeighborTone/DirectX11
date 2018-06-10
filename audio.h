#pragma once
#include <atlbase.h>
#include <XAudio2.h>
#include "wav.h"


#pragma comment(lib,"XAudio2.lib")

class SoundSource
{
private:
	//�����i�[�p�̃o�b�t�@�[
	XAUDIO2_BUFFER buf;

public:
	//�\�[�X���H�C�X(�����ɉ������i�[�����
	IXAudio2SourceVoice * pSource;
	//wav�f�[�^�i�[�p
	WAV					  wav;

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
	
};

//�T�E���h�Ǘ���
class SoundSystem 
{
private:
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
	//�\�[�X�{�C�X�͖����I�ɊJ�����Ȃ�����A�v���P�[�V�����I�����܂Ŏc�葱����
	bool AddSource(SoundSource& source);
	//�o�^�����T�E���h�S�̂̉��ʒ���
	void SetMasterGain(float gain);
	void DestroySystem(SoundSource& source);
};

