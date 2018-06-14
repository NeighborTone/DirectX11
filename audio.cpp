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

void SoundSource::SetEQ(EQ_DESC& eq_desc)
{
	//作成したいエフェクトの種類を設定(FXEQ,FXReverb,FXMasteringLimiter,FXEcho)
	IUnknown* effect;
	CreateFX(__uuidof(FXEQ), &effect);
	XAUDIO2_EFFECT_DESCRIPTOR desc;
	desc.InitialState = TRUE;
	desc.OutputChannels = 2;      // 出力チャンネル数
	desc.pEffect = effect; // エフェクトへのポインタ

	XAUDIO2_EFFECT_CHAIN chain;
	chain.pEffectDescriptors = &desc; // Descriptorへのポインタ、複数個接続する場合は配列の先頭
	chain.EffectCount = 1;     // Descriptorがいくつあるのか

	pSource->SetEffectChain(&chain);
	effect->Release();

	//イコライザー
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
	//セットする
	pSource->SetEffectParameters(0, &eq, sizeof(FXEQ_PARAMETERS));


}

void SoundSource::SetReverb(REVERB_DESC& reverb_desc)
{
	//作成したいエフェクトの種類を設定(FXEQ, FXReverb, FXMasteringLimiter, FXEcho)
	IUnknown* effect;
	CreateFX(__uuidof(FXEQ), &effect);
	XAUDIO2_EFFECT_DESCRIPTOR desc;
	desc.InitialState = TRUE;
	desc.OutputChannels = 2;      // 出力チャンネル数
	desc.pEffect = effect; // エフェクトへのポインタ

	XAUDIO2_EFFECT_CHAIN chain;
	chain.pEffectDescriptors = &desc; // Descriptorへのポインタ、複数個接続する場合は配列の先頭
	chain.EffectCount = 1;     // Descriptorがいくつあるのか

	pSource->SetEffectChain(&chain);
	effect->Release();

	//リバーブ
	FXREVERB_PARAMETERS reverb;
	reverb.Diffusion = reverb_desc.Diffusion;
	reverb.RoomSize = reverb_desc.RoomSize;

	//セットする
	pSource->SetEffectParameters(0, &reverb, sizeof(FXREVERB_PARAMETERS));
}

void SoundSource::SetDelay(EffectParameters::DELAY_DESC & delay_desc)
{
	//作成したいエフェクトの種類を設定(FXEQ, FXReverb, FXMasteringLimiter, FXEcho)
	IUnknown* effect;
	CreateFX(__uuidof(FXEcho), &effect);
	XAUDIO2_EFFECT_DESCRIPTOR desc;
	desc.InitialState = TRUE;
	desc.OutputChannels = 2;      // 出力チャンネル数
	desc.pEffect = effect; // エフェクトへのポインタ

	XAUDIO2_EFFECT_CHAIN chain;
	chain.pEffectDescriptors = &desc; // Descriptorへのポインタ、複数個接続する場合は配列の先頭
	chain.EffectCount = 1;     // Descriptorがいくつあるのか

	pSource->SetEffectChain(&chain);
	effect->Release();

	FXECHO_PARAMETERS delay;
	delay.WetDryMix = delay_desc.WetDryMix;
	delay.Feedback = delay_desc.Feedback;
	delay.WetDryMix = delay_desc.Delay;

	//セットする
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
