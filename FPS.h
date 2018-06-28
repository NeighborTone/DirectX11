#pragma once
#include <atlbase.h>
/*! @class FPS
*   @brief FPSを固定するためのクラスです
*/
class FPS
{
private:
	float time;
	float deltaTime;
	int frameRate;
	float second;
	int frameCount;
	LARGE_INTEGER preCount;			//符号付数値型 long long
	LARGE_INTEGER frequency;
	LARGE_INTEGER GetCounter() const;
	LARGE_INTEGER GetCountFrequency() const;
public:
	FPS();
	~FPS();
	/*!
	* @brief  FPSの同期をします
	*/
	void UpDate();
	/*!
	* @brief  アプリケーション起動時からの時間を返します
	* @retrun [float] time
	*/
	float GetTime() const;
	/*!
	* @brief  前のフレームと今のフレームの時間差を返します
	* @retrun [float] deltaTime
	*/
	float GetDeltaTime() const;
	/*!
	* @brief  現在のフレームレートを返します
	* @retrun [int] frameRate
	*/
	int GetFrameRate() const;

};

