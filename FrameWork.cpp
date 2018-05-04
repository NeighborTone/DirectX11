#include "FrameWork.h"
#include "SystemDefs.h"

#include<iostream>
using namespace std;


FrameWork::FrameWork()
{

}


FrameWork::~FrameWork()
{

	if (Defs::FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	Engine::GetInst()->Release();

	UnregisterClass(appName, hInst);
	hInst = nullptr;

}

bool FrameWork::init()
{
	if (!CreateDXWindow("DX11", Defs::WINDOW_POSX, Defs::WINDOW_POSY, Defs::SCREEN_WIDTH, Defs::SCREEN_HEIGHT))
	{
		return false;
	}

	if (!Engine::GetInst()->Create(hInst, Engine::GetInst()->GetGraphics()->GetHwnd()))
	{
		return false;
	}
	return true;
}

void FrameWork::Run()
{
	MSG msg;
	SecureZeroMemory(&msg, sizeof(MSG));

	//ループ
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//Run&Draw
			Engine::GetInst()->Run();
		}
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	HDC hdc;


	if (GetKeyState(VK_ESCAPE) & 0x8000)
	{
		PostQuitMessage(0);
		DestroyWindow(hwnd);

		return 0;
	}

	switch (message)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
			DestroyWindow(hwnd);
		}
		break;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		DestroyWindow(hwnd);
		break;
	}
	default:
	{
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	};

	return 0;
}

bool FrameWork::CreateDXWindow(char* title, int x, int y, int width, int height)
{
	HWND hwnd;
	WNDCLASSEX wc;
	SecureZeroMemory(&wc, sizeof(wc));

	appName = title;

	hInst = GetModuleHandle(NULL);

	wc.style = WS_OVERLAPPED;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = appName;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "ウィンドウの初期化に失敗", "error", S_OK);
		return false;
	}
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (Defs::FULL_SCREEN)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//フルスクリーンに切り替え
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}
	else
	{
		screenWidth = width;
		screenHeight = height;
	}
	int nStyle = WS_OVERLAPPEDWINDOW;
	hwnd = CreateWindowEx(WS_EX_APPWINDOW, title, title, nStyle, x, y, screenWidth, screenHeight, NULL, NULL, hInst, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "ウィンドウの作成に失敗", "Error", MB_OK);
		Engine::GetInst()->Release();
		PostQuitMessage(0);

		return false;
	}



	if (!Engine::GetInst()->CreateGraphics(hwnd))
	{
		MessageBox(hwnd, "DirectX11の初期化に失敗", "Error", S_OK);
		Engine::GetInst()->Release();
		PostQuitMessage(0);
		UnregisterClass(appName, hInst);
		DestroyWindow(hwnd);
	}

	Engine::GetInst()->GetGraphics()->SetHwnd(hwnd);

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	return true;
}