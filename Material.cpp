#include "Material.h"
#include "Engine.h"
#include "Utility.hpp"
#include <fstream>
#include <vector>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")


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

Material::Material(const char* const filePath) :
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
	//�t�@�C���ǂݍ���
	std::ifstream sourceFile(filePath);
	//�t�@�C����eof�܂œǂݐi�߂�
	std::istreambuf_iterator<char> iterator(sourceFile);
	std::istreambuf_iterator<char> last;
	//�����i�[
	std::string source(iterator, last);
	sourceFile.close();
	//�}�e���A��(�V�F�[�_�[)�����
	Create(source);
	
}

void Material::Create(const std::string & source)
{
	//�}�e���A��(�V�F�[�_�[)�̍쐬
	HRESULT hr;
	vertexShader.Release();
	//ID3DBlob�͔C�ӂ̒����̃f�[�^��Ԃ����߂Ɏg�p�����
	ATL::CComPtr<ID3DBlob> vertexShaderBlob = nullptr;  
	CompileShader(
		source,							//������|�C���^
		"VS",								//�R���p�C���^�[�Q�b�g(�ǂ�ȃV�F�[�_�[��)
		"vs_5_0",						//�R���p�C���I�v�V����(�V�F�[�_�[�̃o�[�W����)
		&vertexShaderBlob);		//�G���[�����������Ƃ��Ƀ��b�Z�[�W���i�[
	
	hr = Engine::GetDXDevice3D().CreateVertexShader(
		vertexShaderBlob->GetBufferPointer(), 
		vertexShaderBlob->GetBufferSize(), 
		nullptr, 
		&vertexShader);
	ErrorMessage(hr, "�o�[�e�b�N�X�V�F�[�_�[�̍쐬�Ɏ��s", "Error");

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
	ErrorMessage(hr, "�s�N�Z���V�F�[�_�[�̍쐬�Ɏ��s", "Error");

	inputLayout.Release();
	//�V�F�[�_�ɓ���f�[�^�̃��C�A�E�g��ݒ肷��,���̓A�Z���u���[�X�e�[�W�̒P��̗v�f�ɂ��Ă̋L�q
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc;
	
	inputElementDesc.push_back({
		"POSITION",						//�V�F�[�_�[���͏����ł��̗v�f�Ɋ֘A�t�����Ă���HLSL�Z�}���e�B�N�X(�\�[�X�R�[�h���ŗ��p����Ă���ϐ��╶�����������삷�邩�𔻒f����)
		0,								//�v�f�̃Z�}���e�B�N�X�C���f�b�N�X
		DXGI_FORMAT_R32G32B32_FLOAT,    //�v�f�f�[�^�̃f�[�^�^
		0,								//���̓A�Z���u���[�����ʂ��鐮���l
		0,								//(�ȗ��\)�e�v�f�Ԃ̃I�t�Z�b�g (�o�C�g�P��)
		D3D11_INPUT_PER_VERTEX_DATA,	//�P��̓��̓X���b�g�̓��̓f�[�^ �N���X������
		0 });							//�o�b�t�@�[�̒��ŗv�f�� 1 ���i�ޑO�ɁA�C���X�^���X�P�ʂ̓����f�[�^���g�p���ĕ`�悷��C���X�^���X�̐�
	inputElementDesc.push_back({ "NORMAL", 	0, 	DXGI_FORMAT_R32G32B32_FLOAT, 	0, 	12, 	D3D11_INPUT_PER_VERTEX_DATA, 0 });
	inputElementDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	inputElementDesc.push_back({ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	inputElementDesc.push_back({ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	inputElementDesc.push_back({ "BLENDINDICES", 1, DXGI_FORMAT_R32G32B32A32_UINT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	inputElementDesc.push_back({ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 76, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	inputElementDesc.push_back({ "BLENDWEIGHT", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 92, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	//���_���C�A�E�g(���̓��C�A�E�g)�I�u�W�F�N�g���쐬���f�o�C�X�ɓo�^
	//�C���v�b�g���C�A�E�g�́A���_���ƃV�F�[�_�[�̏����W�񂵂܂�
	hr = Engine::GetDXDevice3D().CreateInputLayout(
		inputElementDesc.data(),
		inputElementDesc.size(),
		vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(),
		&inputLayout);
	ErrorMessage(hr, "�C���v�b�g���C�A�E�g�̍쐬�Ɏ��s", "Error");
}

void Material::SetBuffer(int slot, void* cbuffer, size_t size)
{
	HRESULT hr;
	constbuff[slot].ptr = cbuffer;

	constbuff[slot].buffer.Release();
	D3D11_BUFFER_DESC constDesc;
	SecureZeroMemory(&constDesc, sizeof(constDesc));
	constDesc.ByteWidth = size;
	constDesc.Usage = D3D11_USAGE_DEFAULT;
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//�o�b�t�@�[(�V�F�[�_�[�萔�o�b�t�@�[) ���쐬
	hr = Engine::GetDXDevice3D().CreateBuffer(
		&constDesc, 
		nullptr, 
		&constbuff[slot].buffer);
	ErrorMessage(hr,"�V�F�[�_�[�萔�o�b�t�@�[�̍쐬�Ɏ��s","Error");
}

void Material::SetTexture(int slot, Texture* texture)
{
	textures[slot] = texture;
}

void Material::Initialize()
{
	Engine::COMInitialize();

	for (int i = 0; i < 10; i++)
	{
		textures[i] = nullptr;
	}
}

void Material::CompileShader(const std::string& source, const char* const entryPoint, const char* const shaderModel, ID3DBlob** out)
{
	UINT shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ATL::CComPtr<ID3DBlob> errorBlob = nullptr;
	//HLSL�R�[�h�����̃^�[�Q�b�g�̃o�C�g�R�[�h�ɃR���p�C��
	D3DCompile(
		source.c_str(),			//���R���p�C���̃V�F�[�_�[�f�[�^�ւ̃|�C���^�[
		source.length(),		//���̒���
		nullptr,				//(�ȗ��\)�V�F�[�_�[ �t�@�C���̖��O
		nullptr,				//(�ȗ��\)NULL �I�[�}�N����`�̔z��
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //(�ȗ��\)�C���N���[�h �t�@�C�����������邽�߂�ID3D10Include�ւ̃|�C���^�[�B�����NULL�ɐݒ肷��ƁA�V�F�[�_�[��#include���L�q����Ă���ꍇ�̓R���p�C���G���[������
		entryPoint,				//�V�F�[�_�[ �G���g���[�|�C���g�֐��̖��O
		shaderModel,			//�R���p�C���̑ΏۂƂ���V�F�[�_�[�^�[�Q�b�g�܂��̓V�F�[�_�[�@�\�Z�b�g
		shaderFlags,			//�V�F�[�_�[�̃R���p�C���I�v�V����
		0,						//�G�t�F�N�g�̃R���p�C�� �I�v�V����
		out,					//�R���p�C�����ꂽ�R�[�h���i�[����ID3D10Blob�̃A�h���X
		&errorBlob);			//(�ȗ��\)�R���p�C���̃G���[���b�Z�[�W���i�[����ID3D10Blob�ւ̃|�C���^�[�B�G���[���Ȃ������ꍇ��NULL���i�[

	//�G���[���Ԃ��Ăė�����
	if (errorBlob != nullptr)
	{
		OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
		MessageBoxA(Engine::GetWindowHandle(), static_cast<char*>(errorBlob->GetBufferPointer()), "Shader Error", MB_OK);
	}
}

void Material::Attach()
{
	//�R���e�L�X�g�ɃV�F�[�_�[�ƒ萔�o�b�t�@��ݒ肷��
	if (vertexShader != nullptr)
		Engine::GetDXContext3D().VSSetShader(vertexShader, nullptr, 0);

	if (pixelShader != nullptr)
		Engine::GetDXContext3D().PSSetShader(pixelShader, nullptr, 0);

	if (inputLayout != nullptr)
		Engine::GetDXContext3D().IASetInputLayout(inputLayout);

	for (int i = 0; i < 10; i++)
	{
		if (constbuff[i].ptr != nullptr)
		{
			Engine::GetDXContext3D().UpdateSubresource(constbuff[i].buffer, 0, nullptr, constbuff[i].ptr, 0, 0);
			Engine::GetDXContext3D().VSSetConstantBuffers(i, 1, &constbuff[i].buffer.p);
			Engine::GetDXContext3D().HSSetConstantBuffers(i, 1, &constbuff[i].buffer.p);
			Engine::GetDXContext3D().DSSetConstantBuffers(i, 1, &constbuff[i].buffer.p);
			Engine::GetDXContext3D().GSSetConstantBuffers(i, 1, &constbuff[i].buffer.p);
			Engine::GetDXContext3D().PSSetConstantBuffers(i, 1, &constbuff[i].buffer.p);
		}
	}

	for (int i = 0; i < 10; i++)
	{
		if (textures[i] != nullptr)
		{
			textures[i]->Attach(i);
		}
	}
}
