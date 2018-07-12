#pragma once
#pragma warning (disable : 4100)	
#include <DirectXMath.h>
#include "Effekseer.h"
#include "EffekseerRendererDX11.h"
#include "EffekseerSoundXAudio2.h"
#include "Camera.h"
#include <string>
#include <unordered_map>
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
	std::unordered_map<std::string, Effekseer::Effect*> effects;
	void Create();
	void Update();
	Camera* pCamera;
	void EffectDraw();
public:
	void SetMatrix(Camera &camera);
	void AddEffect(const std::string name, const char* filePass);
	void DeleteEffect(const char* name);
	//�G�t�F�N�g�𑖍�����n���h���ԍ����Ԃ�
	Effekseer::Handle Play(const std::string& name, Vec3 pos);
	//�����I�ɒ�~�A�Ă񂾏u�Ԃɂ���������
	void Stop(Effekseer::Handle handle);
	//�����I�ɒ�~�A�������G�t�F�N�g�̐�������߂邾���Ȃ̂ł��΂炭�c��
	void StopRoot(Effekseer::Handle handle);
	Particle();
	~Particle();

};

