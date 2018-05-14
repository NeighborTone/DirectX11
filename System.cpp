#include "System.h"
#include "Engine.h"

System::System(std::string title, int width, int height)
{
	Engine::COMInitialize();
	Create(title,width,height);
}

System::~System()
{
	DestroyWindow(handle);		//ウィンドウの破棄
	CoUninitialize();				//COMの破棄
}

bool System::Run()
{
	//メッセージを取得
	GetMessage(&msg, NULL, 0, 0);
	//メッセージループ(入力などの命令を読む)
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//メッセージをデコードしてWinProcに渡す
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			return true;
		}
		else
		{

		}
	}
	
	PostMessage(handle, WM_APP, 0, 0);

	return true;

}

//LRESULT CALLBACK System::WndProc(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam)
//{
//
//	switch (mes)
//	{
//	case WM_DESTROY:
//		PostQuitMessage(0);		//アプリケーションの終了
//		return 0;
//
//
//	default:
//		break;
//	}
//	//ESCAPEでもアプリケーションの終了
//	if (GetKeyState(VK_ESCAPE) & 0x8000)
//	{
//		PostQuitMessage(0);		
//		return 0;
//	}
//
//	//デフォルトウィンドウ状態
//	return DefWindowProc(hWnd, mes, wParam, lParam);
//}

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
	HINSTANCE instance = GetModuleHandleW(nullptr);
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
	if (!RegisterClassEx(&wcex)) 
	{
		MessageBox(NULL, "ウィンドウの登録に失敗しました", "Error", MB_OK); 
		return false;
	}
	this->width = width;
	this->height = height;
	//ウインドウ生成
	handle = CreateWindow(
				str.c_str(),				//ウィンドウクラスの名前
				str.c_str(),				//タイトルバーのテキスト
				WS_OVERLAPPEDWINDOW,	//ウィンドウのスタイル
				200,							//ウィンドウの水平位置のデフォルト
				50,								//ウィンドウの垂直位置のデフォルト
				width,						//幅
				height,			 			//高さ
				NULL,							//親ウィンドウなし
				NULL,							//メニューなし
				instance,					//アプリケーションインスタンスへのハンドル
				NULL);						//ウィンドウパラメータなし
	//生成チェック
	if (!handle)
	{ 
		MessageBox(NULL, "ウィンドウの生成に失敗しました", "Error", MB_OK);
		return false;
	}

	//ウィンドウサイズと初期座標位置
	SetSize(width,height);
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

int System::GetWidth()
{
	return width;
}

int System::GetHeight()
{
	return height;
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

