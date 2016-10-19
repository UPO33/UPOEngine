#include "UGameWindow.h"
#include <windows.h>

namespace UPO
{
	LRESULT WINAPI WNDProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{

	}
	class GameWindowWin : public GameWindow
	{
		HINSTANCE mAppHandle = nullptr;
		HWND mHWND = nullptr;
		bool mFullScreen;
		unsigned mWidth;
		unsigned mHeight;
		wchar_t* mWindowName;

		void InitWindow()
		{
			mFullScreen = GEngineConfig()->AsBool("Window.FullScreen");
			mWidth = GEngineConfig()->AsNumber("Window.Width");
			mHeight = GEngineConfig()->AsNumber("Window.Height");

			mWindowName = L"UPOEngine";

			WNDCLASSEX wc;
			DEVMODE dmScreenSettings;
			int posX, posY;

			// Get the instance of this application.
			mAppHandle = GetModuleHandle(NULL);

			// Setup the windows class with default settings.
			wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wc.lpfnWndProc = WNDProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = mAppHandle;
			wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
			wc.hIconSm = wc.hIcon;
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			wc.lpszMenuName = NULL;
			wc.lpszClassName = mWindowName;
			wc.cbSize = sizeof(WNDCLASSEX);

			// Register the window class.
			RegisterClassEx(&wc);

			// Determine the resolution of the clients desktop screen.
			unsigned screenWidth = GetSystemMetrics(SM_CXSCREEN);
			unsigned screenHeight = GetSystemMetrics(SM_CYSCREEN);

			// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
			if (mFullScreen)
			{
				// If full screen set the screen to maximum size of the users desktop and 32bit.
				memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
				dmScreenSettings.dmSize = sizeof(dmScreenSettings);
				dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
				dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
				dmScreenSettings.dmBitsPerPel = 32;
				dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

				// Change the display settings to full screen.
				ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

				// Set the position of the window to the top left corner.
				posX = posY = 0;
			}
			else
			{
				// If windowed then set it to 800x600 resolution.
				screenWidth = mWidth;
				screenHeight = mHeight;

				// Place the window in the middle of the screen.
				posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
				posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
			}
			unsigned dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_SIZEBOX;
			// Create the window with the screen settings and get the handle to it.
			mHWND = CreateWindowExW(WS_EX_APPWINDOW, mWindowName, mWindowName,
				dwStyle,
				posX, posY, screenWidth, screenHeight, NULL, NULL, mAppHandle, NULL);
			// Bring the window up on the screen and set it as main focus.
			ShowWindow(mHWND, SW_SHOW);
			SetForegroundWindow(mHWND);
			SetFocus(mHWND);

			// Hide the mouse cursor.
			ShowCursor(true);
		}
		//////////////////////////////////////////////////////////////////////////
		void ReleaseWindow()
		{
			// Fix the display settings if leaving full screen mode.
			if (mFullScreen)
			{
				ChangeDisplaySettings(NULL, 0);
			}

			// Remove the window.
			DestroyWindow(mHWND);
			mHWND = nullptr;

			// Remove the application instance.
			UnregisterClass(mWindowName, mAppHandle);
			mAppHandle = NULL;
		}
		LRESULT MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
	};

	
	GameWindow* GameWindow::Get()
	{

	}

};