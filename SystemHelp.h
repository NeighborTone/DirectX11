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
		//�E�B���h�E�T�C�Y
		static constexpr int SCREEN_WIDTH = 680;
		static constexpr int SCREEN_HEIGHT = 480;
		 //�E�B���h�E�����ʒu
		static constexpr int WINDOW_POSX = 0;
		static constexpr int WINDOW_POSY = 0;
		 //�[�x
		static constexpr float SCREEN_DEPTH = 1000.0f;
		static constexpr float SCREEN_NERA = 0.1f;
		 //�t���X�N���[��
		static constexpr bool FULL_SCREEN = false;
		 //�����������s����
		static constexpr bool VSYNC_ENABLED = true;
};