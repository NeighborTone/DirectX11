#include "Direct_Input.h"


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
	//���͂��󂯕t���Ȃ�����(�f�o�C�X�ւ̃A�N�Z�X�����������B)
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
	//IDirectInput8�̍쐬
	HRESULT hr = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(LPVOID*)&pDI,
		NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, "DirectInput�I�u�W�F�N�g�̐����Ɏ��s���܂���", "Error", MB_OK);
		return false;
	}

	//�f�o�C�X�̎擾
	hr = pDI->CreateDevice(
		GUID_SysKeyboard,
		&pKey,
		NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�L�[�{�[�h�f�o�C�X�̍쐬�Ɏ��s���܂���", "Error", MB_OK);
		return false;
	}

	//���̓f�[�^�̃t�H�[�}�b�g
	hr = pKey->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(NULL, "���̓f�[�^�̐ݒ�Ɏ��s���܂���", "Error", MB_OK);
		return false;
	}

	//�r������
	hr = pKey->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�r������̐ݒ�Ɏ��s���܂���", "Error", MB_OK);
		return false;
	}
	//����J�n
	pKey->Acquire();
	int keyList[Input::MAX_KEY] =
	{
		DIK_UP,				//��				
		DIK_LEFT,			//��
		DIK_RIGHT,			//��			
		DIK_DOWN	,		//��
		DIK_ESCAPE,			//Esc
		DIK_RETURN,	        //Enter
		DIK_LSHIFT,	        //Shift(��)	
		DIK_RSHIFT,			//Shift(�E)
		DIK_LCONTROL,	    //Ctrl(��)
		DIK_RCONTROL,	    //Ctrl(�E)					
		DIK_LMENU,			//Alt(��)
		DIK_RMENU,			//Alt(�E)
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
	//�O�̃o�b�t�@��ۑ�
	for (int i = 0; i < 256; ++i)
	{
		prebuf[i] = buf[i];
	}
	HRESULT hr = pKey->GetDeviceState(sizeof(buf), &buf);
	if (FAILED(hr))
	{
		//���s�Ȃ�ĊJ�����Ă�����x�擾
		pKey->Acquire();
		//�o�b�t�@�ɓ��͂�ۑ�
		pKey->GetDeviceState(sizeof(buf), &buf);
	}
}

bool Input::Press(const int key)
{
	//���͂�ۑ�
	unsigned char input = 0x00;	//0x00(�����ĂȂ�)0x80(�����Ă�)
	unsigned char prevInput = 0x00;
	//���͔ԍ��ɂ���ĕς���
	input = buf[dikCode[key]] & 0x80;
	prevInput = prebuf[dikCode[key]] & 0x80;
	//�O�̓��͂�������Ԃ�,���݉�����Ă�����
	return input != 0 && prevInput == 0;
}

bool Input::Push(const int key)
{

	unsigned char input = 0x00;

	input = this->buf[dikCode[key]] & 0x80;
	//������Ă��邩
	return input != 0;
}

bool Input::Free(const int key)
{
	//���͕ۑ��p
	unsigned char input = 0x00;
	//���͔ԍ��ɂ���ĕς���
	input = this->buf[dikCode[key]] & 0x80;
	//������Ă��Ȃ���
	return input == 0;
}

bool Input::Pull(const int key)
{
	//���͕ۑ��p
	unsigned char input = 0x00;
	unsigned char prevInput = 0x00;
	//���͔ԍ��ɂ���ĕς���
	input = this->buf[dikCode[key]] & 0x80;
	prevInput = this->prebuf[dikCode[key]] & 0x80;
	//�O�̓��͂����茻�݂̓��͂����邩
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
	//�ۑ�
	static GUID padGuid[padNum];
	//���łɎ擾�ς݂̃f�o�C�X�������ꍇ�ɂ͂�����x���s����
	for (int no = 0; no < padNo; ++no)
	{
		if (inst->guidInstance == padGuid[no])
		{
			return DIENUM_CONTINUE;	//�񋓂𑱍s
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
	return DIENUM_STOP;				//�񋓂��~
}

BOOL CALLBACK PadInput::EnumAxisCallback(
	const DIDEVICEOBJECTINSTANCE* objInst,
	void* pContext)
{
	//���͈͎w��
	DIPROPRANGE prg;
	prg.diph.dwSize = sizeof(prg);
	prg.diph.dwHeaderSize = sizeof(prg);
	prg.diph.dwHow = DIPH_BYID;
	prg.diph.dwObj = objInst->dwType;
	prg.lMin = -1000;	//�X���ŏ��l
	prg.lMax = 1000;	//�X���ő�l

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
		//�p�b�h���ڑ�����ĂȂ���ΏI��
		if (!pPad[padNo])
		{
			continue;
		}
		//���͂��󂯕t���Ȃ�
		pPad[padNo]->Unacquire();
		//�f�o�C�X�J��
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

		//DirectInput�f�o�C�X�쐬
		if (FAILED(in.GetpDI()->EnumDevices(
			DI8DEVCLASS_GAMECTRL,
			EnumPadCallback,
			NULL, DIEDFL_ATTACHEDONLY)))
		{
			continue;
		}
		//�擾�Ɏ��s������I��
		if (!pPad[padNo])
		{
			continue;
		}
		//�W���C�p�b�h�̃f�[�^�t�H�[�}�b�g�̐ݒ�
		if (FAILED(pPad[padNo]->SetDataFormat(&c_dfDIJoystick)))
		{
			continue;
		}
		//�������x���ݒ�
		if (FAILED(pPad[padNo]->SetCooperativeLevel(
			hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		{
			continue;
		}
		//�p�b�h�f�[�^�擾
		this->didevCaps[padNo].dwSize = sizeof(DIDEVCAPS);
		if (FAILED(pPad[padNo]->GetCapabilities(&didevCaps[padNo])))
		{
			continue;
		}
		//�p�b�h�ݒ�
		if (FAILED(pPad[padNo]->EnumObjects(
			EnumAxisCallback,
			hwnd, DIDFT_AXIS)))
		{
			continue;
		}
		//�f�o�C�X�̎擾
		if (FAILED(pPad[padNo]->Poll()))
		{
			while (pPad[padNo]->Acquire() == DIERR_INPUTLOST);
		}
	}
}
void PadInput::UpDate()
{
	for (padNo = 0; padNo < padNum; ++padNo) {
		//�p�b�h���擾���Ă��Ȃ���Ώ������s��Ȃ�
		if (!pPad[padNo])
		{
			continue;
		}
		HRESULT hr = pPad[padNo]->Acquire();

		//���������f�o�C�X���擾�������A�������Ă��Ȃ�������
		if (hr == DI_OK || hr == S_FALSE)
		{
			//�f�[�^�̎擾
			if (SUCCEEDED(pPad[padNo]->Poll()))
			{
				//�O�̃o�b�t�@��ۑ�
				prebuf[padNo] = buf[padNo];
				//�o�b�t�@�ɓ��͂�ۑ�
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

BYTE PadInput::GetPovPosition(const int no)
{
	if (!pPad[0])
	{
		return (BYTE)0xffffffff;	//�p�b�h���ڑ�����Ă��Ȃ���Ζ�������Ԃ�Ԃ�
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

	return (BYTE)0xffffffff;	//��������

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
	// �}�E�X�p�Ƀf�o�C�X�I�u�W�F�N�g���쐬
	ret = in.GetpDI()->CreateDevice(GUID_SysMouse, &pDIMouse, NULL);
	if (FAILED(ret))
	{
		MessageBox(NULL, "�}�E�X�f�o�C�X�̍쐬�Ɏ��s���܂���", "Error", MB_OK);
		return false;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	ret = pDIMouse->SetDataFormat(&c_dfDIMouse);	// �}�E�X�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
	if (FAILED(ret))
	{
		MessageBox(NULL, "�}�E�X�f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s���܂���", "Error", MB_OK);
		// �f�[�^�t�H�[�}�b�g�Ɏ��s
		return false;
	}

	// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	ret = pDIMouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(ret))
	{
		MessageBox(NULL, "�}�E�X���[�h�̐ݒ�Ɏ��s���܂���", "Error", MB_OK);
		// ���[�h�̐ݒ�Ɏ��s
		return false;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j

	ret = pDIMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(ret))
	{
		MessageBox(NULL, "�}�E�X�f�o�C�X�̐ݒ�Ɏ��s���܂���", "Error", MB_OK);
		// �f�o�C�X�̐ݒ�Ɏ��s
		return false;
	}
	//���͐���J�n
	pDIMouse->Acquire();

	return true;
}
void DxMouse::UpDate()
{
	// �ǎ�O�̒l��ێ����܂�
	DIMOUSESTATE g_zdiMouseState_bak;	// �}�E�X���(�ω����m�p)
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

POINT DxMouse::GetMousePos()
{
	GetCursorPos(&mouse.pos);
	ScreenToClient(pwnd, &mouse.pos);
	return mouse.pos;
}

