#include "SoundEffect.h"
namespace SoundEngine 
{
	SoundEffect::SoundEffect()
	{

	}

	SoundEffect::~SoundEffect()
	{
	}

	void SoundEffect::AttachSimpleReverb(SoundSource& source)
	{
		//作成したいエフェクトの種類を設定(FXEQ, FXReverb, FXMasteringLimiter, FXEcho)
		IUnknown* effect;

		CreateFX(__uuidof(FXReverb), &effect);
		XAUDIO2_EFFECT_DESCRIPTOR desc;
		desc.InitialState = TRUE;
		desc.OutputChannels = source.GetChannels();      // 出力チャンネル数
		desc.pEffect = effect; // エフェクトへのポインタ

		XAUDIO2_EFFECT_CHAIN chain;
		chain.pEffectDescriptors = &desc; //Descriptorへのポインタ、複数個接続する場合は配列の先頭
		chain.EffectCount = 1;     //Descriptorがいくつあるのか

		source.GetSource()->SetEffectChain(&chain);
		effect->Release();

		//リバーブ
		FXREVERB_PARAMETERS reverb;
		reverb.Diffusion = simpleReverb.Diffusion;
		reverb.RoomSize = simpleReverb.RoomSize;

		//セットする
		source.GetSource()->SetEffectParameters(0, &reverb, sizeof(FXREVERB_PARAMETERS));
	}

	void SoundEffect::AttachEqualizer(SoundSource& source)
	{
		//作成したいエフェクトの種類を設定(FXEQ,FXReverb,FXMasteringLimiter,FXEcho)
		IUnknown* effect;
		CreateFX(__uuidof(FXEQ), &effect);
		XAUDIO2_EFFECT_DESCRIPTOR desc;
		desc.InitialState = TRUE;
		desc.OutputChannels = source.GetChannels();      // 出力チャンネル数
		desc.pEffect = effect; // エフェクトへのポインタ

		XAUDIO2_EFFECT_CHAIN chain;
		chain.pEffectDescriptors = &desc; // Descriptorへのポインタ、複数個接続する場合は配列の先頭
		chain.EffectCount = 1;     // Descriptorがいくつあるのか

		source.GetSource()->SetEffectChain(&chain);
		effect->Release();

		//イコライザー
		FXEQ_PARAMETERS fx_eq;
		fx_eq.Bandwidth0 = eq.Bandwidth0;
		fx_eq.Bandwidth1 = eq.Bandwidth1;
		fx_eq.Bandwidth2 = eq.Bandwidth2;
		fx_eq.Bandwidth3 = eq.Bandwidth3;
		fx_eq.Gain0 = eq.Gain0;
		fx_eq.Gain1 = eq.Gain1;
		fx_eq.Gain2 = eq.Gain2;
		fx_eq.Gain3 = eq.Gain3;
		fx_eq.FrequencyCenter0 = eq.FrequencyCenter0;
		fx_eq.FrequencyCenter1 = eq.FrequencyCenter1;
		fx_eq.FrequencyCenter2 = eq.FrequencyCenter2;
		fx_eq.FrequencyCenter3 = eq.FrequencyCenter3;
		//セットする
		source.GetSource()->SetEffectParameters(0, &fx_eq, sizeof(FXEQ_PARAMETERS));
	}

	void SoundEffect::AttachDelay(SoundSource & source)
	{
		//作成したいエフェクトの種類を設定(FXEQ, FXReverb, FXMasteringLimiter, FXEcho)
		IUnknown* effect;
		CreateFX(__uuidof(FXEcho), &effect);
		XAUDIO2_EFFECT_DESCRIPTOR desc;
		desc.InitialState = TRUE;
		desc.OutputChannels = source.GetChannels();      // 出力チャンネル数
		desc.pEffect = effect; // エフェクトへのポインタ

		XAUDIO2_EFFECT_CHAIN chain;
		chain.pEffectDescriptors = &desc; // Descriptorへのポインタ、複数個接続する場合は配列の先頭
		chain.EffectCount = 1;     // Descriptorがいくつあるのか

		source.GetSource()->SetEffectChain(&chain);
		effect->Release();

		FXECHO_PARAMETERS delay_desc;
		delay_desc.WetDryMix = delay.WetDryMix;
		delay_desc.Feedback = delay.Feedback;
		delay_desc.Delay = delay.DelayTime;

		//セットする
		source.GetSource()->SetEffectParameters(0, &delay_desc, sizeof(FXECHO_PARAMETERS));
	}

	void SoundEffect::AttachReverb(SoundSource& source)
	{
		IUnknown* effect;
		XAudio2CreateReverb(&effect);
		XAUDIO2_EFFECT_DESCRIPTOR desc;
		desc.InitialState = TRUE;
		desc.OutputChannels = source.GetChannels();      // 出力チャンネル数
		desc.pEffect = effect; // エフェクトへのポインタ

		XAUDIO2_EFFECT_CHAIN chain;
		chain.pEffectDescriptors = &desc; // Descriptorへのポインタ、複数個接続する場合は配列の先頭
		chain.EffectCount = 1;     // Descriptorがいくつあるのか

		source.GetSource()->SetEffectChain(&chain);
		effect->Release();

		XAUDIO2FX_REVERB_PARAMETERS rev;
		rev.WetDryMix = reverb.WetDryMix;
		rev.ReflectionsDelay = reverb.ReflectionsDelay;
		rev.ReverbDelay = reverb.ReverbDelay;
		rev.RearDelay = reverb.RearDelay;
		rev.PositionLeft = reverb.PositionLeft;
		rev.PositionRight = reverb.PositionRight;
		rev.PositionMatrixLeft = reverb.PositionMatrixLeft;
		rev.PositionMatrixRight = reverb.PositionMatrixRight;
		rev.EarlyDiffusion = reverb.EarlyDiffusion;
		rev.LateDiffusion = reverb.LateDiffusion;
		rev.LowEQGain = reverb.LowEQGain;
		rev.LowEQCutoff = reverb.LowEQCutoff;
		rev.HighEQGain = reverb.HighEQGain;
		rev.HighEQCutoff = reverb.HighEQCutoff;
		rev.RoomFilterFreq = reverb.RoomFilterFreq;
		rev.RoomFilterMain = reverb.RoomFilterMain;
		rev.RoomFilterHF = reverb.RoomFilterHF;
		rev.ReflectionsGain = reverb.ReflectionsGain;
		rev.ReverbGain = reverb.ReverbGain;
		rev.DecayTime = reverb.DecayTime;
		rev.Density = reverb.Density;
		rev.RoomSize = reverb.RoomSize;

		//セットする
		source.GetSource()->SetEffectParameters(0, &rev, sizeof(XAUDIO2FX_REVERB_PARAMETERS));
	}

	void SoundEffect::AttachLimiter(SoundSource & source)
	{
		//作成したいエフェクトの種類を設定(FXEQ, FXReverb, FXMasteringLimiter, FXEcho)
		IUnknown* effect;
		CreateFX(__uuidof(FXMasteringLimiter), &effect);
		XAUDIO2_EFFECT_DESCRIPTOR desc;
		desc.InitialState = TRUE;
		desc.OutputChannels = source.GetChannels();      // 出力チャンネル数
		desc.pEffect = effect; // エフェクトへのポインタ

		XAUDIO2_EFFECT_CHAIN chain;
		chain.pEffectDescriptors = &desc; // Descriptorへのポインタ、複数個接続する場合は配列の先頭
		chain.EffectCount = 1;     // Descriptorがいくつあるのか

		source.GetSource()->SetEffectChain(&chain);
		effect->Release();

		FXMASTERINGLIMITER_PARAMETERS limiter_desc;
		limiter_desc.Release = limiter.Release;
		limiter_desc.Loudness = limiter.Loudness;

		//セットする
		source.GetSource()->SetEffectParameters(0, &limiter, sizeof(FXMASTERINGLIMITER_PARAMETERS));
	}

	void SoundEffect::Disable(SoundSource& source)
	{
		source.GetSource()->DisableEffect(0);
	}

	void SoundEffect::Enable(SoundSource& source)
	{
		source.GetSource()->EnableEffect(0);
	}

}