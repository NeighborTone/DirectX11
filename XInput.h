#pragma once
#pragma comment(lib,"xinput.lib ")
#include <DirectXMath.h>
#include <Windows.h>
#include <vector>
#include <XInput.h>

namespace DX = DirectX;
class XInput
{
public:
	enum class ButtonID
	{
		//�\���L�[
		UP = 0,
		DOWN = 1,
		LEFT= 2,
		RIHGT = 3,
		//�X�^�[�g�A�o�b�N
		START = 4,
		BACK = 5,
		//�A�i���OL,R
		L_THUMB = 6,
		R_THUMB = 7,
		//�g���K�[
		L = 9,
		R = 10,
		A = 12,
		B = 13,
		X = 14,
		Y = 15,
		MAX
	};

	enum class ID
	{
		P1 = 0,
		P2 = 1,
		P3 = 2,
		P4 = 3,
		MAX
	};
private:
	//�R���g���[���[�����p
	class Controller
	{
	private:
		//�j���[�g�������̂����тђl�͈̔�
		const float deadZone = static_cast<float>(0x7FFF)*0.24f;
		int id;	//�R���g���[���[�̔ԍ�
		XINPUT_STATE state;	//�{�^�����
		XINPUT_VIBRATION vibration;	//�o�C�u���[�V����
		bool isConnected;	//�ڑ�����Ă��邩
		WORD now;	//���̃t���[���̃{�^�����
		WORD prev;	//�O�̃t���[���̃{�^�����

		struct ButtonState
		{
			int holdFrame;	//������Ă���t���[����
			int pullFrame;	//������Ă���t���[����
		} frameState[(int)ButtonID::MAX];

		//�A�i���O�p�b�h���̎擾
		DX::XMFLOAT2 GetThumb(const short x, const short y) const;

	public:
		Controller() = delete;
		Controller(ID id);

		void UpDate();
		//������Ă��邩
		bool CheckPress(const ButtonID buttonID, int interval = 1) const;
		//������Ă��Ȃ���
		bool CheckFree(const ButtonID buttonID, int interval = 1) const;
		//�����ꂽ��
		bool CheckPush(const ButtonID buttonID, int interval = 1) const;
		//�����ꂽ��
		bool CheckPull(const ButtonID buttonID, int interval = 1) const;
		//�g���K�[�X�e�B�b�N�̉E�̏����擾,0.0f~1.0f
		float GetRightThumb(ID id = ID::P1) const;
		//�g���K�[�X�e�B�b�N�̍��̏����擾,0.0f~1.0f
		float GetLeftThumb(ID id = ID::P1) const;
		//�p�b�h��U��������(����)
		void Vibration(float leftPow, float rihgtPow, ID id = ID::P1);
		//�p�b�h��U��������(�E��)
		void VibrationR(float rihgtPow, ID id = ID::P1);
		//�p�b�h��U��������(����)
		void VibrationL(float leftPow, ID id = ID::P1);
	};
	std::vector<Controller> controllers;
	//�V���O���g���Ȃ̂Ńv���C�x�[�g
	XInput(const XInput&) {};
	XInput(const XInput&&) {};
	XInput();

public:
	static XInput* GetInstance();
	void UpDate();
	//������Ă��邩
	bool CheckPress(const ButtonID buttonID, int interval = 1, ID id = ID::P1) const;
	//������Ă��Ȃ���
	bool CheckFree(const ButtonID buttonID, int interval = 1, ID id = ID::P1) const;
	//�����ꂽ��
	bool CheckPush(const ButtonID buttonID, ID id = ID::P1) const;
	//�����ꂽ��
	bool CheckPull(const ButtonID buttonID, ID id = ID::P1) const;
	//1P~4P�܂łǂ̃{�^����������Ă��邩
	bool ChekAnyPress(const ButtonID buttonID) const;
	//1P~4P�܂łǂ̃{�^���������ꂽ��
	bool ChekAnyPush(const ButtonID buttonID) const;
	//1P~4P�܂łǂ̃{�^����������Ă��邩
	bool ChekAnyFree(const ButtonID buttonID) const;
	//1P~4P�܂łǂ̃{�^���������ꂽ��
	bool ChekAnyPull(const ButtonID buttonID) const;
	//�g���K�[�X�e�B�b�N�̉E�̏����擾,0.0f~1.0f
	float GetRightThumb(ID id = ID::P1) const;
	//�g���K�[�X�e�B�b�N�̍��̏����擾,0.0f~1.0f
	float GetLeftThumb(ID id = ID::P1) const;
	//�p�b�h��U��������(����)
	void Vibration(float leftPow, float rihgtPow, ID id = ID::P1);
	//�p�b�h��U��������(�E��)
	void VibrationR(float rihgtPow, ID id = ID::P1);
	//�p�b�h��U��������(����)
	void VibrationL(float leftPow, ID id = ID::P1);

	//�֐��ŃA�N�Z�X�ł���悤�ɂ���
	//static void UpDate();
	// //������Ă��邩
	//static bool CheckPress(const ButtonID buttonID, int interval = 1);
	// //������Ă��Ȃ���
	//static bool CheckFree(const ButtonID buttonID, int interval = 1);
	// //�����ꂽ��
	//static bool CheckPush(const ButtonID buttonID, int interval = 1);
	// //�����ꂽ��
	//static bool CheckPull(const ButtonID buttonID, int interval = 1);
	// //1P~4P�܂łǂ̃{�^����������Ă��邩
	//static bool ChekAnyPress(const ButtonID buttonID);
	// //1P~4P�܂łǂ̃{�^���������ꂽ��
	//static bool ChekAnyPush(const ButtonID buttonID);
	// //1P~4P�܂łǂ̃{�^����������Ă��邩
	//static bool ChekAnyFree(const ButtonID buttonID);
	// //1P~4P�܂łǂ̃{�^���������ꂽ��
	//static bool ChekAnyPull(const ButtonID buttonID);
	// //�g���K�[�X�e�B�b�N�̉E�̏����擾,0.0f~1.0f
	//static float GetRightThumb();
	// //�g���K�[�X�e�B�b�N�̍��̏����擾,0.0f~1.0f
	//static float GetLeftThumb();
	// //�p�b�h��U��������(����)
	//static void Vibration(float leftPow, float rihgtPow);
	// //�p�b�h��U��������(�E��)
	//static void VibrationR(float rihgtPow);
	// //�p�b�h��U��������(����)
	//static void VibrationL(float leftPow);
};

