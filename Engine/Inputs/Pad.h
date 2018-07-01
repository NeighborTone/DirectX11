#pragma once
#include "Direct_Input.h"
#include "Keyboard.h"
class DInput
{
public:
	enum class NO {
		P1,
		P2,
	};
	enum class Button
	{
		//仮(Logicool Gamepad F310)
		PAD_X,		//0
		PAD_A,		//1
		PAD_B,		//2
		PAD_Y,		//3
		PAD_L1,		//4
		PAD_R1,		//5
		PAD_L2,		//6
		PAD_R2,		//7
		PAD_SELECT,	//8(BACK)
		PAD_START,	//9
		PAD_L3,		//10
		PAD_R3,		//11
	
	};

	// パッドの方向キー用  
	enum class Stick
	{
		UP = 0x1,
		RIGHT = 0x2,
		DOWN = 0x4,
		LEFT = 0x8,
		UP_RIGHT = 0x1 | 0x2,
		UP_LEFT = 0x1 | 0x8,
		DOWN_RIGHT = 0x4 | 0x2,
		DOWN_LEFT = 0x4 | 0x8,
		NONE,
	};
 
	static void UpDate();

	static bool Down(const Button& key, const NO& no = DInput::NO::P1);

	static bool On(const Button& key, const NO& no = DInput::NO::P1);

	static bool Free(const Button& key, const NO& no = DInput::NO::P1);

	static bool Pull(const Button& key, const NO& no = DInput::NO::P1);

	static bool StickOn(const Stick& key, const NO& no = DInput::NO::P1);

	static long GetLStickXAngle(const NO& no = DInput::NO::P1);

	static long GetLStickYAngle(const NO& no = DInput::NO::P1);
};