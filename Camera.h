#pragma once
#include "Engine.h"
#include "Vector.h"

/*! @class Camera
*   @brief �A�v���P�[�V������3D��Ԃ��f���܂�
*   @detail ���̃N���X�͓Ɨ����Ďg�p���܂��B�C���X�^���X���ƍX�V�������Ă΂Ȃ��Ƌ@�\���܂���
*/
class Camera
{
public:
	Vec3 pos;
	Vec3 angles;
	Float4 color;

	Camera();
	~Camera();
	void SetPerspective(float fieldOfView, float nearClip, float farClip);
	void SetOrthographic(float size, float nearClip, float farClip);
	void SetDepthTest(bool isDepthTest);
	void Update();

private:
	struct Constant
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	bool isPerspective;
	float fieldOfView;
	float size;
	float nearClip;
	float farClip;
	bool isDepthTest;
	Constant constant;
	ATL::CComPtr<ID3D11RenderTargetView> renderTargetView;
	ATL::CComPtr<ID3D11DepthStencilView> depthStencilView;
	ATL::CComPtr<ID3D11Texture2D>			 renderTexture;
	ATL::CComPtr<ID3D11Texture2D>			 depthTexture;
	ATL::CComPtr<ID3D11Buffer>				 constantBuffer;
	
	void  Create();
};

