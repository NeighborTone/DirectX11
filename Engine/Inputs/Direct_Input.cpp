#include "Direct_Input.h"
#include "../Engine.h"

//global------------------------------------
Input in;
//------------------------------------------

Input::Input() :buf(), prebuf()
{
	pDI = nullptr;
	pKey = nullptr;
}

Input::~Input()
{
	//入力を受け付けなくする(デバイスへのアクセス権を解放する。)
	pKey->Unacquire();
	if (pDI != nullptr)
	{
		pDI->Release();
	}
	if (pKey != nullptr)
	{
		pKey->Release();
	}
}

bool Input::CreateInput(const HWND& hwnd)
{
	//IDirectInput8の作成
	HRESULT hr = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(LPVOID*)&pDI,
		NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, "DirectInputオブジェクトの生成に失敗しました", "Error", MB_OK);
		return false;
	}

	//デバイスの取得
	hr = pDI->CreateDevice(
		GUID_SysKeyboard,
		&pKey,
		NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, "キーボードデバイスの作成に失敗しました", "Error", MB_OK);
		return false;
	}

	//入力データのフォーマット
	hr = pKey->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(NULL, "入力データの設定に失敗しました", "Error", MB_OK);
		return false;
	}

	//排他制御
	hr = pKey->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(hr))
	{
		MessageBox(NULL, "排他制御の設定に失敗しました", "Error", MB_OK);
		return false;
	}
	//動作開始
	pKey->Acquire();
	int keyList[Input::MAX_KEY] =
	{
		DIK_UP,				//↑				
		DIK_LEFT,			//←
		DIK_RIGHT,			//→			
		DIK_DOWN	,		//↓
		DIK_ESCAPE,			//Esc
		DIK_RETURN,	        //Enter
		DIK_LSHIFT,	        //Shift(左)	
		DIK_RSHIFT,			//Shift(右)
		DIK_LCONTROL,	    //Ctrl(左)
		DIK_RCONTROL,	    //Ctrl(右)					
		DIK_LMENU,			//Alt(左)
		DIK_RMENU,			//Alt(右)
		DIK_SPACE,			//Space
		DIK_BACK	,		//Back Space
		DIK_TAB,			//Tab
		DIK_1,     			//1
		DIK_2,     			//2
		DIK_3,     			//3
		DIK_4,     			//4
		DIK_5,     			//5
		DIK_6,     			//6
		DIK_7,     			//7
		DIK_8,     			//8
		DIK_9,     			//9
		DIK_0,     			//0
		DIK_Q,				//Q
		DIK_W,				//W
		DIK_E,				//E
		DIK_R,				//R
		DIK_T,				//T
		DIK_Y,				//Y
		DIK_U,				//U
		DIK_I,				//I
		DIK_O,				//O
		DIK_P,				//P
		DIK_A,				//A
		DIK_S,				//S
		DIK_D,				//D
		DIK_F,				//F
		DIK_G,				//G
		DIK_H,				//H
		DIK_J,				//J
		DIK_K,				//K
		DIK_L,				//L
		DIK_Z,				//Z
		DIK_X,				//X
		DIK_C,				//C
		DIK_V,				//V
		DIK_B,				//B
		DIK_N,				//N
		DIK_M,				//M			
		DIK_F1,				//F1
		DIK_F2,				//F2
		DIK_F3,				//F3
		DIK_F4,				//F4
		DIK_F5,				//F5
		DIK_F6,				//F6
		DIK_F7,				//F7
		DIK_F8,				//F8
		DIK_F9,				//F9
		DIK_F10,				//F10		
	};
	memcpy_s(dikCode, sizeof(dikCode), keyList, sizeof(keyList));
	return true;
}

void Input::UpDate()
{
	//前のバッファを保存
	for (int i = 0; i < 256; ++i)
	{
		prebuf[i] = buf[i];
	}
	HRESULT hr = pKey->GetDeviceState(sizeof(buf), &buf);
	if (FAILED(hr))
	{
		//失敗なら再開させてもう一度取得
		pKey->Acquire();
		//バッファに入力を保存
		pKey->GetDeviceState(sizeof(buf), &buf);
	}
}

bool Input::Press(const int key)
{
	//入力を保存
	unsigned char input = 0x00;	//0x00(押してない)0x80(押してる)
	unsigned char prevInput = 0x00;
	//入力番号によって変える
	input = buf[dikCode[key]] & 0x80;
	prevInput = prebuf[dikCode[key]] & 0x80;
	//前の入力が無い状態で,現在押されていたら
	return input != 0 && prevInput == 0;
}

bool Input::Push(const int key)
{

	unsigned char input = 0x00;

	input = this->buf[dikCode[key]] & 0x80;
	//押されているか
	return input != 0;
}

bool Input::Free(const int key)
{
	//入力保存用
	unsigned char input = 0x00;
	//入力番号によって変える
	input = this->buf[dikCode[key]] & 0x80;
	//押されていないか
	return input == 0;
}

bool Input::Pull(const int key)
{
	//入力保存用
	unsigned char input = 0x00;
	unsigned char prevInput = 0x00;
	//入力番号によって変える
	input = this->buf[dikCode[key]] & 0x80;
	prevInput = this->prebuf[dikCode[key]] & 0x80;
	//前の入力があり現在の入力があるか
	return input == 0 && prevInput != 0;
}
//global------------------------------------
PadInput pad;
//------------------------------------------

//static------------------------------------
int PadInput::padNo = 0;
LPDIRECTINPUTDEVICE8 PadInput::pPad[PadInput::padNum] =
{
	nullptr
};

BOOL CALLBACK PadInput::EnumPadCallback(const DIDEVICEINSTANCE* inst, void* pContext)
{

	//保存
	static GUID padGuid[padNum];
	//すでに取得済みのデバイスだった場合にはもう一度実行する
	for (int no = 0; no < padNo; ++no)
	{
		if (inst->guidInstance == padGuid[no])
		{
			return DIENUM_CONTINUE;	//列挙を続行
		}
	}
	if (FAILED(in.GetpDI()->CreateDevice(
		inst->guidInstance,
		&pPad[padNo],
		NULL)))
	{
		return DIENUM_CONTINUE;
	}
	padGuid[padNo] = inst->guidInstance;
	return DIENUM_STOP;				//列挙を停止
}

BOOL CALLBACK PadInput::EnumAxisCallback(
	const DIDEVICEOBJECTINSTANCE* objInst,
	void* pContext)
{
	//軸範囲指定
	DIPROPRANGE prg;
	prg.diph.dwSize = sizeof(prg);
	prg.diph.dwHeaderSize = sizeof(prg.diph);
	prg.diph.dwHow = DIPH_BYID;
	prg.diph.dwObj = objInst->dwType;
	prg.lMin = -1000;	//傾き最小値
	prg.lMax = 1000;	//傾き最大値

	HRESULT hr = pPad[padNo]->SetProperty(
		DIPROP_RANGE,
		&prg.diph);
	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}
//------------------------------------------

PadInput::PadInput() : didevCaps(), buf(), prebuf()
{

}

PadInput::~PadInput()
{
	for (padNo = 0; padNo < padNum; ++padNo)
	{
		//パッドが接続されてなければ終了
		if (!pPad[padNo])
		{
			continue;
		}
		//入力を受け付けない
		pPad[padNo]->Unacquire();
		//デバイス開放
		if (pPad[padNo] != nullptr)
		{
			pPad[padNo]->Release();
		}
	}
}

void PadInput::CreatePadInput(const HWND& hwnd)
{
	for (padNo = 0; padNo < padNum; ++padNo)
	{

		//DirectInputデバイス作成
		if (FAILED(in.GetpDI()->EnumDevices(
			DI8DEVCLASS_GAMECTRL,
			EnumPadCallback,
			NULL, DIEDFL_ATTACHEDONLY)))
		{
			continue;
		}
		//取得に失敗したら終了
		if (!pPad[padNo])
		{
			continue;
		}
		//ジョイパッドのデータフォーマットの設定
		if (FAILED(pPad[padNo]->SetDataFormat(&c_dfDIJoystick)))
		{
			continue;
		}
		//強調レベル設定
		if (FAILED(pPad[padNo]->SetCooperativeLevel(
			hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		{
			continue;
		}
		//パッドデータ取得
		this->didevCaps[padNo].dwSize = sizeof(DIDEVCAPS);
		if (FAILED(pPad[padNo]->GetCapabilities(&didevCaps[padNo])))
		{
			continue;
		}
		//パッド設定
		if (FAILED(pPad[padNo]->EnumObjects(
			EnumAxisCallback,
			hwnd, DIDFT_AXIS)))
		{
			continue;
		}
		//デバイスの取得
		if (FAILED(pPad[padNo]->Poll()))
		{
			while (pPad[padNo]->Acquire() == DIERR_INPUTLOST);
		}
	}
}
void PadInput::UpDate()
{
	for (padNo = 0; padNo < padNum; ++padNo) {
		//パッドを取得していなければ処理を行わない
		if (!pPad[padNo])
		{
			continue;
		}
		HRESULT hr = pPad[padNo]->Acquire();

		//消失したデバイスを取得したか、消失していなかったら
		if (hr == DI_OK || hr == S_FALSE)
		{
			//データの取得
			if (SUCCEEDED(pPad[padNo]->Poll()))
			{
				//前のバッファを保存
				prebuf[padNo] = buf[padNo];
				//バッファに入力を保存
				pPad[padNo]->GetDeviceState(sizeof(DIJOYSTATE), &buf);
			}
		}
	}
}

bool PadInput::Press(const int key, const int no)
{
	if (!pPad[0])
	{
		return false;
	}
	unsigned char input = buf[no].rgbButtons[key] & 0x80;

	return input != 0;
}

bool PadInput::Push(const int key, const int no)
{
	if (!pPad[0])
	{
		return false;
	}
	unsigned char input = buf[no].rgbButtons[key] & 0x80;
	unsigned char prevInput = prebuf[no].rgbButtons[key] & 0x80;

	return input != 0 && prevInput == 0;
}

bool PadInput::Free(const int key_, const int no_)
{
	unsigned char input = buf[no_].rgbButtons[key_] & 0x80;

	return input == 0;
}

bool PadInput::Pull(const int key, const int no)
{
	unsigned char input = buf[no].rgbButtons[key] & 0x80;
	unsigned char prevInput = prebuf[no].rgbButtons[key] & 0x80;

	return input == 0 && prevInput != 0;
}

int PadInput::GetPovPosition(const int no)
{
	if (!pPad[0])
	{
		return 0xffffffff;	//パッドが接続されていなければ未押下状態を返す
	}
	switch (buf[no].rgdwPOV[no])
	{

	case 0:
		return PAD_UP;
	case 4500:
		return PAD_UP | PAD_RIGHT;
	case 9000:
		return PAD_RIGHT;
	case 13500:
		return PAD_RIGHT | PAD_DOWN;
	case 18000:
		return PAD_DOWN;
	case 22500:
		return PAD_DOWN | PAD_LEFT;
	case 27000:
		return PAD_LEFT;
	case 31500:
		return PAD_LEFT | PAD_UP;
	}

	return 0xffffffff;	//未押下時

}

long PadInput::GetLStickXAngle(const int no)
{
	if (!pPad[0])
	{
		return 0xffffffff;	//パッドが接続されていなければ未押下状態を返す
	}
	
	return buf[no].lX;
}

long PadInput::GetLStickYAngle(const int no)
{
	if (!pPad[0])
	{
		return 0xffffffff;	//パッドが接続されていなければ未押下状態を返す
	}

	return buf[no].lY;
}


//-------------------
DxMouse mouse;
//-------------------

DxMouse::DxMouse() :
	pDIMouse(nullptr)
{
	pos = { 0,0 };
	pMouseState = { 0 };
}
DxMouse::~DxMouse()
{
	if (pDIMouse)
	{
		pDIMouse->Release();
		pDIMouse = nullptr;
	}

}

bool DxMouse::CreateMousenput(const HWND & hwnd)
{
	pwnd = hwnd;
	HRESULT ret;
	// マウス用にデバイスオブジェクトを作成
	ret = in.GetpDI()->CreateDevice(GUID_SysMouse, &pDIMouse, NULL);
	if (FAILED(ret))
	{
		MessageBox(NULL, "マウスデバイスの作成に失敗しました", "Error", MB_OK);
		return false;
	}

	// データフォーマットを設定
	ret = pDIMouse->SetDataFormat(&c_dfDIMouse);	// マウス用のデータ・フォーマットを設定
	if (FAILED(ret))
	{
		MessageBox(NULL, "マウスデータフォーマットの設定に失敗しました", "Error", MB_OK);
		// データフォーマットに失敗
		return false;
	}

	// モードを設定（フォアグラウンド＆非排他モード）
	ret = pDIMouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(ret))
	{
		MessageBox(NULL, "マウスモードの設定に失敗しました", "Error", MB_OK);
		// モードの設定に失敗
		return false;
	}

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	ret = pDIMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(ret))
	{
		MessageBox(NULL, "マウスデバイスの設定に失敗しました", "Error", MB_OK);
		// デバイスの設定に失敗
		return false;
	}
	//入力制御開始
	pDIMouse->Acquire();

	return true;
}
void DxMouse::Run()
{
	// 読取前の値を保持します
	DIMOUSESTATE g_zdiMouseState_bak;	// マウス情報(変化検知用)
	memcpy(&g_zdiMouseState_bak, &pMouseState, sizeof(g_zdiMouseState_bak));
	HRESULT	hr = pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &pMouseState);
	if (hr == DIERR_INPUTLOST) {
		pDIMouse->Acquire();
		hr = pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &pMouseState);
	}

}

bool DxMouse::LPush()
{
	if (pMouseState.rgbButtons[0] & 0x80)
	{
		return true;
	}

	return false;
}
bool DxMouse::RPush()
{
	if (pMouseState.rgbButtons[1] & 0x80)
	{
		return true;
	}
	return false;
}

POINT DxMouse::GetMousePosClient()
{
	GetCursorPos(&mouse.pos);
	ScreenToClient(pwnd, &mouse.pos);
	return mouse.pos;
}

POINT DxMouse::GetMousePosClientCenter()
{
	POINT point = {};
	point.y = (Mouse::GetMousePos().y - (Engine::GetWindowSize().y / 2));
	point.x = (Mouse::GetMousePos().x - (Engine::GetWindowSize().x / 2));
	return point;
}

void DxMouse::SetMousePos(int x, int y)
{
	if (GetActiveWindow() != Engine::GetWindowHandle())
		return;

	POINT point = {};
	point.x = (int)x + Engine::GetWindowSize().x / 2;
	point.y = (int)-y + Engine::GetWindowSize().y / 2;
	ClientToScreen(Engine::GetWindowHandle(), &point);
	SetCursorPos(point.x, point.y);
	mouse.pos.x = x;
	mouse.pos.y = y;
}

void DxMouse::DrawCursor(bool enable)
{
	ShowCursor(enable);
}
