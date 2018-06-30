#pragma once
#include "../Engine.h"
#include "../Utilitys/Vec.hpp"

/*! @class Camera
*   @brief アプリケーションの3D空間を映します
*   @detail このクラスは独立して使用します。インスタンス化と更新処理を呼ばないと機能しません
*/
class Camera : public System::Proceedable
{
public:
	Vec3 pos;
	Vec3 angle;
	Float4 color;

	Camera();
	~Camera();

	/*!
	*   @brief カメラが映す奥行きを設定します
	*   @param (fieldOfView) 視野(ディグリー単位)
	*   @param (nearClip) 絵が見え始める距離
	*   @param (farClip)  絵が見えなくなる距離
	*/
	void SetPerspective(float fieldOfView, float nearClip, float farClip);
	
	/*!
	*   @brief 正射影変換を行います。画面の奥行きの概念をなくした射影変換です
	*   @param (size) クライアント領域のサイズ基本的には1を指定します
	*   @param (nearClip) 絵が見え始める距離
	*   @param (farClip)  絵が見えなくなる距離
	*   @detail 
	*/
	void SetOrthographic(float size, float nearClip, float farClip);
	/*!
	*   @brief 深度テストを設定します(ClearDepthStencilViewを行います)
	*   @param (enable) 有効にするか
	*/
	void SetDepthTest(bool enable);
	/*!
	*   @brief カメラを更新します
	*   @param (clearBack) 背景を塗りつぶすか
	*/
	void Run(bool clearBack);

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
	
	ATL::CComPtr<ID3D11RenderTargetView> renderTargetView;
	ATL::CComPtr<ID3D11DepthStencilView> depthStencilView;
	ATL::CComPtr<ID3D11Texture2D>			 renderTexture;
	ATL::CComPtr<ID3D11Texture2D>			 depthTexture;
	ATL::CComPtr<ID3D11Buffer>				 constantBuffer;
	
	bool Create();
	void OnProceed(HWND, UINT message, WPARAM, LPARAM) override;
public:
	Constant constant;
};

