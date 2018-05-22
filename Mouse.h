#pragma once
#include "Direct_Input.h"
class Mouse
{
public:
	Mouse();
	~Mouse();
	static void Run();
	static bool L_On();
	static bool R_On();
	static POINT GetMousePos();
};

