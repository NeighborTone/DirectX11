#pragma once
//この順番でインクルード
#include "Texture.h"	//<d3d11_1.h>は先にインクルドする
#include "Graphics.h"
#include "Shader.h"
#include "TextureShader.h"
#include "VertexBuffer.h"

class Engine final
{
private:
	Engine();

	void Update();
	void Draw();

	Graphics* pGraphics;
	static Engine* instance;

	//TEMP
	VertexBuffer*		vBuf;
	TextureShader*	textureShader;
	Texture*			texture;
public:
	
	~Engine();
	bool CreateGraphics(HWND hwnd);
	bool Create(HINSTANCE hinstance, HWND hwnd);
	void Run();
	void Release();
	Graphics* GetGraphics();
	static Engine* GetInst();

};

