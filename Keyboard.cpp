#include "Keyboard.h"

void KeyBoard::UpDate()
{
	in.UpDate();
}
//�����Ă���
bool KeyBoard::On(const Key& key)
{
	return in.Push((int)key);
}
//������
bool KeyBoard::Down(const Key& key)
{
	return in.Press((int)key);
}
//���͂��Ȃ�
bool KeyBoard::Free(const Key& key)
{
	return in.Free((int)key);
}
//�����ꂽ
bool KeyBoard::Pull(const Key& key)
{
	return in.Pull((int)key);
}