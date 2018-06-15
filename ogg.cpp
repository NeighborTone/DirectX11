#include "ogg.h"

Ogg::Ogg():
	pOggFile(nullptr),
	wavData(nullptr),
	readSize(0),
	wave(WAVEFORMATEX() = {0})
{

}

Ogg::~Ogg()
{
	ov_clear(&ovf);
	delete[] wavData;
}

bool Ogg::Load(const std::string path)
{
	//File�I�[�v��
	pOggFile = fopen(path.c_str(), "rb");
	if (!pOggFile)
	{
		throw std::runtime_error("OggFile���J���܂���");
		return false;
	}

	//Ogg�I�[�v��
	if (ov_open(pOggFile, &ovf, NULL, 0) != 0)
	{
		throw std::runtime_error("OggFile�̏��擾�Ɏ��s���܂���");
	}
	// Ogg�t�@�C���̉����t�H�[�}�b�g���
	vorbis_info* oggInfo = ov_info(&ovf, -1);

	//�\���̂ւ̏�������
	wave.wFormatTag = WAVE_FORMAT_PCM;
	wave.nChannels = oggInfo->channels;
	wave.nSamplesPerSec = oggInfo->rate;
	wave.wBitsPerSample = 16;
	wave.nBlockAlign = oggInfo->channels * 16 / 8;
	wave.nAvgBytesPerSec = wave.nSamplesPerSec * wave.nBlockAlign;
	wave.cbSize = 0;
	wavData = new char[4096];
	int current;
	readSize = ov_read(
		&ovf,
		wavData,
		4096,
		0,
		2,
		1,
		&current
		);

	
	return true;
}

const WAVEFORMATEX Ogg::GetWav() const
{
	return wave;
}

const BYTE* Ogg::GetData() const
{
	return (BYTE*)wavData;
}

const size_t Ogg::GetSize() const
{
	return readSize;
}

