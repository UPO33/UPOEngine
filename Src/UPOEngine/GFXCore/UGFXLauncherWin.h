#pragma once

#include "UGFXDeviceDX.h"

namespace UPO
{
#if 0
	class UAPI GFXLauncherWin
	{
	public:
		struct InitParam
		{
			wchar_t*	mWindowName = L"UPOEngine";
			bool		mFullScreen = false;
			unsigned	mWidth = 800;
			unsigned	nHeight = 600;
			bool		mShowCursor = true;
			D3D_DRIVER_TYPE mDriverType = D3D_DRIVER_TYPE_HARDWARE;
			bool		mVSyncEnable = true;
		};



		InitParam mParam;
		HINSTANCE mAppHandle = nullptr;
		HWND mHWND = nullptr;

		ID3D11Device* mDXDevice = nullptr;
		ID3D11DeviceContext* mDXDeviceContext = nullptr;
		D3D_FEATURE_LEVEL mUsedFeatureLevel = D3D_FEATURE_LEVEL_11_0;
		IDXGISwapChain* mSwapchain = nullptr;
		ID3D11RenderTargetView* mBackBufferView = nullptr;
		
		GFXTexture2DDX* mBackBuffer = nullptr;

		auto GetSwapChain() const { return mSwapchain; }
		auto GetBackBufferView() const { return mBackBufferView; }
		GFXTexture2DDX* GetBackBuffer() const { return mBackBuffer; }

		class Renderer* mRenderer = nullptr;

		void Init(const InitParam& param);
		void Shutdown();

		bool InitDX();
		void InitWindow();

		void Run();
		void ShutdownWindow();
		void ShutdownDX();
		bool Tick();

		void ClearBackBuffer(Color clearColor);
		void SawpChainPresent();

		void SetCaption(const char* str);

		LRESULT MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

		static GFXLauncherWin* Get();


		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	};
#endif
};