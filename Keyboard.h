#pragma once
#include "Direct_Input.h"

class	KeyBoard
{
public:
	
	 enum class Key
	{
		KEY_UP,				//��				
		KEY_LEFT,			//��
		KEY_RIGHT,			//��			
		KEY_DOWN,			//��
		KEY_ESCAPE,			//Esc
		KEY_RETURN,	        //Enter
		KEY_LSHIFT,	        //Shift(��)	
		KEY_RSHIFT,			//Shift(�E)
		KEY_LCONTROL,	    //Ctrl(��)
		KEY_RCONTROL,	    //Ctrl(�E)					
		KEY_LMENU,			//Alt(��)
		KEY_RMENU,			//Alt(�E)
		KEY_SPACE,			//Space
		KEY_BACK,			//Back Space
		KEY_TAB,				//Tab
		KEY_1,     			//1
		KEY_2,     			//2
		KEY_3,     			//3
		KEY_4,     			//4
		KEY_5,     			//5
		KEY_6,     			//6
		KEY_7,     			//7
		KEY_8,     			//8
		KEY_9,     			//9
		KEY_0,     			//0
		KEY_Q,				//Q
		KEY_W,				//W
		KEY_E,				//E
		KEY_R,				//R
		KEY_T,				//T
		KEY_Y,				//Y
		KEY_U,				//U
		KEY_I,				//I
		KEY_O,				//O
		KEY_P,				//P
		KEY_A,				//A
		KEY_S,				//S
		KEY_D,				//D
		KEY_F,				//F
		KEY_G,				//G
		KEY_H,				//H
		KEY_J,				//J
		KEY_K,				//K
		KEY_L,				//L
		KEY_Z,				//Z
		KEY_X,				//X
		KEY_C,				//C
		KEY_V,				//V
		KEY_B,				//B
		KEY_N,				//N
		KEY_M,				//M			
		KEY_F1,				//F1
		KEY_F2,				//F2
		KEY_F3,				//F3
		KEY_F4,				//F4
		KEY_F5,				//F5
		KEY_F6,				//F6
		KEY_F7,				//F7
		KEY_F8,				//F8
		KEY_F9,				//F9
		KEY_F10,				//F10	
	};
	 KeyBoard() = delete;
	static void UpDate();
	//�����Ă���
	static bool On(const Key& key);
	//������
	static bool Down(const Key& key);
	//���͂��Ȃ�
	static bool Free(const Key& key);
	//�����ꂽ
	static bool Pull(const Key& key);
};