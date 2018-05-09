#include "Mainloop.h"
#include "SystemHelp.h"

bool Mainloop::Init(HINSTANCE hInstance, int nCmdShow)
{
	//ウィンドウ生成
	const bool iswin = 	window.Create("MyFrame",hInstance, nCmdShow);
	//DirectXのデバイス生成

	//ウィンドウ描画
	const bool isshow = ShowWindow(window.GetWindow(), nCmdShow);
	if (iswin  && isshow && dx.Create(SystemHelp::SCREEN_WIDTH,SystemHelp::SCREEN_HEIGHT,SystemHelp::VSYNC_ENABLED,window.GetWindow(),SystemHelp::FULL_SCREEN))
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
			dx.BeginScene(0,0,0,1);
			dx.EndScene();
		}

	//	ShowCursor(false);	//マウス非表示
	
	}

}
