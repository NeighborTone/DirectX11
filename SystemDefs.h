#pragma once

namespace Defs
{
	//ウィンドウサイズ
	constexpr int SCREEN_WIDTH = 680;
	constexpr int SCREEN_HEIGHT = 480;
	//ウィンドウ初期位置
	constexpr int WINDOW_POSX = 0;
	constexpr int WINDOW_POSY = 0;
	//深度
	constexpr float SCREEN_DEPTH = 1000.0f;
	constexpr float SCREEN_NERA = 0.1f;
	//フルスクリーン
	constexpr bool FULL_SCREEN = false;
	//垂直同期を行うか
	constexpr bool VSYNC_ENABLED = true;
}

namespace Memory
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
 }
