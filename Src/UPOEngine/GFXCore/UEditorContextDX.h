#pragma once

#include "UGFXContext.h"

#include "UGFXDeviceDX.h"

namespace UPO
{ 
#if 0
	class UAPI GFXEditorContextDX : public GFXContext
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

	public:
		 bool Init(InitParam param) override;
		 bool Resize(const Vec2I& newSize) override;
		 bool Release() override;
		 bool PresentSwapChain() override;
		 GFXTexture2D* GetBackBuffer() override;
	};
#endif
};