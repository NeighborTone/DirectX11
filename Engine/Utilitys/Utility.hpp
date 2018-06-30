#pragma once
#pragma warning (disable : 4458)	//thisを使うため消す
#include <Windows.h>
#include <DirectXMath.h>
#include <math.h>
#include <stdint.h>

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
class ErrorMessage
{
public:
	ErrorMessage(HRESULT hr, const char* text, const char* category)
	{
		if (FAILED(hr))
		{
			MessageBox(NULL, text, category, MB_OK);
		}
	}
	ErrorMessage(int isTrue, const char* text, const char* category)
	{
		if (!isTrue)
		{
			MessageBox(NULL, text, category, MB_OK);
		}
	}
	ErrorMessage(bool isTrue, const char* text, const char* category)
	{
		if (!isTrue)
		{
			MessageBox(NULL, text, category, MB_OK);
		}
	}
	ErrorMessage(const char* text, const char* category)
	{
		MessageBox(NULL, text, category, MB_OK);
	}
};



