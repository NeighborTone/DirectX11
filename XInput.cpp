#include "XInput.h"

DX::XMFLOAT2 XInput::Controller::GetThumb(const short x, const short y) const
{
	if (!isConnected)
	{
		return DX::XMFLOAT2(0,0);
	}
	float ret_x = x >= 0 ? 3276.0f : 32768.0f;
	float ret_y = y >= 0 ? 3276.0f : 32768.0f;
	return DX::XMFLOAT2
	(
		static_cast<float>(ret_x / x),
		static_cast<float>(ret_y / y)
	);
}

void XInput::Controller::EliminationOfPlayValue(XINPUT_STATE & st)
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

bool XInput::Controller::CheckPush(const ButtonID buttonID, int interval) const
{
	if (!isConnected)
	{
		return false;
	}
	WORD flag = 1 << static_cast<int>(buttonID);
	return ((prev & flag) != 0 && (now & flag) == 0);
}

bool XInput::Controller::CheckPull(const ButtonID buttonID, int interval) const
{
	if (!isConnected)
	{
		return false;
	}
	WORD flag = 1 << static_cast<int>(buttonID);
	return ((prev & flag) != 0) && ((now & flag) == 0);
}

float XInput::Controller::GetRightTrigger(ID id) const
{
	if (!isConnected)
	{
		return false;
	}
	return static_cast<float>(state.Gamepad.bRightTrigger) / 255.0f;
}

float XInput::Controller::GetLeftTrigger(ID id) const
{
	if (!isConnected)
	{
		return false;
	}
	return static_cast<float>(state.Gamepad.bLeftTrigger) / 255.0f;
}

DX::XMFLOAT2 XInput::Controller::RightThumb() const
{
	return DX::XMFLOAT2();
}

DX::XMFLOAT2 XInput::Controller::LeftThumb() const
{
	return DX::XMFLOAT2();
}

void XInput::Controller::Vibration(float leftPow, float rihgtPow, ID id)
{
}

void XInput::Controller::VibrationR(float rihgtPow, ID id)
{
}

void XInput::Controller::VibrationL(float leftPow, ID id)
{
}

XInput * XInput::GetInstance()
{
	return nullptr;
}

void XInput::UpDate()
{
}

bool XInput::CheckPress(const ButtonID buttonID, int interval, ID id) const
{
	return false;
}

bool XInput::CheckFree(const ButtonID buttonID, int interval, ID id) const
{
	return false;
}

bool XInput::CheckPush(const ButtonID buttonID, ID id) const
{
	return false;
}

bool XInput::CheckPull(const ButtonID buttonID, ID id) const
{
	return false;
}

bool XInput::ChekAnyPress(const ButtonID buttonID) const
{
	return false;
}

bool XInput::ChekAnyPush(const ButtonID buttonID) const
{
	return false;
}

bool XInput::ChekAnyFree(const ButtonID buttonID) const
{
	return false;
}

bool XInput::ChekAnyPull(const ButtonID buttonID) const
{
	return false;
}

float XInput::RightThumb(ID id) const
{
	return 0.0f;
}

float XInput::LeftThumb(ID id) const
{
	return 0.0f;
}

void XInput::VibrationAll(float leftPow, float rihgtPow, ID id)
{
}

void XInput::Vibration_R(float rihgtPow, ID id)
{
}

void XInput::Vibration_L(float leftPow, ID id)
{
}


//----------//
//-statics-//
//---------//
void XInput::Run()
{
}

bool XInput::On(const ButtonID buttonID, int interval, ID id)
{
	return false;
}

bool XInput::Free(const ButtonID buttonID, int interval, ID id)
{
	return false;
}

bool XInput::Down(const ButtonID buttonID, int interval, ID id)
{
	return false;
}

bool XInput::Pull(const ButtonID buttonID, int interval, ID id)
{
	return false;
}

bool XInput::ChekAllOn(const ButtonID buttonID)
{
	return false;
}

bool XInput::ChekAllDown(const ButtonID buttonID)
{
	return false;
}

bool XInput::ChekAllFree(const ButtonID buttonID)
{
	return false;
}

bool XInput::ChekAllPull(const ButtonID buttonID)
{
	return false;
}

float XInput::GetRightTrigger(ID id)
{
	return 0.0f;
}

float XInput::GetLeftTrigger(ID id)
{
	return 0.0f;
}

void XInput::Vibration(float leftPow, float rihgtPow, ID id)
{
}

void XInput::VibrationR(float rihgtPow, ID id)
{
}

void XInput::VibrationL(float leftPow, ID id)
{
}
