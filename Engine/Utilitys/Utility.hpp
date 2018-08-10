#pragma once
#pragma warning (disable : 4458)	//thisを使うため消す
#include <Windows.h>
#include <DirectXMath.h>
#include <math.h>
#include <stdint.h>
#include <iostream>

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
	template<class T>void SafeDeleteArray(T& t)
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


/*! @class ErrorMessage
*   @brief エラーメッセージをメッセージボックスで出します	
*   @detail コンストラクタでチェック用変数と文字列を設定してください
*   @detail HRUSULT, int, bool型のいずれかでチェックできます
*/
class Message
{
public:
	Message(HRESULT hr, const char* text, const char* category)
	{
		if (FAILED(hr))
		{
			MessageBox(NULL, text, category, MB_OK);
		}
	}
	Message(int isTrue, const char* text, const char* category)
	{
		if (!isTrue)
		{
			MessageBox(NULL, text, category, MB_OK);
		}
	}
	Message(bool isTrue, const char* text, const char* category)
	{
		if (!isTrue)
		{
			MessageBox(NULL, text, category, MB_OK);
		}
	}
	Message(const char* text, const char* category)
	{
		MessageBox(NULL, text, category, MB_OK);
	}
	static bool IsFullScreen()
	{
		int flag;
		flag = MessageBox(
			NULL,
			TEXT("フルスクリーンモードで起動しますか？"),
			TEXT("スクリーン設定"),
			MB_YESNO | MB_ICONQUESTION);
		if (flag == IDNO)
		{
			return true;
		}
		return false;
	}
};
//デバッグビルドのみ有効にする
#ifdef _DEBUG
#define DOUT std::cout
#else 
#define DOUT 0 && std::cout
#endif