#pragma once
#pragma warning (disable : 4100)	//�R�[���o�b�N�֐�
#include <atlbase.h>
#include <Windows.h>
#include <xaudio2.h>
#include <x3daudio.h>

#pragma comment(lib,"xaudio2")
namespace SoundEngine
{
	class XAudio2Callback : public IXAudio2VoiceCallback
	{
	public:
		HANDLE handle;
		XAudio2Callback() :
			handle(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE))
		{ }
		virtual ~XAudio2Callback()
		{
			CloseHandle(handle);
		}
		//�X�g���[�~���O�p?
		void __stdcall OnStreamEnd() override
		{
			SetEvent(handle);
		}
		void __stdcall OnBufferStart(void* pBufferContext) override
		{
			SetEvent(handle);
		}
		//�X�^�u
		void __stdcall OnVoiceProcessingPassStart(UINT32 BytesRequired) override
		{ }
		void __stdcall OnVoiceProcessingPassEnd() override
		{ }
		void __stdcall OnBufferEnd(void* pBufferContext) override
		{ }
		void __stdcall OnLoopEnd(void* pBufferContext) override
		{ }
		void __stdcall OnVoiceError(void* pBufferContext, HRESULT Error) override
		{ }

	};

	class EngineCallBack : public IXAudio2EngineCallback
	{
	public:
		void __stdcall OnProcessingPassEnd() override {}
		void __stdcall OnProcessingPassStart() override {}
		void __stdcall OnCriticalError(HRESULT Error) override {}
	};

	class Audio
	{
	private:
		//�R�[���o�b�N
		EngineCallBack engineCallBack;
		//�쐬�t���O�A���̓`�����l���A����у{�C�X�̃T���v�����[�g�Ɋւ�����
		XAUDIO2_VOICE_DETAILS voiceDetails;
		//�C���^�[�t�F�[�X
		ATL::CComPtr<IXAudio2> pXAudio2;
		//3D�C���^�[�t�F�[�X
		X3DAUDIO_HANDLE x3DInstance;
		//�}�X�^�����O���H�C�X(�����COM�ł͂Ȃ�)
		IXAudio2MasteringVoice* pMaster;
		//�T�E���h�V�X�e���̏������A��x�������΂悢
		void Create();
		//�V���O���g��
		Audio();
	public:
		~Audio();
		IXAudio2* GetXAudio2() const;
		IXAudio2MasteringVoice* GetMaster() const;
		X3DAUDIO_HANDLE& Get3DInstance();
		XAUDIO2_VOICE_DETAILS& GetVoiceDetails();
		static Audio* GetInstance();

	};
}
