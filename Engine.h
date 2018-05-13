#pragma once
#include "System.h"
#include "Direct3D.h"

/*! @class Engine
*   @brief �`�擙�̍X�V������e�n���h�����܂Ƃ߂܂�
*/
class Engine final
{
public:
	Engine();
	~Engine();
	/*!
	* @brief COM�̏��������s���܂��B��������s����܂���
	*/
	static void COMInitialize();
	/*!
	* @brief �e�@�\�̍X�V�������s���܂�
	* @return ��肪�Ȃ����true��Ԃ��܂�
	*/
	static bool Run();
private:
	static System& GetWindow();
};

