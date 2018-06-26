#include <iostream>
#include "WavDecoder.h"

using namespace std;

namespace SoundEngine
{
	WavDecoder::WavDecoder()
	{
		mmio = NULL;
		SecureZeroMemory(&fmt, sizeof(fmt));
		wavData.clear();
	}

	WavDecoder::~WavDecoder()
	{
		Close();
	}

	bool WavDecoder::Open(const std::string& path)
	{
		name = path;

		//ファイル名のコピー(mmioOpenに渡す)
		mmio = mmioOpen(&name[0], NULL, MMIO_READ);

		if (!mmio)
		{
			MessageBox(NULL, "wavを開けませんでした", "Error", MB_OK);
			return false;
		}

		return true;
	}

	bool WavDecoder::Close()
	{
		if (mmioClose(mmio, MMIO_FHOPEN) == MMIOERR_CANNOTWRITE)
		{
			return false;
		}
		return true;
	}

	bool WavDecoder::IsFMTChunk()
	{
		fmt.ckid = mmioFOURCC('f', 'm', 't', ' ');

		if (mmioDescend(mmio, &fmt, &riff, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
		{
			MessageBox(NULL, "FMTチャンクが存在しません", "Error", MB_OK);
			return false;
		}
		return true;
	}

	bool WavDecoder::IsDATAChunk()
	{
		data.ckid = mmioFOURCC('d', 'a', 't', 'a');

		if (mmioDescend(mmio, &data, &riff, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
		{
			MessageBox(NULL, "DATAチャンクが存在しません", "Error", MB_OK);
			return false;
		}
		return true;
	}

	bool WavDecoder::ReadToWaveFmtEx()
	{
		//mmioReadは成功すると読み込んだサイズを返す
		//waveに情報を格納
		LONG readSize = mmioRead(mmio, reinterpret_cast<HPSTR>(&wave), fmt.cksize);
		switch (readSize)
		{
			//それ以上読み込めない
		case 0:
			MessageBox(NULL, "読み込んだwavはこれ以上読み込めません", "Error", MB_OK);
			break;
			//ファイルの読み取りに失敗
		case -1:
			MessageBox(NULL, "wavは読み取れませんでした", "Error", MB_OK);
			break;
		default:
			if ((unsigned)readSize != fmt.cksize)
			{
				MessageBox(NULL, "読み込んだwavのデータサイズが異なります", "Error", MB_OK);
				return false;
			}

		}

		return true;
	}

	bool WavDecoder::ReadToWaveData()
	{
		//WAVEデータ用にリサイズ
		wavData.resize(data.cksize);

		if (mmioRead(
			mmio,
			reinterpret_cast<HPSTR>(&wavData[0]), data.cksize) != (signed)data.cksize)
		{
			MessageBox(NULL, "読み込んだwavのデータサイズが異なります", "Error", MB_OK);
			return false;
		}
		return true;
	}

	bool WavDecoder::IsWAVEHeader()
	{
		riff.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		if (mmioDescend(mmio, &riff, NULL, MMIO_FINDRIFF) != MMSYSERR_NOERROR)
		{
			MessageBox(NULL, "読み込んだwavにヘッダが存在しません", "Error", MB_OK);
			return false;
		}
		return true;
	}
	bool WavDecoder::Load(const std::string &path)
	{
		if (!Open(path.c_str()))
		{
			return false;
		}

		if (!IsWAVEHeader())
		{
			return false;
		}

		if (!IsFMTChunk())
		{
			return false;
		}

		if (!ReadToWaveFmtEx())
		{
			return false;
		}

		//読み込み位置を先頭に戻す
		mmioAscend(mmio, &fmt, SEEK_SET);

		if (!IsDATAChunk())
		{
			return false;
		}

		if (!ReadToWaveData())
		{
			return false;
		}

		//ファイルを閉じる
		Close();

		return true;
	}

	const std::string& WavDecoder::GetFileName()const
	{
		return name;
	}

	const WAVEFORMATEX& WavDecoder::GetWaveFmtEx()const
	{
		return wave;
	}

	const BYTE* WavDecoder::GetWaveData()const
	{
		return &wavData[0];
	}

	const size_t WavDecoder::GetWaveByteSize()const
	{
		return wavData.size();
	}
}