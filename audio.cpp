#include "audio.h"
#include "Engine.h"

SoundSystem::SoundSystem() :
	pXAudio2(nullptr),
	pMaster(nullptr)
{
	Engine::COMInitialize();
	SecureZeroMemory(&x3DInstance, sizeof(x3DInstance));
	SecureZeroMemory(&voiceDetails, sizeof(voiceDetails));
	x3DInstance;

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

IXAudio2* SoundSystem::GetXAudio2() const
{
	return pXAudio2.p;
}

IXAudio2MasteringVoice * SoundSystem::GetMaster() const
{
	return pMaster;
}

X3DAUDIO_HANDLE& SoundSystem::Get3DInstance()
{
	return x3DInstance;
}

XAUDIO2_VOICE_DETAILS& SoundSystem::GetVoiceDetails()
{
	return voiceDetails;
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
	//Detail�̏�����
	pMaster->GetVoiceDetails(&voiceDetails);
	//3D�C���^�[�t�F�[�X�̏�����
	DWORD speakerMask;

	pMaster->GetChannelMask(&speakerMask);

	hr = X3DAudioInitialize(speakerMask, X3DAUDIO_SPEED_OF_SOUND, x3DInstance);
	if (FAILED(hr))
	{
		MessageBox(NULL, "X3DAudio�̏������Ɏ��s���܂���", "Error", MB_OK);
		return false;
	}
	return true;
}


void SoundSystem::SetMasterGain(float gain)
{
	pMaster->SetVolume(gain);
}


