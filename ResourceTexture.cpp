#include "ResourceTexture.h"
#include "SystemDefs.h"

ResourceTexture::ResourceTexture():
	tex(nullptr)

{
	
}

ResourceTexture::~ResourceTexture()
{
	Memory::SafeDelete(tex);
}

bool ResourceTexture::Load(ID3D11Device * device, const char * texturePath)
{
	tex = new Texture();
	if (!tex->Create(device, texturePath))
	{
		Memory::SafeDelete(tex);
		return false;
	}
	return true;
}

Texture * ResourceTexture::GetTexture()
{
	return tex;
}

std::string ResourceTexture::GetName()
{
	return tex->GetName();
}
