#pragma once
#include <Windows.h>
#include <tchar.h>
#include <string>


/*! @class System
*   @brief �E�B���h�E�𐶐����܂�
*/
class System final
{
private:
	int width;
	int height;
	HWND handle;
	MSG msg;
	HINSTANCE ins;
	bool Create(std::string str, int width,int height);
public:

	/*!
	* @brief �R���X�g���N�^�ł��B
	* @param (title) �E�B���h�E�^�C�g��
	* @param (width) �E�B���h�E��
	* @param (height) �E�B���h�E����
	*/
	System(std::string title, int width, int height);
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
	/*!
	* @brief �E�B���h�E�̕����擾���܂�
	* @return int
	*/
	int GetWidth();
	/*!
	* @brief �E�B���h�E�̍������擾���܂�
	* @return int
	*/
	int GetHeight();
};