#pragma once
#include <vorbis/vorbisfile.h>
#include <Windows.h>
#include <string>
#pragma comment (lib, "libogg_static.lib")
#pragma comment (lib, "libvorbis_static.lib")
#pragma comment (lib, "libvorbisfile_static.lib")

namespace SoundEngine
{
	class OggDecoder
	{
	private:
		OggVorbis_File ovf;
		WAVEFORMATEX fmt;
		UINT size;
		char *data;
		//ogg�����ׂ�
		bool IsOgg(const std::string path);
	public:
		OggDecoder();
		~OggDecoder();

		bool Load(const std::string &path);
		const WAVEFORMATEX&	GetWaveFmtEx() const;	// WAVE �t�H�[�}�b�g���擾
		const BYTE* GetWaveData() const;		// Wave �f�[�^���擾
		const size_t GetWaveByteSize() const;		// Wave �f�[�^�̃T�C�Y���擾
	};
}
