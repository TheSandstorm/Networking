#pragma once
#include "Player.h"

enum KEYSTATE {
	KEY_FIRST_PRESS,
	KEY_RELEASED,
	KEY_HELD,
};

class CInputManager
{
public:
	CInputManager();
	static void ProcessKeyInput(std::shared_ptr<CPlayer> _Obj);
	static void ProcessKeyInput();

	static unsigned char KeyArray[255];
	static unsigned char KeySpecialArray[255];

private:
	static void NormKeyDown(unsigned char key, int x, int y);
	static void NormKeyUp(unsigned char key, int x, int y);
	static void SpecialKeyDown(int key, int x, int y);
	static void SpecialKeyUp(int key, int x, int y);
};