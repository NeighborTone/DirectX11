#include "System.h"

System::System()
{
	SetWindowSize();
}

System::~System()
{
	DestroyWindow(handle);		//ウィンドウの破棄
}

void System::SetWindowSize(UINT w, UINT h)
{
	SCREEN_W = w;
	SCREEN_H = h;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam)
{

	switch (mes)
	{
	case WM_DESTROY:
		PostQuitMessage(0);		//アプリケーションの終了
		return 0;


	default:
		break;
	}
	//ESCAPEでもアプリケーションの終了
	if (GetKeyState(VK_ESCAPE) & 0x8000)
	{
		PostQuitMessage(0);		
		return 0;
	}

	//デフォルトウィンドウ状態
	return DefWindowProc(hWnd, mes, wParam, lParam);
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
				200,							//ウィンドウの水平位置のデフォルト
				50,								//ウィンドウの垂直位置のデフォルト
				Width(),						//幅
				Height(),						//高さ
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

	//ウィンドウプロシージャにWM_PAINTメッセージを送る
	UpdateWindow(handle);
	
	return true;
}

HWND System::GetWindow()
{
	return handle;
}