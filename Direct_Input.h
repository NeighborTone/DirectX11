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
	DIJOYSTATE buf[padNum];
	//���O�̓��͕ۑ�
	DIJOYSTATE prebuf[padNum];

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

	BYTE GetPovPosition(const int no);          // �\���L�[�̈ʒu���擾 
};

class DxMouse
{
private:
	LPDIRECTINPUTDEVICE8 pDIMouse;	//�}�E�X�f�o�C�X
	DIMOUSESTATE pMouseState;				//�}�E�X���
	HWND pwnd;
public:
	DxMouse();
	~DxMouse();
	POINT pos;
	bool CreateMousenput(const HWND& hwnd);

	void UpDate();
	//������Ă���
	bool LPush();

	bool RPush();
	POINT GetMousePos();
};
extern Input in;	//DirectX���������ɐ�������
extern PadInput pad;
extern DxMouse mouse;
