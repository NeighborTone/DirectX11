#include "Mainloop.h"


bool Mainloop::Init(HINSTANCE hInstance, int nCmdShow)
{
	//ウィンドウ生成
	const bool iswin = 	window.Create("DirectX11",hInstance, nCmdShow);
	//DirectX11の初期化
	const bool isDx = DX_device->GetInstace()->InitDirect3D(window);
	//ウィンドウ描画
	const bool isshow = ShowWindow(window.GetWindow(), nCmdShow);
	
	if (iswin  && isshow && isDx)
	{
		
		return true;
	}
	return false;
}


void Mainloop::Run()
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
		}
		else
		{
		
			DX_device->GetInstace()->Draw();

		}

	//	ShowCursor(false);	//マウス非表示
	
	}

}
