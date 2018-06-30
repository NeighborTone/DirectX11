#pragma once
#include "WavDecoder.h"
#include "OggDecoder.h"

namespace SoundEngine 
{
	class PCM
	{
	public:
		PCM() = default;
		virtual ~PCM() = default;
		virtual bool Load(const std::string &path) = 0;
		virtual const WAVEFORMATEX& GetWaveFmtEx() const = 0;	//WAVE �t�H�[�}�b�g���擾
		virtual const BYTE* GetWaveData() const = 0;			//Wave �f�[�^���擾
		virtual const size_t	GetWaveByteSize() const = 0;	//Wave �f�[�^�̃T�C�Y���擾
	};

	class Wav : public PCM
	{
	private:
		WavDecoder wav;
	public:
		Wav(const std::string& path);
		bool Load(const std::string& path) override;
		const WAVEFORMATEX& GetWaveFmtEx() const override;
		const BYTE* GetWaveData() const override;
		const size_t GetWaveByteSize() const override;
	};

	class Ogg : public PCM
	{
	private:
		OggDecoder ogg;
	public:
		Ogg(const std::string& path);
		bool Load(const std::string& path) override;
		const WAVEFORMATEX& GetWaveFmtEx() const override;
		const BYTE* GetWaveData() const override;
		const size_t GetWaveByteSize() const override;
	};
}