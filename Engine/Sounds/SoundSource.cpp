#include <iostream>
#include "SoundSource.h"
namespace SoundEngine 
{
	SoundSource::SoundSource()
	{
		data = new SourceData();
	}

	SoundSource::~SoundSource()
	{
		Destroy();
	}

	void SoundSource::Load(const std::string path, bool is3DSound)
	{
		std::string str_ogg = ".ogg";
		std::string str_wav = ".wav";

		try {
			if (std::equal(path.begin() + path.find("."), path.end(), str_ogg.begin()))
			{
				pcm = new Ogg(path);
			}
			else if (std::equal(path.begin() + path.find("."), path.end(), str_wav.begin()))
			{
				pcm = new Wav(path);
			}

			HRESULT hr = 0;
			XAUDIO2_SEND_DESCRIPTOR sendDescription = {0};
			XAUDIO2_VOICE_SENDS sendList = {0};

			data->is3D = is3DSound;

			hr = Audio::GetInstance()->GetXAudio2()->CreateSubmixVoice(
				&data->subMixVoice,
				pcm->GetWaveFmtEx().nChannels,
				pcm->GetWaveFmtEx().nSamplesPerSec,
				XAUDIO2_VOICE_USEFILTER);
			if (FAILED(hr))
			{
				throw "SubMixVoice Create failed";
			}

			sendDescription.Flags = 0;
			sendDescription.pOutputVoice = data->subMixVoice;

			sendList.pSends = &sendDescription;
			sendList.SendCount = 1;

			//�\�[�X�{�C�X�쐬
			hr = Audio::GetInstance()->GetXAudio2()->CreateSourceVoice(
				&data->pSource,
				&pcm->GetWaveFmtEx(),
				0,
				XAUDIO2_DEFAULT_FREQ_RATIO,
				&data->voiceCallBack,
				&sendList,
				0);
			if (FAILED(hr))
			{
				throw "SourceVoice Create failed";
			}
		}
		catch (const char* e)
		{
			std::cerr << e << std::endl;
		}

	}

	void SoundSource::PlayBGM(int loopNum, float gain, float pitch)
	{
		HRESULT hr = 0;
		data->buf = { 0 };
		try {
			data->buf.AudioBytes = (UINT32)pcm->GetWaveByteSize();
			data->buf.pAudioData = pcm->GetWaveData();
			data->buf.pContext = this;
			data->buf.Flags = XAUDIO2_END_OF_STREAM;	//���̃o�b�t�@�̌�Ƀf�[�^���Ȃ����Ƃ��\�[�X�{�C�X�ɓ`����
			data->buf.LoopCount = loopNum;	//���[�v�񐔂��w��B�f�t�H���g�Ŗ������[�v�ɂ��Ă���
			data->buf.LoopBegin = 0;
			data->pSource->SetFrequencyRatio(pitch);	//�s�b�`
			data->pSource->SetVolume(gain);				//�Q�C��
			hr = data->pSource->SubmitSourceBuffer(&data->buf, nullptr);	//Source�ɉ����̏��𑗂�
			if (FAILED(hr))
			{
				throw "SourceVoice Played failed";
			}
		}
		catch (const char* e)
		{
			std::cerr << e << std::endl;
		}
		if (data->pSource)
		{
			data->pSource->Start();
		}

		while(data->xstate.BuffersQueued > 0)
		{
			//�C�x���g���g���K�[�����̂�҂�
			WaitForSingleObject(data->voiceCallBack.handle, INFINITE);
		}

	}

	void SoundSource::PlaySE(int loopNum, float gain, float pitch)
	{
		HRESULT hr = 0;
		data->buf = { 0 };
		try {
			data->buf.AudioBytes = (UINT)pcm->GetWaveByteSize();
			data->buf.pAudioData = pcm->GetWaveData();
			data->buf.pContext = this;
			data->buf.Flags = XAUDIO2_END_OF_STREAM;
			data->buf.LoopCount = 0;	//���[�v�񐔂��w��B
			data->buf.LoopBegin = 0;	//���[�v�����̈�̍ŏ��̃T���v��
			data->buf.LoopLength = 0;
			data->pSource->SetFrequencyRatio(pitch);		//�s�b�`
			data->pSource->SetVolume(gain);					//�Q�C��
			data->pSource->Stop(0);									//��U��~
			data->pSource->FlushSourceBuffers();				//�{�C�X�L���[���폜(�Đ��ʒu��߂�����)
			hr = data->pSource->SubmitSourceBuffer(&data->buf, nullptr);	//Source�ɉ����̏��𑗂�
			if (FAILED(hr))
			{
				throw "SourceVoice Played failed";
			}
		}
		catch (const char* e)
		{
			std::cerr << e << std::endl;
		}
		if (data->pSource)
		{
			data->pSource->Start();
		}
	}

	void SoundSource::GetState()
	{
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
		data->pSource->GetState(&data->xstate, 0);
#else
		data->pSource->GetState(&xstate);
#endif
	}

	void SoundSource::Stop() const
	{
		if (data->pSource)
		{
			data->pSource->Stop(0);								//��U��~
			data->pSource->FlushSourceBuffers();			//�{�C�X�L���[���폜(�Đ��ʒu��߂�����)
			data->pSource->SubmitSourceBuffer(&data->buf, nullptr);	//Source�ɉ����̏��𑗂�
		}
	}

	void SoundSource::Pause()
	{
		GetState();
		UINT isPlay = data->xstate.BuffersQueued;	//�Đ����Ȃ�0�ȊO���Ԃ�
		if (data->pSource && isPlay != 0)
		{
			data->pSource->Stop(0);
		}
	}

	void SoundSource::UpDate3DSound(Vec3&& pos, Vec3&& listenerPos)
	{
		if (!data->is3D)
		{
			return;
		}
		XAUDIO2_VOICE_DETAILS voiceDetails;
		XAUDIO2_VOICE_DETAILS& deviceDetails = Audio::GetInstance()->GetVoiceDetails();
		data->subMixVoice->GetVoiceDetails(&voiceDetails);

		float emitterAzimuths[1] = { 0.0f };
		data->emitter.pCone = NULL;
		data->emitter.Position = { pos.x, pos.y, pos.z };
		data->emitter.OrientFront = { 0, 0, 1 };
		data->emitter.OrientTop = { 0 , 1, 0 };
		data->emitter.ChannelCount = voiceDetails.InputChannels;	//�\�[�X�̃`�����l����
		data->emitter.ChannelRadius = 1.0f;							//�G�~�b�^�ł̍s��̌v�Z�݂̂Ɏg�p�B���̒l��0.0f�ȏ�ł��邱�Ƃ��K�v
		data->emitter.pChannelAzimuths = emitterAzimuths;	//���ʊp�B�`�����l�����a�Ƌ��Ɏg�p�����B�s��̌v�Z�݂̂Ɏg�p
		data->emitter.InnerRadius = 1.0f;						//�������a�̌v�Z�Ɏg�p�����l�B0.0f �` MAX_FLT�̒l���w��
		data->emitter.InnerRadiusAngle = 1.0f;			//�����p�x�̌v�Z�Ɏg�p�����l�B0.0f �` X3DAUDIO_PI/4.0 �̒l���w��
		data->emitter.pVolumeCurve = NULL;				//�{�����[�����x�������J�[�u�B�s��̌v�Z�ɂ̂ݎg�p
		data->emitter.pLFECurve = NULL;					//LFE ���[���I�t�����J�[�u
		data->emitter.pLPFDirectCurve = NULL;			//���[�p�X�t�B���^�[(LPF)�_�C���N�g �p�X�W�������J�[�uNULL�ŋK��l
		data->emitter.pLPFReverbCurve = NULL;			//LPF���o�[�u�p�X�W�������J�[�u
		data->emitter.pReverbCurve = NULL;				//���o�[�u�Z���h���x�������J�[�u�B
		data->emitter.CurveDistanceScaler = 1.6f;		//���X�i�[�ɕ�������͈�
																			//�ق��̌v�Z�ɉe�����Ȃ��B���̒l��FLT_MIN�`FLT_MAX�͈̔͂ɂ���K�v������
		data->emitter.DopplerScaler = 1.0f;				//�h�b�v���[�Έڌ��ʂ��������邽�߂Ɏg�p����h�b�v���[�ΈڃX�P�[���[�B0.0f �` FLT_MAX �͈͓̔��ɂ���K�v������

		data->listener.OrientFront = X3DAUDIO_VECTOR(0, 0, 1);		//�O�������̒�`
		data->listener.OrientTop = X3DAUDIO_VECTOR(0, 1, 0);			//������̒�`
		data->listener.pCone = NULL;													//NULL�͑S�������Ɠ���
		data->listener.Position = X3DAUDIO_VECTOR(listenerPos.x, listenerPos.y, listenerPos.z);
		data->listener.Velocity = X3DAUDIO_VECTOR(1.0f, 0.0f, 1.0f);	//�h�b�v���[���ʂɗp����Position�ɂ͉e�����Ȃ�
		data->dsp.SrcChannelCount = data->emitter.ChannelCount;
		data->dsp.DstChannelCount = deviceDetails.InputChannels;
		if (!data->dsp.pMatrixCoefficients)
		{
			data->dsp.pMatrixCoefficients = new FLOAT32[data->dsp.SrcChannelCount * data->dsp.DstChannelCount];
			memset(data->dsp.pMatrixCoefficients, 0, sizeof(FLOAT32) * (data->dsp.SrcChannelCount * data->dsp.DstChannelCount));
		}
		/*emitter�\���̂ɂ�NULL���w�肵���ϐ��ɂ���ē��삪�ς��̂Œ���!!*/
		DWORD calculateFlags =
			X3DAUDIO_CALCULATE_MATRIX |						//�s��W���e�[�u���̌v�Z��L���ɂ��܂��B 
			//X3DAUDIO_CALCULATE_DELAY |						//�x�����Ԕz��̌v�Z��L���ɂ��܂� (�X�e���I�̂�)�B 
			X3DAUDIO_CALCULATE_LPF_DIRECT |				//���[�p�X�t�B���^�[ (LPF) �_�C���N�g�p�X�W���̌v�Z��L���ɂ��܂��B
			//X3DAUDIO_CALCULATE_LPF_REVERB |				//LPF ���o�[�u�p�X�W���̌v�Z��L���ɂ��܂��B 
			//	X3DAUDIO_CALCULATE_REVERB |					//���o�[�u�Z���h���x���̌v�Z��L���ɂ��܂��B 
			X3DAUDIO_CALCULATE_DOPPLER |						//�h�b�v���[�ΈڌW���̌v�Z��L���ɂ��܂��B 
			X3DAUDIO_CALCULATE_EMITTER_ANGLE;			//�G�~�b�^/���X�i�[�̓��p�̌v�Z��L���ɂ��܂��B 
			//X3DAUDIO_CALCULATE_ZEROCENTER |			//�����`�����l���ɖ����̒l��ݒ肵�܂��B 
			//X3DAUDIO_CALCULATE_REDIRECT_TO_LFE;		//���ׂẴ\�[�X�`�����l���̋ϓ��~�b�N�X�����g���� (LFE) �f�X�e�B�l�[�V�����`�����l���ɓK�p���܂��B

		X3DAudioCalculate(
			Audio::GetInstance()->Get3DInstance(),
			&data->listener,
			&data->emitter,
			calculateFlags,
			&data->dsp);

		data->subMixVoice->SetOutputMatrix(NULL, data->dsp.SrcChannelCount, data->dsp.DstChannelCount, data->dsp.pMatrixCoefficients);
		data->pSource->SetFrequencyRatio(data->dsp.DopplerFactor);

		XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 4.0f *data->dsp.LPFDirectCoefficient),  1.0f };

		data->subMixVoice->SetFilterParameters(&filterParameters);

	}

	void SoundSource::ExitLoop() const
	{
		data->pSource->ExitLoop();
	}

	void SoundSource::Destroy()
	{
		//�\�[�X�̍Đ����~�߂Ă���łȂ��ƃA�N�Z�X�ᔽ���N����
		data->pSource->Stop(0);
		if (data->dsp.pMatrixCoefficients != nullptr)
		{
			delete data->dsp.pMatrixCoefficients;
			data->dsp.pMatrixCoefficients = nullptr;
		}
		if (data != nullptr)
		{
			delete data;
			data = nullptr;
		}
		if (pcm != nullptr)
		{
			delete pcm;
			pcm = nullptr;
		}
	}

	long long SoundSource::GetCurrentSampleTime()
	{
		GetState();
		long long currentTime = -1;
		if (data->xstate.BuffersQueued > 0)
		{
			currentTime = (long long)data->xstate.SamplesPlayed;
		}
		return currentTime;
	}

	IXAudio2SourceVoice* SoundSource::GetSource()
	{
		return data->pSource;
	}

	WORD SoundSource::GetChannels()
	{
		return pcm->GetWaveFmtEx().nChannels;
	}

}