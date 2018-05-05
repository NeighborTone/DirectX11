#include "System.h"
#include "SystemDefs.h"
System::System()
{
	
}

System::~System()
{
	DestroyWindow(handle);		//�E�B���h�E�̔j��
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT mes, WPARAM wParam, LPARAM lParam)
{

	switch (mes)
	{
	case WM_DESTROY:
		PostQuitMessage(0);		//�A�v���P�[�V�����̏I��
		DestroyWindow(hwnd);
		return 0;


	default:
		break;
	}
	//ESCAPE�ł��A�v���P�[�V�����̏I��
	if (GetKeyState(VK_ESCAPE) & 0x8000)
	{
		PostQuitMessage(0);		
		DestroyWindow(hwnd);
		return 0;
	}

	//�f�t�H���g�E�B���h�E���
	return DefWindowProc(hwnd, mes, wParam, lParam);
}
bool System::Create(std::string str ,HINSTANCE& hInstance, int& nCmdShow)
{
	WNDCLASSEX wcex;
	SecureZeroMemory(&wcex, sizeof(wcex));
	
	wcex.lpfnWndProc = WndProc;												//�E�B���h�E�v���V�[�W���̱��ڽ
	wcex.lpszClassName = str.c_str();											//�E�B���h�E�N���X�̖��O
	wcex.lpszMenuName = str.c_str();											//���j���[
	wcex.style = WS_OVERLAPPED;												//�E�B���h�E�X�^�C��
	wcex.cbSize = sizeof(WNDCLASSEX);										//�\���̂̃T�C�Y
	wcex.cbClsExtra = 0;																//�g���N���X�������Ȃ�
	wcex.cbWndExtra = 0;															//�g���E�B���h�E�������Ȃ�
	wcex.hInstance = hInstance;													//WinMain()�̃C���X�^���X�n���h��
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);		//�N���C�A���g�̈�̔w�i�F(�f�t�H���g)
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);					//���O��`����Ă�����J�[�\��
	wcex.hIcon = LoadIcon(hInstance, "MYICON");;						//�A�C�R��
	wcex.hIconSm = NULL;															//�������A�C�R��
	ins = hInstance;
	if (!RegisterClassEx(&wcex)) 
	{
		MessageBox(NULL, "�E�B���h�E�̓o�^�Ɏ��s���܂���", "Error", MB_OK); 
		return false;
	}

	//�E�C���h�E����
	handle = CreateWindow(
				str.c_str(),				//�E�B���h�E�N���X�̖��O
				str.c_str(),				//�^�C�g���o�[�̃e�L�X�g
				WS_OVERLAPPEDWINDOW,	//�E�B���h�E�̃X�^�C��
				Defs::WINDOW_POSX,		//�E�B���h�E�̐����ʒu�̃f�t�H���g
				Defs::WINDOW_POSY,		//�E�B���h�E�̐����ʒu�̃f�t�H���g
				Defs::SCREEN_WIDTH,		//��
				Defs::SCREEN_HEIGHT,		//����
				NULL,							//�e�E�B���h�E�Ȃ�
				NULL,							//���j���[�Ȃ�
				hInstance,					//�A�v���P�[�V�����C���X�^���X�ւ̃n���h��
				NULL);						//�E�B���h�E�p�����[�^�Ȃ�
	//�����`�F�b�N
	if (!handle)
	{ 
		MessageBox(NULL, "�E�B���h�E�̐����Ɏ��s���܂���", "Error", MB_OK);
		return false;
	}

	//�E�B���h�E�쐬
	ShowWindow(handle, nCmdShow);
	SetForegroundWindow(handle);
	//�E�B���h�E�v���V�[�W����WM_PAINT���b�Z�[�W�𑗂�
	UpdateWindow(handle);
	
	return true;
}

HWND System::GetWindow()
{
	return handle;
}