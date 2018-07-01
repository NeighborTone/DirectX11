#pragma once
#include "Direct_Input.h"
class Mouse
{
public:
	Mouse();
	~Mouse();
	static void UpDate();
	static bool L_On();
	static bool R_On();
	static POINT GetMousePos();
	static POINT GetMousePosCenter();
	static void SetMousePos(int x, int y);
	static void DrawCursor(bool enable);
};

