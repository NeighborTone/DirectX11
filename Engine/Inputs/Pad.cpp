#include "Pad.h"

void DInput::UpDate()
{
	pad.UpDate();
}

bool DInput::Down(const Button& key, const NO& no)
{
	return pad.Push((int)key, (int)no);
}

bool DInput::On(const Button& key, const NO& no)
{
	return pad.Press((int)key, (int)no);
}

bool DInput::Free(const Button& key, const NO& no)
{
	return pad.Free((int)key, (int)no);
}

bool DInput::Pull(const Button& key, const NO& no)
{
	return pad.Pull((int)key, (int)no);
}

bool DInput::StickOn(const Stick& key, const NO& no)
{
	if ((BYTE)key == pad.GetPovPosition((int)no))
	{
		return true;
	}

	return false;
}

long DInput::GetLStickXAngle(const NO& no)
{
	return pad.GetLStickXAngle((int)no);
}

long DInput::GetLStickYAngle(const NO & no)
{
	return pad.GetLStickYAngle((int)no);
}
