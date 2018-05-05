#pragma once
//���̏��ԂŃC���N���[�h
#include "Texture.h"	//<d3d11_1.h>�͐�ɃC���N���h����
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

