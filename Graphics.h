#pragma once
#include "DXManager.h"
#include "SystemDefs.h"

class Graphics
{
private:
	DXManager* pManager;
	HWND hwnd;

public:
	Graphics();
	~Graphics();
	
	bool CreateDX(HWND hwnd);
	void Create();
	void BeginScene(float r, float g, float b, float a);
	void EndScene();

	void EnableAlphaBlending(bool enable);
	void EnableZBuffer(bool enable);

	DXManager* GetDXManager();
	HWND GetHwnd();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void SetHwnd(HWND& hwnd);

};

