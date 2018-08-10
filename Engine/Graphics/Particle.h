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
	void SetMatrix(Camera& camera);
	Camera* pCamera;
	void EffectDraw();
public:
	void UpDate(Camera&& camera);
	
	void AddEffect(const std::string name, const char* filePass);

	void DeleteEffect(const char* name);
	//エフェクトを走査するハンドル番号が返る
	Effekseer::Handle Play(const std::string& name, Vec3&& pos);
	//明示的に停止、呼んだ瞬間にすぐ消える
	void Stop(Effekseer::Handle handle);
	//明示的に停止、ただしエフェクトの生成をやめるだけなのでしばらく残る
	void StopRoot(Effekseer::Handle handle);
	//ハンドルを指定し座標を設定する
	void SetPos(Effekseer::Handle handle, Vec3&& pos);

	void SetScale(Effekseer::Handle handle, Vec3&& scale);

	void SetAngles(Effekseer::Handle handle, Vec3&& angles);
	Particle();
	~Particle();

};

