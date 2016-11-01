#include "UGFXContext.h"
#include "../Engine/UGameWindow.h"
#include "UEditorContextDX.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class UAPI GFXContextDX : public GFXContext
	{
		HWND mHWND = nullptr;

		ID3D11Device* mDXDevice = nullptr;
		ID3D11DeviceContext* mDXDeviceContext = nullptr;
		D3D_FEATURE_LEVEL mUsedFeatureLevel = D3D_FEATURE_LEVEL_11_0;
		IDXGISwapChain* mSwapchain = nullptr;

		ID3D11RenderTargetView* mBackBufferView = nullptr;
		ID3D11Texture2D* mBackBuffer = nullptr;

		GFXTexture2DDX* mBackBufferTexture2D = nullptr;

		unsigned mWidth, mHeight;

		bool mVSyncEnable;

		GameWindow* mGameWindow = nullptr;

	public:
		bool Init(GameWindow* win) override
		{
			mVSyncEnable = GEngineConfig()->AsBool("GFX.VSync");

			mGameWindow = win;

			mHWND = (HWND)win->GetWinHandle();
			mWidth = win->GetWidth();
			mHeight = win->GetHeight();


			ULOG_MESSAGE("creating render target w: %d h: %d ...", mWidth, mHeight);

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
				if (displayModeList[i].Width == mWidth)
				{
					if (displayModeList[i].Height == mHeight)
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
			swapChainDesc.BufferDesc.Width = mWidth;
			swapChainDesc.BufferDesc.Height = mHeight;
			// Set regular 32-bit surface for the back buffer.
			swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			// Set the refresh rate of the back buffer.
			if (mVSyncEnable)
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
			swapChainDesc.SampleDesc.Count = Max(GEngineConfig()->AsNumber("GFX.MultiSample"), 1.0f);
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.Windowed = true;
			// Set the scan line ordering and scaling to unspecified.
			swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			// Discard the back buffer contents after presenting.
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			// Don't set the advanced flags.
			swapChainDesc.Flags = 0;

			if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
				D3D11_CREATE_DEVICE_SINGLETHREADED // flags
				, nullptr, 0, D3D11_SDK_VERSION,
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

			mBackBuffer = backBuffer;
			mBackBufferView = backBufferView;


			mBackBufferTexture2D = new GFXTexture2DDX();
			mBackBufferTexture2D->mTexture = backBuffer;
			mBackBufferTexture2D->mResourceView = nullptr;
			mBackBufferTexture2D->mDepthStencilView = nullptr;
			mBackBufferTexture2D->mRenderTargetView = backBufferView;


			D3D11_VIEWPORT viewport;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = mWidth;
			viewport.Height = mHeight;
			viewport.MaxDepth = 1;
			viewport.MinDepth = 0;

			mDXDeviceContext->RSSetViewports(1, &viewport);

			mDXDeviceContext->OMSetRenderTargets(0, 0, nullptr);

			///////////
			gGFXDX = new GFXDeviceDX(mDXDevice, mDXDeviceContext);
			gGFX = gGFXDX;

			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		bool Resize(unsigned newW, unsigned newH) override
		{
			mWidth = newW;
			mHeight = newH;

			mDXDeviceContext->OMSetRenderTargets(0, 0, 0);

			//You can use the IDXGISwapChain::ResizeBuffers method to handle window resizing.Before you call ResizeBuffers, 
			//you must release all outstanding references to the swap chain's buffers. The object that typically holds
			//a reference to a swap chain's buffer is a render - target - view.

			// Release all outstanding references to the swap chain's buffers.
			mBackBufferView->Release();
			mBackBuffer->Release();


			// Preserve the existing buffer count and format.
			// Automatically choose the width and height to match the client rect for HWNDs.
			if (FAILED(mSwapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0)))
			{
				ULOG_FATAL("failed to resize buffer");
				return false;
			}

			ID3D11Texture2D* backBuffer = nullptr;
			ID3D11RenderTargetView* backbufferView = nullptr;

			// Perform error handling here!

			// Get buffer and create a render-target-view.
			if (FAILED(mSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer)))
			{
				ULOG_FATAL("failed to get swapchain buffer");
				return false;
			}

			// Perform error handling here!

			if (FAILED(mDXDevice->CreateRenderTargetView(backBuffer, NULL, &backbufferView)))
			{
				ULOG_FATAL("failed to crate render target view");
				return false;
			}

			mBackBuffer = backBuffer;
			mBackBufferView = backbufferView;

			mBackBufferTexture2D->mTexture = backBuffer;
			mBackBufferTexture2D->mResourceView = nullptr;
			mBackBufferTexture2D->mDepthStencilView = nullptr;
			mBackBufferTexture2D->mRenderTargetView = backbufferView;


			// Set up the viewport.
			D3D11_VIEWPORT vp;
			vp.Width = newW;
			vp.Height = newH;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			mDXDeviceContext->RSSetViewports(1, &vp);

			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		bool Release() override
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
				if (mBackBuffer)
				{
					mBackBuffer->Release();
					mBackBuffer = nullptr;
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
					mSwapchain = nullptr;
				}

				if (mBackBufferTexture2D)
				{
					mBackBufferTexture2D->mTexture = nullptr;
					mBackBufferTexture2D->mRenderTargetView = nullptr;

					delete mBackBufferTexture2D;
					mBackBufferTexture2D = nullptr;
				}

				///////////////////
				if (gGFXDX) delete gGFXDX;
				gGFXDX = nullptr;
				gGFX = nullptr;

				return true;
		}
		//////////////////////////////////////////////////////////////////////////
		bool PresentSwapChain() override
		{
			if (mVSyncEnable)
			{
				// Lock to screen refresh rate.
				mSwapchain->Present(1, 0);
			}
			else
			{
				// Present as fast as possible.
				mSwapchain->Present(0, 0);
			}

			return true;
		}
		GFXTexture2D* GetBackBuffer() override { return mBackBufferTexture2D; }
		GameWindow* GetGameWindow() override { return mGameWindow; }
		unsigned BackBufferWidth() override { return mWidth; }
		unsigned BackBufferHeight() override { return mHeight; }
	};

	GFXContext* GFXContext::New()
	{
		return new GFXContextDX;
	}

	void GFXContext::Delete(GFXContext* ctx)
	{
		delete ctx;
	}

};