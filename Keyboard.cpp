#include "Keyboard.h"

void KeyBoard::UpDate()
{
	in.UpDate();
}
//‰Ÿ‚µ‚Ä‚¢‚é
bool KeyBoard::On(const Key& key)
{
	return in.Push((int)key);
}
//‰Ÿ‚µ‚½
bool KeyBoard::Down(const Key& key)
{
	return in.Press((int)key);
}
//“ü—Í‚ª‚È‚¢
bool KeyBoard::Free(const Key& key)
{
	return in.Free((int)key);
}
//—£‚³‚ê‚½
bool KeyBoard::Pull(const Key& key)
{
	return in.Pull((int)key);
}