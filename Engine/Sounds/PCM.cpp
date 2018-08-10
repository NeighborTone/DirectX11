#include <iostream>
#include "PCM.h"
namespace SoundEngine
{
	Wav::Wav(const std::string & path)
	{
		try
		{
			if (!Load(path.c_str()))
				throw "Wav Failed to load";
		}
		catch(const char* e)
		{
			std::cerr << e << std::endl;
		}
	}

	bool Wav::Load(const std::string& path)
	{
		return wav.Load(path.c_str());
	}

	const WAVEFORMATEX& Wav::GetWaveFmtEx() const
	{
		return wav.GetWaveFmtEx();
	}

	const BYTE* Wav::GetWaveData() const
	{
		return wav.GetWaveData();
	}

	const size_t Wav::GetWaveByteSize() const
	{
		return wav.GetWaveByteSize();
	}

	const UINT64 Wav::GetSample() const
	{
		return wav.GetWaveFmtEx().nSamplesPerSec;
	}

	Ogg::Ogg(const std::string& path)
	{
		try
		{
			if (!Load(path.c_str()))
				throw "Ogg Failed to load";
		}
		catch (const char* e)
		{
			std::cerr << e << std::endl;
		}
	}
	bool Ogg::Load(const std::string& path)
	{
		return ogg.Load(path);
	}
	const WAVEFORMATEX& Ogg::GetWaveFmtEx() const
	{
		return ogg.GetWaveFmtEx();
	}
	const BYTE* Ogg::GetWaveData() const
	{
		return ogg.GetWaveData();
	}
	const size_t Ogg::GetWaveByteSize() const
	{
		return ogg.GetWaveByteSize();
	}
	const UINT64 Ogg::GetSample() const
	{
		return ogg.GetWaveFmtEx().nSamplesPerSec;
	}
}