#pragma once
#include <Windows.h>
#include <tchar.h>
#include <string>
#include "SystemDefs.hpp"

/*! @class System
*   @brief �E�B���h�E�𐶐����܂�
*/
class System final
{
private:
	HWND handle;
	MSG msg;
	HINSTANCE ins;
	bool Create(std::string str);
public:

	/*!
	* @brief �R���X�g���N�^�ł��B
	* @param (title) �E�B���h�E�^�C�g��
	*/
	System(std::string title);
	System(const System&) = delete;
	~System();

	/*!
	* @brief �E�B���h�E�X�V
	* @return ��肪�Ȃ����true
	*/
	bool Run();

	/*!
	* @brief �E�B���h�E�n���h�����擾���܂�
	* @return HWND
	*/
	HWND GetHwnd();
};