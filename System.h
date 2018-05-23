#pragma once
#include <Windows.h>
#include <tchar.h>
#include <string>
#include <DirectXMath.h>
#include <forward_list>

/*! @class System
*   @brief �E�B���h�E�𐶐����܂�
*/
class System 
{
public:
	/*! @class Proceedable
	*   @brief WindowsAPI��DirectX�̃��b�Z�[�W�������󂯓n���C���^�[�t�F�[�X�ł�
	*/
	class Proceedable
	{
	public:
		virtual void OnProceed(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) = 0;
		
	};

private:
	void SetSize(int width, int height);
	int width;
	int height;
	HWND handle;
	MSG msg;
	HINSTANCE ins;
	bool Create(std::string str, int width,int height);
public:

	/*!
	* @brief �R���X�g���N�^�ł��B
	* @param (title) �E�B���h�E�^�C�g��
	* @param (width) �E�B���h�E��
	* @param (height) �E�B���h�E����
	*/
	System(std::string title, int width, int height);
	System(const System&) = delete;
	~System();

	/*!
	* @brief �E�B���h�E�X�V
	* @return ��肪�Ȃ����true
	*/
	bool UpDate();

	/*!
	* @brief �E�B���h�E�n���h�����擾���܂�
	* @return HWND
	*/
	HWND GetHwnd();

	/*!
	* @brief �E�B���h�E�̃N���C�A���g�̈���擾���܂�
	* @return DirectX::XINT2
	*/
	DirectX::XMINT2 GetSize();

private:
	static std::forward_list<Proceedable*>& GetProcedures()
	{
		static std::forward_list<Proceedable*> procedures;
		return procedures;
	}
	static LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

public:
	/*!
	* @brief Windows�̃��b�Z�[�W������DirectX�̏������֘A�����܂�
	*/
	static void AddProcedure(Proceedable* const procedure)
	{
		GetProcedures().push_front(procedure);
	}
	/*!
	* @brief DirectX����̃��b�Z�[�W���M��j�����܂�
	*/
	static void RemoveProcedure(Proceedable* const procedure)
	{
		GetProcedures().remove(procedure);
	}
};