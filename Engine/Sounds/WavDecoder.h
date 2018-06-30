#pragma once
#pragma comment(lib,"winmm.lib")
#include <Windows.h>
#include <mmsystem.h>
#include <vector>
#include <string>

namespace SoundEngine
{
	class WavDecoder
	{
	private:
		HMMIO mmio;					//MMIO ハンドル
		MMCKINFO riff;				//RIFF チャンク
		MMCKINFO fmt;				//FMT チャンク
		MMCKINFO data;				//DATA チャンク
		WAVEFORMATEX wave;			//WAVE フォーマット構造体	
		std::vector<BYTE> wavData;	//音楽データ	
		std::string name;			//ファイル名

		bool Open(const std::string &path);		//WAVE ファイルオープン
		bool Close();							//WAVE ファイルクローズ
		bool ReadToWaveFmtEx();					//WAVEFORMATEX にデータを読み込む
		bool ReadToWaveData();					//wavData に WAVE データを読み込む
		bool IsWAVEHeader();					//WAVE ヘッダがあるかチェック
		bool IsFMTChunk();						//FMT チャンクがあるかチェック
		bool IsDATAChunk();						//DAtA チャンクがあるかチェック

	public:
		WavDecoder();
		~WavDecoder();

		bool Load(const std::string &path);
		const WAVEFORMATEX&	GetWaveFmtEx()const;	//WAVE フォーマットを取得
		const std::string& GetFileName()const;		//ファイル名の取得
		const BYTE* GetWaveData()const;				//Wave データを取得
		const size_t GetWaveByteSize()const;		//Wave データのサイズを取得
	};
}