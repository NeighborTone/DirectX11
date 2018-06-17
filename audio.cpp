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
	//MEMO:3Dサウンドはモノラル
	dsp = { 0 };
	emitter = {0};
	emitter.pCone = NULL;
	emitter.Position = { 0, 0, 0 };
	emitter.OrientFront = { 0, 0, 1 };
	emitter.OrientTop = { 0 , 1, 0 };
	emitter.ChannelCount = INPUTCHANNELS;		//ソースのチャンネル数
	emitter.ChannelRadius = 1.0f;							//エミッタでの行列の計算のみに使用。この値は0.0f以上であることが必要
	emitter.pChannelAzimuths = emitterAzimuths;	//方位角。チャンネル半径と共に使用される。行列の計算のみに使用
	emitter.InnerRadius = 0.0f;					 //内部半径の計算に使用される値。0.0f ～ MAX_FLTの値を指定
	emitter.InnerRadiusAngle = 0.0f;			//内部角度の計算に使用される値。0.0f ～ X3DAUDIO_PI/4.0 の値を指定
	emitter.pVolumeCurve = NULL;				//ボリュームレベル距離カーブ。行列の計算にのみ使用
	emitter.pLFECurve = NULL;					//LFE ロールオフ距離カーブ
	emitter.pLPFDirectCurve = NULL;			//ローパスフィルター(LPF)ダイレクト パス係数距離カーブNULLで規定値
	emitter.pLPFReverbCurve = NULL;			//LPFリバーブパス係数距離カーブ
	emitter.pReverbCurve = NULL;				//リバーブセンドレベル距離カーブ。
	emitter.CurveDistanceScaler = FLT_MIN;	//正規化された距離カーブをユーザー定義のワールド単位にスケーリングするために、またはその効果を強調するために使用するカーブ距離スケーラ。
																//ほかの計算に影響しない。この値はFLT_MIN～FLT_MAXの範囲にする必要がある
	emitter.DopplerScaler = 1.0f;					//ドップラー偏移効果を強調するために使用するドップラー偏移スケーラー。0.0f ～ FLT_MAX の範囲内にする必要がある

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
		ErrorMessage(ogg.Load(path), "oggの読み込みに失敗しました", "Error");
	}
	else if (std::equal(path.begin() + path.find("."), path.end(), str_wav.begin()))
	{
		fileType = WAVE;
		ErrorMessage(wav.Load(path.c_str()), "wavの読み込みに失敗しました", "Error");
	}
	else
	{
		ErrorMessage("不正なファイルかサポート外のフォーマットです", "Error");
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
	buf.Flags = XAUDIO2_END_OF_STREAM;	//このバッファの後にデータがないことをソースボイスに伝える
	buf.LoopCount = loopNum;	//ループ回数を指定。デフォルトで無限ループにしておく
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
	if (fileType == WAVE)
	{
		buf.AudioBytes = wav.GetWaveByteSize();
		buf.pAudioData = wav.GetWaveData();
	}
	else if (fileType == OGG)
	{
		buf.AudioBytes = ogg.GetWaveByteSize();	//オーディオ データのサイズ (バイト単位) 
		buf.pAudioData = ogg.GetWaveData();	//オーディオ データへのポインター
	}
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.LoopCount = 0;	//ループ回数を指定。
	buf.LoopBegin = 0;	//ループされる領域の最初のサンプル
	buf.LoopLength = 0;
	buf.pContext = nullptr;
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
	GetState();
	UINT isPlay = xstate.BuffersQueued;	//再生中なら0以外が返る
	if (pSource && isPlay != 0)
	{
		pSource->Stop(0);
	}
}
void SoundSource::Stop() const
{
	if (pSource)
	{
		pSource->Stop(0);							//一旦停止
		pSource->FlushSourceBuffers();			//ボイスキューを削除(再生位置を戻すため)
		pSource->SubmitSourceBuffer(&buf, nullptr);	//Sourceに音源の情報を送る
	}

}

void SoundSource::UpData()
{
	//MEMO : まだよくわからん
	dsp.SrcChannelCount = INPUTCHANNELS;		//ソース チャンネルの数。X3DAudioCalculateを呼び出す前にエミッタチャンネルの数に初期化する必要がある。
	dsp.DstChannelCount = Engine::GetSoundSystem().GetNumChannels();		//デスティネーション チャンネルの数。X3DAudioCalculateを呼び出す前に最終ミックスチャンネルの数に初期化する必要がある。
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
	//作成したいエフェクトの種類を設定(FXEQ,FXReverb,FXMasteringLimiter,FXEcho)
	IUnknown* effect;
	CreateFX(__uuidof(FXEQ), &effect);
	XAUDIO2_EFFECT_DESCRIPTOR desc;
	desc.InitialState = TRUE;
	desc.OutputChannels = wav.GetWaveFmtEx().nChannels;      // 出力チャンネル数
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

void SoundSource::SetSimpleReverb(SimpleReverb_DESC& reverb_desc)
{
	//作成したいエフェクトの種類を設定(FXEQ, FXReverb, FXMasteringLimiter, FXEcho)
	IUnknown* effect;

	CreateFX(__uuidof(FXReverb), &effect);
	XAUDIO2_EFFECT_DESCRIPTOR desc;
	desc.InitialState = TRUE;
	desc.OutputChannels = wav.GetWaveFmtEx().nChannels;      // 出力チャンネル数
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

void SoundSource::SetReverb(Reverb_DESC& reverb_desc)
{
	IUnknown* effect;
	XAudio2CreateReverb(&effect);
	XAUDIO2_EFFECT_DESCRIPTOR desc;
	desc.InitialState = TRUE;
	desc.OutputChannels = wav.GetWaveFmtEx().nChannels;      // 出力チャンネル数
	desc.pEffect = effect; // エフェクトへのポインタ

	XAUDIO2_EFFECT_CHAIN chain;
	chain.pEffectDescriptors = &desc; // Descriptorへのポインタ、複数個接続する場合は配列の先頭
	chain.EffectCount = 1;     // Descriptorがいくつあるのか

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


	//セットする
	pSource->SetEffectParameters(0, &rev, sizeof(XAUDIO2FX_REVERB_PARAMETERS));
}

void SoundSource::SetDelay(Delay_DESC & delay_desc)
{
	//作成したいエフェクトの種類を設定(FXEQ, FXReverb, FXMasteringLimiter, FXEcho)
	IUnknown* effect;
	CreateFX(__uuidof(FXEcho), &effect);
	XAUDIO2_EFFECT_DESCRIPTOR desc;
	desc.InitialState = TRUE;
	desc.OutputChannels = wav.GetWaveFmtEx().nChannels;      // 出力チャンネル数
	desc.pEffect = effect; // エフェクトへのポインタ

	XAUDIO2_EFFECT_CHAIN chain;
	chain.pEffectDescriptors = &desc; // Descriptorへのポインタ、複数個接続する場合は配列の先頭
	chain.EffectCount = 1;     // Descriptorがいくつあるのか

	pSource->SetEffectChain(&chain);
	effect->Release();

	FXECHO_PARAMETERS delay;
	delay.WetDryMix = delay_desc.WetDryMix;
	delay.Feedback = delay_desc.Feedback;
	delay.Delay = delay_desc.DelayTime;

	//セットする
	pSource->SetEffectParameters(0, &delay, sizeof(FXECHO_PARAMETERS));
}

void SoundSource::SetLimiter(Limiter_DESC& limiter_desc)
{
	//作成したいエフェクトの種類を設定(FXEQ, FXReverb, FXMasteringLimiter, FXEcho)
	IUnknown* effect;
	CreateFX(__uuidof(FXMasteringLimiter), &effect);
	XAUDIO2_EFFECT_DESCRIPTOR desc;
	desc.InitialState = TRUE;
	desc.OutputChannels = wav.GetWaveFmtEx().nChannels;      // 出力チャンネル数
	desc.pEffect = effect; // エフェクトへのポインタ

	XAUDIO2_EFFECT_CHAIN chain;
	chain.pEffectDescriptors = &desc; // Descriptorへのポインタ、複数個接続する場合は配列の先頭
	chain.EffectCount = 1;     // Descriptorがいくつあるのか

	pSource->SetEffectChain(&chain);
	effect->Release();

	FXMASTERINGLIMITER_PARAMETERS limiter;
	limiter.Release = limiter_desc.Release;
	limiter.Loudness = limiter_desc.Loudness;

	//セットする
	pSource->SetEffectParameters(0, &limiter, sizeof(FXMASTERINGLIMITER_PARAMETERS));
}

void SoundSource::SetMultiEffecter(Equalizer_DESC& eq_desc, Reverb_DESC& reverb_desc, Delay_DESC& delay_desc, Limiter_DESC& limiter_desc)
{
	//作成したいエフェクトの種類を設定(FXEQ, FXReverb, FXMasteringLimiter, FXEcho)
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
	chain.pEffectDescriptors = &desc[0]; // Descriptorへのポインタ、複数個接続する場合は配列の先頭
	chain.EffectCount = 4;     // Descriptorがいくつあるのか

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

	//セットする
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

	//XAudio2の初期化
	hr = XAudio2Create(&pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(NULL, "XAudio2の初期化に失敗しました", "Error", MB_OK);
		return false;
	}
	
	listener.Position = { 0,0,0 };	//XMFLOTA3
	//以下二つは直交にしなければならない
	listener.OrientFront = { 0,0,1 };	//前方方向の定義
	listener.OrientTop = { 0,1,0 };	//上方向の定義
	listener.pCone = nullptr;				//NULLは全方向性と同じ
	listener.Velocity = { 0,0,0 };	//ドップラー効果に用いるPositionには影響しない

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

	//3Dインターフェースの初期化
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
		MessageBox(NULL, "ソースボイスの追加に失敗しました", "Error", MB_OK);
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
		MessageBox(NULL, "ソースボイスの追加に失敗しました", "Error", MB_OK);
		return false;
	}
	return true;
}
