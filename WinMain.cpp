#include "Mainloop.h"


#include <cstdio>
#include <cstdlib>
#include <crtdbg.h>
#include <new>
#include <memory>

#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(X) _malloc_dbg(X, _CLIENT_BLOCK, __FILE__, __LINE__)xpand(p, s)         _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	//_CrtSetBreakAlloc(167);
	Mainloop main;
	if (main.Init(hInstance, nCmdShow))
	{
		main.Run();
	}

	
	//終了
	return 0;
	
}