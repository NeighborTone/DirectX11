#pragma once
#pragma warning (disable : 4005)	//DirectX���̌x��(�}�N����d��`)
#include "System.h"
#include <d2d1_1.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <dwrite.h>
#include <wincodec.h>
#include <atlbase.h>
#include <wrl.h>


#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")


/*! @class Direct3D
*   @brief DirectX11�̏��������s���܂�
*/
class Direct3D : public System::Proceedable
{
public:

	/*!
	* @brief �R���X�g���N�^�ł��B
	* @param (isFullScreen) �t���X�N���[���ɂ��邩(false�őS���)
	*/
	Direct3D(bool isFullScreen);
	virtual ~Direct3D();
	ID3D11Device& GetDevice3D() const;
	ID3D11DeviceContext& GetContext3D() const;
	ID2D1Device& GetDevice2D() const;
	ID2D1DeviceContext& GetContext2D() const;
	IDXGISwapChain& GetSwapChain() const;
	IWICImagingFactory& GetTextureFactory() const;
	IDWriteFactory& GetTextFactory() const;
	D3D11_VIEWPORT GetViewPort() const;
	/*!
	*   @brief ��ʂ̍X�V�������s���܂�
	*/
	void UpDate();
	

private:
	//ComPtr�}�C�N���\�t�g�̃V�F�A�|�C���^
	//CComPtr�}�C�N���\�t�g�̃��j�[�N�|�C���^

	bool Create();
	bool IsCreateDevice();
	bool IsCreateBlendState();
	bool IsCreateD2D();
	void CreateSwapChain();
	void SetViewport();
	void OnProceed(HWND, UINT message, WPARAM, LPARAM) override;
	static bool isFullScreen;
	//! DirectX11�̃f�o�C�X
	ATL::CComPtr<ID3D11Device>			     device3D;
	//! DirectX11�̃f�o�C�X�R���e�L�X�g
	ATL::CComPtr<ID3D11DeviceContext>      context3D;
	//! Direct2D�̃f�o�C�X
	ATL::CComPtr<ID2D1Device>				      device2D;
	//! Direct2D�̃f�o�C�X�R���e�L�X�g
	ATL::CComPtr<ID2D1DeviceContext>        context2D;
	//! Direct2D.3D�ŕ`�悷�邽�߂̃X���b�v�`�F�[��(��ʏo��)
	ATL::CComPtr<IDXGISwapChain>			      swapChain;
	//! Direct2D�ŉ摜��`�悷�邽�߂̏���ێ�����
	ATL::CComPtr<IWICImagingFactory>  textureFactory;
	//! �t�H���g�f�[�^�[�Ȃǂ̏�ԏ���ێ�����
	Microsoft::WRL::ComPtr<IDWriteFactory> textFactory;		

	D3D11_VIEWPORT viewPort;
};
