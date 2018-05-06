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

	//�N�A�b�h�ɕK�v�ȏ��
	vertexCount = 4;		//���_��
	indexCount = 6;		//���񂾐��̐�

	pVertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	float halfSize = size / 2.0f;

	//���_�̍쐬�ƃ|���S���ɓ\��e�N�X�`����UV�l�̐ݒ�
	pVertices[0].pos	= D3DXVECTOR3(-halfSize, -halfSize, 0.0f);
	pVertices[0].uv   = D3DXVECTOR2(0.0f, 1.0f);
	pVertices[1].pos	= D3DXVECTOR3(-halfSize, halfSize, 0.0f);
	pVertices[1].uv	= D3DXVECTOR2(0.0f, 0.0f);
	pVertices[2].pos	= D3DXVECTOR3(halfSize, halfSize, 0.0f);
	pVertices[2].uv	= D3DXVECTOR2(1.0f, 0.0f);
	pVertices[3].pos	= D3DXVECTOR3(halfSize, -halfSize, 0.0f);
	pVertices[3].uv	= D3DXVECTOR2(1.0f, 1.0f);

	//���_������
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	//�o�[�e�b�N�X�o�b�t�@�[�̐ݒ�
	vertexBufferDesc.Usage = (writeable) ?										//�o�b�t�@�[�őz�肳��Ă���ǂݍ��݂���я������݂̕��@������									
										D3D11_USAGE_DYNAMIC :							//GPU(�ǂݎ��̂�)��CPU(�������݂̂�)�ɂ��A�N�Z�X���\�ȃ��\�[�X�BCPU �ɂ���ăt���[�����Ƃ�1��ȏ�X�V����郊�\�[�X�ɂ́A���I���\�[�X��I�����邱�Ƃ���������Ă���
										D3D11_USAGE_DEFAULT;							//�ł���ʓI�Ȓl
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;	//�o�b�t�@�[�̃T�C�Y(�o�C�g�P��)
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//�o�b�t�@�[���ǂ̂悤�Ƀp�C�v���C���Ƀo�C���h���邩������
	vertexBufferDesc.CPUAccessFlags = (writeable) ?							//CPU�A�N�Z�X�̃t���O
													  D3D11_CPU_ACCESS_WRITE :		//���\�[�X���}�b�v�\�ɂ��ACPU�����̃��\�[�X�̓��e��ύX�ł���悤�ɂ���
													  0;										//CPU�A�N�Z�X���K�v�łȂ��ꍇ
	vertexBufferDesc.MiscFlags = 0;													//���\�[�X�Ɏg�p�����A���܂��ʓI�łȂ����̑��̃I�v�V����
	vertexBufferDesc.StructureByteStride = 0;									//�\���̂��\�����o�b�t�@�[��\���ꍇ�A���̍\���̂̃T�C�Y (�o�C�g�P��)

	//�T�u���\�[�X�̏������Ɏg�p�����f�[�^���w��
	vertexData.pSysMem = pVertices;		//�������f�[�^�ւ̃|�C���^�[
	vertexData.SysMemPitch = 0;			//�e�N�X�`���[�ɂ���1�{�̐��̐�[����ׂ̐��܂ł̋���(�o�C�g�P��) 
	vertexData.SysMemSlicePitch = 0;	//1�̐[�x���x���̐�[����ׂ̐[�x���x���܂ł̋���(�o�C�g�P��)

	//�o�[�e�b�N�X�o�b�t�@�[�̍쐬
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &pVertexBuffer);
	if (FAILED(result))
	{
		MessageBox(NULL, "�o�[�e�b�N�X�o�b�t�@�̍쐬�Ɏ��s", "Error", MB_OK);
		return false;
	}

	//�C���f�b�N�X�o�b�t�@�[�̐ݒ�
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//�C���f�b�N�X�f�[�^�̐ݒ�
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//�C���f�b�N�X�o�b�t�@�[�̍쐬
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &pIndexBuffer);
	if (FAILED(result))
	{
		MessageBox(NULL, "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s", "Error", MB_OK);
		return false;
	}

	//�j��
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
