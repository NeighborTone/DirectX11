#pragma once
#include "System.h"
#include "Direct3D.h"

/*! @class Engine
*   @brief 描画等の更新処理や各ハンドルをまとめます
*/
class Engine final
{
public:
	Engine();
	~Engine();
	/*!
	* @brief COMの初期化を行います。複数回実行されません
	*/
	static void COMInitialize();
	/*!
	* @brief 各機能の更新処理を行います
	* @return 問題がなければtrueを返します
	*/
	static bool Run();
private:
	static System& GetWindow();
};

