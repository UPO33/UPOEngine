#include "UGameWindow.h"
#include "UCanvas.h"
#include "UWorld.h"
#include "UWorldRS.h"
#include "UEngineBase.h"
#include "UInput.h"

#include "../GFX/UPrimitiveBatch.h"
#include "../GFX/USelectionBuffer.h"

#include "../GFXCore/UGFXCore.h"


#ifdef UPLATFORM_WIN
#include "UGameWindow_Win.h"
#else 
#error
#endif

namespace UPO
{

	TArray<GameWindow*> GameWindow::Instances;


	void GameWindow::BeginRender()
	{
		mFrameTimer.Start();
	}

	void GameWindow::EndRender()
	{
		mFrameElapsedSeconds = mFrameTimer.SinceSeconds();
	}

	bool GameWindow::InitAndReg(const GameWindowCreationParam& param)
	{
		UASSERT(!mRegistered);

		mCreationParam = param;
		OnCreateWindow();
		CreateSwapChain();
		if (mCreationParam.mCreateCanvas) CreateCanvas();

		mSelectionBuffer = new SelectionBuffer(GetWinSize());
		mInputState = new InputState;

		EnqueueRenderCommandAndWait([this]() {
			Instances.Add(this);
			mRegistered = true;
		});
		return true;
	}



	bool GameWindow::ReleaseAndUnreg()
	{
		ULOG_MESSAGE("");
		if (!mRegistered) return true;

		SafeDelete(mSelectionBuffer);
		SafeDelete(mInputState);

		if (mCanvas) DestroyCanvas();
		mCanvas = nullptr;
		if (mSwapchain) DestroySwapChain();
		mSwapchain = nullptr;

		OnDestroyWindow();

		EnqueueRenderCommandAndWait([this]() {
			Instances.RemoveShift(Instances.Find(this));

			//detaching game window from world if any, a world may have more that one game window though a world itself only
			//keep one pointer to main game window. in launcher we have a game window but in editor we may need to render a word in many windows
			if (mWorld)
			{
				if (mWorld->mMainWindow == this)
				{
					mWorld->mMainWindow = nullptr;
					if(auto rs = mWorld->GetRS()) rs->mMainWindow = nullptr;
				}
			}
		});

		
		return true;
	}

	GameWindow::GameWindow()
	{
		mHasFocus = true;
		mRegistered = false;
	}

	GameWindow::~GameWindow()
	{
		ULOG_MESSAGE("");
	}

	bool GameWindow::CreateCanvas()
	{
		mCanvas = new Canvas(this);
		return true;
	}

	bool GameWindow::DestroyCanvas()
	{
		SafeDelete(mCanvas);
		return true;
	}

	bool GameWindow::CreateSwapChain()
	{
		ULOG_MESSAGE("");
		GFXSwapChainDesc scDesc;
		scDesc.mGameWindow = this;
		scDesc.mSampleCount = Max((int)GEngineConfig()->AsNumber("GFX.MultiSample"), 1);
		scDesc.mVSyncEnable = mCreationParam.mVSyncEnable;
		scDesc.mFullScreem = mCreationParam.mFulllScreen;

		mSwapchain = gGFX->CreateSwapChain(scDesc);
		if (mSwapchain == nullptr)
		{
			return false;
		}

		return true;
	}

	bool GameWindow::DestroySwapChain()
	{
		if (mSwapchain) delete mSwapchain;
			mSwapchain = nullptr;
		
		return true;
	}

	void GameWindow::SetWorld(World* world)
	{
		if (mWorld == world) return;

		EnqueueRenderCommandAndWait([this, world]()
		{
			mWorld = world;
			if (mWorld)
			{
				mWorld->mMainWindow = this;
				if(auto wrs = mWorld->GetRS())
					wrs->mMainWindow = this;
			}
		});

	}

	GameWindow* GameWindow::CreateLauncherWin(const GameWindowCreationParam& param)
	{
#ifdef UPLATFORM_WIN
		GameWindow* gw = new GameWindowWin();

#else
#error
#endif

		if (gw->InitAndReg(param))
		{
			return gw;
		}
		else
		{
			delete gw;
			return nullptr;
		}
	}



	void GameWindow::Destroy(GameWindow* wnd)
	{
		if (wnd)
		{
			wnd->ReleaseAndUnreg();
			delete wnd;
		}
	}

	void GameWindow::DestroyAll()
	{
		for (GameWindow* gw : GameWindow::Instances)
		{
			GameWindow::Destroy(gw);
		}
		GameWindow::Instances.RemoveAll();
	}

	GameWindowCreationParam::GameWindowCreationParam(InitConfig)
	{
		mFulllScreen = GEngineConfig()->AsBool("Window.FullScreen");
		mSize.mX = GEngineConfig()->AsNumber("Window.Width");
		mSize.mY = GEngineConfig()->AsNumber("Window.Height");
		mVSyncEnable = GEngineConfig()->AsBool("GFX.VSync");
	}

#if 0

	//////////////////////////////////////////////////////////////////////////
	class GameWindowWin : public GameWindow
	{
		TinyLock mSizeLock;
		Vec2I mSize;

		HINSTANCE mAppHandle = nullptr;
		bool mFullScreen;
		wchar_t* mWindowName;

	public:
		//renderer read this every frame so that it must be tread safe
		void GetWinSize(Vec2I& out) override
		{
			USCOPE_LOCK(mSizeLock);
			out = mSize;
		}
		GameWindowWin(const GameWindowCreationnParam& param) : GameWindow(param)
		{
			// 			gGameWindowWin = this;
			gGameWindows.Add(this);
		}
		~GameWindowWin()
		{
			// 			gGameWindowWin = nullptr;
			gGameWindows.RemoveSwap(this);
		}
		void OnCreateWindow() override
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


			ULOG_MESSAGE("");
			// Create the window with the screen settings and get the handle to it.
			mWindowHandle = (void*)CreateWindowExW(WS_EX_APPWINDOW, mWindowName, mWindowName,
				mFullScreen ? dwStyleNoborder : dwStyleBorder,
				posX, posY, screenWidth, screenHeight, NULL, NULL, mAppHandle, NULL);
			// Bring the window up on the screen and set it as main focus.
			ShowWindow((HWND)mWindowHandle, SW_SHOW);
			SetForegroundWindow((HWND)mWindowHandle);
			SetFocus((HWND)mWindowHandle);

			// Hide the mouse cursor.
			ShowCursor(true);
		}
		//////////////////////////////////////////////////////////////////////////
		void OnDestroyWindow() override
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
		}
		//////////////////////////////////////////////////////////////////////////
		bool TickWindow() override
		{
			MSG msg;
			ZeroType(msg);

			Input::Tick();

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
		bool gAnyKeyDow = false;
		//////////////////////////////////////////////////////////////////////////
		LRESULT MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
		{
			// 			Input::SetMouseState(EMouseButton::EMB_WheelForward, false);
			// 			Input::SetMouseState(EMouseButton::EMB_WheelBackward, false);
			Input::SetKeyState(EKC_MouseWheelForward, false);
			Input::SetKeyState(EKC_MouseWheelBackward, false);
			Input::SetMouseWheelDelta(0);




			switch (umsg)
			{
				// Check if a key has been pressed on the keyboard.
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			{
				EKeyCode key = UWin32VKToEKeyCode(UMapLeftRightVKey(wparam, lparam));
				Input::SetKeyState(key, true);
				Input::SetKeyState(EKeyCode::EKC_Any, true);
				ULOG_MESSAGE("Key down [%s]", EnumToStr(key));
				return 0;
			}
			case WM_KEYUP:
			{
				Input::SetKeyState(UWin32VKToEKeyCode(UMapLeftRightVKey(wparam, lparam)), false);
				Input::SetKeyState(EKeyCode::EKC_Any, false);
				return 0;
			}
			case WM_MOUSEMOVE:
				Input::SetMousePos(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
				return 0;
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
				unsigned zDelta = GET_WHEEL_DELTA_WPARAM(wparam);
				ULOG_MESSAGE("mouse wheel delta %i", zDelta);
				Input::SetMouseWheelDelta(zDelta);
				// 				if (zDelta > 0)
				// 					Input::SetMouseState(EMouseButton::EMB_WheelForward, true);
				// 				if (zDelta < 0)
				// 					Input::SetMouseState(EMouseButton::EMB_WheelBackward, true);
				if (zDelta > 0)
					Input::SetKeyState(EKC_MouseWheelForward, true);
				if (zDelta < 0)
					Input::SetKeyState(EKC_MouseWheelBackward, true);

				return 0;
			}
			case WM_SIZE:
			{
				unsigned w = LOWORD(lparam);
				unsigned h = HIWORD(lparam);
				ULOG_MESSAGE("window size changed %d  %d", w, h);
				mSizeLock.Enter();
				mSize = Vec2I(w, h);
				mSizeLock.Leave();
				return 0;
			}

			}
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	};




	bool GameWindow::PeekMessages()
	{
		MSG msg;
		ZeroType(msg);

		Input::Tick();

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

		for (GameWindowWin* gw : gGameWindows)
		{
			if (gw->mWindowHandle == hwnd)
			{
				return gw->MessageHandler(hwnd, umsg, wparam, lparam);
			}
		}
		// 		return gGameWindowWin->MessageHandler(hwnd, umsg, wparam, lparam);
	}
#endif // 0

};