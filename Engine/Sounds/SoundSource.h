#pragma once
#include <string>
#include "Audio.h"
#include "PCM.h"
#include "../Utilitys/Vec.hpp"
namespace SoundEngine
{
	class SoundSource
	{
	private:
		struct SourceData
		{
			IXAudio2SubmixVoice* subMixVoice;
			//コールバック
			XAudio2Callback voiceCallBack;
			//X3DAudioCalculateの呼び出し結果を受け取る。
			X3DAUDIO_DSP_SETTINGS dsp;
			//音源格納用のバッファー
			XAUDIO2_BUFFER buf;
			//ソースヴォイス(ここに音源が格納される
			IXAudio2SourceVoice* pSource;
			//ソースの情報格納用
			XAUDIO2_VOICE_STATE xstate;
			//3Dオーディオソースの定義
			X3DAUDIO_EMITTER emitter;
			//リスナー位置
			X3DAUDIO_LISTENER listener;
			//3D音源フラグ
			bool is3D;
		}*data;
		PCM* pcm;
		void GetState();

	public:
		SoundSource();
		~SoundSource();
		void Load(const std::string path, bool is3DSound);
		void PlayBGM(int loopNum = XAUDIO2_LOOP_INFINITE, float gain = 1.0f, float pitch = 1.0f);	//ストリーミングにする予定
		void PlaySE(int loopNum = 0, float gain = 1.0f, float pitch = 1.0f);	//全部読み込む
		void Stop() const;
		void Pause();
		//ボリューム0~1と聞いたがそれ以上でも問題ないようだった
		void SetGain(float gain);
		//3Dサウンドを使うときのみ
		void UpDate3DSound(Vec3&& pos, Vec3&& listenerPos);
		//曲の終端に来たらループをやめる
		void ExitLoop() const;
		//ループ区間を設定
		void SetLoopPoint(UINT32 begine, UINT32 end);
		//
		void SetLoopNum(int num);
		void Destroy();
		UINT64 GetCurrentSampleTime();
		IXAudio2SourceVoice* GetSource();
		WORD GetChannels();
	};

}
