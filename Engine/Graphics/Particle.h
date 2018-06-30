#pragma once
#pragma warning (disable : 4100)	
#include <DirectXMath.h>
#include "Effekseer.h"
#include "EffekseerRendererDX11.h"
#include "EffekseerSoundXAudio2.h"
#include "Camera.h"
#if defined(_DEBUG)
#pragma comment(lib, "Effekseer.Debug.lib" )
#pragma comment(lib, "EffekseerRendererDX11.Debug.lib" )
#pragma comment(lib, "EffekseerSoundXAudio2.Debug.lib" )
#else
#pragma comment(lib, "Effekseer.Release.lib" )
#pragma comment(lib, "EffekseerRendererDX11.Release.lib" )
#pragma comment(lib, "EffekseerSoundXAudio2.Release.lib" )
#endif
class Particle
{
private:
	static constexpr int MAX_SPRITS = 2000;
	EffekseerRenderer::Renderer* renderer;
	Effekseer::Manager* manager;
	Effekseer::Effect* effect;
	Effekseer::Handle handle;
	void Create();
	void Update();
	void EffectDraw(Camera& camera);
	void Init();
public:
	Vec3 pos;
	Vec3 scale;
	Vec3 angle;
	Particle(const char* path);
	Particle();
	~Particle();
	void Load(const char* path);
	void Play();
	void Stop();
	void Draw(Camera &camera);
};

