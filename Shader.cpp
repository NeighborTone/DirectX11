#include "Shader.h"
#include <d3dcompiler.h>
#pragma comment(lib,"d3dCompiler.lib")
Shader::Shader(ID3D11Device* device, HWND hwnd, const char* shaderPath) :
	  pVertex(nullptr)
	, pPixel(nullptr)
	, pLayout(nullptr)
	, pMatrixBuf(nullptr)
	, isInit(Load(device, hwnd, shaderPath))
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
	
	
}

//private---------------------------------------------------------------------------------------------------------------------------------------------------
bool Shader::CreateShader(ID3D11Device* device, HWND hwnd, const char* shaderPath)
{
	HRESULT hr;
	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexBuf		= nullptr;
	ID3D10Blob* psBuf			= nullptr;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBuf;

	//�o�[�e�b�N�V�F�[�_�[���R���p�C��
	hr = D3DX11CompileFromFileA(
		(LPCSTR)shaderPath,
		NULL, 
		NULL, 
		"VSMain",
		"vs_4_0",
		D3DCOMPILE_ENABLE_STRICTNESS,
		0,
		NULL,
		&vertexBuf, 
		&errorMessage,
		NULL);

	if (FAILED(hr))
	{
		if (errorMessage)
		{
			OutputShaderError(errorMessage, hwnd, shaderPath);
		}
		else
		{
			MessageBox(hwnd, (LPCSTR)shaderPath, "Error in Shade File", MB_OK);
		}

		return false;
	}

	//�s�N�Z���V�F�[�_�[���R���p�C��
	hr = D3DX11CompileFromFileA(
		(LPCSTR)shaderPath,
		NULL,
		NULL,
		"PSMain",
		"ps_4_0",
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
			OutputShaderError(errorMessage, hwnd, shaderPath);
		}
		else
		{
			MessageBox(hwnd, (LPCSTR)shaderPath, "Error in Shade File", MB_OK);
		}

		return false;
	}

	//�o�[�e�b�N�X�V�F�[�_�[�̃o�b�t�@�����
	hr = device->CreateVertexShader(
		vertexBuf->GetBufferPointer(), 
		vertexBuf->GetBufferSize(), 
		NULL, 
		&pVertex);
	if (FAILED(hr))
	{
		return false;
	}

	//�s�N�Z���V�F�[�_�[�̃o�b�t�@�����
	hr = device->CreatePixelShader(
		psBuf->GetBufferPointer(), 
		psBuf->GetBufferSize(), 
		NULL, 
		&pPixel);
	if (FAILED(hr))
	{
		return false;
	}

	//�V�F�[�_�ɓ���f�[�^�̃��C�A�E�g��ݒ肷��
	polygonLayout[0].SemanticName = "POSITION";										//�V�F�[�_�[���͏����ł��̗v�f�Ɋ֘A�t�����Ă���HLSL�Z�}���e�B�N�X(�\�[�X�R�[�h���ŗ��p����Ă���ϐ��╶�����������삷�邩�𔻒f����)
	polygonLayout[0].SemanticIndex = 0;													//�v�f�̃Z�}���e�B�N�X�C���f�b�N�X
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;						//�v�f�f�[�^�̃f�[�^�^
	polygonLayout[0].InputSlot = 0;															//���̓A�Z���u���[�����ʂ��鐮���l
	polygonLayout[0].AlignedByteOffset = 0;												//(�ȗ��\)�e�v�f�Ԃ̃I�t�Z�b�g (�o�C�g�P��)
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;			//�P��̓��̓X���b�g�̓��̓f�[�^ �N���X������
	polygonLayout[0].InstanceDataStepRate = 0;											//�o�b�t�@�[�̒��ŗv�f�� 1 ���i�ޑO�ɁA�C���X�^���X�P�ʂ̓����f�[�^���g�p���ĕ`�悷��C���X�^���X�̐�

	polygonLayout[1].SemanticName = "TEXCOORD";										
	polygonLayout[1].SemanticIndex = 0;													
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;							
	polygonLayout[1].InputSlot = 0;															
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;		
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;			
	polygonLayout[1].InstanceDataStepRate = 0;											

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);			

	//���̓��C�A�E�g�I�u�W�F�N�g���쐬
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

	//�쐬���I������̂ŗ]���ȃo�b�t�@��j��
	RELEASE(vertexBuf);
	RELEASE(psBuf);

	//�}�g���b�N�X�o�b�t�@�̐ݒ�
	matrixBuf.ByteWidth = sizeof(MatrixBufferType);				//�o�b�t�@�[�̃T�C�Y (�o�C�g�P��)
	matrixBuf.Usage = D3D11_USAGE_DYNAMIC;							//�o�b�t�@�[�őz�肳��Ă���ǂݍ��݂���я������݂̕��@������
	matrixBuf.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//�o�b�t�@�[���ǂ̂悤�Ƀp�C�v���C���Ƀo�C���h���邩������
	matrixBuf.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//CPU�A�N�Z�X�̃t���O
	matrixBuf.MiscFlags = 0;												//���\�[�X�Ɏg�p�����A���܂��ʓI�łȂ����̑��̃I�v�V����
	matrixBuf.StructureByteStride = 0;								//�\���̂��\�����o�b�t�@�[��\���ꍇ�A���̍\���̂̃T�C�Y (�o�C�g�P��) 

	//�o�b�t�@�|�C���^���쐬����
	hr = device->CreateBuffer(&matrixBuf, NULL, &pMatrixBuf);
	if (FAILED(hr))
	{
		MessageBox(hwnd, "�}�g���b�N�X�o�b�t�@�̍쐬�Ɏ��s", "Error", MB_OK);
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
bool Shader::Load(ID3D11Device* device, HWND hwnd, const std::string shaderPath)
{
	//�V�F�[�_�[�t�@�C����ǂݍ���
	bool result;
	std::string shaderFilePath(shaderPath);
	name = std::string(shaderFilePath.begin(), shaderFilePath.end());
	int pos = name.find_last_of("/");
	if (pos >= 0)
	{
		name = name.substr(pos + 1, name.length());
	}

	//���_�ƃs�N�Z���̏�����
	result = CreateShader(device, hwnd,(LPCSTR)shaderPath.c_str());
	return result;
}

//public-----------------------------------------------------------------------------------------------------------------------------------------------------
void Shader::Begin(ID3D11DeviceContext* context, int indexCount)
{
	//�R���e�L�X�g�ɒ��_�̐ݒ������
	context->IASetInputLayout(pLayout);
	
	//�����_�����O�Ɏg�p���钸�_�V�F�[�_�ƃs�N�Z���V�F�[�_��ݒ肷��
	context->VSSetShader(pVertex, NULL, 0);
	context->PSSetShader(pPixel, NULL, 0);

	//�`��J�n
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

bool Shader::SetShaderParameters(ID3D11DeviceContext* context, Matrix world, Matrix view, Matrix proj)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	//�萔�o�b�t�@�����b�N���ď������݉\�ɂ���
	hr = context->Map(pMatrixBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		return false;
	}

	//�萔�o�b�t�@���̃f�[�^�ւ̃|�C���^���擾
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	//�s���ϊ����ăV�F�[�_�p�ɏ���
	world.MatrixTranspose(&world);
	view.MatrixTranspose(&view);
	proj.MatrixTranspose(&proj);
	//�s���萔�o�b�t�@�ɃR�s�[
	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = proj;

	//�萔�o�b�t�@�̃��b�N������
	context->Unmap(pMatrixBuf, 0);

	//���_�V�F�[�_�ł̒萔�o�b�t�@�̈ʒu��ݒ�
	bufferNumber = 0;

	//�X�V���ꂽ�l�Œ��_�V�F�[�_�̒萔�o�b�t�@��ݒ�
	context->VSSetConstantBuffers(bufferNumber, 1, &pMatrixBuf);

	return true;
}

std::string Shader::GetName()
{
	return name;
}

bool Shader::IsInit()
{
	return isInit;
}
