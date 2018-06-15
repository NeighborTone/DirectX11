#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <vorbis/vorbisfile.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <memory>
#pragma comment (lib, "libogg_static.lib")
#pragma comment (lib, "libvorbis_static.lib")
#pragma comment (lib, "libvorbisfile_static.lib")

class Ogg
{
private:

	char* wavData;
	FILE* pOggFile;
	OggVorbis_File ovf;
	WAVEFORMATEX wave;
	size_t readSize;
public:
	Ogg();
	~Ogg();

	bool Load(const std::string path);
	const WAVEFORMATEX GetWav() const;
	const BYTE* GetData() const;
	const size_t GetSize() const;
};

