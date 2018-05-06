#include "VertexBuffer.h"
#include "SystemDefs.h"
VertexBuffer::VertexBuffer() :
	pVertices(nullptr),
	pVertexBuffer(nullptr),
	pIndexBuffer(nullptr),
	pShader(nullptr)
{
}


VertexBuffer::~VertexBuffer()
{
	Memory::SafeDeleteArr(pVertices);
	Memory::SafeRelease(pVertexBuffer);
	Memory::SafeRelease(pIndexBuffer);

}

bool VertexBuffer::Create(ID3D11Device* device, Shader* shader, float size, bool writeable)
{
	pShader = shader;

	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//クアッドに必要な情報
	vertexCount = 4;		//頂点数
	indexCount = 6;		//結んだ線の数

	pVertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	float halfSize = size / 2.0f;

	//頂点の作成とポリゴンに貼るテクスチャのUV値の設定
	pVertices[0].pos	= D3DXVECTOR3(-halfSize, -halfSize, 0.0f);
	pVertices[0].uv   = D3DXVECTOR2(0.0f, 1.0f);
	pVertices[1].pos	= D3DXVECTOR3(-halfSize, halfSize, 0.0f);
	pVertices[1].uv	= D3DXVECTOR2(0.0f, 0.0f);
	pVertices[2].pos	= D3DXVECTOR3(halfSize, halfSize, 0.0f);
	pVertices[2].uv	= D3DXVECTOR2(1.0f, 0.0f);
	pVertices[3].pos	= D3DXVECTOR3(halfSize, -halfSize, 0.0f);
	pVertices[3].uv	= D3DXVECTOR2(1.0f, 1.0f);

	//頂点を結ぶ
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	//バーテックスバッファーの設定
	vertexBufferDesc.Usage = (writeable) ?										//バッファーで想定されている読み込みおよび書き込みの方法を識別									
										D3D11_USAGE_DYNAMIC :							//GPU(読み取りのみ)とCPU(書き込みのみ)によるアクセスが可能なリソース。CPU によってフレームごとに1回以上更新されるリソースには、動的リソースを選択することが推奨されている
										D3D11_USAGE_DEFAULT;							//最も一般的な値
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;	//バッファーのサイズ(バイト単位)
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//バッファーをどのようにパイプラインにバインドするかを識別
	vertexBufferDesc.CPUAccessFlags = (writeable) ?							//CPUアクセスのフラグ
													  D3D11_CPU_ACCESS_WRITE :		//リソースをマップ可能にし、CPUがそのリソースの内容を変更できるようにする
													  0;										//CPUアクセスが必要でない場合
	vertexBufferDesc.MiscFlags = 0;													//リソースに使用される、あまり一般的でないその他のオプション
	vertexBufferDesc.StructureByteStride = 0;									//構造体が構造化バッファーを表す場合、その構造体のサイズ (バイト単位)

	//サブリソースの初期化に使用されるデータを指定
	vertexData.pSysMem = pVertices;		//初期化データへのポインター
	vertexData.SysMemPitch = 0;			//テクスチャーにある1本の線の先端から隣の線までの距離(バイト単位) 
	vertexData.SysMemSlicePitch = 0;	//1つの深度レベルの先端から隣の深度レベルまでの距離(バイト単位)

	//バーテックスバッファーの作成
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &pVertexBuffer);
	if (FAILED(result))
	{
		MessageBox(NULL, "バーテックスバッファの作成に失敗", "Error", MB_OK);
		return false;
	}

	//インデックスバッファーの設定
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//インデックスデータの設定
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//インデックスバッファーの作成
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &pIndexBuffer);
	if (FAILED(result))
	{
		MessageBox(NULL, "インデックスバッファの作成に失敗", "Error", MB_OK);
		return false;
	}

	//破棄
	Memory::SafeDeleteArr(indices);
	

	return true;
}

void VertexBuffer::Draw(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	pShader->Begin(deviceContext, indexCount);

	deviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pShader->End(deviceContext);
}

VertexBuffer::VertexType* VertexBuffer::GetVertices()
{
	return pVertices;
}


ID3D11Buffer* VertexBuffer::GetVertexBuffer()
{
	return pVertexBuffer;
}

int VertexBuffer::GetIndexCount()
{
	return indexCount;
}

int VertexBuffer::GetVertexCount()
{
	return vertexCount;
}
