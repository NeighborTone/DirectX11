#pragma once
#include <atlbase.h>
/*! @class FPS
*   @brief FPS���Œ肷�邽�߂̃N���X�ł�
*/
class FPS
{
private:
	float time;
	float deltaTime;
	int frameRate;
	float second;
	int frameCount;
	LARGE_INTEGER preCount;			//�����t���l�^ long long
	LARGE_INTEGER frequency;
	LARGE_INTEGER GetCounter() const;
	LARGE_INTEGER GetCountFrequency() const;
public:
	FPS();
	~FPS();
	/*!
	* @brief  FPS�̓��������܂�
	*/
	void UpDate();
	/*!
	* @brief  �A�v���P�[�V�����N��������̎��Ԃ�Ԃ��܂�]
	* @retrun [float] time
	*/
	float GetTime() const;
	/*!
	* @brief  �O�̃t���[���ƍ��̃t���[���̎��ԍ���Ԃ��܂�
	* @retrun [float] deltaTime
	*/
	float GetDeltaTime() const;
	/*!
	* @brief  ���݂̃t���[�����[�g��Ԃ��܂�
	* @retrun [int] frameRate
	*/
	int GetFrameRate() const;

};

