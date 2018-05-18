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
		"POSITION",										//�V�F�[�_�[���͏����ł��̗v�f�Ɋ֘A�t�����Ă���HLSL�Z�}���e�B�N�X(�\�[�X�R�[�h���ŗ��p����Ă���ϐ��╶�����������삷�邩�𔻒f����)
		0,														//�v�f�̃Z�}���e�B�N�X�C���f�b�N�X
		DXGI_FORMAT_R32G32B32_FLOAT,   //�v�f�f�[�^�̃f�[�^�^
		0,														//���̓A�Z���u���[�����ʂ��鐮���l
		0,														//(�ȗ��\)�e�v�f�Ԃ̃I�t�Z�b�g (�o�C�g�P��)
		D3D11_INPUT_PER_VERTEX_DATA,	//�P��̓��̓X���b�g�̓��̓f�[�^ �N���X������
		0 });													//�o�b�t�@�[�̒��ŗv�f�� 1 ���i�ޑO�ɁA�C���X�^���X�P�ʂ̓����f�[�^���g�p���ĕ`�悷��C���X�^���X�̐�
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
