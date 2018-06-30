#pragma once
#include "Inputs/Keyboard.h"
#include "Inputs/Mouse.h"
#include "Inputs/Pad.h"
#include "Inputs/Direct_Input.h"
#include "Graphics/System.h"
#include "Graphics/Direct3D.h"
#include "Graphics/Camera.h"
#include "Graphics/Text.h"
#include "Utilitys/FPS.h"
#include "Physics/Physics.h"

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
	Engine() = delete;
	Engine(const Engine&) = delete;
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
	* @brief �E�B���h�E�̃N���C�A���g�̈���擾���܂�
	* @return DirectX::XINT2
	*/
	static DirectX::XMINT2 GetWindowSize();
	/*!
	* @brief DirectX11�̃f�o�C�X���擾���܂�
	* @return ID3D11Device&
	*/
	static ID3D11Device& GetDXDevice3D();
	/*!
	* @brief DirectX11�̃f�o�C�X�R���e�L�X�g���擾���܂�
	* @return ID3D11DeviceContext&
	*/
	static ID3D11DeviceContext& GetDXContext3D();
	/*!
	* @brief Direct2D�̃f�o�C�X���擾���܂�
	* @return ID2D1Device&
	*/
	static ID2D1Device& GetDXDevice2D();
	/*!
	* @brief Direct2D�̃f�o�C�X�R���e�L�X�g���擾���܂�
	* @return ID2D1DeviceContext&
	*/
	static ID2D1DeviceContext& GetDXContext2D();
	/*!
	* @brief �X���b�v�`�F�C�����擾���܂�
	* @return IDXGISwapChain&
	*/
	static IDXGISwapChain& GetDXSwapChain();
	/*!
	* @brief �e�N�X�`���[�t�@�N�g���[���擾���܂�
	* @return IWICImagingFactory&
	*/
	static IWICImagingFactory& GetTextureFactory();
	/*!
	* @brief �e�L�X�g�t�@�N�g���[���擾���܂�
	* @return IDWriteFactory&
	*/
	static IDWriteFactory& GetTextFactory();
	/*!
	* @brief FPS�N���X���擾���܂�
	* @return FPS&
	*/
	static FPS& GetFps();

	/*!
	* @brief Physics�N���X���擾���܂�
	* @return Physics&
	*/
	static Physics& GetPhysics();

private:
	static std::string title;
	static int width;
	static int height;
	static bool isFullScreen;
	static System& GetWindow();
	static Direct3D& GetDirect3D();

};

