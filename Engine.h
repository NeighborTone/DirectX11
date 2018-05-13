#pragma once
#include "System.h"
#include "Direct3D.h"

/*! @class Engine
*   @brief �`�擙�̍X�V������e�n���h�����܂Ƃ߂܂�
*   @detail �R���X�g���N�^�Ő�������܂��B���O�A���A�����A�S��ʂɂ��邩�w�肵�Ă�������
*/
class Engine final
{
public:

	/*!
	* @brief �R���X�g���N�^�ł��B
	* @param (title) �E�B���h�E�^�C�g��
	* @param (width) �E�B���h�E��
	* @param (height) �E�B���h�E����
	* @param (isFullScreen) �t���X�N���[���ɂ��邩(false�őS���)
	*/
	Engine(std::string WindowTitle,int width,int height, bool isFullScreen);
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
	/*!
	* @brief �E�B���h�E�̕���Ԃ��܂�
	* @return int
	*/
	static int GetWindowWidth();
	/*!
	* @brief �E�B���h�E�̍�����Ԃ��܂�
	* @return int
	*/
	static int GetWindowHeight();
private:
	static std::string title;
	static int width;
	static int height;
	static bool isFullScreen;
	static System& GetWindow();
	static Direct3D& GetDirect3D();
};

