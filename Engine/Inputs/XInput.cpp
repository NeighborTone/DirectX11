#include "XInput.h"

DX::XMFLOAT2 XInput::Controller::GetThumb(const short x, const short y) const
{
	if (!isConnected)
	{
		return DX::XMFLOAT2(0,0);
	}
	float ret_x = x >= 0 ? 32767.0f : 32768.0f;
	float ret_y =	y >= 0 ? 32767.0f : 32768.0f;
	return DX::XMFLOAT2
	(
		static_cast<float>(ret_x) / x,
		static_cast<float>(ret_y) / y
	);
}

void XInput::Controller::EliminationOfPlayValue(XINPUT_STATE& st)
{
	if ((st.Gamepad.sThumbLX < deadZone &&
		st.Gamepad.sThumbLX > -deadZone) &&
		(st.Gamepad.sThumbLY < deadZone &&
		st.Gamepad.sThumbLY > -deadZone))
	{
		st.Gamepad.sThumbLX = 0;
		st.Gamepad.sThumbLY = 0;
	}
	if ((st.Gamepad.sThumbRX < deadZone &&
		st.Gamepad.sThumbRX > -deadZone) &&
		(st.Gamepad.sThumbRY < deadZone &&
			st.Gamepad.sThumbRY > -deadZone))
	{
		st.Gamepad.sThumbRX = 0;
		st.Gamepad.sThumbRY = 0;
	}
}

XInput::Controller::Controller(ID controllerID) :
	state(),
	vibration(),
	isConnected(false),
	now(0),
	prev(0),
	id(static_cast<int>(controllerID))
{
	for (auto& it : frameState)
	{
		it.holdFrame = it.pullFrame = 0;
	}
}

void XInput::Controller::UpDate()
{
	//前回のチェック
	prev = now;
	//パッドが抜けてないかチェック
	if (XInputGetState(id, &state) != ERROR_SUCCESS)
	{
		isConnected = false;
		now = 0;
		return;
	}
	isConnected = true;
	//現在のボタン情報の取得
	now = state.Gamepad.wButtons;
	//ニュートラル時のあそび値を消去
	EliminationOfPlayValue(state);
	
	//各ボタンに対し、押されているフレームと離されているフレームをカウント
	for (int i = 0; i < static_cast<int>(ButtonID::MAX); ++i)
	{
		ButtonID buttonID = static_cast<ButtonID>(i);
		//現在のフレームで対象のボタンが押されたか離されたかでフレームカウンタを0クリア
		if (CheckPush(buttonID) || CheckPull(buttonID))
		{
			frameState[i].holdFrame = 0;
			frameState[i].pullFrame = 0;
		}
		//押されているか話されているかでインクリメント
		if (CheckPress(buttonID))
		{
			++frameState[i].holdFrame;
		}
		if (CheckFree(buttonID))
		{
			++frameState[i].pullFrame;
		}
	}

}

bool XInput::Controller::CheckPress(const ButtonID buttonID, int interval) const
{
	if (!isConnected)
	{
		return false;
	}
	WORD flag = 1 << static_cast<int>(buttonID);

	return ((now & flag) == flag && (frameState[static_cast<int>(buttonID)].holdFrame % interval) == 0);
}

bool XInput::Controller::CheckFree(const ButtonID buttonID, int interval) const
{
	if (!isConnected)
	{
		return false;
	}
	WORD flag = 1 << static_cast<int>(buttonID);
	return ((prev & flag) == 0 && (frameState[static_cast<int>(buttonID)].pullFrame % interval) == 0);
}

bool XInput::Controller::CheckPush(const ButtonID buttonID) const
{
	if (!isConnected)
	{
		return false;
	}
	WORD flag = 1 << static_cast<int>(buttonID);
	return ((prev & flag) != 0 && (now & flag) == 0);
}

bool XInput::Controller::CheckPull(const ButtonID buttonID) const
{
	if (!isConnected)
	{
		return false;
	}
	WORD flag = 1 << static_cast<int>(buttonID);
	return ((prev & flag) != 0) && ((now & flag) == 0);
}

float XInput::Controller::GetRightTrigger() const
{
	if (!isConnected)
	{
		return false;
	}
	return static_cast<float>(state.Gamepad.bRightTrigger) / 255.0f;
}

float XInput::Controller::GetLeftTrigger() const
{
	if (!isConnected)
	{
		return false;
	}
	return static_cast<float>(state.Gamepad.bLeftTrigger) / 255.0f;
}

DX::XMFLOAT2 XInput::Controller::RightThumb() const
{
	return GetThumb(state.Gamepad.sThumbRX,state.Gamepad.sThumbRY);
}

DX::XMFLOAT2 XInput::Controller::LeftThumb() const
{
	return GetThumb(state.Gamepad.sThumbLX, state.Gamepad.sThumbLY);
}

void XInput::Controller::Vibration(float leftPow, float rihgtPow)
{
	vibration.wLeftMotorSpeed = static_cast<WORD>(65535.0f * leftPow);
	vibration.wRightMotorSpeed = static_cast<WORD>(65535.0f * rihgtPow);
	XInputSetState(id, &vibration);
}

void XInput::Controller::VibrationR(float rihgtPow)
{
	vibration.wRightMotorSpeed = static_cast<WORD>(65535.0f * rihgtPow);
	XInputSetState(id, &vibration);
}

void XInput::Controller::VibrationL(float leftPow)
{
	vibration.wLeftMotorSpeed = static_cast<WORD>(65535.0f * leftPow);
	XInputSetState(id, &vibration);
}

XInput::XInput()
{
	controllers.clear();
	controllers.push_back(Controller(ID::P1));
	controllers.push_back(Controller(ID::P2));
	controllers.push_back(Controller(ID::P3));
	controllers.push_back(Controller(ID::P4));
}

XInput::~XInput()
{
}

XInput* XInput::GetInstance()
{
	static XInput instance;
	return &instance;
}

void XInput::Run()
{
	for (auto& it : controllers)
	{
		it.UpDate();
	}
}

bool XInput::CheckPress(const ButtonID buttonID, int interval, ID id) const
{
	return controllers[static_cast<int>(id)].CheckPress(buttonID,interval);
}

bool XInput::CheckFree(const ButtonID buttonID, int interval, ID id) const
{
	return controllers[static_cast<int>(id)].CheckFree(buttonID, interval);
}

bool XInput::CheckPush(const ButtonID buttonID, ID id) const
{
	return controllers[static_cast<int>(id)].CheckPush(buttonID);
}

bool XInput::CheckPull(const ButtonID buttonID, ID id) const
{
	return controllers[static_cast<int>(id)].CheckPull(buttonID);
}

bool XInput::ChekAnyPress(const ButtonID buttonID) const
{
	for (const auto& it : controllers)
	{
		if (it.CheckPress(buttonID, 1))
		{
			return true;
		}
	}
	return false;
}

bool XInput::ChekAnyPush(const ButtonID buttonID) const
{
	for (const auto& it : controllers)
	{
		if (it.CheckPush(buttonID))
		{
			return true;
		}
	}
	return false;
}

bool XInput::ChekAnyFree(const ButtonID buttonID) const
{
	for (const auto& it : controllers)
	{
		if (it.CheckFree(buttonID,1))
		{
			return true;
		}
	}
	return false;
}

bool XInput::ChekAnyPull(const ButtonID buttonID) const
{
	for (const auto& it : controllers)
	{
		if (it.CheckPull(buttonID))
		{
			return true;
		}
	}
	return false;
}

float XInput::RightTrigger(ID id) const
{
	return controllers[static_cast<int>(id)].GetRightTrigger();
}

float XInput::LeftTrigger(ID id) const
{
	return controllers[static_cast<int>(id)].GetLeftTrigger();
}

DX::XMFLOAT2 XInput::RightThumb(ID id) const
{
	return controllers[static_cast<int>(id)].RightThumb();
}

DX::XMFLOAT2 XInput::LeftThumb(ID id) const
{
	return controllers[static_cast<int>(id)].LeftThumb();
}

void XInput::VibrationAll(float leftPow, float rihgtPow, ID id)
{
	controllers[static_cast<int>(id)].Vibration(leftPow, rihgtPow);
}

void XInput::Vibration_R(float rihgtPow, ID id)
{
	controllers[static_cast<int>(id)].VibrationR(rihgtPow);
}

void XInput::Vibration_L(float leftPow, ID id)
{
	controllers[static_cast<int>(id)].VibrationL(leftPow);
}


//----------//
//-statics-//
//---------//
void XInput::UpDate()
{
	GetInstance()->Run();
}

bool XInput::On(const ButtonID buttonID, int interval, ID id)
{
	return GetInstance()->CheckPress(buttonID, interval, id);
}

bool XInput::Free(const ButtonID buttonID, int interval, ID id)
{
	return GetInstance()->CheckFree(buttonID, interval, id);
}

bool XInput::Down(const ButtonID buttonID, ID id)
{
	return GetInstance()->CheckPush(buttonID, id);
}

bool XInput::Pull(const ButtonID buttonID,  ID id)
{
	return GetInstance()->CheckPull(buttonID, id);
}

bool XInput::ChekAllOn(const ButtonID buttonID)
{
	return GetInstance()->ChekAnyPress(buttonID);
}

bool XInput::ChekAllDown(const ButtonID buttonID)
{
	return GetInstance()->ChekAnyPush(buttonID);
}

bool XInput::ChekAllFree(const ButtonID buttonID)
{
	return GetInstance()->ChekAnyFree(buttonID);
}

bool XInput::ChekAllPull(const ButtonID buttonID)
{
	return GetInstance()->ChekAnyPull(buttonID);
}

float XInput::GetRightTrigger(ID id)
{
	return GetInstance()->RightTrigger(id);
}

float XInput::GetLeftTrigger(ID id)
{
	return GetInstance()->LeftTrigger(id);
}

DX::XMFLOAT2 XInput::GetRightThumb(ID id)
{
	return GetInstance()->RightThumb(id);
}

DX::XMFLOAT2 XInput::GetLeftThumb(ID id)
{
	return  GetInstance()->LeftThumb(id);
}

void XInput::Vibration(float leftPow, float rihgtPow, ID id)
{
	GetInstance()->VibrationAll(leftPow, rihgtPow, id);
}

void XInput::VibrationR(float rihgtPow, ID id)
{
	GetInstance()->Vibration_R(rihgtPow, id);
}

void XInput::VibrationL(float leftPow, ID id)
{
	GetInstance()->Vibration_L(leftPow, id);
}
