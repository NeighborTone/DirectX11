#pragma once
#pragma warning (disable : 4458)
#pragma comment(lib,"xinput.lib ")
#include <DirectXMath.h>
#include <Windows.h>
#include <vector>
#include <XInput.h>

namespace DX = DirectX;
class XInput
{
public:
	enum class ButtonID
	{
		//十字キー
		UP = 0,
		DOWN = 1,
		LEFT= 2,
		RIHGT = 3,
		//スタート、バック
		START = 4,
		BACK = 5,
		//アナログL,R
		L_THUMB = 6,
		R_THUMB = 7,
		//トリガー
		L = 9,
		R = 10,
		A = 12,
		B = 13,
		X = 14,
		Y = 15,
		MAX
	};

	enum class ID
	{
		P1 = 0,
		P2 = 1,
		P3 = 2,
		P4 = 3,
		MAX
	};
private:
	//コントローラー処理用
	class Controller
	{
	private:
		//ニュートラル時のあそび値の範囲
		const float deadZone = static_cast<float>(0x7FFF)*0.24f;
		int id;	//コントローラーの番号
		XINPUT_STATE state;	//ボタン情報
		XINPUT_VIBRATION vibration;	//バイブレーション
		bool isConnected;	//接続されているか
		WORD now;	//今のフレームのボタン情報
		WORD prev;	//前のフレームのボタン情報

		struct ButtonState
		{
			int holdFrame;	//押されているフレーム数
			int pullFrame;	//離されているフレーム数
		} frameState[(int)ButtonID::MAX];

		//アナログパッド情報の取得
		DX::XMFLOAT2 GetThumb(const short x, const short y) const;
		void EliminationOfPlayValue(XINPUT_STATE& st);
	public:
		Controller() = delete;
		Controller(ID controllerID);

		//毎フレーム呼ぶ
		void UpDate();
		//押されているか
		bool CheckPress(const ButtonID buttonID, int interval = 1) const;
		//押されていないか
		bool CheckFree(const ButtonID buttonID, int interval = 1) const;
		//押されたか
		bool CheckPush(const ButtonID buttonID) const;
		//離されたか
		bool CheckPull(const ButtonID buttonID) const;
		//トリガースティックの右の情報を取得,0.0f~1.0f
		float GetRightTrigger() const;
		//トリガースティックの左の情報を取得,0.0f~1.0f
		float GetLeftTrigger() const;
		//アナログスティック右の情報を取得,-1.0f~1.0f
		DX::XMFLOAT2 RightThumb() const;
		//アナログスティック左の情報を取得,-1.0f~1.0f
		DX::XMFLOAT2 LeftThumb() const;
		//パッドを振動させる(両手)
		void Vibration(float leftPow, float rihgtPow);
		//パッドを振動させる(右手)
		void VibrationR(float rihgtPow);
		//パッドを振動させる(左手)
		void VibrationL(float leftPow);
	};
	std::vector<Controller> controllers;
	//シングルトンなのでプライベート
	XInput(const XInput&) {};
	XInput(const XInput&&) {};
	XInput();
	~XInput();
public:
	static XInput* GetInstance();
private:
	
	void Run();
	//押されているか
	bool CheckPress(const ButtonID buttonID, int interval = 1, ID id = ID::P1) const;
	//押されていないか
	bool CheckFree(const ButtonID buttonID, int interval = 1, ID id = ID::P1) const;
	//押されたか
	bool CheckPush(const ButtonID buttonID, ID id = ID::P1) const;
	//離されたか
	bool CheckPull(const ButtonID buttonID, ID id = ID::P1) const;
	//1P~4Pまでどのボタンが押されているか
	bool ChekAnyPress(const ButtonID buttonID) const;
	//1P~4Pまでどのボタンが押されたか
	bool ChekAnyPush(const ButtonID buttonID) const;
	//1P~4Pまでどのボタンが離されているか
	bool ChekAnyFree(const ButtonID buttonID) const;
	//1P~4Pまでどのボタンが離されたか
	bool ChekAnyPull(const ButtonID buttonID) const;
	//トリガースティックの右の情報を取得,0.0f~1.0f
	float RightTrigger(ID id = ID::P1) const;
	//トリガースティックの左の情報を取得,0.0f~1.0f
	float LeftTrigger(ID id = ID::P1) const;
	//アナログスティック右の情報を取得,0.0f~1.0f
	DX::XMFLOAT2 RightThumb(ID id = ID::P1) const;
	//アナログスティック左の情報を取得,0.0f~1.0f
	DX::XMFLOAT2 LeftThumb(ID id = ID::P1) const;
	//パッドを振動させる(両手)
	void VibrationAll(float leftPow, float rihgtPow, ID id = ID::P1);
	//パッドを振動させる(右手)
	void Vibration_R(float rihgtPow, ID id = ID::P1);
	//パッドを振動させる(左手)
	void Vibration_L(float leftPow, ID id = ID::P1);
public:
	//関数でアクセスできるようにする
	static void UpDate();
	 //押されているか,
	static bool On(const ButtonID buttonID, int interval = 1, ID id = ID::P1);
	 //押されていないか
	static bool Free(const ButtonID buttonID, int interval = 1, ID id = ID::P1);
	 //押されたか
	static bool Down(const ButtonID buttonID,  ID id = ID::P1);
	 //離されたか
	static bool Pull(const ButtonID buttonID, ID id = ID::P1);
	 //1P~4Pまでどのボタンが押されているか
	static bool ChekAllOn(const ButtonID buttonID);
	 //1P~4Pまでどのボタンが押されたか
	static bool ChekAllDown(const ButtonID buttonID);
	 //1P~4Pまでどのボタンが離されているか
	static bool ChekAllFree(const ButtonID buttonID);
	 //1P~4Pまでどのボタンが離されたか
	static bool ChekAllPull(const ButtonID buttonID);
	 //トリガースティックの右の情報を取得,0.0f~1.0f
	static float GetRightTrigger(ID id = ID::P1);
	 //トリガースティックの左の情報を取得,0.0f~1.0f
	static float GetLeftTrigger(ID id = ID::P1);
	//アナログスティック右の情報を取得,0.0f~1.0f
	static DX::XMFLOAT2 GetRightThumb(ID id = ID::P1);
	//アナログスティック左の情報を取得,0.0f~1.0f
	static DX::XMFLOAT2 GetLeftThumb(ID id = ID::P1);
	 //パッドを振動させる(両手)
	static void Vibration(float leftPow, float rihgtPow, ID id = ID::P1);
	 //パッドを振動させる(右手)
	static void VibrationR(float rihgtPow, ID id = ID::P1);
	 //パッドを振動させる(左手)
	static void VibrationL(float leftPow, ID id = ID::P1);
};

