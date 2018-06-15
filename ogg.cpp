#include "ogg.h"

Ogg::Ogg():
	pOggFile(nullptr)
{

}

Ogg::~Ogg()
{
	// �o�b�t�@�J��
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
		throw std::runtime_error("ogg�̃t�@�C���I�[�v���Ɏ��s���܂����B");
	}
	// Ogg�t�@�C���̏����擾
	vorbis_info* vi = ov_info(&ovf, -1);
	if (vi == nullptr)
	{
		throw std::runtime_error("ogg�t�@�C���̏��擾�Ɏ��s���܂����B");
	}
	wave.wFormatTag = WAVE_FORMAT_PCM;
	wave.nChannels = vi->channels;
	wave.nSamplesPerSec = vi->rate;
	wave.nAvgBytesPerSec = vi->rate * vi->channels * 2;
	wave.nBlockAlign = vi->channels * 2;
	wave.wBitsPerSample = 16;
	wave.cbSize = sizeof(WAVEFORMATEX);

	// �f�R�[�h��̃f�[�^�T�C�Y�̎擾
	DWORD decodeSize = static_cast<DWORD>(ov_pcm_total(&ovf, -1)) * vi->channels * 2;

	// �Đ��p�o�b�t�@�̍쐬
	playBuf = new BYTE[decodeSize];
	
	DWORD totalReadSize = 0;
	INT bitstream;
	bool result = true;

	while (totalReadSize < decodeSize) {
		// �f�[�^�̃f�R�[�h
		LONG readSize = ov_read(
			&ovf,
			(char*)playBuf + totalReadSize,
			decodeSize - totalReadSize,
			0,
			2,
			1,
			&bitstream
		);

		if (readSize == 0) {      // �t�@�C���̏I�[�܂œǂݍ���
			result = true;
			break;
		}
		else if (readSize < 0) {  // �G���[����
			result = false;
			break;
		}

		totalReadSize += readSize;
	}


	// �t�@�C���ǂݍ��݂̐��ۃ`�F�b�N
	if (!result)
		throw std::runtime_error("OggVorbis�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B");

	// OggVorbis�t�@�C�������
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

