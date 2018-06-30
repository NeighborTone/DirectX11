#include "Mouse.h"



Mouse::Mouse()
{
}


Mouse::~Mouse()
{
}

void Mouse::UpDate()
{
	mouse.Run();
}

bool Mouse::L_On()
{
	return mouse.LPush();
}

bool Mouse::R_On()
{
	return mouse.RPush();
}

POINT Mouse::GetMousePos()
{
	return mouse.GetMousePos();
}
