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
	return mouse.GetMousePosClient();
}

POINT Mouse::GetMousePosCenter()
{
	return mouse.GetMousePosClientCenter();
}

void Mouse::SetMousePos(int x, int y)
{
	mouse.SetMousePos(x,y);
}

void Mouse::DrawCursor(bool enable)
{
	mouse.DrawCursor(enable);
}
