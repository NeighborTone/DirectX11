#pragma once
#include "../Utilitys/Vec.hpp"
#include "../Utilitys/Vertex.hpp"
#include "Material.h"
#include <DirectXMath.h>
#include <vector>

/*! @class Mesh
*   @brief ポリゴンの集まりを使用するためのクラスです
*   @detail cubeと平面が描画できます、SetTextureでリソースをセットしてください
*/
class Mesh
{
private:
	struct Constant
	{
		DirectX::XMMATRIX world;
	};

	Material material;
	Constant constant;
	ATL::CComPtr<ID3D11Buffer> vertexBuffer;
	ATL::CComPtr<ID3D11Buffer> indexBuffer;
	ATL::CComPtr<ID3D11RasterizerState> rasterizerState;
public:
	Vec3 pos;
	Vec3 angle;
	Vec3 scale;

	std::vector<Vertex> vertices;
	std::vector<UINT> indices;
	Mesh();
	~Mesh();
	/*!
	* @brief 点を作成します
	* @param (p1)	ポリゴンの初期位置
	* @param (offset) ポリゴンのオフセット値
	* @param (shouldClear) 頂点、インデックスの初期化
	* @param (forwardDirection) 法線の向き
	*/
	void CreatePoint(
		Vec3 p1,
		Vec3 offset = Vec3(0.0f, 0.0f, 0.0f),
		Vec3 forwardDirection = Vec3(0.0f, 0.0f, 1.0f),
		bool shouldClear = true
		);

	/*!
	* @brief 平面を作成します
	* @param (size)	ポリゴンのサイズ
	* @param (offset) ポリゴンの初期位置
	* @param (shouldClear) 頂点、インデックスの初期化
	* @param (leftDirection) 左方向の点
	* @param (upDirection) 上方向の点
	* @param (forwardDirection) 法線の向き
	*/
	void CreatePlane(
		Vec2 size,
		Vec3 offset = Vec3(0.0f, 0.0f, 0.0f), 
		bool shouldClear = true, 
		Vec3 leftDirection = Vec3(1.0f, 0.0f, 0.0f), 
		Vec3 upDirection = Vec3(0.0f, 1.0f, 0.0f), 
		Vec3 forwardDirection = Vec3(0.0f, 0.0f, 1.0f));
	/*!
	* @brief 直方体を作成します
	* @param (shouldClear) 頂点、インデックスの初期化
	*/
	void CreateCube(bool shouldClear = true);
	/*!
	* @brief Materialを取得します
	* @return Material&
	*/
	Material& GetMaterial();
	/*!
	* @brief 描画タイプを指定します
	* @param (cullingMode) カリングの設定,D3D11_CULL_MODE
	* @param (fillMode) メッシュの描画オプション,D3D11_FILL_MODE
	*/
	void SetDrawMode(D3D11_CULL_MODE cullingMode, D3D11_FILL_MODE fillMode);
	/*!
	* @brief 変更した頂点をパイプラインに流します
	*/
	void Apply();
	/*!
	* @brief Meshを描画します
	*/
	void Draw();
	/*!
	* @brief Meshに回転行列を指定して描画します
	* @detail これは主に物理エンジンの演算結果を得るために用います
	* @param (rota) 回転行列 
	*/
	void Draw(DirectX::XMMATRIX rota);
	/*!
	* @brief 点を描画します
	*/
	void DrawPoint();


	//アラインメント対策
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
};

