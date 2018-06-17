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

void SoundSource::Init()
{
	float emitterAzimuths[INPUTCHANNELS] = { 0.0f };
	pSource = nullptr;
	//MEMO:3D�T�E���h�̓��m����
	dsp = { 0 };
	emitter = {0};
	emitter.pCone = NULL;
	emitter.Position = { 0, 0, 0 };
	emitter.OrientFront = { 0, 0, 1 };
	emitter.OrientTop = { 0 , 1, 0 };
	emitter.ChannelCount = INPUTCHANNELS;		//�\�[�X�̃`�����l����
	emitter.ChannelRadius = 1.0f;							//�G�~�b�^�ł̍s��̌v�Z�݂̂Ɏg�p�B���̒l��0.0f�ȏ�ł��邱�Ƃ��K�v
	emitter.pChannelAzimuths = emitterAzimuths;	//���ʊp�B�`�����l�����a�Ƌ��Ɏg�p�����B�s��̌v�Z�݂̂Ɏg�p
	emitter.InnerRadius = 0.0f;					 //�������a�̌v�Z�Ɏg�p�����l�B0.0f �` MAX_FLT�̒l���w��
	emitter.InnerRadiusAngle = 0.0f;			//�����p�x�̌v�Z�Ɏg�p�����l�B0.0f �` X3DAUDIO_PI/4.0 �̒l���w��
	emitter.pVolumeCurve = NULL;				//�{�����[�����x�������J�[�u�B�s��̌v�Z�ɂ̂ݎg�p
	emitter.pLFECurve = NULL;					//LFE ���[���I�t�����J�[�u
	emitter.pLPFDirectCurve = NULL;			//���[�p�X�t�B���^�[(LPF)�_�C���N�g �p�X�W�������J�[�uNULL�ŋK��l
	emitter.pLPFReverbCurve = NULL;			//LPF���o�[�u�p�X�W�������J�[�u
	emitter.pReverbCurve = NULL;				//���o�[�u�Z���h���x�������J�[�u�B
	emitter.CurveDistanceScaler = FLT_MIN;	//���K�����ꂽ�����J�[�u�����[�U�[��`�̃��[���h�P�ʂɃX�P�[�����O���邽�߂ɁA�܂��͂��̌��ʂ��������邽�߂Ɏg�p����J�[�u�����X�P�[���B
																//�ق��̌v�Z�ɉe�����Ȃ��B���̒l��FLT_MIN�`FLT_MAX�͈̔͂ɂ���K�v������
	emitter.DopplerScaler = 1.0f;					//�h�b�v���[�Έڌ��ʂ��������邽�߂Ɏg�p����h�b�v���[�ΈڃX�P�[���[�B0.0f �` FLT_MAX �͈͓̔��ɂ���K�v������

}

SoundSource::SoundSource()
{
	Init();
}

SoundSource::SoundSource(const std::string path)
{
	Init();
	Load(path);
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
	GetState();
	UINT isPlay = xstate.BuffersQueued;	//�Đ����Ȃ�0�ȊO���Ԃ�
	if (pSource && isPlay != 0)
	{
		pSource->Stop(0);
	}
}
void SoundSource::Stop() const
{
	if (pSource)
	{
		pSource->Stop(0);							//��U��~
		pSource->FlushSourceBuffers();			//�{�C�X�L���[���폜(�Đ��ʒu��߂�����)
		pSource->SubmitSourceBuffer(&buf, nullptr);	//Source�ɉ����̏��𑗂�
	}

}

void SoundSource::UpData()
{
	//MEMO : �܂��悭�킩���
	dsp.SrcChannelCount = INPUTCHANNELS;		//�\�[�X �`�����l���̐��BX3DAudioCalculate���Ăяo���O�ɃG�~�b�^�`�����l���̐��ɏ���������K�v������B
	dsp.DstChannelCount = Engine::GetSoundSystem().GetNumChannels();		//�f�X�e�B�l�[�V���� �`�����l���̐��BX3DAudioCalculate���Ăяo���O�ɍŏI�~�b�N�X�`�����l���̐��ɏ���������K�v������B
	dsp.pMatrixCoefficients = new float[dsp.DstChannelCount];
	X3DAudioCalculate(Engine::GetSoundSystem().Get3DInstance(), &Engine::GetSoundSystem().GetListener(), &emitter, X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER, &dsp);
	pSource->SetOutputMatrix(Engine::GetSoundSystem().GetMaster(),1, dsp.DstChannelCount, dsp.pMatrixCoefficients);
	pSource->SetFrequencyRatio(dsp.DopplerFactor);
	Memory::SafeDelete(dsp.pMatrixCoefficients);
	
}
void SoundSource::UpDataPosition(Vec3&& pos)
{
	emitter.Position = { pos .x,pos .y,pos.z};
	UpData();
}
void SoundSource::ExitLoop() const
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
	desc.OutputChannels = wav.GetWaveFmtEx().nChannels;      // �o�̓`�����l����
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
	desc.OutputChannels = wav.GetWaveFmtEx().nChannels;      // �o�̓`�����l����
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
	desc.OutputChannels = wav.GetWaveFmtEx().nChannels;      // �o�̓`�����l����
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
	desc.OutputChannels = wav.GetWaveFmtEx().nChannels;      // �o�̓`�����l����
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
	desc.OutputChannels = wav.GetWaveFmtEx().nChannels;      // �o�̓`�����l����
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
		it.OutputChannels = wav.GetWaveFmtEx().nChannels;
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

IXAudio2MasteringVoice * SoundSystem::GetMaster() const
{
	return pMaster;
}

X3DAUDIO_LISTENER SoundSystem::GetListener() const
{
	return listener;
}

X3DAUDIO_HANDLE& SoundSystem::Get3DInstance()
{
	return x3DInstance;
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
	
	listener.Position = { 0,0,0 };	//XMFLOTA3
	//�ȉ���͒����ɂ��Ȃ���΂Ȃ�Ȃ�
	listener.OrientFront = { 0,0,1 };	//�O�������̒�`
	listener.OrientTop = { 0,1,0 };	//������̒�`
	listener.pCone = nullptr;				//NULL�͑S�������Ɠ���
	listener.Velocity = { 0,0,0 };	//�h�b�v���[���ʂɗp����Position�ɂ͉e�����Ȃ�

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

	//3D�C���^�[�t�F�[�X�̏�����
	X3DAudioInitialize(GetChannelMask(), X3DAUDIO_SPEED_OF_SOUND, x3DInstance);

	return true;
}

UINT SoundSystem::GetNumChannels()
{
		XAUDIO2_VOICE_DETAILS d;
		pMaster->GetVoiceDetails(&d);
		return d.InputChannels;
}

UINT SoundSystem::GetChannelMask() const
{
	UINT mask;
	pMaster->GetChannelMask((DWORD*)&mask);
	return mask;
}

void SoundSystem::SetMasterGain(float gain)
{
	pMaster->SetVolume(gain);
}

void SoundSystem::SetListenerPosition(float x, float y, float z)
{
	listener.Position = { x, y, z };
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
