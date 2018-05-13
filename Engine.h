#pragma once
#include "System.h"
#include "Direct3D.h"

/*! @class Engine
*   @brief �`�擙�̍X�V������e�n���h�����܂Ƃ߂܂�
*/
class Engine final
{
public:
	Engine(std::string WindowTitle);
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

	/*!
	* @brief �E�B���h�E�n���h����Ԃ��܂�
	* @return HWND
	*/
	static HWND GetWindowHandle();
private:
	static std::string title;
	static System& GetWindow();
	static Direct3D& GetDirect3D();
};

