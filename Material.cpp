#include "Material.h"
#include "Engine.h"
#include "Utility.hpp"
#include <fstream>
#include <vector>

Material::Material():
	vertexShader(nullptr),
	pixelShader(nullptr),
	inputLayout(nullptr)
{
	Initialize();
}

Material::~Material()
{
}

Material::Material(const char * const filePath) :
	vertexShader(nullptr),
	pixelShader(nullptr),
	inputLayout(nullptr)
{
	Initialize();
	Load(filePath);
}

Material::Material(const std::string & source) :
	vertexShader(nullptr),
	pixelShader(nullptr),
	inputLayout(nullptr)
{
	Initialize();
	Create(source);
}

void Material::Load(const char* const filePath)
{
	//ファイル読み込み
	std::ifstream sourceFile(filePath);
	//ファイルをeofまで読み進める
	std::istreambuf_iterator<char> iterator(sourceFile);
	std::istreambuf_iterator<char> last;
	//情報を格納
	std::string source(iterator, last);
	sourceFile.close();
	//マテリアル(シェーダー)を作る
	Create(source);
	
}

void Material::Create(const std::string & source)
{
	//マテリアル(シェーダー)の作成
	HRESULT hr;
	vertexShader.Release();
	//ID3DBlobは任意の長さのデータを返すために使用される
	ATL::CComPtr<ID3DBlob> vertexShaderBlob = nullptr;  
	CompileShader(
		source,							//文字列ポインタ
		"VS",								//コンパイルターゲット(どんなシェーダーか)
		"vs_5_0",						//コンパイルオプション(シェーダーのバージョン)
		&vertexShaderBlob);		//エラーが発生したときにメッセージを格納
	
	hr = Engine::GetDXDevice3D().CreateVertexShader(
		vertexShaderBlob->GetBufferPointer(), 
		vertexShaderBlob->GetBufferSize(), 
		nullptr, 
		&vertexShader);
	ErrorMessage(hr, "バーテックスシェーダーの作成に失敗", "Error");

	pixelShader.Release();
	ATL::CComPtr<ID3DBlob> pixelShaderBlob = nullptr;
	CompileShader(
		source,
		"PS", 
		"ps_5_0", 
		&pixelShaderBlob);
	
	hr = Engine::GetDXDevice3D().CreatePixelShader(
		pixelShaderBlob->GetBufferPointer(), 
		pixelShaderBlob->GetBufferSize(),
		nullptr, 
		&pixelShader);
	ErrorMessage(hr, "ピクセルシェーダーの作成に失敗", "Error");

	inputLayout.Release();
	//シェーダに入るデータのレイアウトを設定する,入力アセンブラーステージの単一の要素についての記述
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc;
	
	inputElementDesc.push_back({
		"POSITION",										//シェーダー入力署名でこの要素に関連付けられているHLSLセマンティクス(ソースコード中で利用されている変数や文が正しく動作するかを判断する基準)
		0,														//要素のセマンティクスインデックス
		DXGI_FORMAT_R32G32B32_FLOAT,   //要素データのデータ型
		0,														//入力アセンブラーを識別する整数値
		0,														//(省略可能)各要素間のオフセット (バイト単位)
		D3D11_INPUT_PER_VERTEX_DATA,	//単一の入力スロットの入力データ クラスを識別
		0 });													//バッファーの中で要素の 1 つ分進む前に、インスタンス単位の同じデータを使用して描画するインスタンスの数
	inputElementDesc.push_back({ 
		"NORMAL", 
		0, 
		DXGI_FORMAT_R32G32B32_FLOAT, 
		0, 
		12, 
		D3D11_INPUT_PER_VERTEX_DATA, 
		0 });
	inputElementDesc.push_back({ 
		"TEXCOORD", 
		0, 
		DXGI_FORMAT_R32G32_FLOAT, 
		0,
		24, 
		D3D11_INPUT_PER_VERTEX_DATA, 
		0 });
	
	//頂点レイアウト(入力レイアウト)オブジェクトを作成しデバイスに登録
	//インプットレイアウトは、頂点情報とシェーダーの情報を集約します
	hr = Engine::GetDXDevice3D().CreateInputLayout(
		inputElementDesc.data(),
		inputElementDesc.size(),
		vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(),
		&inputLayout);
	ErrorMessage(hr, "インプットレイアウトの作成に失敗", "Error");
}

void Material::SetBuffer(int slot, void* cbuffer, size_t size)
{
	constbuff[slot].ptr = cbuffer;

	constbuff[slot].buffer.Release();
	D3D11_BUFFER_DESC constDesc;
	SecureZeroMemory(&constDesc, sizeof(constDesc));
	constDesc.ByteWidth = size;
	constDesc.Usage = D3D11_USAGE_DEFAULT;
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Engine::GetDXDevice3D().CreateBuffer(
		&constDesc, 
		nullptr, 
		&constbuff[slot].buffer);
}

void Material::SetTexture(int slot, Texture* texture)
{
	textures[slot] = texture;
}

void Material::Initialize()
{

}

void Material::CompileShader(const std::string& source, const char* const entryPoint, const char* const shaderModel, ID3DBlob** out)
{

}
void Material::Attach()
{
}
