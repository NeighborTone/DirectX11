#pragma once
#include "audio.h"
enum FileType
{
	WAVE,
	OGG
};
class SoundSource
{
private:
	IXAudio2SubmixVoice* subMixVoice;
	XAudio2Callback voiceCallBack;
	//X3DAudioCalculateの呼び出し結果を受け取る。
	X3DAUDIO_DSP_SETTINGS dsp;
	//音源格納用のバッファー
	XAUDIO2_BUFFER buf;
	//ソースヴォイス(ここに音源が格納される
	IXAudio2SourceVoice* pSource;
	//wavデータ格納用
	Wav wav;
	//Ogg ogg;
	//ソースの情報格納用
	XAUDIO2_VOICE_STATE xstate;
	//3Dオーディオソースの定義
	X3DAUDIO_EMITTER emitter;
	//リスナー位置
	X3DAUDIO_LISTENER listener;


	//ファイル情報
	FileType fileType;

	void GetState();

	bool is3D;

public:
	SoundSource();
	SoundSource(const std::string path, bool is3DSound);
	SoundSource(SoundSource& sound);
	~SoundSource();
	bool Load(const std::string path, bool is3DSound);
	//音源の位置などの更新
	void UpData3DSound(Vec3&& pos, Vec3&& listenerPos);
	//BGM再生に適した再生を行います
	void PlayBGM(int loopNum = XAUDIO2_LOOP_INFINITE, float gain = 1.0f, float pitch = 1.0f);
	//効果音再生に適した再生を行います
	void PlaySE(float gain = 1.0f, float pitch = 1.0f);
	//一時停止
	void Pause();
	//停止
	void Stop() const;

	//ループの終端に来たらループを停止する
	void ExitLoop() const;
	//ソース破棄
	void Destroy();

	/*初期化時にセットすることを推奨(毎フレーム呼ぶとノイズが乗るため)*/
	//MEMO : クラス化案件
	void SetEQ(EffectParameters::Equalizer_DESC& eq_desc);
	void SetSimpleReverb(EffectParameters::SimpleReverb_DESC& reverb_desc);
	void SetReverb(EffectParameters::Reverb_DESC& reverb_desc);
	void SetDelay(EffectParameters::Delay_DESC& delay_desc);
	void SetLimiter(EffectParameters::Limiter_DESC& limiter_desc);
	void SetMultiEffecter(
		EffectParameters::Equalizer_DESC& eq_desc,
		EffectParameters::Reverb_DESC& reverb_desc,
		EffectParameters::Delay_DESC& delay_desc,
		EffectParameters::Limiter_DESC& limiter_desc
	);

	//サンプル数で再生時間を返す
	int GetCurrentSampleTime();
	IXAudio2SourceVoice** GetSource();
	Wav GetWav();
	//Ogg GetOgg();
	FileType GetFileType();
};
