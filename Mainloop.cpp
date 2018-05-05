#include "Mainloop.h"
#include "SystemDefs.h"

bool Mainloop::Init(HINSTANCE hInstance, int nCmdShow)
{
	//�E�B���h�E����
	const bool iswin = 	window.Create("title",hInstance, nCmdShow);
	//DirectX�̃f�o�C�X����
	const bool isEngine = Engine::GetInst()->Create(window.GetWindow());
	//�E�B���h�E�`��
	const bool isshow = ShowWindow(window.GetWindow(), nCmdShow);
	if (iswin  && isshow && isEngine)
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
		Engine::GetInst()->Run();
		Engine::GetInst()->Draw();

		}

	//	ShowCursor(false);	//�}�E�X��\��
	
	}
	Engine::GetInst()->Release();
}
