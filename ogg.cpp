#include "ogg.h"

Ogg::Ogg():
	pOggFile(nullptr)
{

}

Ogg::~Ogg()
{
	// バッファ開放
	if (playBuf != nullptr)
	{
		delete[] playBuf;
		playBuf = nullptr;
	}
}

bool Ogg::Load(const std::string path)
{
	
	try {
	int err = ov_fopen(path.c_str(), &ovf);
	if (err != 0)
	{
		throw std::runtime_error("oggのファイルオープンに失敗しました。");
	}
	// Oggファイルの情報を取得
	vorbis_info* vi = ov_info(&ovf, -1);
	if (vi == nullptr)
	{
		throw std::runtime_error("oggファイルの情報取得に失敗しました。");
	}
	wave.wFormatTag = WAVE_FORMAT_PCM;
	wave.nChannels = vi->channels;
	wave.nSamplesPerSec = vi->rate;
	wave.nAvgBytesPerSec = vi->rate * vi->channels * 2;
	wave.nBlockAlign = vi->channels * 2;
	wave.wBitsPerSample = 16;
	wave.cbSize = sizeof(WAVEFORMATEX);

	// デコード後のデータサイズの取得
	DWORD decodeSize = static_cast<DWORD>(ov_pcm_total(&ovf, -1)) * vi->channels * 2;

	// 再生用バッファの作成
	playBuf = new BYTE[decodeSize];
	
	DWORD totalReadSize = 0;
	INT bitstream;
	bool result = true;

	while (totalReadSize < decodeSize) {
		// データのデコード
		LONG readSize = ov_read(
			&ovf,
			(char*)playBuf + totalReadSize,
			decodeSize - totalReadSize,
			0,
			2,
			1,
			&bitstream
		);

		if (readSize == 0) {      // ファイルの終端まで読み込んだ
			result = true;
			break;
		}
		else if (readSize < 0) {  // エラー発生
			result = false;
			break;
		}

		totalReadSize += readSize;
	}


	// ファイル読み込みの成否チェック
	if (!result)
		throw std::runtime_error("OggVorbisファイルの読み込みに失敗しました。");

	// OggVorbisファイルを閉じる
	ov_clear(&ovf);

	

}
catch (std::exception& e) {
	if (isOpened)
		ov_clear(&ovf);

	delete[] playBuf;

	MessageBoxA(NULL, e.what(), NULL, MB_OK | MB_ICONERROR);
	return false;
}



	return true;
}

const WAVEFORMATEX Ogg::GetWav() const
{
	return wave;
}

BYTE* Ogg::GetData()
{
	return playBuf;
}

size_t Ogg::GetSize()
{
	return sizeof(playBuf);
}

