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

	HRESULT hr = 0;
	XAUDIO2_SEND_DESCRIPTOR sendDescription;
	XAUDIO2_VOICE_SENDS sendList;

	is3D = is3DSound;


	hr = Engine::GetSoundSystem().GetXAudio2()->CreateSubmixVoice(
		&subMixVoice,
		wav.GetWaveFmtEx().nChannels,
		wav.GetWaveFmtEx().nSamplesPerSec,
		XAUDIO2_VOICE_USEFILTER);
	ErrorMessage(hr, "SubMixVoiceの作成に失敗しました", "Error");

	sendDescription.Flags = 0;
	sendDescription.pOutputVoice = subMixVoice;

	sendList.pSends = &sendDescription;
	sendList.SendCount = 1;

	//ソースボイス作成
	hr = Engine::GetSoundSystem().GetXAudio2()->CreateSourceVoice(
		&pSource,
		&wav.GetWaveFmtEx(),
		0,
		XAUDIO2_DEFAULT_FREQ_RATIO,
		&voiceCallBack,
		&sendList,
		0);

	ErrorMessage(hr, "SourceVoiceの作成に失敗しました", "Error");

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
	buf.LoopCount = 0;	//ループ回数を指定。
	buf.LoopBegin = 0;	//ループされる領域の最初のサンプル
	buf.LoopLength = 0;
	pSource->SetFrequencyRatio(pitch);		//ピッチ
	pSource->SetVolume(gain);					//ゲイン
	pSource->Stop(0);									//一旦停止
	pSource->FlushSourceBuffers();				//ボイスキューを削除(再生位置を戻すため)
	pSource->SubmitSourceBuffer(&buf, nullptr);	//Sourceに音源の情報を送る
	if (FAILED(hr))
	{
		MessageBox(NULL, "音楽データの送信に失敗しました", "Error", MB_OK);
	}

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
		pSource->Stop(0);								//一旦停止
		pSource->FlushSourceBuffers();			//ボイスキューを削除(再生位置を戻すため)
		pSource->SubmitSourceBuffer(&buf, nullptr);	//Sourceに音源の情報を送る
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
	emitter.ChannelCount = voiceDetails.InputChannels;;		//ソースのチャンネル数
	emitter.ChannelRadius = 1.0f;							//エミッタでの行列の計算のみに使用。この値は0.0f以上であることが必要
	emitter.pChannelAzimuths = emitterAzimuths;	//方位角。チャンネル半径と共に使用される。行列の計算のみに使用
	emitter.InnerRadius = 1.0f;					 //内部半径の計算に使用される値。0.0f ～ MAX_FLTの値を指定
	emitter.InnerRadiusAngle = 1.0f;			//内部角度の計算に使用される値。0.0f ～ X3DAUDIO_PI/4.0 の値を指定
	emitter.pVolumeCurve = NULL;				//ボリュームレベル距離カーブ。行列の計算にのみ使用
	emitter.pLFECurve = NULL;					//LFE ロールオフ距離カーブ
	emitter.pLPFDirectCurve = NULL;			//ローパスフィルター(LPF)ダイレクト パス係数距離カーブNULLで規定値
	emitter.pLPFReverbCurve = NULL;			//LPFリバーブパス係数距離カーブ
	emitter.pReverbCurve = NULL;				//リバーブセンドレベル距離カーブ。
	emitter.CurveDistanceScaler = 10.2f;		//リスナーに聞こえる範囲
																//ほかの計算に影響しない。この値はFLT_MIN～FLT_MAXの範囲にする必要がある
	emitter.DopplerScaler = 1.0f;					//ドップラー偏移効果を強調するために使用するドップラー偏移スケーラー。0.0f ～ FLT_MAX の範囲内にする必要がある

	listener.OrientFront = DirectX::XMFLOAT3(0, 0, 1);			//前方方向の定義
	listener.OrientTop = DirectX::XMFLOAT3(0, 1, 0);				//上方向の定義
	listener.pCone = NULL;													//NULLは全方向性と同じ
	listener.Position = DirectX::XMFLOAT3(listenerPos.x, listenerPos.y, listenerPos.z);
	listener.Velocity = DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f);	//ドップラー効果に用いるPositionには影響しない
	dsp.SrcChannelCount = emitter.ChannelCount;
	dsp.DstChannelCount = deviceDetails.InputChannels;
	if (!dsp.pMatrixCoefficients)
	{
		dsp.pMatrixCoefficients = new FLOAT32[dsp.SrcChannelCount * dsp.DstChannelCount];
		memset(dsp.pMatrixCoefficients, 0, sizeof(FLOAT32) * (dsp.SrcChannelCount * dsp.DstChannelCount));
	}
	/*emitter構造体ににNULLを指定した変数によって動作が変わるので注意!!*/
	DWORD calculateFlags =
		X3DAUDIO_CALCULATE_MATRIX |						//行列係数テーブルの計算を有効にします。 
		//X3DAUDIO_CALCULATE_DELAY |						//遅延時間配列の計算を有効にします (ステレオのみ)。 
		X3DAUDIO_CALCULATE_LPF_DIRECT |				//ローパスフィルター (LPF) ダイレクトパス係数の計算を有効にします。
		//X3DAUDIO_CALCULATE_LPF_REVERB |				//LPF リバーブパス係数の計算を有効にします。 
	//	X3DAUDIO_CALCULATE_REVERB |						//リバーブセンドレベルの計算を有効にします。 
		X3DAUDIO_CALCULATE_DOPPLER |					//ドップラー偏移係数の計算を有効にします。 
		X3DAUDIO_CALCULATE_EMITTER_ANGLE;		//エミッタ/リスナーの内角の計算を有効にします。 
		//X3DAUDIO_CALCULATE_ZEROCENTER |			//中央チャンネルに無音の値を設定します。 
		//X3DAUDIO_CALCULATE_REDIRECT_TO_LFE;	//すべてのソースチャンネルの均等ミックスを低周波効果 (LFE) デスティネーションチャンネルに適用します。

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