#include "Audio.h"
#include <iostream>
#include <memory>
namespace SoundEngine
{
	void Audio::Create()
	{
		HRESULT hr;
		try {
			//XAudio2�̏�����
			hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
			if (FAILED(hr))
			{
				throw "XAudio2 Initialization failed";
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
				throw  "MasteringVoice Initialization failed";
			}
			//Detail�̏�����
			pMaster->GetVoiceDetails(&voiceDetails);
			//3D�C���^�[�t�F�[�X�̏�����
			DWORD speakerMask;

			pMaster->GetChannelMask(&speakerMask);

			hr = X3DAudioInitialize(speakerMask, X3DAUDIO_SPEED_OF_SOUND, x3DInstance);
			if (FAILED(hr))
			{
				throw  "X3DAudio Initialization failed";
			}

		}
		catch (const char* e)
		{
			std::cerr << e << std::endl;
		}
		//�R�[���o�b�N���X�g�ɒǉ�
		pXAudio2->RegisterForCallbacks(&engineCallBack);
	}

	Audio::Audio()
	{
		CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		SecureZeroMemory(&x3DInstance, sizeof(x3DInstance));
		SecureZeroMemory(&voiceDetails, sizeof(voiceDetails));
		SecureZeroMemory(&x3DInstance, sizeof(x3DInstance));
		Create();
	}

	Audio::~Audio()
	{
		//�ǉ������R�[���o�b�N���X�g���폜
		//���X�g���̍ŏ��̃C���X�^���X�̂ݍ폜
		pXAudio2->UnregisterForCallbacks(&engineCallBack);
		//�}�X�^�[�{�C�X�j��
		if (pMaster != nullptr)
		{
			pMaster->DestroyVoice();
			pMaster = nullptr;
		}
		CoUninitialize();		//COM�I�u�W�F�N�g�̔j��
	}

	IXAudio2* Audio::GetXAudio2() const
	{
		return pXAudio2.p;
	}

	IXAudio2MasteringVoice* Audio::GetMaster() const
	{
		return pMaster;
	}

	X3DAUDIO_HANDLE& Audio::Get3DInstance()
	{
		return x3DInstance;
	}

	XAUDIO2_VOICE_DETAILS& Audio::GetVoiceDetails()
	{
		return voiceDetails;
	}

	Audio* Audio::GetInstance()
	{
		static std::unique_ptr<Audio> inst(new Audio());
		return inst.get();
	}
}
