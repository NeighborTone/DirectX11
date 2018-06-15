#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <vorbis/vorbisfile.h>
#include <Windows.h>
#include <string>
#include <vector>

#pragma comment (lib, "libogg_static.lib")
#pragma comment (lib, "libvorbis_static.lib")
#pragma comment (lib, "libvorbisfile_static.lib")

class Ogg
{
private:
	BYTE* playBuf = nullptr;
	bool isOpened = false;
	FILE* pOggFile;
	OggVorbis_File ovf;
	WAVEFORMATEX wave;
public:
	Ogg();
	~Ogg();

	bool Load(const std::string path);
	const WAVEFORMATEX GetWav() const;
	BYTE* GetData();
	size_t GetSize();
};

