#pragma once
#pragma warning (disable : 4100)	//コールバック関数
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
		//ストリーミング用?
		void __stdcall OnStreamEnd() override
		{
			SetEvent(handle);
		}
		void __stdcall OnBufferStart(void* pBufferContext) override
		{
			SetEvent(handle);
		}
		//スタブ
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

	class Audio
	{
	private:
		class EngineCallBack : public IXAudio2EngineCallback
		{
		public:
			void __stdcall OnProcessingPassEnd() override {}
			void __stdcall OnProcessingPassStart() override {}
			void __stdcall OnCriticalError(HRESULT Error) override {}
		};
		//コールバック
		EngineCallBack engineCallBack;
		//作成フラグ、入力チャンネル、およびボイスのサンプルレートに関する情報
		XAUDIO2_VOICE_DETAILS voiceDetails;
		//インターフェース
		ATL::CComPtr<IXAudio2> pXAudio2;
		//3Dインターフェース
		X3DAUDIO_HANDLE x3DInstance;
		//マスタリングヴォイス(これはCOMではない)
		IXAudio2MasteringVoice* pMaster;
		//サウンドシステムの初期化、一度だけ作ればよい
		void Create();
		//シングルトン
		Audio();
	public:
		~Audio();
		IXAudio2* GetXAudio2() const;
		IXAudio2MasteringVoice* GetMaster() const;
		X3DAUDIO_HANDLE& Get3DInstance();
		XAUDIO2_VOICE_DETAILS& GetVoiceDetails();
		static Audio* GetInstance();
		void SetMasterGain(float gain);
	};
}
