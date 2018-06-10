#pragma once
#include "DirectXMath.h"
#include "Effekseer.h"
#include "EffekseerRendererDX11.h"
#include "EffekseerSoundXAudio2.h"
#include "Camera.h"
#pragma comment(lib, "Effekseer.Debug.lib" )
#pragma comment(lib, "EffekseerRendererDX11.Debug.lib" )
#pragma comment(lib, "EffekseerSoundXAudio2.Debug.lib" )

class Effecter
{
private:
	EffekseerRenderer::Renderer* renderer;
	Effekseer::Manager* manager;
	Effekseer::Effect* effect;
	Effekseer::Handle handle;

public:
	Effecter();
	void Play();
	void Run();
	void Draw(Camera& camera,DirectX::XMMATRIX v, DirectX::XMMATRIX p);
	~Effecter();
};

