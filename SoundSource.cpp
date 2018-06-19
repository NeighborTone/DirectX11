#include "SoundSource.h"
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

SoundSource::SoundSource() :
	pSource(nullptr),
	is3D(false)
{
	
}

SoundSource::SoundSource(const std::string path, bool is3DSound) :
	pSource(nullptr),
	is3D(is3DSound)
{
	Load(path, is3D);
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

bool SoundSource::Load(const std::string path, bool is3DSound)
{
	std::string str_ogg = ".ogg";
	std::string str_wav = ".wav";
	SecureZeroMemory(&emitter, sizeof(emitter));
	SecureZeroMemory(&dsp, sizeof(dsp));
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

	HRESULT hr = 0;
	XAUDIO2_SEND_DESCRIPTOR sendDescription;
	XAUDIO2_VOICE_SENDS sendList;

	is3D = is3DSound;


	hr = Engine::GetSoundSystem().GetXAudio2()->CreateSubmixVoice(
		&subMixVoice,
		wav.GetWaveFmtEx().nChannels,
		wav.GetWaveFmtEx().nSamplesPerSec,
		XAUDIO2_VOICE_USEFILTER);
	ErrorMessage(hr, "SubMixVoice�̍쐬�Ɏ��s���܂���", "Error");

	sendDescription.Flags = 0;
	sendDescription.pOutputVoice = subMixVoice;

	sendList.pSends = &sendDescription;
	sendList.SendCount = 1;

	//�\�[�X�{�C�X�쐬
	hr = Engine::GetSoundSystem().GetXAudio2()->CreateSourceVoice(
		&pSource,
		&wav.GetWaveFmtEx(),
		0,
		XAUDIO2_DEFAULT_FREQ_RATIO,
		&voiceCallBack,
		&sendList,
		0);

	ErrorMessage(hr, "SourceVoice�̍쐬�Ɏ��s���܂���", "Error");

	return true;
}

void SoundSource::PlayBGM(int loopNum, float gain, float pitch)
{
	HRESULT hr = 0;
	buf = { 0 };
	if (fileType == WAVE)
	{
		buf.AudioBytes = wav.GetWaveByteSize();
		buf.pAudioData = wav.GetWaveData();
	}
	else if (fileType == OGG)
	{
		//buf.AudioBytes = ogg.GetWaveByteSize();
		//buf.pAudioData = ogg.GetWaveData();
	}
	buf.pContext = this;
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
	HRESULT hr = 0;
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
	buf.pContext = this;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.LoopCount = 0;	//���[�v�񐔂��w��B
	buf.LoopBegin = 0;	//���[�v�����̈�̍ŏ��̃T���v��
	buf.LoopLength = 0;
	pSource->SetFrequencyRatio(pitch);		//�s�b�`
	pSource->SetVolume(gain);					//�Q�C��
	pSource->Stop(0);									//��U��~
	pSource->FlushSourceBuffers();				//�{�C�X�L���[���폜(�Đ��ʒu��߂�����)
	pSource->SubmitSourceBuffer(&buf, nullptr);	//Source�ɉ����̏��𑗂�
	if (FAILED(hr))
	{
		MessageBox(NULL, "���y�f�[�^�̑��M�Ɏ��s���܂���", "Error", MB_OK);
	}

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
		pSource->Stop(0);								//��U��~
		pSource->FlushSourceBuffers();			//�{�C�X�L���[���폜(�Đ��ʒu��߂�����)
		pSource->SubmitSourceBuffer(&buf, nullptr);	//Source�ɉ����̏��𑗂�
	}
}

void SoundSource::UpDate3DSound(Vec3&& pos, Vec3&& listenerPos)
{
	if (!is3D)
	{
		return;
	}
	XAUDIO2_VOICE_DETAILS voiceDetails;
	XAUDIO2_VOICE_DETAILS& deviceDetails = Engine::GetSoundSystem().GetVoiceDetails();
	subMixVoice->GetVoiceDetails(&voiceDetails);

	
	float emitterAzimuths[1] = { 0.0f };
	emitter.pCone = NULL;
	emitter.Position = { 0, 0, 0 };
	emitter.OrientFront = { 0, 0, 1 };
	emitter.OrientTop = { 0 , 1, 0 };
	emitter.ChannelCount = voiceDetails.InputChannels;;		//�\�[�X�̃`�����l����
	emitter.ChannelRadius = 1.0f;							//�G�~�b�^�ł̍s��̌v�Z�݂̂Ɏg�p�B���̒l��0.0f�ȏ�ł��邱�Ƃ��K�v
	emitter.pChannelAzimuths = emitterAzimuths;	//���ʊp�B�`�����l�����a�Ƌ��Ɏg�p�����B�s��̌v�Z�݂̂Ɏg�p
	emitter.InnerRadius = 1.0f;					 //�������a�̌v�Z�Ɏg�p�����l�B0.0f �` MAX_FLT�̒l���w��
	emitter.InnerRadiusAngle = 1.0f;			//�����p�x�̌v�Z�Ɏg�p�����l�B0.0f �` X3DAUDIO_PI/4.0 �̒l���w��
	emitter.pVolumeCurve = NULL;				//�{�����[�����x�������J�[�u�B�s��̌v�Z�ɂ̂ݎg�p
	emitter.pLFECurve = NULL;					//LFE ���[���I�t�����J�[�u
	emitter.pLPFDirectCurve = NULL;			//���[�p�X�t�B���^�[(LPF)�_�C���N�g �p�X�W�������J�[�uNULL�ŋK��l
	emitter.pLPFReverbCurve = NULL;			//LPF���o�[�u�p�X�W�������J�[�u
	emitter.pReverbCurve = NULL;				//���o�[�u�Z���h���x�������J�[�u�B
	emitter.CurveDistanceScaler = 10.2f;		//���X�i�[�ɕ�������͈�
																//�ق��̌v�Z�ɉe�����Ȃ��B���̒l��FLT_MIN�`FLT_MAX�͈̔͂ɂ���K�v������
	emitter.DopplerScaler = 1.0f;					//�h�b�v���[�Έڌ��ʂ��������邽�߂Ɏg�p����h�b�v���[�ΈڃX�P�[���[�B0.0f �` FLT_MAX �͈͓̔��ɂ���K�v������

	listener.OrientFront = DirectX::XMFLOAT3(0, 0, 1);			//�O�������̒�`
	listener.OrientTop = DirectX::XMFLOAT3(0, 1, 0);				//������̒�`
	listener.pCone = NULL;													//NULL�͑S�������Ɠ���
	listener.Position = DirectX::XMFLOAT3(listenerPos.x, listenerPos.y, listenerPos.z);
	listener.Velocity = DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f);	//�h�b�v���[���ʂɗp����Position�ɂ͉e�����Ȃ�
	dsp.SrcChannelCount = emitter.ChannelCount;
	dsp.DstChannelCount = deviceDetails.InputChannels;
	if (!dsp.pMatrixCoefficients)
	{
		dsp.pMatrixCoefficients = new FLOAT32[dsp.SrcChannelCount * dsp.DstChannelCount];
		memset(dsp.pMatrixCoefficients, 0, sizeof(FLOAT32) * (dsp.SrcChannelCount * dsp.DstChannelCount));
	}
	/*emitter�\���̂ɂ�NULL���w�肵���ϐ��ɂ���ē��삪�ς��̂Œ���!!*/
	DWORD calculateFlags =
		X3DAUDIO_CALCULATE_MATRIX |						//�s��W���e�[�u���̌v�Z��L���ɂ��܂��B 
		//X3DAUDIO_CALCULATE_DELAY |						//�x�����Ԕz��̌v�Z��L���ɂ��܂� (�X�e���I�̂�)�B 
		X3DAUDIO_CALCULATE_LPF_DIRECT |				//���[�p�X�t�B���^�[ (LPF) �_�C���N�g�p�X�W���̌v�Z��L���ɂ��܂��B
		//X3DAUDIO_CALCULATE_LPF_REVERB |				//LPF ���o�[�u�p�X�W���̌v�Z��L���ɂ��܂��B 
	//	X3DAUDIO_CALCULATE_REVERB |						//���o�[�u�Z���h���x���̌v�Z��L���ɂ��܂��B 
		X3DAUDIO_CALCULATE_DOPPLER |					//�h�b�v���[�ΈڌW���̌v�Z��L���ɂ��܂��B 
		X3DAUDIO_CALCULATE_EMITTER_ANGLE;		//�G�~�b�^/���X�i�[�̓��p�̌v�Z��L���ɂ��܂��B 
		//X3DAUDIO_CALCULATE_ZEROCENTER |			//�����`�����l���ɖ����̒l��ݒ肵�܂��B 
		//X3DAUDIO_CALCULATE_REDIRECT_TO_LFE;	//���ׂẴ\�[�X�`�����l���̋ϓ��~�b�N�X�����g���� (LFE) �f�X�e�B�l�[�V�����`�����l���ɓK�p���܂��B

	X3DAudioCalculate(
		Engine::GetSoundSystem().Get3DInstance(), 
		&listener, 
		&emitter, 
		calculateFlags,
		&dsp);

	subMixVoice->SetOutputMatrix(NULL, dsp.SrcChannelCount, dsp.DstChannelCount, dsp.pMatrixCoefficients);
	pSource->SetFrequencyRatio(dsp.DopplerFactor);

	XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 4.0f * dsp.LPFDirectCoefficient),  1.0f };

	subMixVoice->SetFilterParameters(&filterParameters);
	
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
	Memory::SafeDelete(dsp.pMatrixCoefficients);
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

long long SoundSource::GetCurrentSampleTime()
{
	GetState();
	long long currentTime = -1;
	if (xstate.BuffersQueued > 0)
	{
		currentTime = (long long)xstate.SamplesPlayed;
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