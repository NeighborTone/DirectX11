#include "Pad.h"

void Pad::Run()
{
	pad.UpDate();
}

bool Pad::Down(const Button& key, const NO& no)
{
	return pad.Push((int)key, (int)no);
}

bool Pad::On(const Button& key, const NO& no)
{

	return pad.Press((int)key, (int)no);
}

bool Pad::Free(const Button& key, const NO& no)
{
	return pad.Free((int)key, (int)no);
}

bool Pad::Pull(const Button& key, const NO& no)
{
	return pad.Pull((int)key, (int)no);
}

bool Pad::StickOn(const Stick& key, const NO& no)
{
	if ((BYTE)key == pad.GetPovPosition((int)no))
	{
		return true;
	}

	return false;
}
