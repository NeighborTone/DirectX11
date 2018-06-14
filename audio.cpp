#include "audio.h"
#include "Engine.h"
using namespace EffectParameters;

void SoundSource::GetState()
{
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
	pSource->GetState(&xstate, 0);
#else
	pSource->GetState(&xstate);
#endif
}

SoundSource::SoundSource()
{
	pSource = nullptr;
}

SoundSource::SoundSource(SoundSource& sound)
{
	pSource = sound.pSource;
	wav = sound.wav;
}

SoundSource::~SoundSource()
{
	Destroy();
}

bool SoundSource::Load(const char* path)
{
	if (!wav.Load(path))
	{
		MessageBox(NULL, "�\�[�X�{�C�X�̍쐬�Ɏ��s���܂���", "Error", MB_OK);
		return false;
	}

	return true;
}

void SoundSource::PlayBGM(int loopNum,float gain, float pitch)
{	
	HRESULT hr;
	buf = { 0 };
	buf.AudioBytes = wav.GetWaveSize();
	buf.pAudioData = wav.GetWaveData();
	buf.Flags = XAUDIO2_END_OF_STREAM;	//���̃o�b�t�@�̌�Ƀf�[�^���Ȃ����Ƃ��\�[�X�{�C�X�ɓ`����
	buf.LoopCount = loopNum;	//���[�v�񐔂��w��B�f�t�H���g�Ŗ������[�v
	buf.LoopBegin = 0;
	pSource->SetFrequencyRatio(pitch);	//�s�b�`
	pSource->SetVolume(gain);					//�Q�C��
	hr = pSource->SubmitSourceBuffer(&buf, nullptr);	//Source�ɉ����̏��𑗂�
	if (FAILED(hr))
	{
		MessageBox(NULL, "���y�f�[�^�̑��M�Ɏ��s���܂���", "Error", MB_OK);
	}

	if (pSource)
	{
		pSource->Start();
	}
}

void SoundSource::PlaySE(float gain, float pitch)
{
	buf = { 0 };
	buf.AudioBytes = wav.GetWaveSize();
	buf.pAudioData = wav.GetWaveData();
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.LoopCount = 0;	//���[�v�񐔂��w��B
	buf.LoopBegin = 0;

	
	pSource->SetFrequencyRatio(pitch);			//�s�b�`
	pSource->SetVolume(gain);					//�Q�C��
	pSource->Stop(0);							//��U��~
	pSource->FlushSourceBuffers();				//�{�C�X�L���[���폜(�Đ��ʒu��߂�����)
	pSource->SubmitSourceBuffer(&buf, nullptr);	//Source�ɉ����̏��𑗂�
	
	if (pSource)
	{
		pSource->Start();
	}
}

void SoundSource::Pause()
{
	XAUDIO2_VOICE_STATE xa2state;
	pSource->GetState(&xa2state);
	auto isPlay = xa2state.BuffersQueued;	//�Đ����Ȃ�0�ȊO���Ԃ�
	if (pSource && isPlay != 0)
	{
		pSource->Stop(0);
	}
}
void SoundSource::Stop()
{
	if (pSource)
	{
		pSource->Stop(0);							//��U��~
		pSource->FlushSourceBuffers();			//�{�C�X�L���[���폜(�Đ��ʒu��߂�����)
		pSource->SubmitSourceBuffer(&buf, nullptr);	//Source�ɉ����̏��𑗂�
	}
	
}
void SoundSource::Destroy()
{
	if (pSource != nullptr)
	{
		pSource->Stop(0);
		pSource->DestroyVoice();
		pSource = nullptr;
	}
}

void SoundSource::SetEQ(EQ_DESC& eq_desc)
{
	//�쐬�������G�t�F�N�g�̎�ނ�ݒ�(FXEQ,FXReverb,FXMasteringLimiter,FXEcho)
	IUnknown* effect;
	CreateFX(__uuidof(FXEQ), &effect);
	XAUDIO2_EFFECT_DESCRIPTOR desc;
	desc.InitialState = TRUE;
	desc.OutputChannels = 2;      // �o�̓`�����l����
	desc.pEffect = effect; // �G�t�F�N�g�ւ̃|�C���^

	XAUDIO2_EFFECT_CHAIN chain;
	chain.pEffectDescriptors = &desc; // Descriptor�ւ̃|�C���^�A�����ڑ�����ꍇ�͔z��̐擪
	chain.EffectCount = 1;     // Descriptor����������̂�

	pSource->SetEffectChain(&chain);
	effect->Release();

	//�C�R���C�U�[
	FXEQ_PARAMETERS eq;
	eq.Bandwidth0 = eq_desc.Bandwidth0;
	eq.Bandwidth1 = eq_desc.Bandwidth1;
	eq.Bandwidth2 = eq_desc.Bandwidth2;
	eq.Bandwidth3 = eq_desc.Bandwidth3;
	eq.Gain0 = eq_desc.Gain0;
	eq.Gain1 = eq_desc.Gain1;
	eq.Gain2 = eq_desc.Gain2;
	eq.Gain3 = eq_desc.Gain3;
	eq.FrequencyCenter0 = eq_desc.FrequencyCenter0;
	eq.FrequencyCenter1 = eq_desc.FrequencyCenter1;
	eq.FrequencyCenter2 = eq_desc.FrequencyCenter2;
	eq.FrequencyCenter3 = eq_desc.FrequencyCenter3;
	//�Z�b�g����
	pSource->SetEffectParameters(0, &eq, sizeof(FXEQ_PARAMETERS));


}

void SoundSource::SetReverb(REVERB_DESC& reverb_desc)
{
	//�쐬�������G�t�F�N�g�̎�ނ�ݒ�(FXEQ, FXReverb, FXMasteringLimiter, FXEcho)
	IUnknown* effect;
	CreateFX(__uuidof(FXEQ), &effect);
	XAUDIO2_EFFECT_DESCRIPTOR desc;
	desc.InitialState = TRUE;
	desc.OutputChannels = 2;      // �o�̓`�����l����
	desc.pEffect = effect; // �G�t�F�N�g�ւ̃|�C���^

	XAUDIO2_EFFECT_CHAIN chain;
	chain.pEffectDescriptors = &desc; // Descriptor�ւ̃|�C���^�A�����ڑ�����ꍇ�͔z��̐擪
	chain.EffectCount = 1;     // Descriptor����������̂�

	pSource->SetEffectChain(&chain);
	effect->Release();

	//���o�[�u
	FXREVERB_PARAMETERS reverb;
	reverb.Diffusion = reverb_desc.Diffusion;
	reverb.RoomSize = reverb_desc.RoomSize;

	//�Z�b�g����
	pSource->SetEffectParameters(0, &reverb, sizeof(FXREVERB_PARAMETERS));
}

void SoundSource::SetDelay(EffectParameters::DELAY_DESC & delay_desc)
{
	//�쐬�������G�t�F�N�g�̎�ނ�ݒ�(FXEQ, FXReverb, FXMasteringLimiter, FXEcho)
	IUnknown* effect;
	CreateFX(__uuidof(FXEcho), &effect);
	XAUDIO2_EFFECT_DESCRIPTOR desc;
	desc.InitialState = TRUE;
	desc.OutputChannels = 2;      // �o�̓`�����l����
	desc.pEffect = effect; // �G�t�F�N�g�ւ̃|�C���^

	XAUDIO2_EFFECT_CHAIN chain;
	chain.pEffectDescriptors = &desc; // Descriptor�ւ̃|�C���^�A�����ڑ�����ꍇ�͔z��̐擪
	chain.EffectCount = 1;     // Descriptor����������̂�

	pSource->SetEffectChain(&chain);
	effect->Release();

	FXECHO_PARAMETERS delay;
	delay.WetDryMix = delay_desc.WetDryMix;
	delay.Feedback = delay_desc.Feedback;
	delay.WetDryMix = delay_desc.Delay;

	//�Z�b�g����
	pSource->SetEffectParameters(0, &delay, sizeof(FXECHO_PARAMETERS));
}


unsigned __int64 SoundSource::GetCurrentSampleTime()
{
	GetState();
	int currentTime = -1;
	if (xstate.BuffersQueued > 0)
	{
		currentTime = (int)xstate.SamplesPlayed;
	}
	return currentTime;
}

IXAudio2SourceVoice** SoundSource::GetSource()
{
	return &pSource;
}

WAV SoundSource::GetWav()
{
	return wav;
}


SoundSystem::SoundSystem():
	pXAudio2(nullptr),
	pMaster(nullptr)
{
	Engine::COMInitialize();
	Create();
}

void SoundSystem::DestroySystem(SoundSource& source)
{
	//�������
	//Source��Master��XAudio2
	source.Destroy();
	//�}�X�^�[�{�C�X�j��
	if (pMaster != nullptr)
	{
		pMaster->DestroyVoice();
		pMaster = nullptr;
	}
}

SoundSystem::~SoundSystem()
{
	//�}�X�^�[�{�C�X�j��
	if (pMaster != nullptr)
	{
		pMaster->DestroyVoice();
		pMaster = nullptr;
	}
}


bool SoundSystem::Create()
{
	HRESULT hr;
	//XAudio2�̏�����
	hr = XAudio2Create(&pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(NULL, "XAudio2�̏������Ɏ��s���܂���", "Error", MB_OK);
		return false;
	}
	//�}�X�^�[�{�C�X�̐���
	hr = pXAudio2->CreateMasteringVoice(
		&pMaster, 
		XAUDIO2_DEFAULT_CHANNELS,
		XAUDIO2_DEFAULT_SAMPLERATE,
		0,
		0,
		NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�}�X�^�[�{�C�X�̏������Ɏ��s���܂���", "Error", MB_OK);
		return false;
	}
	return true;
}





void SoundSystem::SetMasterGain(float gain)
{
	pMaster->SetVolume(gain);
}

bool SoundSystem::AddSource(SoundSource& source)
{
	HRESULT hr;
	hr = pXAudio2->CreateSourceVoice(
		source.GetSource(),
		&source.GetWav().GetWaveFmtEx());
	if (FAILED(hr))
	{
		MessageBox(NULL, "�\�[�X�{�C�X�̒ǉ��Ɏ��s���܂���", "Error", MB_OK);
		return false;
	}
	return true;
}

bool SoundSystem::AddSourceUseCallBack(SoundSource & source)
{
	HRESULT hr;
	hr = pXAudio2->CreateSourceVoice(
		source.GetSource(),
		&source.GetWav().GetWaveFmtEx(),
		0, 
		XAUDIO2_DEFAULT_FREQ_RATIO, 
		&voiceCallback, 
		NULL, 
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "�\�[�X�{�C�X�̒ǉ��Ɏ��s���܂���", "Error", MB_OK);
		return false;
	}
	return true;
}
