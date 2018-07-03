#pragma once
#define DIRECTINPUT_VERSION 0x0800
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include <dinput.h>

class Input
{
private:
	LPDIRECTINPUT8 pDI;
	LPDIRECTINPUTDEVICE8 pKey;
	unsigned char buf[256];
	unsigned char prebuf[256];
	static const int MAX_KEY = 61;
	int dikCode[MAX_KEY];
public:

	Input();
	~Input();
	bool CreateInput(const HWND& hwnd);
	void UpDate();
	//������
	bool Press(const int key);
	//�����Ă���
	bool Push(const int key);
	//������Ă��Ȃ�
	bool Free(const int key);
	//�����ꂽ
	bool Pull(const int key);
	LPDIRECTINPUT8 GetpDI() const
	{
		return pDI;
	}
};
// �p�b�h�̕����L�[�p  
#define PAD_UP 0x1  
#define PAD_RIGHT 0x2  
#define PAD_DOWN 0x4  
#define PAD_LEFT 0x8  
class PadInput
{
private:

	//�ڑ�����Ă���pad�����m����
	static BOOL CALLBACK EnumPadCallback(
		const DIDEVICEINSTANCE* inst,
		void* pContext
	);
	//�p�b�h�̐ݒ�
	static BOOL CALLBACK EnumAxisCallback(
		const DIDEVICEOBJECTINSTANCE* objInst,
		void* pContext);
	//���ݎg�p���Ă���p�b�h
	static int padNo;
	//�g�p����p�b�h��(�ő�4�܂�)
	static const int padNum = 1;
	//DirectInput�f�o�C�X�ւ̃|�C���^(�ő�4�܂�)
	static LPDIRECTINPUTDEVICE8 pPad[padNum];
	//�f�o�C�X���ۑ��p
	DIDEVCAPS didevCaps[padNum];
	//���݂̓��͕ۑ��p
	DIJOYSTATE2 buf[padNum];
	//���O�̓��͕ۑ�
	DIJOYSTATE2 prebuf[padNum];

public:
	PadInput();
	~PadInput();

	void CreatePadInput(const HWND& hwnd);
	void UpDate();
	//������
	bool Press(const int key, const int no);
	//������Ă���
	bool Push(const int key, const int no);
	//�͂Ȃ���Ă���
	bool Free(const int key, const int no);
	//�͂Ȃ��ꂽ
	bool Pull(const int key, const int no);
	// �\���L�[�̈ʒu���擾 
	int GetPovPosition(const int no);

	long GetLStickXAngle(const int no);

	long GetLStickYAngle(const int no);
};

class DxMouse
{
private:
	LPDIRECTINPUTDEVICE8 pDIMouse;	//�}�E�X�f�o�C�X
	DIMOUSESTATE pMouseState;				//�}�E�X���
	HWND pwnd;

	int buttonCnt[2] = { 0,0 };
public:
	DxMouse();
	~DxMouse();
	POINT pos = { 0,0 };
	bool CreateMousenput(const HWND& hwnd);

	void Run();
	//������Ă���t���[�������Ԃ��Ă���
	int LPush();
	int RPush();
	//�}�E�X�̃N���C�A���g�̈�̍��W���擾
	POINT GetMousePosClient();
	//�}�E�X�̃N���C�A���g�̈�̍��W(0,0)�𒆐S�Ƃ������W���擾
	POINT GetMousePosClientCenter();
	//�}�E�X�̃N���C�A���g�̈�̍��W(0,0)�𒆐S�Ƃ������W��ݒ�
	void SetMousePos(int x, int y);
	//�}�E�X�J�[�\����`�悷�邩���Ȃ���
	void DrawCursor(bool enable);
};
extern Input in;	//DirectX���������ɐ�������
extern PadInput pad;
extern DxMouse mouse;
