#include "System.h"
#include "../Engine.h"
#include "../Utilitys/Utility.hpp"

System::System(std::string title, int width, int height)
{
	Engine::COMInitialize();
	Create(title,width,height);
}

System::~System()
{
	
	CoUninitialize();		//COM�I�u�W�F�N�g�̔j���B�K�������ŌĂ�
}

bool System::UpDate()
{
	//���b�Z�[�W���擾
	GetMessage(&msg, NULL, 0, 0);
	//���b�Z�[�W���[�v(���͂Ȃǂ̖��߂�ǂ�)
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	PostMessage(handle, WM_APP, 0, 0);
	if (GetSize().x <= 0.0f || GetSize().y <= 0.0f)
		Sleep(100);
	return true;

}

void System::SetSize(int width, int height)
{
	RECT windowRect = {};
	RECT clientRect = {};
	GetWindowRect(handle, &windowRect);
	GetClientRect(handle, &clientRect);

	int w = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left) + width;
	int h = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top) + height;
	int x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
	int y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

	SetWindowPos(handle, nullptr, x, y, w, h, SWP_FRAMECHANGED);
}

bool System::Create(std::string str, int width, int height)
{
	WNDCLASSEX wcex;
	HINSTANCE instance = GetModuleHandle(nullptr);
	SecureZeroMemory(&wcex, sizeof(wcex));
	
	wcex.lpfnWndProc = WinProc;									//�E�B���h�E�v���V�[�W���̱��ڽ
	wcex.lpszClassName = str.c_str();							//�E�B���h�E�N���X�̖��O
	wcex.lpszMenuName = str.c_str();							//���j���[
	wcex.style = WS_OVERLAPPED;									//�E�B���h�E�X�^�C��
	wcex.cbSize = sizeof(WNDCLASSEX);							//�\���̂̃T�C�Y
	wcex.cbClsExtra = 0;												//�g���N���X�������Ȃ�
	wcex.cbWndExtra = 0;												//�g���E�B���h�E�������Ȃ�
	wcex.hInstance = instance;										//WinMain()�̃C���X�^���X�n���h��
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);		//�N���C�A���g�̈�̔w�i�F(�f�t�H���g)
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);			//���O��`����Ă�����J�[�\��
	wcex.hIcon = LoadIcon(instance, "MYICON");				//�A�C�R��
	wcex.hIconSm = NULL;												//�������A�C�R��
	ins = instance;

	ErrorMessage(RegisterClassEx(&wcex), "�E�B���h�E�̓o�^�Ɏ��s���܂���", "Error");

	this->width = width;
	this->height = height;
	//�E�C���h�E����
	handle = CreateWindow(
				str.c_str(),				//�E�B���h�E�N���X�̖��O
				str.c_str(),				//�^�C�g���o�[�̃e�L�X�g
				WS_OVERLAPPEDWINDOW,	//�E�B���h�E�̃X�^�C��
				200,							//�E�B���h�E�̐����ʒu�̃f�t�H���g
				50,								//�E�B���h�E�̐����ʒu�̃f�t�H���g
				0,						//��
				0,			 			//����
				NULL,							//�e�E�B���h�E�Ȃ�
				NULL,							//���j���[�Ȃ�
				instance,					//�A�v���P�[�V�����C���X�^���X�ւ̃n���h��
				NULL);						//�E�B���h�E�p�����[�^�Ȃ�
	//�����`�F�b�N
	ErrorMessage(handle, "�E�B���h�E�̐����Ɏ��s���܂���", "Error");

	//�E�B���h�E�T�C�Y�Ə������W�ʒu
	SetSize(width,height);
	SetWindowLongPtr(handle, GWLP_WNDPROC, (LONG_PTR)WinProc);
	//�E�B���h�E�쐬
	ShowWindow(handle, SW_SHOWNORMAL);

	//�E�B���h�E�v���V�[�W����WM_PAINT���b�Z�[�W�𑗂�
	UpdateWindow(handle);
	
	return true;
}

HWND System::GetHwnd()
{

	return handle;
}

void System::SetFullScreen(bool isFullScreen)
{
	static DirectX::XMINT2 size = GetSize();

	if (!isFullScreen)
	{
		size = GetSize();
		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);
		SetWindowLongPtrW(handle, GWL_STYLE, WS_VISIBLE | WS_POPUP);
		SetWindowPos(handle, HWND_TOP, 0, 0, w, h, SWP_FRAMECHANGED);
	}
	else
	{
		SetWindowLongPtrW(handle, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
		SetWindowPos(handle, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
		SetSize(size.x, size.y);
	}
}

DirectX::XMINT2 System::GetSize()
{
	RECT clientRect = {};
	GetClientRect(handle, &clientRect);

	return DirectX::XMINT2(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
}

LRESULT System::WinProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	{
		for (auto it : GetProcedures())
		{
			it->OnProceed(window, message, wParam, lParam);
		}

		if (message == WM_DESTROY)
			PostQuitMessage(0);

		return DefWindowProc(window, message, wParam, lParam);
	}
}

