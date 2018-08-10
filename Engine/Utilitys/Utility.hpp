#pragma once
#pragma warning (disable : 4458)	//this���g�����ߏ���
#include <Windows.h>
#include <DirectXMath.h>
#include <math.h>
#include <stdint.h>
#include <iostream>

namespace Memory
{
	template<class T>void SafeDelete(T& t)
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}
	template<class T>void SafeDeleteArray(T& t)
	{
		if (t)
		{
			delete[] t;
			t = nullptr;
		}
	}
	template<class T>void SafeRelease(T& t)
	{
		if (t)
		{
			t->Release();
			t = nullptr;
		}
	}
}


/*! @class ErrorMessage
*   @brief �G���[���b�Z�[�W�����b�Z�[�W�{�b�N�X�ŏo���܂�	
*   @detail �R���X�g���N�^�Ń`�F�b�N�p�ϐ��ƕ������ݒ肵�Ă�������
*   @detail HRUSULT, int, bool�^�̂����ꂩ�Ń`�F�b�N�ł��܂�
*/
class Message
{
public:
	Message(HRESULT hr, const char* text, const char* category)
	{
		if (FAILED(hr))
		{
			MessageBox(NULL, text, category, MB_OK);
		}
	}
	Message(int isTrue, const char* text, const char* category)
	{
		if (!isTrue)
		{
			MessageBox(NULL, text, category, MB_OK);
		}
	}
	Message(bool isTrue, const char* text, const char* category)
	{
		if (!isTrue)
		{
			MessageBox(NULL, text, category, MB_OK);
		}
	}
	Message(const char* text, const char* category)
	{
		MessageBox(NULL, text, category, MB_OK);
	}
	static bool IsFullScreen()
	{
		int flag;
		flag = MessageBox(
			NULL,
			TEXT("�t���X�N���[�����[�h�ŋN�����܂����H"),
			TEXT("�X�N���[���ݒ�"),
			MB_YESNO | MB_ICONQUESTION);
		if (flag == IDNO)
		{
			return true;
		}
		return false;
	}
};
//�f�o�b�O�r���h�̂ݗL���ɂ���
#ifdef _DEBUG
#define DOUT std::cout
#else 
#define DOUT 0 && std::cout
#endif