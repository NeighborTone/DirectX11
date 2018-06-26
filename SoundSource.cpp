#include <iostream>
#include "SoundSource.h"
namespace SoundEngine 
{
	SoundSource::SoundSource()
	{
		data = new SourceData();
	}

	SoundSource::~SoundSource()
	{
		Destroy();
	}

	void SoundSource::Load(const std::string path, bool is3DSound)
	{
		std::string str_ogg = ".ogg";
		std::string str_wav = ".wav";

		try {
			if (std::equal(path.begin() + path.find("."), path.end(), str_ogg.begin()))
			{
				pcm = new Ogg(path);
			}
			else if (std::equal(path.begin() + path.find("."), path.end(), str_wav.begin()))
			{
				pcm = new Wav(path);
			}

			HRESULT hr = 0;
			XAUDIO2_SEND_DESCRIPTOR sendDescription = {0};
			XAUDIO2_VOICE_SENDS sendList = {0};

			data->is3D = is3DSound;

			hr = Audio::GetInstance()->GetXAudio2()->CreateSubmixVoice(
				&data->subMixVoice,
				pcm->GetWaveFmtEx().nChannels,
				pcm->GetWaveFmtEx().nSamplesPerSec,
				XAUDIO2_VOICE_USEFILTER);
			if (FAILED(hr))
			{
				throw "SubMixVoice Create failed";
			}

			sendDescription.Flags = 0;
			sendDescription.pOutputVoice = data->subMixVoice;

			sendList.pSends = &sendDescription;
			sendList.SendCount = 1;

			//ソースボイス作成
			hr = Audio::GetInstance()->GetXAudio2()->CreateSourceVoice(
				&data->pSource,
				&pcm->GetWaveFmtEx(),
				0,
				XAUDIO2_DEFAULT_FREQ_RATIO,
				&data->voiceCallBack,
				&sendList,
				0);
			if (FAILED(hr))
			{
				throw "SourceVoice Create failed";
			}
		}
		catch (const char* e)
		{
			std::cerr << e << std::endl;
		}

	}

	void SoundSource::PlayBGM(int loopNum, float gain, float pitch)
	{
		HRESULT hr = 0;
		data->buf = { 0 };
		try {
			data->buf.AudioBytes = (UINT32)pcm->GetWaveByteSize();
			data->buf.pAudioData = pcm->GetWaveData();
			data->buf.pContext = this;
			data->buf.Flags = XAUDIO2_END_OF_STREAM;	//このバッファの後にデータがないことをソースボイスに伝える
			data->buf.LoopCount = loopNum;	//ループ回数を指定。デフォルトで無限ループにしておく
			data->buf.LoopBegin = 0;
			data->pSource->SetFrequencyRatio(pitch);	//ピッチ
			data->pSource->SetVolume(gain);				//ゲイン
			hr = data->pSource->SubmitSourceBuffer(&data->buf, nullptr);	//Sourceに音源の情報を送る
			if (FAILED(hr))
			{
				throw "SourceVoice Played failed";
			}
		}
		catch (const char* e)
		{
			std::cerr << e << std::endl;
		}
		if (data->pSource)
		{
			data->pSource->Start();
		}

		while(data->xstate.BuffersQueued > 0)
		{
			//イベントがトリガーされるのを待つ
			WaitForSingleObject(data->voiceCallBack.handle, INFINITE);
		}

	}

	void SoundSource::PlaySE(int loopNum, float gain, float pitch)
	{
		HRESULT hr = 0;
		data->buf = { 0 };
		try {
			data->buf.AudioBytes = (UINT)pcm->GetWaveByteSize();
			data->buf.pAudioData = pcm->GetWaveData();
			data->buf.pContext = this;
			data->buf.Flags = XAUDIO2_END_OF_STREAM;
			data->buf.LoopCount = 0;	//ループ回数を指定。
			data->buf.LoopBegin = 0;	//ループされる領域の最初のサンプル
			data->buf.LoopLength = 0;
			data->pSource->SetFrequencyRatio(pitch);		//ピッチ
			data->pSource->SetVolume(gain);					//ゲイン
			data->pSource->Stop(0);									//一旦停止
			data->pSource->FlushSourceBuffers();				//ボイスキューを削除(再生位置を戻すため)
			hr = data->pSource->SubmitSourceBuffer(&data->buf, nullptr);	//Sourceに音源の情報を送る
			if (FAILED(hr))
			{
				throw "SourceVoice Played failed";
			}
		}
		catch (const char* e)
		{
			std::cerr << e << std::endl;
		}
		if (data->pSource)
		{
			data->pSource->Start();
		}
	}

	void SoundSource::GetState()
	{
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
		data->pSource->GetState(&data->xstate, 0);
#else
		data->pSource->GetState(&xstate);
#endif
	}

	void SoundSource::Stop() const
	{
		if (data->pSource)
		{
			data->pSource->Stop(0);								//一旦停止
			data->pSource->FlushSourceBuffers();			//ボイスキューを削除(再生位置を戻すため)
			data->pSource->SubmitSourceBuffer(&data->buf, nullptr);	//Sourceに音源の情報を送る
		}
	}

	void SoundSource::Pause()
	{
		GetState();
		UINT isPlay = data->xstate.BuffersQueued;	//再生中なら0以外が返る
		if (data->pSource && isPlay != 0)
		{
			data->pSource->Stop(0);
		}
	}

	void SoundSource::UpDate3DSound(Vec3&& pos, Vec3&& listenerPos)
	{
		if (!data->is3D)
		{
			return;
		}
		XAUDIO2_VOICE_DETAILS voiceDetails;
		XAUDIO2_VOICE_DETAILS& deviceDetails = Audio::GetInstance()->GetVoiceDetails();
		data->subMixVoice->GetVoiceDetails(&voiceDetails);

		float emitterAzimuths[1] = { 0.0f };
		data->emitter.pCone = NULL;
		data->emitter.Position = { pos.x, pos.y, pos.z };
		data->emitter.OrientFront = { 0, 0, 1 };
		data->emitter.OrientTop = { 0 , 1, 0 };
		data->emitter.ChannelCount = voiceDetails.InputChannels;	//ソースのチャンネル数
		data->emitter.ChannelRadius = 1.0f;							//エミッタでの行列の計算のみに使用。この値は0.0f以上であることが必要
		data->emitter.pChannelAzimuths = emitterAzimuths;	//方位角。チャンネル半径と共に使用される。行列の計算のみに使用
		data->emitter.InnerRadius = 1.0f;						//内部半径の計算に使用される値。0.0f ～ MAX_FLTの値を指定
		data->emitter.InnerRadiusAngle = 1.0f;			//内部角度の計算に使用される値。0.0f ～ X3DAUDIO_PI/4.0 の値を指定
		data->emitter.pVolumeCurve = NULL;				//ボリュームレベル距離カーブ。行列の計算にのみ使用
		data->emitter.pLFECurve = NULL;					//LFE ロールオフ距離カーブ
		data->emitter.pLPFDirectCurve = NULL;			//ローパスフィルター(LPF)ダイレクト パス係数距離カーブNULLで規定値
		data->emitter.pLPFReverbCurve = NULL;			//LPFリバーブパス係数距離カーブ
		data->emitter.pReverbCurve = NULL;				//リバーブセンドレベル距離カーブ。
		data->emitter.CurveDistanceScaler = 1.6f;		//リスナーに聞こえる範囲
																			//ほかの計算に影響しない。この値はFLT_MIN～FLT_MAXの範囲にする必要がある
		data->emitter.DopplerScaler = 1.0f;				//ドップラー偏移効果を強調するために使用するドップラー偏移スケーラー。0.0f ～ FLT_MAX の範囲内にする必要がある

		data->listener.OrientFront = X3DAUDIO_VECTOR(0, 0, 1);		//前方方向の定義
		data->listener.OrientTop = X3DAUDIO_VECTOR(0, 1, 0);			//上方向の定義
		data->listener.pCone = NULL;													//NULLは全方向性と同じ
		data->listener.Position = X3DAUDIO_VECTOR(listenerPos.x, listenerPos.y, listenerPos.z);
		data->listener.Velocity = X3DAUDIO_VECTOR(1.0f, 0.0f, 1.0f);	//ドップラー効果に用いるPositionには影響しない
		data->dsp.SrcChannelCount = data->emitter.ChannelCount;
		data->dsp.DstChannelCount = deviceDetails.InputChannels;
		if (!data->dsp.pMatrixCoefficients)
		{
			data->dsp.pMatrixCoefficients = new FLOAT32[data->dsp.SrcChannelCount * data->dsp.DstChannelCount];
			memset(data->dsp.pMatrixCoefficients, 0, sizeof(FLOAT32) * (data->dsp.SrcChannelCount * data->dsp.DstChannelCount));
		}
		/*emitter構造体ににNULLを指定した変数によって動作が変わるので注意!!*/
		DWORD calculateFlags =
			X3DAUDIO_CALCULATE_MATRIX |						//行列係数テーブルの計算を有効にします。 
			//X3DAUDIO_CALCULATE_DELAY |						//遅延時間配列の計算を有効にします (ステレオのみ)。 
			X3DAUDIO_CALCULATE_LPF_DIRECT |				//ローパスフィルター (LPF) ダイレクトパス係数の計算を有効にします。
			//X3DAUDIO_CALCULATE_LPF_REVERB |				//LPF リバーブパス係数の計算を有効にします。 
			//	X3DAUDIO_CALCULATE_REVERB |					//リバーブセンドレベルの計算を有効にします。 
			X3DAUDIO_CALCULATE_DOPPLER |						//ドップラー偏移係数の計算を有効にします。 
			X3DAUDIO_CALCULATE_EMITTER_ANGLE;			//エミッタ/リスナーの内角の計算を有効にします。 
			//X3DAUDIO_CALCULATE_ZEROCENTER |			//中央チャンネルに無音の値を設定します。 
			//X3DAUDIO_CALCULATE_REDIRECT_TO_LFE;		//すべてのソースチャンネルの均等ミックスを低周波効果 (LFE) デスティネーションチャンネルに適用します。

		X3DAudioCalculate(
			Audio::GetInstance()->Get3DInstance(),
			&data->listener,
			&data->emitter,
			calculateFlags,
			&data->dsp);

		data->subMixVoice->SetOutputMatrix(NULL, data->dsp.SrcChannelCount, data->dsp.DstChannelCount, data->dsp.pMatrixCoefficients);
		data->pSource->SetFrequencyRatio(data->dsp.DopplerFactor);

		XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 4.0f *data->dsp.LPFDirectCoefficient),  1.0f };

		data->subMixVoice->SetFilterParameters(&filterParameters);

	}

	void SoundSource::ExitLoop() const
	{
		data->pSource->ExitLoop();
	}

	void SoundSource::Destroy()
	{
		//ソースの再生を止めてからでないとアクセス違反が起きる
		data->pSource->Stop(0);
		if (data->dsp.pMatrixCoefficients != nullptr)
		{
			delete data->dsp.pMatrixCoefficients;
			data->dsp.pMatrixCoefficients = nullptr;
		}
		if (data != nullptr)
		{
			delete data;
			data = nullptr;
		}
		if (pcm != nullptr)
		{
			delete pcm;
			pcm = nullptr;
		}
	}

	long long SoundSource::GetCurrentSampleTime()
	{
		GetState();
		long long currentTime = -1;
		if (data->xstate.BuffersQueued > 0)
		{
			currentTime = (long long)data->xstate.SamplesPlayed;
		}
		return currentTime;
	}

	IXAudio2SourceVoice* SoundSource::GetSource()
	{
		return data->pSource;
	}

	WORD SoundSource::GetChannels()
	{
		return pcm->GetWaveFmtEx().nChannels;
	}

}