#include "System.h"
#include "../Engine.h"
#include "../Utilitys/Utility.hpp"

System::System(std::string title, int width, int height)
{
	Engine::COMInitialize();
	Create(title,width,height);
}

System::~System()
{
	
	CoUninitialize();		//COMオブジェクトの破棄。必ずここで呼ぶ
}

bool System::UpDate()
{
	//メッセージを取得
	GetMessage(&msg, NULL, 0, 0);
	//メッセージループ(入力などの命令を読む)
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	PostMessage(handle, WM_APP, 0, 0);
	if (GetSize().x <= 0.0f || GetSize().y <= 0.0f)
		Sleep(100);
	return true;

}

void System::SetSize(int width, int height)
{
	RECT windowRect = {};
	RECT clientRect = {};
	GetWindowRect(handle, &windowRect);
	GetClientRect(handle, &clientRect);

	int w = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left) + width;
	int h = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top) + height;
	int x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
	int y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

	SetWindowPos(handle, nullptr, x, y, w, h, SWP_FRAMECHANGED);
}

bool System::Create(std::string str, int width, int height)
{
	WNDCLASSEX wcex;
	HINSTANCE instance = GetModuleHandle(nullptr);
	SecureZeroMemory(&wcex, sizeof(wcex));
	
	wcex.lpfnWndProc = WinProc;									//ウィンドウプロシージャのｱﾄﾞﾚｽ
	wcex.lpszClassName = str.c_str();							//ウィンドウクラスの名前
	wcex.lpszMenuName = str.c_str();							//メニュー
	wcex.style = WS_OVERLAPPED;									//ウィンドウスタイル
	wcex.cbSize = sizeof(WNDCLASSEX);							//構造体のサイズ
	wcex.cbClsExtra = 0;												//拡張クラスメモリなし
	wcex.cbWndExtra = 0;												//拡張ウィンドウメモリなし
	wcex.hInstance = instance;										//WinMain()のインスタンスハンドル
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);		//クライアント領域の背景色(デフォルト)
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);			//事前定義されている矢印カーソル
	wcex.hIcon = LoadIcon(instance, "MYICON");				//アイコン
	wcex.hIconSm = NULL;												//小さいアイコン
	ins = instance;

	ErrorMessage(RegisterClassEx(&wcex), "ウィンドウの登録に失敗しました", "Error");

	this->width = width;
	this->height = height;
	//ウインドウ生成
	handle = CreateWindow(
				str.c_str(),				//ウィンドウクラスの名前
				str.c_str(),				//タイトルバーのテキスト
				WS_OVERLAPPEDWINDOW,	//ウィンドウのスタイル
				200,							//ウィンドウの水平位置のデフォルト
				50,								//ウィンドウの垂直位置のデフォルト
				0,						//幅
				0,			 			//高さ
				NULL,							//親ウィンドウなし
				NULL,							//メニューなし
				instance,					//アプリケーションインスタンスへのハンドル
				NULL);						//ウィンドウパラメータなし
	//生成チェック
	ErrorMessage(handle, "ウィンドウの生成に失敗しました", "Error");

	//ウィンドウサイズと初期座標位置
	SetSize(width,height);
	SetWindowLongPtr(handle, GWLP_WNDPROC, (LONG_PTR)WinProc);
	//ウィンドウ作成
	ShowWindow(handle, SW_SHOWNORMAL);

	//ウィンドウプロシージャにWM_PAINTメッセージを送る
	UpdateWindow(handle);
	
	return true;
}

HWND System::GetHwnd()
{

	return handle;
}

void System::SetFullScreen(bool isFullScreen)
{
	static DirectX::XMINT2 size = GetSize();

	if (!isFullScreen)
	{
		size = GetSize();
		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);
		SetWindowLongPtrW(handle, GWL_STYLE, WS_VISIBLE | WS_POPUP);
		SetWindowPos(handle, HWND_TOP, 0, 0, w, h, SWP_FRAMECHANGED);
	}
	else
	{
		SetWindowLongPtrW(handle, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
		SetWindowPos(handle, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
		SetSize(size.x, size.y);
	}
}

DirectX::XMINT2 System::GetSize()
{
	RECT clientRect = {};
	GetClientRect(handle, &clientRect);

	return DirectX::XMINT2(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
}

LRESULT System::WinProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	{
		for (auto it : GetProcedures())
		{
			it->OnProceed(window, message, wParam, lParam);
		}

		if (message == WM_DESTROY)
			PostQuitMessage(0);

		return DefWindowProc(window, message, wParam, lParam);
	}
}

