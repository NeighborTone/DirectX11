#include "audio.h"
#include "Engine.h"

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
		MessageBox(NULL, "ソースボイスの作成に失敗しました", "Error", MB_OK);
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
	buf.Flags = XAUDIO2_END_OF_STREAM;	//このバッファの後にデータがないことをソースボイスに伝える
	buf.LoopCount = loopNum;	//ループ回数を指定。デフォルトで無限ループ
	buf.LoopBegin = 0;
	pSource->SetFrequencyRatio(pitch);	//ピッチ
	pSource->SetVolume(gain);					//ゲイン
	hr = pSource->SubmitSourceBuffer(&buf, nullptr);	//Sourceに音源の情報を送る
	if (FAILED(hr))
	{
		MessageBox(NULL, "音楽データの送信に失敗しました", "Error", MB_OK);
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
	buf.LoopCount = 0;	//ループ回数を指定。
	buf.LoopBegin = 0;

	
	pSource->SetFrequencyRatio(pitch);			//ピッチ
	pSource->SetVolume(gain);					//ゲイン
	pSource->Stop(0);							//一旦停止
	pSource->FlushSourceBuffers();				//ボイスキューを削除(再生位置を戻すため)
	pSource->SubmitSourceBuffer(&buf, nullptr);	//Sourceに音源の情報を送る
	
	if (pSource)
	{
		pSource->Start();
	}
}

void SoundSource::Pause()
{
	XAUDIO2_VOICE_STATE xa2state;
	pSource->GetState(&xa2state);
	auto isPlay = xa2state.BuffersQueued;	//再生中なら0以外が返る
	if (pSource && isPlay != 0)
	{
		pSource->Stop(0);
	}
}
void SoundSource::Stop()
{
	if (pSource)
	{
		pSource->Stop(0);							//一旦停止
		pSource->FlushSourceBuffers();			//ボイスキューを削除(再生位置を戻すため)
		pSource->SubmitSourceBuffer(&buf, nullptr);	//Sourceに音源の情報を送る
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

unsigned __int64 SoundSource::GetCurrentBufferTime()
{
	GetState();

	unsigned __int64 currentTime;
	currentTime = xstate.SamplesPlayed;
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
	//解放順は
	//Source→Master→XAudio2
	source.Destroy();
	//マスターボイス破棄
	if (pMaster != nullptr)
	{
		pMaster->DestroyVoice();
		pMaster = nullptr;
	}
}

SoundSystem::~SoundSystem()
{
	//マスターボイス破棄
	if (pMaster != nullptr)
	{
		pMaster->DestroyVoice();
		pMaster = nullptr;
	}
}


bool SoundSystem::Create()
{
	HRESULT hr;
	//XAudio2の初期化
	hr = XAudio2Create(&pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(NULL, "XAudio2の初期化に失敗しました", "Error", MB_OK);
		return false;
	}
	//マスターボイスの生成
	hr = pXAudio2->CreateMasteringVoice(
		&pMaster, 
		XAUDIO2_DEFAULT_CHANNELS,
		XAUDIO2_DEFAULT_SAMPLERATE,
		0,
		0,
		NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, "マスターボイスの初期化に失敗しました", "Error", MB_OK);
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
		MessageBox(NULL, "ソースボイスの追加に失敗しました", "Error", MB_OK);
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
		MessageBox(NULL, "ソースボイスの追加に失敗しました", "Error", MB_OK);
		return false;
	}
	return true;
}
