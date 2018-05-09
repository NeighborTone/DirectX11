#include "ResourceShader.h"
#include "SystemDefs.h"

ResourceShader::ResourceShader():
	pShader(nullptr)
{

}

ResourceShader::~ResourceShader()
{
	Memory::SafeDelete(pShader);
}

bool ResourceShader::Load(ID3D11Device * device, HWND hwnd, const char * shaderPath, const char* vertexFuncName, const char* pixelFuncName)
{
	pShader = new Shader(device, hwnd, shaderPath, vertexFuncName, pixelFuncName);
	if (!pShader->IsInit())
	{
		Memory::SafeDelete(pShader);
		return false;
	}
	return true;
}

bool ResourceShader::Load(Shader * shader)
{
	if (shader == nullptr)
	{
		return false;
	}
	pShader = shader;
	if (!pShader->IsInit())
	{
		Memory::SafeDelete(pShader);
		return false;
	}
	return true;
}

Shader * ResourceShader::GetShader()
{
	return pShader;
}

std::string ResourceShader::GetName()
{
	return pShader->GetName();
}
