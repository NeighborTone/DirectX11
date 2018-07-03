#pragma once
#include "Direct_Input.h"
class Mouse
{
public:
	Mouse();
	~Mouse();
	static void UpDate();
	static int L_On();
	static int R_On();
	static POINT GetMousePos();
	static POINT GetMousePosCenter();
	static void SetMousePos(int x, int y);
	static void DrawCursor(bool enable);
};

