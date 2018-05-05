#include "Shader.h"

Shader::Shader(ID3D11Device* device, HWND hwnd, const char* shaderPath, const char* vertexFuncName, const char* pixelFuncName) :
	pVertex(nullptr)
	, pPixel(nullptr)
	, pLayout(nullptr)
	, pMatrixBuf(nullptr)
	, isInit(Load(device, hwnd, shaderPath, vertexFuncName, pixelFuncName))
{

}

Shader::~Shader()
{
	if (pVertex)
	{
		RELEASE(pVertex);
	}
	if (pPixel)
	{
		RELEASE(pPixel);
	}
	if (pLayout)
	{
		RELEASE(pLayout);
	}
	if (pMatrixBuf)
	{
		RELEASE(pMatrixBuf);
	}
	if (name)
	{
		delete[] name;
		name = nullptr;
	}
}

//private---------------------------------------------------------------------------------------------------------------------------------------------------
bool Shader::CreateShader(ID3D11Device* device, HWND hwnd, const char* vsPath, const char* psPath, const char* vertexFuncName, const char* pixelFuncName)
{
	HRESULT hr;
	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexBuf = nullptr;
	ID3D10Blob* psBuf = nullptr;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBuf;

	//バーテックシェーダーをコンパイル
	hr = D3DX11CompileFromFile(
		vsPath,
		NULL,
		NULL,
		vertexFuncName,
		"vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		NULL,
		&vertexBuf,
		&errorMessage,
		NULL);

	if (FAILED(hr))
	{
		if (errorMessage)
		{
			OutputShaderError(errorMessage, hwnd, vsPath);
		}
		else
		{
			MessageBox(hwnd, vsPath, "Error in Shade File", MB_OK);
		}

		return false;
	}

	//ピクセルシェーダーをコンパイル
	hr = D3DX11CompileFromFile(
		psPath,
		NULL,
		NULL,
		pixelFuncName,
		"ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		NULL,
		&psBuf,
		&errorMessage,
		NULL);

	if (FAILED(hr))
	{
		if (errorMessage)
		{
			OutputShaderError(errorMessage, hwnd, psPath);
		}
		else
		{
			MessageBox(hwnd, psPath, "Error in Shade File", MB_OK);
		}

		return false;
	}

	//バーテックスシェーダーのバッファを作る
	hr = device->CreateVertexShader(
		vertexBuf->GetBufferPointer(),
		vertexBuf->GetBufferSize(),
		NULL,
		&pVertex);
	if (FAILED(hr))
	{
		return false;
	}

	//ピクセルシェーダーのバッファを作る
	hr = device->CreatePixelShader(
		psBuf->GetBufferPointer(),
		psBuf->GetBufferSize(),
		NULL,
		&pPixel);
	if (FAILED(hr))
	{
		return false;
	}

	//シェーダに入るデータのレイアウトを設定する
	polygonLayout[0].SemanticName = "POSITION";										//シェーダー入力署名でこの要素に関連付けられているHLSLセマンティクス(ソースコード中で利用されている変数や文が正しく動作するかを判断する基準)
	polygonLayout[0].SemanticIndex = 0;													//要素のセマンティクスインデックス
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;						//要素データのデータ型
	polygonLayout[0].InputSlot = 0;															//入力アセンブラーを識別する整数値
	polygonLayout[0].AlignedByteOffset = 0;												//(省略可能)各要素間のオフセット (バイト単位)
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;			//単一の入力スロットの入力データ クラスを識別
	polygonLayout[0].InstanceDataStepRate = 0;											//バッファーの中で要素の 1 つ分進む前に、インスタンス単位の同じデータを使用して描画するインスタンスの数

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//入力レイアウトオブジェクトを作成
	hr = device->CreateInputLayout(
		polygonLayout,
		numElements,
		vertexBuf->GetBufferPointer(),
		vertexBuf->GetBufferSize(),
		&pLayout);
	if (FAILED(hr))
	{
		return false;
	}

	//作成が終わったので余分なバッファを破棄
	RELEASE(vertexBuf);
	RELEASE(psBuf);

	//マトリックスバッファの設定
	matrixBuf.ByteWidth = sizeof(MatrixBufferType);				//バッファーのサイズ (バイト単位)
	matrixBuf.Usage = D3D11_USAGE_DYNAMIC;							//バッファーで想定されている読み込みおよび書き込みの方法を識別
	matrixBuf.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//バッファーをどのようにパイプラインにバインドするかを識別
	matrixBuf.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//CPUアクセスのフラグ
	matrixBuf.MiscFlags = 0;												//リソースに使用される、あまり一般的でないその他のオプション
	matrixBuf.StructureByteStride = 0;								//構造体が構造化バッファーを表す場合、その構造体のサイズ (バイト単位) 

																	//バッファポインタを作成する
	hr = device->CreateBuffer(&matrixBuf, NULL, &pMatrixBuf);
	if (FAILED(hr))
	{
		MessageBox(hwnd, "マトリックスバッファの作成に失敗", "Error", MB_OK);
		return false;
	}

	return true;
}

void Shader::OutputShaderError(ID3D10Blob* errorMes, HWND hwnd, const char* shaderPath)
{
	char* compileError = (char*)errorMes->GetBufferPointer();
	unsigned long bufsize = errorMes->GetBufferSize();

	std::ofstream fout;
	fout.open("shader-error-txt");
	for (unsigned int i = 0; i < bufsize; ++i)
	{
		fout << compileError[i];
	}
	fout.close();

	RELEASE(errorMes);

	MessageBox(hwnd, "Error compiling shader. Check shader-error.txt for message", shaderPath, MB_OK);

}

//protected------------------------------------------------------------------------------------------------------------------------------------------------
bool Shader::Load(ID3D11Device* device, HWND hwnd, const char* shaderPath, const char* vertexFuncName, const char* pixelFuncName)
{
	//シェーダーファイルを読み込む
	bool result;
	name = new char[strlen(shaderPath) + 1]; //ヌル文字分開ける
	memcpy(name, shaderPath, strlen(shaderPath + 1));

	char vsPath[100];
	strcpy_s(vsPath, shaderPath);
	strcat_s(vsPath, ".vs");

	vsPath[strlen(shaderPath) + 4] = '\0';


	char psPath[100];
	strcpy_s(psPath, shaderPath);
	strcat_s(psPath, ".ps");

	psPath[strlen(shaderPath) + 4] = '\0';

	//頂点とピクセルの初期化
	result = CreateShader(device, hwnd, vsPath, psPath, vertexFuncName, pixelFuncName);
	return result;
}

//public-----------------------------------------------------------------------------------------------------------------------------------------------------
void Shader::Begin(ID3D11DeviceContext* context, int indexCount)
{
	//コンテキストに頂点の設定をする
	context->IASetInputLayout(pLayout);

	//レンダリングに使用する頂点シェーダとピクセルシェーダを設定する
	context->VSSetShader(pVertex, NULL, 0);
	context->PSSetShader(pPixel, NULL, 0);

	//描画開始
	context->DrawIndexed(indexCount, 0, 0);

}

void Shader::End(ID3D11DeviceContext* context)
{
	context->IASetInputLayout(NULL);
	context->VSSetShader(NULL, NULL, 0);
	context->PSSetShader(NULL, NULL, 0);
}

bool Shader::SetShaderParameters(ID3D11DeviceContext* context, ID3D11ShaderResourceView* texture)
{
	context->PSSetShaderResources(0, 1, &texture);
	return true;
}

bool Shader::SetShaderParameters(ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	//行列を変換してシェーダ用に準備
	D3DXMatrixTranspose(&world,&world);
	D3DXMatrixTranspose(&view,&view);
	D3DXMatrixTranspose(&proj,&proj);

	//定数バッファをロックして書き込み可能にする
	hr = context->Map(pMatrixBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		return false;
	}

	//定数バッファ内のデータへのポインタを取得
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	//行列を定数バッファにコピー
	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = proj;

	//定数バッファのロックを解除
	context->Unmap(pMatrixBuf, 0);

	//頂点シェーダでの定数バッファの位置を設定
	bufferNumber = 0;

	//更新された値で頂点シェーダの定数バッファを設定
	context->VSSetConstantBuffers(bufferNumber, 1, &pMatrixBuf);

	return true;
}

char* Shader::GetName()
{
	return name;
}

bool Shader::IsInit()
{
	return isInit;
}
