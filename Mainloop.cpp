#include "Mainloop.h"


bool Mainloop::Init(HINSTANCE hInstance, int nCmdShow)
{
	//�E�B���h�E����
	const bool iswin = 	window.Create("MyFrame",hInstance, nCmdShow);
	//DirectX�̃f�o�C�X����

	//�E�B���h�E�`��
	const bool isshow = ShowWindow(window.GetWindow(), nCmdShow);
	if (iswin  && isshow)
	{
		
		return true;
	}
	return false;
}


void Mainloop::Run()
{
	//���b�Z�[�W���擾
	GetMessage(&msg, NULL, 0, 0);
	//���b�Z�[�W���[�v(���͂Ȃǂ̖��߂�ǂ�)
	while (msg.message != WM_QUIT)
	{
	
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//���b�Z�[�W���f�R�[�h����WinProc�ɓn��
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
	
		}

	//	ShowCursor(false);	//�}�E�X��\��
	
	}

}
