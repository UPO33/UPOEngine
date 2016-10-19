#include "UGFXLauncherWin.h"
#include "../Engine/UInput.h"
#include "_URenderer.h"

#include <windowsx.h>
#include <WinUser.h>

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	void GFXLauncherWin::Init(const InitParam& param)
	{
		mParam = param;
		InitWindow();
		InitDX();

		mRenderer = new Renderer(mParam.mWidth, mParam.nHeight);
		mRenderer->Init();
	}

	void GFXLauncherWin::Shutdown()
	{
		mRenderer->Shutdown();
		delete mRenderer;

		ShutdownDX();
		ShutdownWindow();
	}

	//////////////////////////////////////////////////////////////////////////
	void GFXLauncherWin::InitWindow()
	{

		WNDCLASSEX wc;
		DEVMODE dmScreenSettings;
		int posX, posY;

		// Get the instance of this application.
		mAppHandle = GetModuleHandle(NULL);

		// Setup the windows class with default settings.
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = mAppHandle;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = mParam.mWindowName;
		wc.cbSize = sizeof(WNDCLASSEX);

		// Register the window class.
		RegisterClassEx(&wc);

		// Determine the resolution of the clients desktop screen.
		unsigned screenWidth = GetSystemMetrics(SM_CXSCREEN);
		unsigned screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
		if (mParam.mFullScreen)
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
			screenWidth = mParam.mWidth;
			screenHeight = mParam.nHeight;

			// Place the window in the middle of the screen.
			posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
			posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
		}
		unsigned dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_SIZEBOX;
		// Create the window with the screen settings and get the handle to it.
		mHWND = CreateWindowExW(WS_EX_APPWINDOW, mParam.mWindowName, mParam.mWindowName,
			dwStyle,
			posX, posY, screenWidth, screenHeight, NULL, NULL, mAppHandle, NULL);
		// Bring the window up on the screen and set it as main focus.
		ShowWindow(mHWND, SW_SHOW);
		SetForegroundWindow(mHWND);
		SetFocus(mHWND);

		// Hide the mouse cursor.
		ShowCursor(mParam.mShowCursor);
	}
	//////////////////////////////////////////////////////////////////////////
	void GFXLauncherWin::Run()
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		// Loop until there is a quit message from the window or the user.
		bool loop = true;
		while (loop)
		{
			Input::Tick();

			// Handle the windows messages.
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// If windows signals to end the application then exit out.
			if (msg.message == WM_QUIT)
			{
				loop = false;
			}
			else
			{
				loop = Tick();
			}

			//Sleep(10);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void GFXLauncherWin::ShutdownWindow()
	{
		// Show the mouse cursor.
		ShowCursor(true);

		// Fix the display settings if leaving full screen mode.
		if (mParam.mFullScreen)
		{
			ChangeDisplaySettings(NULL, 0);
		}

		// Remove the window.
		DestroyWindow(mHWND);
		mHWND = nullptr;

		// Remove the application instance.
		UnregisterClass(mParam.mWindowName, mAppHandle);
		mAppHandle = NULL;

		return;
	}

	void GFXLauncherWin::ShutdownDX()
	{
		if (mSwapchain)
		{
			mSwapchain->SetFullscreenState(false, nullptr);
		}
		if (mBackBufferView)
		{
			mBackBufferView->Release();
			mBackBufferView = nullptr;
		}
		if (mBackBufferView)
		{
			mBackBufferView->Release();
			mBackBufferView = nullptr;
		}
		if (mDXDeviceContext)
		{
			mDXDeviceContext->Release();
			mDXDeviceContext = nullptr;
		}
		if (mDXDevice)
		{
			mDXDevice->Release();
			mDXDevice = nullptr;
		}

		if (mSwapchain)
		{
			mSwapchain->Release();
			mSwapchain= 0;
		}

		if (mBackBuffer) delete mBackBuffer;
		mBackBuffer = nullptr;

		///////////////////
		if (gGFXDX) delete gGFXDX;
		gGFXDX = nullptr;
		gGFX = nullptr;

	}

	//////////////////////////////////////////////////////////////////////////
	bool GFXLauncherWin::Tick()
	{
		if (Input::IsKeyDown(VK_ESCAPE)) return false;

		mRenderer->Tick();

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void GFXLauncherWin::ClearBackBuffer(Color clearColor)
	{
		mDXDeviceContext->ClearRenderTargetView(mBackBufferView, clearColor.mRGBA);
	}
	//////////////////////////////////////////////////////////////////////////
	void GFXLauncherWin::SawpChainPresent()
	{
		if (mParam.mVSyncEnable)
		{
			// Lock to screen refresh rate.
			mSwapchain->Present(1, 0);
		}
		else
		{
			// Present as fast as possible.
			mSwapchain->Present(0, 0);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void GFXLauncherWin::SetCaption(const char* str)
	{
		SetWindowTextA(mHWND, str);
	}

	//////////////////////////////////////////////////////////////////////////
	bool GFXLauncherWin::InitDX()
	{
		HRESULT result;
		IDXGIFactory* factory;
		IDXGIAdapter* adapter;
		IDXGIOutput* adapterOutput;
		unsigned int numModes, i, numerator, denominator, stringLength;
		DXGI_MODE_DESC* displayModeList;
		DXGI_ADAPTER_DESC adapterDesc;

		// Create a DirectX graphics interface factory.
		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if (FAILED(result))
		{
			ULOG_FATAL("failed to crate DXGIFactory");
			return false;
		}

		// Use the factory to create an adapter for the primary graphics interface (video card).
		result = factory->EnumAdapters(0, &adapter);
		if (FAILED(result))
		{
			ULOG_FATAL("failed to enums adapter");
			return false;
		}

		// Enumerate the primary adapter output (monitor).
		result = adapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(result))
		{
			ULOG_FATAL("failed to enums monitor");
			return false;
		}

		// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if (FAILED(result))
		{
			ULOG_FATAL("failed to get display mode list");
			return false;
		}

		// Create a list to hold all the possible display modes for this monitor/video card combination.
		displayModeList = new DXGI_MODE_DESC[numModes];

		// Now fill the display mode list structures.
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if (FAILED(result))
		{
			ULOG_FATAL("failed to get display mode list");
			return false;
		}

		// Now go through all the display modes and find the one that matches the screen width and height.
		// When a match is found store the numerator and denominator of the refresh rate for that monitor.
		for (i = 0; i < numModes; i++)
		{
			if (displayModeList[i].Width == (unsigned int)mParam.mWidth)
			{
				if (displayModeList[i].Height == (unsigned int)mParam.nHeight)
				{
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		delete[] displayModeList;
		displayModeList = 0;
		adapterOutput->Release();
		adapter->Release();
		factory->Release();

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		// Set to a single back buffer.
		swapChainDesc.BufferCount = 1;
		// Set the width and height of the back buffer.
		swapChainDesc.BufferDesc.Width = mParam.mWidth;
		swapChainDesc.BufferDesc.Height = mParam.nHeight;
		// Set regular 32-bit surface for the back buffer.
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		// Set the refresh rate of the back buffer.
		if (mParam.mVSyncEnable)
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
		}
		else
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		// Set the usage of the back buffer.
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		// Set the handle for the window to render to.
		swapChainDesc.OutputWindow = mHWND;
		// Turn multisampling off.
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = !mParam.mFullScreen;
		// Set the scan line ordering and scaling to unspecified.
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		// Discard the back buffer contents after presenting.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		// Don't set the advanced flags.
		swapChainDesc.Flags = 0;

		if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, mParam.mDriverType, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, 
			&swapChainDesc, &mSwapchain, &mDXDevice, &mUsedFeatureLevel, &mDXDeviceContext)))
		{
			ULOG_FATAL("failed to create DX11 device");
			return false;
		}


		ID3D11Texture2D* backBuffer;
		if (FAILED(mSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer)))
		{
			ULOG_FATAL("failed to get back buffer");
			return false;
		}

		ID3D11RenderTargetView* backBufferView = nullptr;
		// Create the render target view with the back buffer pointer.
		result = mDXDevice->CreateRenderTargetView(backBuffer, NULL, &backBufferView);
		if (FAILED(result))
		{
			ULOG_FATAL("failed to crate back buffer");
			return false;
		}
		mBackBufferView = backBufferView;
		
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = mParam.mWidth;
		viewport.Height = mParam.nHeight;
		viewport.MaxDepth = 1;
		viewport.MinDepth = 0;

		mDXDeviceContext->RSSetViewports(1, &viewport);

		gGFXDX = new GFXDeviceDX(mDXDevice, mDXDeviceContext);
		gGFX = gGFXDX;

		/////////////////////
		mBackBuffer  = new GFXTexture2DDX;
		mBackBuffer->mTexture = backBuffer;
		mBackBuffer->mResourceView = nullptr;
		mBackBuffer->mRenderTargetView = mBackBufferView;
		mBackBuffer->mDepthStencilView = nullptr;

		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	LRESULT GFXLauncherWin::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
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
			if(zDelta < 0)
				Input::SetMouseState(EMouseButton::EMB_WheelBackward, true);
			
			return 0;
		}
		case WM_SIZE:
		{
			unsigned w = LOWORD(lparam);
			unsigned h = HIWORD(lparam);
			ULOG_MESSAGE("window size changed %d  %d", w, h);
			return 0;
		}
			
		// Any other messages send to the default message handler as our application won't make use of them.
		default:
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	GFXLauncherWin* GFXLauncherWin::Get()
	{
		static GFXLauncherWin Ins;
		return &Ins;
	}
	//////////////////////////////////////////////////////////////////////////
	LRESULT CALLBACK GFXLauncherWin::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
	{
		switch (umessage)
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

		// All other messages pass to the message handler in the system class.
		default:
		{
			return Get()->MessageHandler(hwnd, umessage, wparam, lparam);
		}
		}
	}

}
