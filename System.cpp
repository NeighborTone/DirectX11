#include "System.h"
#include "SystemDefs.h"
System::System()
{
	
}

System::~System()
{
	DestroyWindow(handle);		//ウィンドウの破棄
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT mes, WPARAM wParam, LPARAM lParam)
{

	switch (mes)
	{
	case WM_DESTROY:
		PostQuitMessage(0);		//アプリケーションの終了
		DestroyWindow(hwnd);
		return 0;


	default:
		break;
	}
	//ESCAPEでもアプリケーションの終了
	if (GetKeyState(VK_ESCAPE) & 0x8000)
	{
		PostQuitMessage(0);		
		DestroyWindow(hwnd);
		return 0;
	}

	//デフォルトウィンドウ状態
	return DefWindowProc(hwnd, mes, wParam, lParam);
}
bool System::Create(std::string str ,HINSTANCE& hInstance, int& nCmdShow)
{
	WNDCLASSEX wcex;
	SecureZeroMemory(&wcex, sizeof(wcex));
	
	wcex.lpfnWndProc = WndProc;												//ウィンドウプロシージャのｱﾄﾞﾚｽ
	wcex.lpszClassName = str.c_str();											//ウィンドウクラスの名前
	wcex.lpszMenuName = str.c_str();											//メニュー
	wcex.style = WS_OVERLAPPED;												//ウィンドウスタイル
	wcex.cbSize = sizeof(WNDCLASSEX);										//構造体のサイズ
	wcex.cbClsExtra = 0;																//拡張クラスメモリなし
	wcex.cbWndExtra = 0;															//拡張ウィンドウメモリなし
	wcex.hInstance = hInstance;													//WinMain()のインスタンスハンドル
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);		//クライアント領域の背景色(デフォルト)
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);					//事前定義されている矢印カーソル
	wcex.hIcon = LoadIcon(hInstance, "MYICON");;						//アイコン
	wcex.hIconSm = NULL;															//小さいアイコン
	ins = hInstance;
	if (!RegisterClassEx(&wcex)) 
	{
		MessageBox(NULL, "ウィンドウの登録に失敗しました", "Error", MB_OK); 
		return false;
	}

	//ウインドウ生成
	handle = CreateWindow(
				str.c_str(),				//ウィンドウクラスの名前
				str.c_str(),				//タイトルバーのテキスト
				WS_OVERLAPPEDWINDOW,	//ウィンドウのスタイル
				Defs::WINDOW_POSX,		//ウィンドウの水平位置のデフォルト
				Defs::WINDOW_POSY,		//ウィンドウの垂直位置のデフォルト
				Defs::SCREEN_WIDTH,		//幅
				Defs::SCREEN_HEIGHT,		//高さ
				NULL,							//親ウィンドウなし
				NULL,							//メニューなし
				hInstance,					//アプリケーションインスタンスへのハンドル
				NULL);						//ウィンドウパラメータなし
	//生成チェック
	if (!handle)
	{ 
		MessageBox(NULL, "ウィンドウの生成に失敗しました", "Error", MB_OK);
		return false;
	}

	//ウィンドウ作成
	ShowWindow(handle, nCmdShow);
	SetForegroundWindow(handle);
	//ウィンドウプロシージャにWM_PAINTメッセージを送る
	UpdateWindow(handle);
	
	return true;
}

HWND System::GetWindow()
{
	return handle;
}