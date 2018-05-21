#include "wav.h"
using namespace std;

WAV::WAV()
{
	mmio = NULL;
	SecureZeroMemory(&fmt, sizeof(fmt));
	wavData.clear();
}

WAV::~WAV()
{
	Close();
}

bool WAV::Open(const std::string &path)
{
	name = path;

	//�t�@�C�����̃R�s�[(mmioOpen�ɓn��)
	mmio = mmioOpen(&name[0], NULL, MMIO_READ);

	if (!mmio)
	{
		MessageBox(NULL, "wav���J���܂���ł���", "Error", MB_OK);
		return false;
	}

	return true;
}

bool WAV::Close()
{
	if (mmioClose(mmio, MMIO_FHOPEN) == MMIOERR_CANNOTWRITE)
	{
		return false;
	}
	return true;
}

bool WAV::IsFMTChunk()
{
	fmt.ckid = mmioFOURCC('f', 'm', 't', ' ');

	if (mmioDescend(mmio, &fmt, &riff, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
	{
		MessageBox(NULL, "FMT�`�����N�����݂��܂���", "Error", MB_OK);
		return false;
	}
	return true;
}

bool WAV::IsDATAChunk()
{
	data.ckid = mmioFOURCC('d', 'a', 't', 'a');

	if (mmioDescend(mmio, &data, &riff, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
	{
		MessageBox(NULL, "DATA�`�����N�����݂��܂���", "Error", MB_OK);
		return false;
	}
	return true;
}

bool WAV::ReadToWaveFmtEx()
{
	//mmioRead�͐�������Ɠǂݍ��񂾃T�C�Y��Ԃ�
	LONG readSize = mmioRead(mmio, reinterpret_cast<HPSTR>(&wave), fmt.cksize);

	switch (readSize)
	{
		//����ȏ�ǂݍ��߂Ȃ�
	case 0:
		MessageBox(NULL, "�ǂݍ���wav�͂���ȏ�ǂݍ��߂܂���", "Error", MB_OK);
		break;
		//�t�@�C���̓ǂݎ��Ɏ��s
	case -1:
		MessageBox(NULL, "wav�͓ǂݎ��܂���ł���", "Error", MB_OK);
		break;
	default:
		if (readSize != fmt.cksize)
		{
			MessageBox(NULL, "�ǂݍ���wav�̃f�[�^�T�C�Y���قȂ�܂�", "Error", MB_OK);
			return false;
		}
		
	}

	return true;
}

bool WAV::ReadToWaveData()
{
	//WAVE�f�[�^�p�Ƀ��T�C�Y
	wavData.resize(data.cksize);

	if (mmioRead(
		mmio,
		reinterpret_cast<HPSTR>(&wavData[0]), data.cksize) != data.cksize)
	{
		MessageBox(NULL, "�ǂݍ���wav�̃f�[�^�T�C�Y���قȂ�܂�", "Error", MB_OK);
		return false;
	}
	return true;
}

bool WAV::IsWAVEHeader()
{
	riff.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	if (mmioDescend(mmio, &riff, NULL, MMIO_FINDRIFF) != MMSYSERR_NOERROR)
	{
		MessageBox(NULL, "�ǂݍ���wav�Ƀw�b�_�����݂��܂���", "Error", MB_OK);
		return false;
	}
	return true;
}
bool WAV::Load(const std::string &path)
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

	//�ǂݍ��݈ʒu��擪�ɖ߂�
	mmioAscend(mmio, &fmt, SEEK_SET);

	if (!IsDATAChunk())
	{
		return false;
	}

	if (!ReadToWaveData())
	{
		return false;
	}

	//�t�@�C�������
	Close();

	return true;
}

const std::string& WAV::GetFileName()const
{
	return name;
}

const WAVEFORMATEX& WAV::GetWaveFmtEx()const
{
	return wave;
}

const BYTE* WAV::GetWaveData()const
{
	return &wavData[0];
}

const size_t WAV::GetWaveSize()const
{
	return wavData.size();
}