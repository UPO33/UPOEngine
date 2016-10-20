#pragma once

#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define DIRECTINPUT_VERSION 0x0800

class DInput
{
public:
	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* mDirectInput = nullptr;
	IDirectInputDevice8* mKeyboard = nullptr;
	IDirectInputDevice8* mMouse = nullptr;

	char mKeyboardState[256];
	DIMOUSESTATE mMouseState;
	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};