#pragma once
#pragma comment(lib,"winmm.lib")
#include <Windows.h>
#include <mmsystem.h>
#include <vector>
#include <string>

namespace SoundEngine
{
	class WavDecoder
	{
	private:
		HMMIO mmio;					//MMIO �n���h��
		MMCKINFO riff;				//RIFF �`�����N
		MMCKINFO fmt;				//FMT �`�����N
		MMCKINFO data;				//DATA �`�����N
		WAVEFORMATEX wave;			//WAVE �t�H�[�}�b�g�\����	
		std::vector<BYTE> wavData;	//���y�f�[�^	
		std::string name;			//�t�@�C����

		bool Open(const std::string &path);		//WAVE �t�@�C���I�[�v��
		bool Close();							//WAVE �t�@�C���N���[�Y
		bool ReadToWaveFmtEx();					//WAVEFORMATEX �Ƀf�[�^��ǂݍ���
		bool ReadToWaveData();					//wavData �� WAVE �f�[�^��ǂݍ���
		bool IsWAVEHeader();					//WAVE �w�b�_�����邩�`�F�b�N
		bool IsFMTChunk();						//FMT �`�����N�����邩�`�F�b�N
		bool IsDATAChunk();						//DAtA �`�����N�����邩�`�F�b�N

	public:
		WavDecoder();
		~WavDecoder();

		bool Load(const std::string &path);
		const WAVEFORMATEX&	GetWaveFmtEx()const;	//WAVE �t�H�[�}�b�g���擾
		const std::string& GetFileName()const;		//�t�@�C�����̎擾
		const BYTE* GetWaveData()const;				//Wave �f�[�^���擾
		const size_t GetWaveByteSize()const;		//Wave �f�[�^�̃T�C�Y���擾
	};
}