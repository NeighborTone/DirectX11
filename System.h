#pragma once
#include <Windows.h>
#include <tchar.h>
#include <string>
#include <DirectXMath.h>
#include <list>

/*! @class System
*   @brief ウィンドウを生成します
*/
class System 
{
public:
	/*! @class Proceedable
	*   @brief WindowsAPIにDirectXのメッセージ処理を受け渡すインターフェースです
	*/
	class Proceedable
	{
	public:
		virtual void OnProceed(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) = 0;
		
	};

private:
	void SetSize(int width, int height);
	int width;
	int height;
	HWND handle;
	MSG msg;
	HINSTANCE ins;
	bool Create(std::string str, int width,int height);
public:

	/*!
	* @brief コンストラクタです。
	* @param (title) ウィンドウタイトル
	* @param (width) ウィンドウ幅
	* @param (height) ウィンドウ高さ
	*/
	System(std::string title, int width, int height);
	System(const System&) = delete;
	~System();

	/*!
	* @brief ウィンドウ更新
	* @return 問題がなければtrue
	*/
	bool UpDate();

	/*!
	* @brief ウィンドウハンドルを取得します
	* @return HWND
	*/
	HWND GetHwnd();

	/*!
	* @brief ウィンドウのクライアント領域を取得します
	* @return DirectX::XINT2
	*/
	DirectX::XMINT2 GetSize();

	/*!
	* @brief ウィンドウのサイズ変更を行います
	* @param (title) ウィンドウタイトル
	*/
	void SetFullScreen(bool isFullScreen);
private:
	static std::list<Proceedable*>& GetProcedures()
	{
		static std::list<Proceedable*> procedures;
		return procedures;
	}
	static LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

public:
	/*!
	* @brief Windowsのメッセージ処理にDirectXの処理を関連させます
	*/
	static void AddProcedure(Proceedable* const procedure)
	{
		GetProcedures().emplace_back(procedure);
	}
	/*!
	* @brief DirectXからのメッセージ送信を破棄します
	*/
	static void RemoveProcedure(Proceedable* const procedure)
	{
		GetProcedures().remove(procedure);
	}
};