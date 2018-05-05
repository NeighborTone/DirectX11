#include "Mainloop.h"
#include "SystemDefs.h"

bool Mainloop::Init(HINSTANCE hInstance, int nCmdShow)
{
	//ウィンドウ生成
	const bool iswin = 	window.Create("title",hInstance, nCmdShow);
	//DirectXのデバイス生成
	const bool isEngine = Engine::GetInst()->Create(window.GetWindow());
	//ウィンドウ描画
	const bool isshow = ShowWindow(window.GetWindow(), nCmdShow);
	if (iswin  && isshow && isEngine)
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
		Engine::GetInst()->Run();
		Engine::GetInst()->Draw();

		}

	//	ShowCursor(false);	//マウス非表示
	
	}
	Engine::GetInst()->Release();
}
