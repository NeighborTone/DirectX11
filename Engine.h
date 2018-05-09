#pragma once
#include "Graphic.h"
#include "Shader.h"
#include <memory>
#include <DirectXMath.h>

class Shader;

class Engine
{
private:
	Engine();
	static Engine* instance;
	std::unique_ptr<Graphic> pGraphic;
	void Update();
	void Draw();
	
public:
	void Run();
	bool Create(HWND hwnd);
	void Release();
	std::unique_ptr<Shader> shader;
	static Engine* GetEngine();
	Graphic* GetGraphic();
	~Engine();
};

