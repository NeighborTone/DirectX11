#pragma once
#include <Xaudio2fx.h>
#include <XAPOFX.h>
#include "SoundSource.h"

//仮実装なので簡単なことしかできない
namespace SoundEngine
{
	class SoundEffect
	{
	private:
		struct Equalizer_DESC
		{
			float Bandwidth0;			//0.1f~2.0f
			float Bandwidth1;			//
			float Bandwidth2;			//
			float Bandwidth3;			//
			float FrequencyCenter0;		//20.0f~20000.0f
			float FrequencyCenter1;		//
			float FrequencyCenter2;		//
			float FrequencyCenter3;		//

			//0(低い)~3(高い)
			float Gain0;				//0.126f(-18dB) ~  7.94f(+18dB)
			float Gain1;				//
			float Gain2;				//
			float Gain3;				//

			Equalizer_DESC() :
				Bandwidth0(FXEQ_DEFAULT_BANDWIDTH),
				Bandwidth1(FXEQ_DEFAULT_BANDWIDTH),
				Bandwidth2(FXEQ_DEFAULT_BANDWIDTH),
				Bandwidth3(FXEQ_DEFAULT_BANDWIDTH),
				Gain0(FXEQ_DEFAULT_GAIN),
				Gain1(FXEQ_DEFAULT_GAIN),
				Gain2(FXEQ_DEFAULT_GAIN),
				Gain3(FXEQ_DEFAULT_GAIN),
				FrequencyCenter0(FXEQ_DEFAULT_FREQUENCY_CENTER_0),
				FrequencyCenter1(FXEQ_DEFAULT_FREQUENCY_CENTER_1),
				FrequencyCenter2(FXEQ_DEFAULT_FREQUENCY_CENTER_2),
				FrequencyCenter3(FXEQ_DEFAULT_FREQUENCY_CENTER_3)
			{}
		};

		struct SimpleReverb_DESC
		{
			float Diffusion;		//音の広がり(拡散量） 0.0f~1.0f 
			float	RoomSize;		//音が鳴っている施設の大きさ 0.0001f~1.0f

			SimpleReverb_DESC() :
				Diffusion(FXREVERB_DEFAULT_DIFFUSION),
				RoomSize(FXREVERB_DEFAULT_ROOMSIZE)
			{}
		};

		struct Reverb_DESC
		{
			float WetDryMix;
			UINT32 ReflectionsDelay;
			BYTE ReverbDelay;
			BYTE RearDelay;
			BYTE PositionLeft;
			BYTE PositionRight;
			BYTE PositionMatrixLeft;
			BYTE PositionMatrixRight;
			BYTE EarlyDiffusion;
			BYTE LateDiffusion;
			BYTE LowEQGain;
			BYTE LowEQCutoff;
			BYTE HighEQGain;
			BYTE HighEQCutoff;
			float RoomFilterFreq;
			float RoomFilterMain;
			float RoomFilterHF;
			float ReflectionsGain;
			float ReverbGain;
			float DecayTime;
			float Density;
			float RoomSize;

			Reverb_DESC() :
				WetDryMix(XAUDIO2FX_REVERB_DEFAULT_WET_DRY_MIX),
				ReflectionsDelay(XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_DELAY),
				ReverbDelay(XAUDIO2FX_REVERB_DEFAULT_REVERB_DELAY),
				RearDelay(XAUDIO2FX_REVERB_DEFAULT_REAR_DELAY),
				PositionLeft(XAUDIO2FX_REVERB_DEFAULT_POSITION),
				PositionRight(XAUDIO2FX_REVERB_DEFAULT_POSITION),
				PositionMatrixLeft(XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX),
				PositionMatrixRight(XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX),
				EarlyDiffusion(XAUDIO2FX_REVERB_DEFAULT_EARLY_DIFFUSION),
				LateDiffusion(XAUDIO2FX_REVERB_DEFAULT_LATE_DIFFUSION),
				LowEQGain(XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_GAIN),
				LowEQCutoff(XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_CUTOFF),
				HighEQGain(XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_GAIN),
				HighEQCutoff(XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_CUTOFF),
				RoomFilterFreq(XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_FREQ),
				RoomFilterMain(XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_MAIN),
				RoomFilterHF(XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_HF),
				ReflectionsGain(XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_GAIN),
				ReverbGain(XAUDIO2FX_REVERB_DEFAULT_REVERB_GAIN),
				DecayTime(XAUDIO2FX_REVERB_DEFAULT_DECAY_TIME),
				Density(XAUDIO2FX_REVERB_DEFAULT_DENSITY),
				RoomSize(XAUDIO2FX_REVERB_DEFAULT_ROOM_SIZE)
			{}

		};

		struct Delay_DESC
		{
			float WetDryMix;		//原音にどれくらいエフェクトの音を混ぜるか 0.0f~1.0f
			float Feedback;			//跳ね返りの強さ 0.0f~1.0f
			float DelayTime;			//ディレイタイム(ミリ秒)1.0f~2000.0f

			Delay_DESC() :
				WetDryMix(FXECHO_DEFAULT_WETDRYMIX),
				Feedback(FXECHO_DEFAULT_FEEDBACK),
				DelayTime(FXECHO_DEFAULT_DELAY)
			{}
		};

		struct Limiter_DESC
		{
			UINT32 Release;		//1~20
			UINT32 Loudness;		//1~1800

			Limiter_DESC() :
				Release(FXMASTERINGLIMITER_DEFAULT_RELEASE),
				Loudness(FXMASTERINGLIMITER_DEFAULT_LOUDNESS)
			{}
		};

	public:
		Equalizer_DESC eq;
		SimpleReverb_DESC simpleReverb;
		Reverb_DESC reverb;
		Delay_DESC delay;
		Limiter_DESC limiter;
		SoundEffect();
		~SoundEffect();
		void AttachSimpleReverb(SoundSource& source);
		void AttachEqualizer(SoundSource& source);
		void AttachDelay(SoundSource& source);
		void AttachReverb(SoundSource& source);	//何故か64bitだと正しく反映されない
		void AttachLimiter(SoundSource& source);
		void Disable(SoundSource& source);
		void Enable(SoundSource& source);

	};
}
