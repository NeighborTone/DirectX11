#pragma once
#include <string>
#include "Audio.h"
#include "PCM.h"
#include "Utility.hpp"
namespace SoundEngine
{
	class SoundSource
	{
	private:
		struct SourceData
		{
			IXAudio2SubmixVoice* subMixVoice;
			//�R�[���o�b�N
			XAudio2Callback voiceCallBack;
			//X3DAudioCalculate�̌Ăяo�����ʂ��󂯎��B
			X3DAUDIO_DSP_SETTINGS dsp;
			//�����i�[�p�̃o�b�t�@�[
			XAUDIO2_BUFFER buf;
			//�\�[�X���H�C�X(�����ɉ������i�[�����
			IXAudio2SourceVoice* pSource;
			//�\�[�X�̏��i�[�p
			XAUDIO2_VOICE_STATE xstate;
			//3D�I�[�f�B�I�\�[�X�̒�`
			X3DAUDIO_EMITTER emitter;
			//���X�i�[�ʒu
			X3DAUDIO_LISTENER listener;
			//3D�����t���O
			bool is3D;
		}*data;
		PCM* pcm;
		void GetState();

	public:
		SoundSource();
		~SoundSource();
		void Load(const std::string path, bool is3DSound);
		void PlayBGM(int loopNum = XAUDIO2_LOOP_INFINITE, float gain = 1.0f, float pitch = 1.0f);	//�X�g���[�~���O�ɂ���\��
		void PlaySE(int loopNum = 0, float gain = 1.0f, float pitch = 1.0f);	//�S���ǂݍ���
		void Stop() const;
		void Pause();
		void UpDate3DSound(Vec3&& pos, Vec3&& listenerPos);
		void ExitLoop() const;
		void Destroy();
		long long GetCurrentSampleTime();
		IXAudio2SourceVoice* GetSource();
		WORD GetChannels();
	};

}
