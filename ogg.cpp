//#include "ogg.h"
//#include "Utility.hpp"
//
//void Ogg::ClearOggData(OggVorbis_File* oggFile)
//{
//	if (oggFile->datasource != nullptr)
//	{
//		ov_clear(oggFile);
//		oggFile = nullptr;
//	}
//}
//
//void Ogg::OggSeekBegin(OggVorbis_File* oggFile)
//{
//	ov_pcm_seek(oggFile, 0);
//}
//
//DWORD Ogg::GetPcmSize(OggVorbis_File* pOvf)
//{
//	vorbis_info* info;
//	ogg_int64_t sampleNum;
//	DWORD pcmBytes;
//
//	//開いているoggファイルの総サンプル数とチャンネル数を調べ
//	//必要なバイト数を取得する
//	info = ov_info(pOvf, -1);
//	sampleNum = ov_pcm_total(pOvf, -1);
//	pcmBytes = (DWORD)sampleNum * OV_PCM_BITRATE_B * info->channels;
//	return pcmBytes;
//}
//
//DWORD Ogg::GetPcmSize(const std::string path)
//{
//	OggVorbis_File ovf;
//	int error;
//	DWORD pcmBytes;
//
//	error = ov_fopen(path.c_str(), &ovf);
//	if (error != OV_SUCCESSFUL)
//	{
//		return 0;
//	}
//	pcmBytes = GetPcmSize(&ovf);
//	ov_clear(&ovf);
//	return pcmBytes;
//}
//
//bool Ogg::IsOgg(const std::string path)
//{
//	FILE* fp = nullptr;
//	char c[3];
//
//	fopen_s(&fp, path.c_str(), "rb");
//	if (fp == nullptr)
//	{
//		return false;
//	}
//	fread_s(c, sizeof(c), 3, 1, fp);
//	fclose(fp);
//
//	bool isTreu =
//		(c[0] == 'O') &&
//		(c[1] == 'g') &&
//		(c[2] == 'g');
//
//	return  isTreu;
//	
//}
//
//long Ogg::Read(OggVorbis_File* pOvf, char* pBuffer, int* bitstream)
//{
//	return ov_read(
//		pOvf,
//		pBuffer,
//		OV_BUFFER_SIZE,
//		0,	// 0ならリトルエンディアン、ビッグなら1を指定
//		2,	// WORD型のByteを指定、Windowsは2Byte
//		1,	// 符号有り
//		bitstream);
//}
//
//void Ogg::SetWaveFomatEx(WAVEFORMATEX* pWave, DWORD nSamplesPerSec, WORD wChannels, WORD wBitsPerSample)
//{
//	pWave->cbSize = 0;
//	pWave->wFormatTag = WAVE_FORMAT_PCM;
//	pWave->nChannels = wChannels;
//	pWave->nSamplesPerSec = nSamplesPerSec;
//	pWave->wBitsPerSample = wBitsPerSample;
//	pWave->nBlockAlign = pWave->wBitsPerSample * pWave->nChannels / 8;	//BitからByteへ変換
//	pWave->nAvgBytesPerSec = pWave->nSamplesPerSec * pWave->nBlockAlign;
//}
//
//Ogg::Ogg() :
//	waveSize(0)
//{
//	wave = { 0 };
//}
//
//Ogg::~Ogg()
//{
//	
//}
//
//bool Ogg::CreateOggToWavEX(OggVorbis_File * pOvf, const std::string path)
//{
//	int errorNo;
//
//	//	ファイルを開く
//	errorNo = ov_fopen(path.c_str(), pOvf);
//	if (errorNo != OV_SUCCESSFUL)
//	{
//		return false;
//	}
//
//	//	PCMの形式をある程度調べる
//	vorbis_info *info = ov_info(pOvf, -1);
//	if (info == nullptr)
//	{
//		ClearOggData(pOvf);
//	}
//
//	//wave情報をセット
//	SetWaveFomatEx(&wave, (DWORD)info->rate, (WORD)info->channels, OV_PCM_BITRATE);
//
//	return true;
//}
//
//bool Ogg::Load(const std::string path)
//{
//	FILE* pOggFile = fopen(path.c_str(), "rb");
//	if (!pOggFile)
//	{
//		throw std::runtime_error("");
//		return false;
//	}
//
//	if (ov_open(pOggFile, &ovf, NULL, 0) != 0)
//	{
//		throw std::runtime_error("");
//	}
//
//	vorbis_info* oggInfo = ov_info(&ovf, -1);
//
//	wave.wFormatTag = WAVE_FORMAT_PCM;
//	wave.nChannels = (WORD)oggInfo->channels;
//	wave.nSamplesPerSec = oggInfo->rate;
//	wave.wBitsPerSample = 16;
//	wave.nBlockAlign = (WORD)oggInfo->channels * 16 / 8;
//	wave.nAvgBytesPerSec = wave.nSamplesPerSec * wave.nBlockAlign;
//	wave.cbSize = 0;
//	wavData = new char[4096];
//	int current;
//	while (1)
//	{
//		int readSize = ov_read(
//			&ovf,
//			wavData,
//			4096,
//			0,
//			2,
//			1,
//			&current
//		);
//		if (readSize == 0)
//		{
//			break;
//		}
//	}
//	
//	waveSize = GetPcmSize(&ovf);
//	ov_clear(&ovf);
//	return true;
//}
//
//
//
//
//const WAVEFORMATEX Ogg::GetWaveFmtEx() const
//{
//	return wave;
//}
//
//
//const BYTE* Ogg::GetWaveData() const
//{
//	return (BYTE*)&wavData[0];
//}
//
//const size_t Ogg::GetWaveByteSize() const
//{
//	return waveSize;
//}
//
