#include "ogg.h"
#include "Utility.hpp"

void Ogg::ClearOggData(OggVorbis_File* oggFile)
{
	if (oggFile->datasource != nullptr)
	{
		ov_clear(oggFile);
		oggFile = nullptr;
	}
}

void Ogg::OggSeekBegin(OggVorbis_File* oggFile)
{
	ov_pcm_seek(oggFile, 0);
}

DWORD Ogg::GetPcmSize(OggVorbis_File* pOvf)
{
	vorbis_info* info;
	ogg_int64_t sampleNum;
	DWORD pcmBytes;

	//�J���Ă���ogg�t�@�C���̑��T���v�����ƃ`�����l�����𒲂�
	//�K�v�ȃo�C�g�����擾����
	info = ov_info(pOvf, -1);
	sampleNum = ov_pcm_total(pOvf, -1);
	pcmBytes = (DWORD)sampleNum * OV_PCM_BITRATE_B * info->channels;
	return pcmBytes;
}

DWORD Ogg::GetPcmSize(const std::string path)
{
	OggVorbis_File ovf;
	int error;
	DWORD pcmBytes;

	error = ov_fopen(path.c_str(), &ovf);
	if (error != OV_SUCCESSFUL)
	{
		return 0;
	}
	pcmBytes = GetPcmSize(&ovf);
	ov_clear(&ovf);
	return pcmBytes;
}

bool Ogg::IsOgg(const std::string path)
{
	FILE* fp = nullptr;
	char c[3];

	fopen_s(&fp, path.c_str(), "rb");
	if (fp == nullptr)
	{
		return false;
	}
	fread_s(c, sizeof(c), 3, 1, fp);
	fclose(fp);

	bool isTreu =
		(c[0] == 'O') &&
		(c[1] == 'g') &&
		(c[2] == 'g');

	return  isTreu;
	
}

long Ogg::Read(OggVorbis_File* pOvf, char* pBuffer, int* bitstream)
{
	return ov_read(
		pOvf,
		pBuffer,
		OV_BUFFER_SIZE,
		0,	// 0�Ȃ烊�g���G���f�B�A���A�r�b�O�Ȃ�1���w��
		2,	// WORD�^��Byte���w��AWindows��2Byte
		1,	// �����L��
		bitstream);
}

void Ogg::SetWaveFomatEx(WAVEFORMATEX* pWave, DWORD nSamplesPerSec, WORD wChannels, WORD wBitsPerSample)
{
	pWave->cbSize = 0;
	pWave->wFormatTag = WAVE_FORMAT_PCM;
	pWave->nChannels = wChannels;
	pWave->nSamplesPerSec = nSamplesPerSec;
	pWave->wBitsPerSample = wBitsPerSample;
	pWave->nBlockAlign = pWave->wBitsPerSample * pWave->nChannels / 8;	//Bit����Byte�֕ϊ�
	pWave->nAvgBytesPerSec = pWave->nSamplesPerSec * pWave->nBlockAlign;
}

Ogg::Ogg() :
	waveSize(0)
{
	wave = { 0 };
}

Ogg::~Ogg()
{
	
}

bool Ogg::CreateOggToWavEX(OggVorbis_File * pOvf, const std::string path)
{
	int errorNo;

	//	�t�@�C�����J��
	errorNo = ov_fopen(path.c_str(), pOvf);
	if (errorNo != OV_SUCCESSFUL)
	{
		return false;
	}

	//	PCM�̌`����������x���ׂ�
	vorbis_info *info = ov_info(pOvf, -1);
	if (info == nullptr)
	{
		ClearOggData(pOvf);
	}

	//wave�����Z�b�g
	SetWaveFomatEx(&wave, (DWORD)info->rate, (WORD)info->channels, OV_PCM_BITRATE);

	return true;
}

bool Ogg::Load(const std::string path)
{
	//ogg�t�@�C���̃I�[�v��
	ErrorMessage(IsOgg(path),"�����ogg�t�@�C���ł͂���܂���","Error");
	//ov_open����waveEX�ɃZ�b�g
	ErrorMessage(CreateOggToWavEX(&ovf, path),"ogg�t�@�C���I�[�v���Ɏ��s���܂���","Error");
	//�f�R�[�h
	int total = 0;
	while (1)
	{
		total = Read(&ovf, buffer, NULL);
		if (!total)
		{
			break;
		}
	}

	waveSize = GetPcmSize(&ovf);
	ov_clear(&ovf);

	
	return true;
}




const WAVEFORMATEX Ogg::GetWaveFmtEx() const
{
	return wave;
}


const BYTE* Ogg::GetWaveData() const
{
	return (BYTE*)&buffer[0];
}

const size_t Ogg::GetWaveByteSize() const
{
	return waveSize;
}

