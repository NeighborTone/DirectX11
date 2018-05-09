#pragma once
namespace SystemHelp
{
	template<class T>void SafeDelete(T& t)
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}
	template<class T>void SafeDeleteArr(T& t)
	{
		if (t)
		{
			delete[] t;
			t = nullptr;
		}
	}
	template<class T>void SafeRelease(T& t)
	{
		if (t)
		{
			t->Release();
			t = nullptr;
		}
	}
		//ウィンドウサイズ
		static constexpr int SCREEN_WIDTH = 680;
		static constexpr int SCREEN_HEIGHT = 480;
		 //ウィンドウ初期位置
		static constexpr int WINDOW_POSX = 0;
		static constexpr int WINDOW_POSY = 0;
		 //深度
		static constexpr float SCREEN_DEPTH = 1000.0f;
		static constexpr float SCREEN_NERA = 0.1f;
		 //フルスクリーン
		static constexpr bool FULL_SCREEN = false;
		 //垂直同期を行うか
		static constexpr bool VSYNC_ENABLED = true;
};