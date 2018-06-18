//#pragma once
//#define _CRT_SECURE_NO_WARNINGS
//#include <vorbis/vorbisfile.h>
//#include <Windows.h>
//#include <string>
//#include <vector>
//#include <memory>
//#pragma comment (lib, "libogg_static.lib")
//#pragma comment (lib, "libvorbis_static.lib")
//#pragma comment (lib, "libvorbisfile_static.lib")
//
//class Ogg
//{
//private:
//	static constexpr int WAV_BUFFERSIZE = 1024 * 32;
//	static constexpr int WAV_NUMBUFFER = 16;
//	static constexpr int OV_BUFFER_SIZE = 4096;
//	static constexpr int OV_PCM_BITRATE = 16;
//	static constexpr int OV_PCM_BITRATE_B = OV_PCM_BITRATE / 8;
//	static constexpr int OV_SUCCESSFUL = 0;
//	
//	OggVorbis_File ovf;
//	WAVEFORMATEX wave;
//	
//	size_t waveSize;
//	//oggファイルクリア
//	void ClearOggData(OggVorbis_File* oggFile);
//	//再生を先頭に戻す
//	void OggSeekBegin(OggVorbis_File* oggFile);
//	//pcmサイズを得る
//	DWORD GetPcmSize(OggVorbis_File* pOvf);
//	//pcmサイズを得る
//	DWORD GetPcmSize(const std::string path);
//	//oggか調べる
//	bool IsOgg(const std::string path);
//	//oggを読む
//	long Read(OggVorbis_File* pOvf, char* pBuffer, int* bitstream);
//	//WAVEFORMATEXにデータを入れる
//	void SetWaveFomatEx(WAVEFORMATEX* wf_ptr, DWORD nSamplesPerSec, WORD wChannels, WORD wBitsPerSample);
//	//oggからwaveにする
//	bool CreateOggToWavEX(OggVorbis_File* pOvf, const std::string path);
//public:
//	Ogg();
//	~Ogg();
//	char* wavData;
//	bool Load(const std::string path);
//	const WAVEFORMATEX GetWaveFmtEx() const;
//	const BYTE* GetWaveData() const;
//	const size_t GetWaveByteSize() const;
//};
//
