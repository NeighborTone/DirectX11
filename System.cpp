#include "System.h"

System::System()
{
	SetWindowSize();
}

System::~System()
{
	DestroyWindow(handle);		//�E�B���h�E�̔j��
}

void System::SetWindowSize(UINT w, UINT h)
{
	SCREEN_W = w;
	SCREEN_H = h;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam)
{

	switch (mes)
	{
	case WM_DESTROY:
		PostQuitMessage(0);		//�A�v���P�[�V�����̏I��
		return 0;


	default:
		break;
	}
	//ESCAPE�ł��A�v���P�[�V�����̏I��
	if (GetKeyState(VK_ESCAPE) & 0x8000)
	{
		PostQuitMessage(0);		
		return 0;
	}

	//�f�t�H���g�E�B���h�E���
	return DefWindowProc(hWnd, mes, wParam, lParam);
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
				200,							//�E�B���h�E�̐����ʒu�̃f�t�H���g
				50,								//�E�B���h�E�̐����ʒu�̃f�t�H���g
				Width(),						//��
				Height(),						//����
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

	//�E�B���h�E�v���V�[�W����WM_PAINT���b�Z�[�W�𑗂�
	UpdateWindow(handle);
	
	return true;
}

HWND System::GetWindow()
{
	return handle;
}