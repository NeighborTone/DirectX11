#pragma once
#include <string>
#include "Texture.h"

/*! @class Material
*   @brief  シェーダーを使用するためのクラスです
*   @detail このクラスは基本的に単体では使用しません。MeshやSpriteで使用します
*/
class Material
{
public:
	struct ConstantBuffer
	{
		ConstantBuffer() :ptr(nullptr), buffer(nullptr) {};
		void* ptr;
		ATL::CComPtr<ID3D11Buffer> buffer;
	};
private:
	Texture* textures[10];
	ConstantBuffer constbuff[10];
	ATL::CComPtr<ID3D11VertexShader>vertexShader;
	ATL::CComPtr<ID3D11PixelShader>pixelShader;
	ATL::CComPtr<ID3D11InputLayout>inputLayout;

	void 	Initialize();
	static void 	CompileShader(const std::string &source, const char* const entryPoint, const char* const shaderModel, ID3DBlob** out);
public:
	Material();
	~Material();
	Material(const char* const filePath);
	Material(const std::string &source);
	/*!
	* @brief シェーダーファイルを読み込みます
	* @param (filePath) シェーダーファイルのパス
	* @detail 内部でシェーダーのコンパイルを行います
	*/
	void 	Load(const char* const filePath);
	/*!
	* @brief マテリアルを作成します
	* @param (source) シェーダーのソース
	* @detail 内部でシェーダーのコンパイルを行います
	*/
	void 	Create(const std::string &source);
	/*!
	* @brief シェーダーで使うコンスタントバッファーを設定します
	* @param (slot) 設定する番号
	* @param (cbuffer) コンスタントバッファーのポインタ(色とか)
	* @param (size) バッファの大きさ
	*/
	void 	SetBuffer(int slot, void* cbuffer, size_t size);
	/*!
	* @brief シェーダーで使うテクスチャーを設定します
	* @param (slot) 設定する番号
	* @param (texture)  テクスチャーのポインタ
	*/
	void 	SetTexture(int slot, Texture* texture);
	/*!
	* @brief シェーダーをグラフィックスAPIに設定します
	*/
	void 	Attach();

};
