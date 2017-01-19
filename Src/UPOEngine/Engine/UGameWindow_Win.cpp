#include "UGameWindow_Win.h"
#include "UInput.h"

namespace UPO
{ 
	TArray<GameWindowWin*> gGameWindows;

	GameWindowWin* UFindGameWindowByHWND(HWND handle)
	{
		if (handle == nullptr || handle == INVALID_HANDLE_VALUE) return nullptr;
		
		for (GameWindowWin* gw : gGameWindows)
		{
			if (gw->GetWinHandle() == handle) return gw;
		}
		return nullptr;
	}
	WPARAM UMapLeftRightVKey(WPARAM vk, LPARAM lParam)
	{
		WPARAM new_vk = vk;
		UINT scancode = (lParam & 0x00ff0000) >> 16;
		int extended = (lParam & 0x01000000) != 0;

		switch (vk) {
		case VK_SHIFT:
			new_vk = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
			break;
		case VK_CONTROL:
			new_vk = extended ? VK_RCONTROL : VK_LCONTROL;
			break;
		case VK_MENU:
			new_vk = extended ? VK_RMENU : VK_LMENU;
			break;
		default:
			// not a key we map from generic to left/right specialized
			//  just return it.
			new_vk = vk;
			break;
		}

		return new_vk;
	}

	bool GameWindow::PeekMessages()
	{
		MSG msg;
		ZeroType(msg);

		Input::Tick();
		Input::SetMouseWheelDelta(0);

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

	LRESULT WINAPI WNDProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{
		
		switch (umsg)
		{
			// Check if the window is being destroyed.
		case WM_DESTROY:
		{
// 			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			if (GameWindowWin* gw = UFindGameWindowByHWND(hwnd))
			{
				GameWindow::Destroy(gw);
				
				if (gGameWindows.Length() == 0)
					PostQuitMessage(0);
			}
// 			DestroyWindow(hwnd);
// 			PostQuitMessage(0);
			return 0;
		}
		case WM_SETREDRAW:
		case  WM_PAINT:
		{
			ULOG_MESSAGE("paint");
		}
		}


		if (GameWindowWin* gw = UFindGameWindowByHWND(hwnd))
		{
			return gw->MessageHandler(hwnd, umsg, wparam, lparam);
		}
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}

	void GameWindowWin::GetWinSize(Vec2I& out)
	{
		USCOPE_LOCK(mSizeLock);
		out = mSize;
	}



	void GameWindowWin::OnCreateWindow()
	{
		ULOG_MESSAGE("");

		mFullScreen = mCreationParam.mFulllScreen;
		mSize = mCreationParam.mSize;
		mWindowName = mCreationParam.mWindowClassName;

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

		//WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
		unsigned dwStyleBorder = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;
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
			screenWidth = mSize.mX;
			screenHeight = mSize.mY;

			// Place the window in the middle of the screen.
			posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
			posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

			RECT wr = { 0, 0, mSize.mX, mSize.mY };
			AdjustWindowRect(&wr, dwStyleBorder /*WS_OVERLAPPEDWINDOW*/, FALSE);    // adjust the size

			screenWidth = wr.right - wr.left;
			screenHeight = wr.bottom - wr.top;

		}


		
		// Create the window with the screen settings and get the handle to it.
		mWindowHandle = (void*)CreateWindowExW(WS_EX_APPWINDOW, mWindowName, mWindowName,
			mFullScreen ? dwStyleNoborder : dwStyleBorder,
			posX, posY, screenWidth, screenHeight, NULL, NULL, mAppHandle, NULL);
		// Bring the window up on the screen and set it as main focus.
		ShowWindow((HWND)mWindowHandle, SW_SHOW);
		SetForegroundWindow((HWND)mWindowHandle);
		SetFocus((HWND)mWindowHandle);

		ShowCursor(true);
		SetCursorPos(posX + screenWidth / 2, posY + screenHeight / 2);

		gGameWindows.Add(this);
		ULOG_MESSAGE("game window created");
	}

	void GameWindowWin::OnDestroyWindow()
	{
		ULOG_MESSAGE("");

		if (!mWindowHandle) return;

		// Fix the display settings if leaving full screen mode.
		if (mFullScreen)
		{
			ChangeDisplaySettings(NULL, 0);
		}

		// Remove the window.
		DestroyWindow((HWND)mWindowHandle);
		mWindowHandle = nullptr;

		// Remove the application instance.
		UnregisterClass(mWindowName, mAppHandle);
		mAppHandle = NULL;

		UASSERT(gGameWindows.RemoveSwap(this));
	}

	LRESULT GameWindowWin::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{
		switch (umsg)
		{
// 		case WM_DESTROY:
// 		{
// // 			PostQuitMessage(0);
// 			return 0;
// 		}
// 
// 		// Check if the window is being closed.
// 		case WM_CLOSE:
// 		{
// 			DestroyWindow(hwnd);
// // 			PostQuitMessage(0);
// 			return 0;
// 		}
		case WM_KILLFOCUS:	//before win loses focus
		{
			ULOG_MESSAGE("WM_KILLFOCUS");
			mHasFocus = false;
			return 0;
		}
		case WM_SETFOCUS: //after win got focus
		{
			ULOG_MESSAGE("WM_SETFOCUS");
			Input::Reset();
			mHasFocus = true;
			return 0;
		};
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			EKeyCode key = UWin32VKToEKeyCode(UMapLeftRightVKey(wparam, lparam));
			Input::SetKeyState(key, true);
			Input::SetKeyState(EKeyCode::EKC_Any, true);
			ULOG_MESSAGE("Key down [%]", EnumToStr(key));
			return 0;
		}
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			Input::SetKeyState(UWin32VKToEKeyCode(UMapLeftRightVKey(wparam, lparam)), false);
			Input::SetKeyState(EKeyCode::EKC_Any, false);
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			int mouseX = GET_X_LPARAM(lparam);
			int mouseY = GET_Y_LPARAM(lparam);
			ULOG_MESSAGE("mouse move % %", mouseX, mouseY);
			mMousePosition.mX = mouseX;
			mMousePosition.mY = mouseY;
			Input::SetMousePos(mouseX, mouseY);
			return 0;
		}
		case WM_LBUTTONDOWN:
			Input::SetKeyState(EKC_MouseLeft, true);
			// 				Input::SetMouseState(EMouseButton::EMB_Left, true);
			return 0;
		case WM_RBUTTONDOWN:
			Input::SetKeyState(EKC_MouseRight, true);
			// 				Input::SetMouseState(EMouseButton::EMB_Right, true);
			return 0;
		case WM_MBUTTONDOWN:
			Input::SetKeyState(EKC_MouseMiddle, true);
			// 				Input::SetMouseState(EMouseButton::EMB_Middle, true);
			return 0;

		case WM_LBUTTONUP:
			Input::SetKeyState(EKC_MouseLeft, false);
			// 				Input::SetMouseState(EMouseButton::EMB_Left, false);
			return 0;
		case WM_RBUTTONUP:
			Input::SetKeyState(EKC_MouseRight, false);
			// 				Input::SetMouseState(EMouseButton::EMB_Right, false);
			return 0;
		case WM_MBUTTONUP:
			Input::SetKeyState(EKC_MouseMiddle, false);
			// 				Input::SetMouseState(EMouseButton::EMB_Middle, false);
			return 0;

		case WM_MOUSEWHEEL:
		{
			//A positive value indicates that the wheel was rotated forward,
			int zDelta = GET_WHEEL_DELTA_WPARAM(wparam);
			ULOG_MESSAGE("mouse wheel delta %", zDelta);
			Input::SetMouseWheelDelta(zDelta);
			if (zDelta > 0)
				Input::SetKeyState(EKC_MouseWheelForward, true);
			if (zDelta < 0)
				Input::SetKeyState(EKC_MouseWheelBackward, true);

			return 0;
		}
		case WM_MOUSELEAVE:
		{
			ULOG_MESSAGE("mouse leave");
			return 0;
		}
		case WM_MOUSEHOVER:
		{
			ULOG_MESSAGE("mouse hover");
			return 0;
		}
		case WM_SIZE:
		{
			unsigned w = LOWORD(lparam);
			unsigned h = HIWORD(lparam);
			ULOG_MESSAGE("window size changed %  %", w, h);
			mSizeLock.Enter();
			mSize = Vec2I(w, h);
			mSizeLock.Leave();
			return 0;
		}

		}
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}



};