#include "audio.h"
#include "Engine.h"
#include "Utility.hpp"

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

bool SoundSource::Load(const std::string path)
{
	std::string str_ogg = ".ogg";
	std::string str_wav = ".wav";

	if (std::equal(path.begin() + path.find("."), path.end(), str_ogg.begin()))
	{
		fileType = OGG;
		ErrorMessage(ogg.Load(path), "ogg�̓ǂݍ��݂Ɏ��s���܂���", "Error");
	}
	else if (std::equal(path.begin() + path.find("."), path.end(), str_wav.begin()))
	{
		fileType = WAVE;
		ErrorMessage(wav.Load(path.c_str()), "wav�̓ǂݍ��݂Ɏ��s���܂���", "Error");
	}
	else
	{
		ErrorMessage("�s���ȃt�@�C�����T�|�[�g�O�̃t�H�[�}�b�g�ł�", "Error");
		return false;
	}
	return true;
}

void SoundSource::PlayBGM(int loopNum, float gain, float pitch)
{
	HRESULT hr;
	buf = { 0 };
	if (fileType == WAVE)
	{
		buf.AudioBytes = wav.GetWaveByteSize();
		buf.pAudioData = wav.GetWaveData();
	}
	else if (fileType == OGG)
	{
		buf.AudioBytes = ogg.GetWaveByteSize();
		buf.pAudioData = ogg.GetWaveData();
	}
	buf.Flags = XAUDIO2_END_OF_STREAM;	//���̃o�b�t�@�̌�Ƀf�[�^���Ȃ����Ƃ��\�[�X�{�C�X�ɓ`����
	buf.LoopCount = loopNum;	//���[�v�񐔂��w��B�f�t�H���g�Ŗ������[�v�ɂ��Ă���
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
	if (fileType == WAVE)
	{
		buf.AudioBytes = wav.GetWaveByteSize();
		buf.pAudioData = wav.GetWaveData();
	}
	else if (fileType == OGG)
	{
		buf.AudioBytes = ogg.GetWaveByteSize();	//�I�[�f�B�I �f�[�^�̃T�C�Y (�o�C�g�P��) 
		buf.pAudioData = ogg.GetWaveData();	//�I�[�f�B�I �f�[�^�ւ̃|�C���^�[
	}
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.LoopCount = 0;	//���[�v�񐔂��w��B
	buf.LoopBegin = 0;	//���[�v�����̈�̍ŏ��̃T���v��
	buf.LoopLength = 0;
	buf.pContext = nullptr;
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
void SoundSource::ExitLoop()
{
	pSource->ExitLoop();
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

void SoundSource::SetEQ(Equalizer_DESC& eq_desc)
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

void SoundSource::SetSimpleReverb(SimpleReverb_DESC& reverb_desc)
{
	//�쐬�������G�t�F�N�g�̎�ނ�ݒ�(FXEQ, FXReverb, FXMasteringLimiter, FXEcho)
	IUnknown* effect;

	CreateFX(__uuidof(FXReverb), &effect);
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

void SoundSource::SetReverb(Reverb_DESC& reverb_desc)
{
	IUnknown* effect;
	XAudio2CreateReverb(&effect);
	XAUDIO2_EFFECT_DESCRIPTOR desc;
	desc.InitialState = TRUE;
	desc.OutputChannels = 2;      // �o�̓`�����l����
	desc.pEffect = effect; // �G�t�F�N�g�ւ̃|�C���^

	XAUDIO2_EFFECT_CHAIN chain;
	chain.pEffectDescriptors = &desc; // Descriptor�ւ̃|�C���^�A�����ڑ�����ꍇ�͔z��̐擪
	chain.EffectCount = 1;     // Descriptor����������̂�

	pSource->SetEffectChain(&chain);
	effect->Release();

	XAUDIO2FX_REVERB_PARAMETERS rev;
	rev.WetDryMix = reverb_desc.WetDryMix;
	rev.ReflectionsDelay = reverb_desc.ReflectionsDelay;
	rev.ReverbDelay = reverb_desc.ReverbDelay;
	rev.RearDelay = reverb_desc.RearDelay;
	rev.PositionLeft = reverb_desc.PositionLeft;
	rev.PositionRight = reverb_desc.PositionRight;
	rev.PositionMatrixLeft = reverb_desc.PositionMatrixLeft;
	rev.PositionMatrixRight = reverb_desc.PositionMatrixRight;
	rev.EarlyDiffusion = reverb_desc.EarlyDiffusion;
	rev.LateDiffusion = reverb_desc.LateDiffusion;
	rev.LowEQGain = reverb_desc.LowEQGain;
	rev.LowEQCutoff = reverb_desc.LowEQCutoff;
	rev.HighEQGain = reverb_desc.HighEQGain;
	rev.HighEQCutoff = reverb_desc.HighEQCutoff;
	rev.RoomFilterFreq = reverb_desc.RoomFilterFreq;
	rev.RoomFilterMain = reverb_desc.RoomFilterMain;
	rev.RoomFilterHF = reverb_desc.RoomFilterHF;
	rev.ReflectionsGain = reverb_desc.ReflectionsGain;
	rev.ReverbGain = reverb_desc.ReverbGain;
	rev.DecayTime = reverb_desc.DecayTime;
	rev.Density = reverb_desc.Density;
	rev.RoomSize = reverb_desc.RoomSize;


	//�Z�b�g����
	pSource->SetEffectParameters(0, &rev, sizeof(XAUDIO2FX_REVERB_PARAMETERS));
}

void SoundSource::SetDelay(Delay_DESC & delay_desc)
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
	delay.Delay = delay_desc.DelayTime;

	//�Z�b�g����
	pSource->SetEffectParameters(0, &delay, sizeof(FXECHO_PARAMETERS));
}

void SoundSource::SetLimiter(Limiter_DESC& limiter_desc)
{
	//�쐬�������G�t�F�N�g�̎�ނ�ݒ�(FXEQ, FXReverb, FXMasteringLimiter, FXEcho)
	IUnknown* effect;
	CreateFX(__uuidof(FXMasteringLimiter), &effect);
	XAUDIO2_EFFECT_DESCRIPTOR desc;
	desc.InitialState = TRUE;
	desc.OutputChannels = 2;      // �o�̓`�����l����
	desc.pEffect = effect; // �G�t�F�N�g�ւ̃|�C���^

	XAUDIO2_EFFECT_CHAIN chain;
	chain.pEffectDescriptors = &desc; // Descriptor�ւ̃|�C���^�A�����ڑ�����ꍇ�͔z��̐擪
	chain.EffectCount = 1;     // Descriptor����������̂�

	pSource->SetEffectChain(&chain);
	effect->Release();

	FXMASTERINGLIMITER_PARAMETERS limiter;
	limiter.Release = limiter_desc.Release;
	limiter.Loudness = limiter_desc.Loudness;

	//�Z�b�g����
	pSource->SetEffectParameters(0, &limiter, sizeof(FXMASTERINGLIMITER_PARAMETERS));
}

void SoundSource::SetMultiEffecter(Equalizer_DESC& eq_desc, Reverb_DESC& reverb_desc, Delay_DESC& delay_desc, Limiter_DESC& limiter_desc)
{
	//�쐬�������G�t�F�N�g�̎�ނ�ݒ�(FXEQ, FXReverb, FXMasteringLimiter, FXEcho)
	IUnknown* effect[4];
	CreateFX(__uuidof(FXEQ), &effect[0]);
	XAudio2CreateReverb(&effect[1]);
	CreateFX(__uuidof(FXEcho), &effect[2]);
	CreateFX(__uuidof(FXMasteringLimiter), &effect[3]);

	XAUDIO2_EFFECT_DESCRIPTOR desc[4];
	int i = 0;
	for (auto &it : desc)
	{
		it.InitialState = true;
		it.OutputChannels = 2;
		it.pEffect = effect[i];
		++i;
	}


	XAUDIO2_EFFECT_CHAIN chain;
	chain.pEffectDescriptors = &desc[0]; // Descriptor�ւ̃|�C���^�A�����ڑ�����ꍇ�͔z��̐擪
	chain.EffectCount = 4;     // Descriptor����������̂�

	pSource->SetEffectChain(&chain);
	for (auto &it : effect)
	{
		it->Release();
	}

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
	XAUDIO2FX_REVERB_PARAMETERS rev;
	rev.WetDryMix = reverb_desc.WetDryMix;
	rev.ReflectionsDelay = reverb_desc.ReflectionsDelay;
	rev.ReverbDelay = reverb_desc.ReverbDelay;
	rev.RearDelay = reverb_desc.RearDelay;
	rev.PositionLeft = reverb_desc.PositionLeft;
	rev.PositionRight = reverb_desc.PositionRight;
	rev.PositionMatrixLeft = reverb_desc.PositionMatrixLeft;
	rev.PositionMatrixRight = reverb_desc.PositionMatrixRight;
	rev.EarlyDiffusion = reverb_desc.EarlyDiffusion;
	rev.LateDiffusion = reverb_desc.LateDiffusion;
	rev.LowEQGain = reverb_desc.LowEQGain;
	rev.LowEQCutoff = reverb_desc.LowEQCutoff;
	rev.HighEQGain = reverb_desc.HighEQGain;
	rev.HighEQCutoff = reverb_desc.HighEQCutoff;
	rev.RoomFilterFreq = reverb_desc.RoomFilterFreq;
	rev.RoomFilterMain = reverb_desc.RoomFilterMain;
	rev.RoomFilterHF = reverb_desc.RoomFilterHF;
	rev.ReflectionsGain = reverb_desc.ReflectionsGain;
	rev.ReverbGain = reverb_desc.ReverbGain;
	rev.DecayTime = reverb_desc.DecayTime;
	rev.Density = reverb_desc.Density;
	rev.RoomSize = reverb_desc.RoomSize;
	FXECHO_PARAMETERS delay;
	delay.WetDryMix = delay_desc.WetDryMix;
	delay.Feedback = delay_desc.Feedback;
	delay.Delay = delay_desc.DelayTime;
	FXMASTERINGLIMITER_PARAMETERS limiter;
	limiter.Release = limiter_desc.Release;
	limiter.Loudness = limiter_desc.Loudness;

	//�Z�b�g����
	pSource->SetEffectParameters(0, &eq, sizeof(FXEQ_PARAMETERS));
	pSource->SetEffectParameters(1, &rev, sizeof(XAUDIO2FX_REVERB_PARAMETERS));
	pSource->SetEffectParameters(2, &delay, sizeof(FXECHO_PARAMETERS));
	pSource->SetEffectParameters(3, &limiter, sizeof(FXMASTERINGLIMITER_PARAMETERS));
}

int SoundSource::GetCurrentSampleTime()
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

Wav SoundSource::GetWav()
{
	return wav;
}

Ogg SoundSource::GetOgg()
{
	return ogg;
}

FileType SoundSource::GetFileType()
{
	return fileType;
}






SoundSystem::SoundSystem() :
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
	HRESULT hr = 0;
	if (source.GetFileType() == WAVE)
	{
		hr = pXAudio2->CreateSourceVoice(
			source.GetSource(),
			&source.GetWav().GetWaveFmtEx());
	}
	else if (source.GetFileType() == OGG)
	{
		hr = pXAudio2->CreateSourceVoice(
			source.GetSource(),
			&source.GetOgg().GetWaveFmtEx());
	}
	if (FAILED(hr))
	{
		MessageBox(NULL, "�\�[�X�{�C�X�̒ǉ��Ɏ��s���܂���", "Error", MB_OK);
		return false;
	}
	return true;
}

bool SoundSystem::AddSourceUseCallBack(SoundSource& source)
{
	HRESULT hr;
	if (source.GetFileType() == WAVE)
	{
		hr = pXAudio2->CreateSourceVoice(
			source.GetSource(),
			&source.GetWav().GetWaveFmtEx(),
			0,
			XAUDIO2_DEFAULT_FREQ_RATIO,
			&voiceCallback,
			NULL,
			NULL);
	}
	else
	{
		hr = pXAudio2->CreateSourceVoice(
			source.GetSource(),
			&source.GetOgg().GetWaveFmtEx(),
			0,
			XAUDIO2_DEFAULT_FREQ_RATIO,
			&voiceCallback,
			NULL,
			NULL);
	}
	if (FAILED(hr))
	{
		MessageBox(NULL, "�\�[�X�{�C�X�̒ǉ��Ɏ��s���܂���", "Error", MB_OK);
		return false;
	}
	return true;
}
