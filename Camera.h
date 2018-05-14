#pragma once
#include "Engine.h"
#include "Vector.h"

/*! @class Camera
*   @brief アプリケーションの3D空間を映します
*   @detail このクラスは独立して使用します。インスタンス化と更新処理を呼ばないと機能しません
*/
class Camera : public System::Proceedable
{
public:
	Vec3 pos;
	Vec3 angles;
	Float4 color;

	Camera();
	~Camera();

	/*!
	*   @brief　カメラが映す奥行きを設定します
	*   @param (fieldOfView) 視野
	*   @param (nearClip) 絵が見え始める距離
	*   @param (farClip)  絵が見えなくなる距離
	*/
	void SetPerspective(float fieldOfView, float nearClip, float farClip);
	
	/*!
	*   @brief　正射影変換を行います
	*   @param (size) クライアント領域のサイズ
	*   @param (nearClip) 絵が見え始める距離
	*   @param (farClip)  絵が見えなくなる距離
	*   @detail 画面の奥行きの概念をなくした射影変換です
	*/
	void SetOrthographic(float size, float nearClip, float farClip);
	
	void SetDepthTest(bool isDepthTest);
	void Update();

private:
	struct Constant
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	//!深度テスト
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
	
	bool Create();
	void OnProceed(HWND, UINT message, WPARAM, LPARAM) override;
};

