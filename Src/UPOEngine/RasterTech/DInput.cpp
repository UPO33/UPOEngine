#include "DInput.h"


bool DInput::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;


	// Store the screen size which will be used for positioning the mouse cursor.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Initialize the location of the mouse on the screen.
	m_mouseX = 0;
	m_mouseY = 0;

	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDirectInput, NULL);
	if (FAILED(result))
	{
		return false;
	}
	// Initialize the direct input interface for the keyboard.
	result = mDirectInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = mKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}
	// Set the cooperative level of the keyboard to not share with other programs.
	result = mKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}
	// Now acquire the keyboard.
	result = mKeyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}


	// Initialize the direct input interface for the mouse.
	result = mDirectInput->CreateDevice(GUID_SysMouse, &mMouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = mMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}
	// Set the cooperative level of the mouse to share with other programs.
	result = mMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}
	// Acquire the mouse.
	result = mMouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}
void DInput::Shutdown()
{
	// Release the mouse.
	if (mMouse)
	{
		mMouse->Unacquire();
		mMouse->Release();
		mMouse = 0;
	}

	// Release the keyboard.
	if (mKeyboard)
	{
		mKeyboard->Unacquire();
		mKeyboard->Release();
		mKeyboard = 0;
	}

	// Release the main interface to direct input.
	if (mDirectInput)
	{
		mDirectInput->Release();
		mDirectInput = 0;
	}

	return;
}

bool DInput::Frame()
{
	bool result;


	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
		return false;
	// Read the current state of the mouse.
	result = ReadMouse();
	if (!result)
		return false;

	// Process the changes in the mouse and keyboard.
	ProcessInput();

	return true;
}

bool DInput::ReadKeyboard()
{
	HRESULT result;


	// Read the keyboard device.
	result = mKeyboard->GetDeviceState(sizeof(mKeyboardState), (LPVOID)&mKeyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			mKeyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}
bool DInput::ReadMouse()
{
	HRESULT result;


	// Read the mouse device.
	result = mMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mMouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			mMouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void DInput::ProcessInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	m_mouseX += mMouseState.lX;
	m_mouseY += mMouseState.lY;

	// Ensure the mouse location doesn't exceed the screen width or height.
	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }

	return;
}
bool DInput::IsEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (mKeyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}

void DInput::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}
