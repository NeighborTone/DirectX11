#pragma once
#define WIN32_LEAN_AND_MEAN

#include "Engine.h"

class FrameWork final
{

private:
	
	bool CreateDXWindow(char* title,int x,int y,int width,int height);
public:
	FrameWork();
	~FrameWork();

	bool init();
	void Run();
	
	char* appName;
	HINSTANCE hInst;


};

