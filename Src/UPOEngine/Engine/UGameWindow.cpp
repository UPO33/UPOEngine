#include "UGameWindow.h"
#include "UInput.h"

#include <windows.h>
#include <windowsx.h>
#include <WinUser.h>


namespace UPO
{

	LRESULT WINAPI WNDProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

	class GameWindowWin* gGameWindowWin = nullptr;

	//////////////////////////////////////////////////////////////////////////
	class GameWindowWin : public GameWindow
	{
		HINSTANCE mAppHandle = nullptr;
		HWND mHWND = nullptr;
		bool mFullScreen;
		unsigned mWidth;
		unsigned mHeight;
		wchar_t* mWindowName;

	public:

		unsigned GetWidth() override { return mWidth; }
		unsigned GetHeight() override { return mHeight; }
		
		void* GetWinHandle() override { return reinterpret_cast<void*>(mHWND); }

		GameWindowWin()
		{
			gGameWindowWin = this;
		}
		~GameWindowWin()
		{
			gGameWindowWin = nullptr;
		}
		void Init() override
		{
			ULOG_MESSAGE("");
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
			
			unsigned dwStyleBorder = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_SIZEBOX;
			unsigned dwStyleNoborder = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;

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

				RECT wr = { 0, 0, mWidth, mHeight };
				AdjustWindowRect(&wr, dwStyleBorder /*WS_OVERLAPPEDWINDOW*/, FALSE);    // adjust the size
				
				screenWidth = wr.right - wr.left;
				screenHeight = wr.bottom - wr.top;

			}


			ULOG_MESSAGE("");
			// Create the window with the screen settings and get the handle to it.
			mHWND = CreateWindowExW(WS_EX_APPWINDOW, mWindowName, mWindowName,
				dwStyleBorder,
				posX, posY, screenWidth, screenHeight, NULL, NULL, mAppHandle, NULL);
			// Bring the window up on the screen and set it as main focus.
			ShowWindow(mHWND, SW_SHOW);
			SetForegroundWindow(mHWND);
			SetFocus(mHWND);

			// Hide the mouse cursor.
			ShowCursor(true);
		}
		//////////////////////////////////////////////////////////////////////////
		void Release() override
		{
			ULOG_MESSAGE("");
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
		//////////////////////////////////////////////////////////////////////////
		bool Tick() override
		{
			MSG msg;
			ZeroType(msg);

			// Handle the windows messages.
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				// If windows signals to end the application then exit out.
				if (msg.message == WM_QUIT)
				{
					return false;
				}
			}
			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		LRESULT MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
		{
			Input::SetMouseState(EMouseButton::EMB_WheelForward, false);
			Input::SetMouseState(EMouseButton::EMB_WheelBackward, false);
			Input::SetMouseWheelDelta(0);

			switch (umsg)
			{
				// Check if a key has been pressed on the keyboard.
			case WM_KEYDOWN:
				Input::SetKeyState(wparam, true);
				return 0;
			case WM_KEYUP:
				Input::SetKeyState(wparam, false);
				return 0;
			case WM_MOUSEMOVE:
				Input::SetMousePos(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
				return 0;
			case WM_LBUTTONDOWN:
				Input::SetMouseState(EMouseButton::EMB_Left, true);
				return 0;
			case WM_RBUTTONDOWN:
				Input::SetMouseState(EMouseButton::EMB_Right, true);
				return 0;
			case WM_MBUTTONDOWN:
				Input::SetMouseState(EMouseButton::EMB_Middle, true);
				return 0;

			case WM_LBUTTONUP:
				Input::SetMouseState(EMouseButton::EMB_Left, false);
				return 0;
			case WM_RBUTTONUP:
				Input::SetMouseState(EMouseButton::EMB_Right, false);
				return 0;
			case WM_MBUTTONUP:
				Input::SetMouseState(EMouseButton::EMB_Middle, false);
				return 0;

			case WM_MOUSEWHEEL:
			{
				//A positive value indicates that the wheel was rotated forward,
				unsigned zDelta = GET_WHEEL_DELTA_WPARAM(wparam);
				Input::SetMouseWheelDelta(zDelta);
				if (zDelta > 0)
					Input::SetMouseState(EMouseButton::EMB_WheelForward, true);
				if (zDelta < 0)
					Input::SetMouseState(EMouseButton::EMB_WheelBackward, true);

				return 0;
			}
			case WM_SIZE:
			{
				unsigned w = LOWORD(lparam);
				unsigned h = HIWORD(lparam);
				ULOG_MESSAGE("window size changed %d  %d", w, h);
				mWidth = w;
				mHeight = h;
				return 0;
			}
	
			}
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	};

	

	GameWindow* GameWindow::New()
	{
		return new GameWindowWin;
	}

	void GameWindow::Delete(GameWindow* gw)
	{
		delete gw;
	}


	

	LRESULT WINAPI WNDProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{
		switch (umsg)
		{
			// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		}
		return gGameWindowWin->MessageHandler(hwnd, umsg, wparam, lparam);
	}
};