#pragma once
#include <string>
#include <Windows.h>
//参考元↓
//https://www.wabiapp.com/WabiSampleSource/windows/string_to_wstring.html
//

class StringConverter
{
public:
	/*!
	*   @brief stringをwstringへ変換する
	*/
	static std::wstring StringToWString(std::string oString)
	{
		// SJIS → wstring
		int iBufferSize = MultiByteToWideChar(
			CP_ACP,
			0,
			oString.
			c_str(),
			-1,
			(wchar_t*)NULL,
			0);

		// バッファの取得
		wchar_t* cpUCS2 = new wchar_t[iBufferSize];

		// SJIS → wstring
		MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, cpUCS2
			, iBufferSize);

		// stringの生成
		std::wstring oRet(cpUCS2, cpUCS2 + iBufferSize - 1);

		// バッファの破棄
		delete[] cpUCS2;

		// 変換結果を返す
		return(oRet);
	}

	/*!
	*   @brief wstringをstringへ変換
	*/
	static std::string WStringToString(std::wstring oWString)
	{
		// wstring → SJIS
		int iBufferSize = WideCharToMultiByte(
			CP_OEMCP,
			0,
			oWString.c_str(),
			-1,
			(char *)NULL,
			0,
			NULL,
			NULL);

		// バッファの取得
		CHAR* cpMultiByte = new CHAR[iBufferSize];

		// wstring → SJIS
		WideCharToMultiByte(
			CP_OEMCP,
			0,
			oWString.c_str(),
			-1,
			cpMultiByte,
			iBufferSize,
			NULL,
			NULL);

		// stringの生成
		std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

		// バッファの破棄
		delete[] cpMultiByte;

		// 変換結果を返す
		return(oRet);
	}
};
