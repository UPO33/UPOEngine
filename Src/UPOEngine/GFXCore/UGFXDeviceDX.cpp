#pragma once

#include "UGFXDeviceDX.h"

#include "../Engine/UGameWindow.h"

#include <DxErr.h>

#define DXERRORFATAL(hresult) if(FAILED(hresult)) { ULOG_FATAL(" %  %", DXGetErrorStringA(hresult), DXGetErrorDescriptionA(hresult)); }

namespace UPO
{
	const char* EnumToStr(D3D_FEATURE_LEVEL in)
	{
		switch (in)
		{
		case D3D_FEATURE_LEVEL_9_1: return "FEATURE_LEVEL_9_1";
		case D3D_FEATURE_LEVEL_9_2: return "FEATURE_LEVEL_9_2";
		case D3D_FEATURE_LEVEL_9_3: return "FEATURE_LEVEL_9_3";
		case D3D_FEATURE_LEVEL_10_0: return "FEATURE_LEVEL_10_0";
		case D3D_FEATURE_LEVEL_10_1: return "FEATURE_LEVEL_10_1";
		case D3D_FEATURE_LEVEL_11_0: return "FEATURE_LEVEL_11_0";
		case D3D_FEATURE_LEVEL_11_1: return "FEATURE_LEVEL_11_1";
		}
		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	GFXDeviceDX* GFXDeviceDX::Create()
	{
		ULOG_MESSAGE("creating gfx devic ...");

		ID3D11Device* device;
		ID3D11DeviceContext* immediateContext;
		D3D_FEATURE_LEVEL featureLevel;
		IDXGIFactory* giFactory = nullptr;
		IDXGIAdapter* adapters[16];
		String adaptersName[16];
		IDXGIAdapter* adapter = nullptr;
		int preferedAdapterIndex = 0;

		{
			
		}
		// Create a DirectX graphics interface factory.
		HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&giFactory);
		if (FAILED(result))
		{
			ULOG_FATAL("failed to crate DXGIFactory");
			return nullptr;
		}

		//enumerating all adapters
		{
			UINT numAdapter = 0;
			IDXGIAdapter * pAdapter;
			while (giFactory->EnumAdapters(numAdapter, &pAdapter) != DXGI_ERROR_NOT_FOUND)
			{
				adapters[numAdapter] = pAdapter;
				//longing adapter's name
				{
					DXGI_ADAPTER_DESC adapterDesc;
					pAdapter->GetDesc(&adapterDesc);
					char buffer[128];
					wcstombs_s(nullptr, buffer, adapterDesc.Description, 128);
					adaptersName[numAdapter] = buffer;
					ULOG_MESSAGE("adapter % : %", numAdapter, buffer);
				}
				

				++numAdapter;
			}
			UASSERT(numAdapter > 0, "no adapter found");
			preferedAdapterIndex = (int)GEngineConfig()->AsNumber("GFX.AdapterIndex", 0);
			if (preferedAdapterIndex < numAdapter)
			{
				adapter = adapters[preferedAdapterIndex];
				ULOG_MESSAGE("adapter % selected", preferedAdapterIndex);
			}
			else
			{
				ULOG_WARN("cant get the specified adapter in 'GFX.AdapterIndex' uses first adapter instead");
				adapter = adapters[0];
			}
		}
		bool createDebug = false;
		unsigned flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
		if (createDebug)
			flags |= D3D11_CREATE_DEVICE_DEBUG;

		D3D_DRIVER_TYPE driverType = adapter ?  D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE;
		if (FAILED(D3D11CreateDevice(adapter, driverType, nullptr,flags, nullptr, 0, D3D11_SDK_VERSION, &device, &featureLevel, &immediateContext)))
		{
			ULOG_FATAL("failed to create DX11 device");
			return nullptr;
		}
		UASSERT(featureLevel >= D3D_FEATURE_LEVEL_11_0);
		ULOG_MESSAGE("DX11 device created, adapter [%] feature level: [%]", adaptersName[preferedAdapterIndex], EnumToStr(featureLevel));

		GFXDeviceDX* gfxDevice = new GFXDeviceDX;
		gfxDevice->mGIFactory = giFactory;
		gfxDevice->mAdapter = adapter;
		gfxDevice->mDevice = device;
		gfxDevice->mImmediateContext = immediateContext;
		gfxDevice->mFeatureLevel = featureLevel;
		
		gGFX = gfxDevice;

		return gfxDevice;
	}
	//////////////////////////////////////////////////////////////////////////
	GFXSwapChain* GFXDeviceDX::CreateSwapChain(const GFXSwapChainDesc& param)
	{
		UASSERT(IsGameThread());
		UASSERT(param.mGameWindow);

		HWND hwnd = (HWND)param.mGameWindow->GetWinHandle();
		Vec2I backBufferSize = Vec2I(0);
		param.mGameWindow->GetWinSize(backBufferSize);

		ULOG_MESSAGE("creating swapchain % ...", backBufferSize);

		HRESULT result;
		IDXGIOutput* adapterOutput;
		unsigned int numModes, i, numerator, denominator, stringLength;
		DXGI_ADAPTER_DESC adapterDesc;

		// Enumerate the primary adapter output (monitor).
		result = mAdapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(result))
		{
			ULOG_ERROR("no monitor found");
			return nullptr;
		}

		// Create a list to hold all the possible display modes for this monitor/video card combination.
		DXGI_MODE_DESC displayModeList[128];

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
			if (displayModeList[i].Width == backBufferSize.mX)
			{
				if (displayModeList[i].Height == backBufferSize.mY)
				{
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
			ULOG_MESSAGE("DML % w: %  h:% ", i, displayModeList[i].Width, displayModeList[i].Height);
		}


		adapterOutput->Release();
// 		adapter->Release();

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		// Set to a single back buffer.
		swapChainDesc.BufferCount = 1;
		// Set the width and height of the back buffer.
		swapChainDesc.BufferDesc.Width = backBufferSize.mX;
		swapChainDesc.BufferDesc.Height = backBufferSize.mY;
		// Set regular 32-bit surface for the back buffer.
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		// Set the refresh rate of the back buffer.
		if (param.mVSyncEnable)
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
		swapChainDesc.OutputWindow = hwnd;
		// Turn multisampling off.
		swapChainDesc.SampleDesc.Count = param.mSampleCount; /*Max(GEngineConfig()->AsNumber("GFX.MultiSample"), 1.0f)*/;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = !param.mFullScreem;
		// Set the scan line ordering and scaling to unspecified.
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		// Discard the back buffer contents after presenting.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		// Don't set the advanced flags.
		swapChainDesc.Flags = 0;
		IDXGISwapChain* swapChain = nullptr;
		if (FAILED(mGIFactory->CreateSwapChain(mDevice, &swapChainDesc, &swapChain)))
		{
			ULOG_FATAL("failed to create swapchain");
		}

		ID3D11Texture2D* backBuffer = nullptr;
		if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer)))
		{
			ULOG_FATAL("failed to get back buffer");
			return nullptr;
		}
		UASSERT(backBuffer);

		ID3D11RenderTargetView* rtv = nullptr;
		if (FAILED(mDevice->CreateRenderTargetView(backBuffer, nullptr, &rtv)))
		{
			ULOG_FATAL("");
		}
		backBuffer->Release();

		GFXSwapChainDX* gfxSwapchain = new GFXSwapChainDX(swapChain);
		gfxSwapchain->mBackBufferSize = backBufferSize;
		gfxSwapchain->mBackBufferView = new GFXRenderTargetViewDX(rtv);
		gfxSwapchain->mDesc = param;
		return gfxSwapchain;
	}





	void GFXDeviceDX::SetViewport(const GFXViewport& viewport)
	{
		D3D11_VIEWPORT dxViewport;
		dxViewport.TopLeftX = viewport.mTopLeftX;
		dxViewport.TopLeftY = viewport.mTopLeftY;
		dxViewport.Width = viewport.mWidth;
		dxViewport.Height = viewport.mHeight;
		dxViewport.MinDepth = viewport.mMinDepth;
		dxViewport.MaxDepth = viewport.mMaxDepth;
		mImmediateContext->RSSetViewports(1, &dxViewport);
	}





	void GFXDeviceDX::ClearRenderTarget(GFXRenderTargetView* view, const Color& color)
	{
		UASSERT(view);
		ID3D11RenderTargetView* rtv = view->HandleAs<ID3D11RenderTargetView*>();
		UASSERT(rtv);
		mImmediateContext->ClearRenderTargetView(rtv, color.mRGBA);
	}

	void GFXDeviceDX::ClearDepthStencil(GFXDepthStencilView* view, bool clearDepth, bool clearStencil, float depth, char stencil)
	{
		UASSERT(view);
		ID3D11DepthStencilView* dsv = view->HandleAs<ID3D11DepthStencilView*>();
		UASSERT(dsv);
		UINT clearFlag = 0;
		if (clearDepth) clearFlag |= D3D11_CLEAR_DEPTH;
		if (clearStencil) clearFlag |= D3D11_CLEAR_STENCIL;
		mImmediateContext->ClearDepthStencilView(dsv, clearFlag, depth, stencil);
	}

	GFXIndexBuffer* GFXDeviceDX::CreateIndexBuffer(const GFXIndexBufferDesc& param)
	{
		if (param.mInitialData == nullptr && param.mImmutable)
		{
			ULOG_ERROR("immutable buffer needs initial data");
			return nullptr;
		}

		D3D11_BUFFER_DESC desc;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = param.mSize;

		desc.CPUAccessFlags = 0;

		if (param.mImmutable)
		{
			desc.Usage = D3D11_USAGE_IMMUTABLE;
		}
		else if (param.mCPUReadAccess)
		{
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
			desc.Usage = D3D11_USAGE_STAGING;
		}
		else if (param.mDynamic)
		{
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.Usage = D3D11_USAGE_DYNAMIC;
		}

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = param.mInitialData;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		ID3D11Buffer* handle = nullptr;
		if (SUCCEEDED(mDevice->CreateBuffer(&desc, param.mInitialData ? &data : nullptr, &handle)))
		{
			return new GFXIndexBufferDX(handle, param);
		}
		ULOG_ERROR("Failed to create index buffer");
		return nullptr;
	}

	GFXVertexBuffer* GFXDeviceDX::CreateVertexBuffer(const GFXVertexBufferDesc& param)
	{
		if (param.mInitialData == nullptr && param.mImmutable)
		{
			ULOG_FATAL("immutable buffer needs initial data");
			return nullptr;
		}

		D3D11_BUFFER_DESC desc;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = param.mSize;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;

		if (param.mImmutable)
		{
			desc.Usage = D3D11_USAGE_IMMUTABLE;
		}
		else if (param.mDynamic)
		{
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.Usage = D3D11_USAGE_DYNAMIC;
		}

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = param.mInitialData;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		D3D11_SUBRESOURCE_DATA* pData = nullptr;
		if (param.mInitialData)
			pData = &data;

		ID3D11Buffer* handle = nullptr;
		if (SUCCEEDED(mDevice->CreateBuffer(&desc, pData, &handle)))
		{
			return new GFXVertexBufferDX(handle, param);
		}
		ULOG_ERROR("Failed to create vertex buffer");
		return nullptr;
	}

	GFXConstantBuffer* GFXDeviceDX::CreateConstantBuffer(const GFXConstantBufferDesc& param)
	{
		// 			UASSERT(param.mSize % 16 == 0);
		// 
		// 			D3D11_BUFFER_DESC desc;
		// 			desc.MiscFlags = 0;
		// 			desc.StructureByteStride = 0;
		// 			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		// 			desc.ByteWidth = param.mSize;
		// 			desc.CPUAccessFlags =  D3D11_CPU_ACCESS_WRITE;
		// 			desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		// 
		// 			D3D11_SUBRESOURCE_DATA data;
		// 			data.pSysMem = param.mInitialData;
		// 			data.SysMemPitch = 0;
		// 			data.SysMemSlicePitch = 0;
		// 
		// 			D3D11_SUBRESOURCE_DATA* pData = nullptr;
		// 			if (param.mInitialData) pData = &data;
		// 
		// 			ID3D11Buffer* handle = nullptr;
		// 			if (SUCCEEDED(mDevice->CreateBuffer(&desc, pData, &handle)))
		// 			{
		// 				GFXConstantBufferDX* ret = new GFXConstantBufferDX;
		// 				ret->mDevice = mDevice;
		// 				ret->mDeviceContext = mImmediateContext;
		// 				ret->mHandle = handle;
		// 				ret->mDesc = param;
		// 				ret->mDesc.mInitialData = nullptr;
		// 				ULOG_SUCCESS("constant buffer created");
		// 				return ret;
		// 			}
		// 			ULOG_ERROR("failed to create constant buffer");
		return nullptr;
	}

	GFXConstantBuffer* GFXDeviceDX::CreateConstantBuffer(unsigned size)
	{
		size = RoundUpToMulP2(size, 16);

		D3D11_BUFFER_DESC desc;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = size;
		desc.CPUAccessFlags =  D3D11_CPU_ACCESS_WRITE;
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

		ID3D11Buffer* handle = nullptr;
		if (SUCCEEDED(mDevice->CreateBuffer(&desc, nullptr, &handle)))
		{
			return new GFXConstantBufferDX(handle, GFXConstantBufferDesc());
		}
		ULOG_ERROR("failed to create constant buffer");
		return nullptr;
	}

	GFXDepthStencilState* GFXDeviceDX::CreateDepthStencilState(const GFXDepthStencilStateDesc& param)
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		desc.DepthEnable = param.mDepthEnable;
		desc.DepthWriteMask = param.mDepthWriteAll ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.DepthFunc = ToDXType(param.mDepthFunc);

		desc.StencilEnable = param.mStencilEnable;
		desc.StencilReadMask = param.mStencilReadMask;
		desc.StencilWriteMask = param.mStencilWriteMask;

		desc.FrontFace.StencilFailOp = ToDXType(param.mFrontFaceStencilFailOp);
		desc.FrontFace.StencilDepthFailOp = ToDXType(param.mFrontFaceStencilDepthFailOp);
		desc.FrontFace.StencilPassOp = ToDXType(param.mFrontFaceStencilPassOp);
		desc.FrontFace.StencilFunc = ToDXType(param.mFrontFaceStencilFunc);

		desc.BackFace.StencilFailOp = ToDXType(param.mBackFaceStencilFailOp);
		desc.BackFace.StencilDepthFailOp = ToDXType(param.mBackFaceStencilDepthFailOp);
		desc.BackFace.StencilPassOp = ToDXType(param.mBackFaceStencilPassOp);
		desc.BackFace.StencilFunc = ToDXType(param.mBackFaceStencilFunc);

		ID3D11DepthStencilState* handle = nullptr;
		if (SUCCEEDED(mDevice->CreateDepthStencilState(&desc, &handle)))
		{
			return new GFXDepthStencilStateDX(param, handle);
		}
		ULOG_ERROR("Failed to create depth stencil state");
		return nullptr;
	}

	GFXSamplerState* GFXDeviceDX::CreateSamplerState(const GFXSamplerStateDesc& param)
	{
		D3D11_SAMPLER_DESC desc;
		desc.Filter = ToDXType(param.mFilter);
		desc.AddressU = ToDXType(param.mAddressU);
		desc.AddressV = ToDXType(param.mAddressV);
		desc.AddressW = ToDXType(param.mAddressW);
		desc.MipLODBias = param.mMipLODBias;
		desc.MaxAnisotropy = param.mMaxAnisotropy;
		desc.ComparisonFunc = ToDXType(param.mComparisonFunc);
		param.mBorderColor.StoreTo(desc.BorderColor);
		desc.MinLOD = param.mMinLOD;
		desc.MaxLOD = param.mMaxLOD;

		ID3D11SamplerState* handle = nullptr;
		if (SUCCEEDED(mDevice->CreateSamplerState(&desc, &handle)))
		{
			return new GFXSamplerStateDX(param, handle);
		}
		ULOG_ERROR("Failed to create sampler state");
		return nullptr;
	}

	GFXRasterizerState* GFXDeviceDX::CreateRasterizerState(const GFXRasterizerStateDesc& param)
	{
		D3D11_RASTERIZER_DESC desc;
		desc.FillMode = param.mWireframe ? D3D11_FILL_MODE::D3D11_FILL_WIREFRAME : D3D11_FILL_MODE::D3D11_FILL_SOLID;
		desc.CullMode = ToDXType(param.mCullMode);
		desc.DepthBias = param.mDepthBias;
		desc.DepthBiasClamp = param.mDepthBiasClamp;
		desc.DepthClipEnable = param.mDepthClipEnable;
		desc.AntialiasedLineEnable = param.mAntialiasedLineEnable;
		desc.FrontCounterClockwise = param.mFrontCounterClockWise;
		desc.MultisampleEnable = param.mMultisampleEnable;
		desc.ScissorEnable = param.mScissorEnable;
		desc.SlopeScaledDepthBias = param.mSlopeScaledDepthBias;

		ID3D11RasterizerState* handle = nullptr;
		if (SUCCEEDED(mDevice->CreateRasterizerState(&desc, &handle)))
		{
			return new GFXRasterizerStateDX(param, handle, nullptr);
		}
		ULOG_ERROR("Failde to create rasterizer");
		return nullptr;
	}

	void GFXDeviceDX::SetVertexBuffer(GFXVertexBuffer* buffer, unsigned slot, unsigned stride, unsigned offset)
	{
		if (buffer == nullptr)
		{
			mImmediateContext->IASetVertexBuffers(slot, 1, nullptr, nullptr, nullptr);
			return;
		}

		ID3D11Buffer* buffers[1] = { buffer->HandleAs<ID3D11Buffer*>() };
		UINT strides[1] = { stride };
		UINT offsets[1] = { offset };
		mImmediateContext->IASetVertexBuffers(slot, 1, buffers, strides, offsets);
	}

	void GFXDeviceDX::SetIndexBuffer(GFXIndexBuffer* buffer, unsigned offset /*= 0*/)
	{
		if (buffer == nullptr)
		{
			mImmediateContext->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);
			return;
		}
		mImmediateContext->IASetIndexBuffer(buffer->HandleAs<ID3D11Buffer*>(), ToDXType(buffer->GetDesc().mType), offset);
	}

	void GFXDeviceDX::SetConstentBuffer(GFXConstantBuffer** buffers, unsigned startSlot, unsigned numBuffers, EShaderType whichShader)
	{
		UASSERT(buffers);
		ID3D11Buffer* dxBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
		for (unsigned i = 0; i < numBuffers; i++)
		{
			dxBuffers[i] = nullptr;
			if (buffers[i])
				dxBuffers[i] = buffers[i]->HandleAs<ID3D11Buffer*>();
		}

		switch (whichShader)
		{
		case UPO::EShaderType::EVertex: mImmediateContext->VSSetConstantBuffers(startSlot, numBuffers, dxBuffers);
			break;
		case UPO::EShaderType::EHull: mImmediateContext->HSSetConstantBuffers(startSlot, numBuffers, dxBuffers);
			break;
		case UPO::EShaderType::EDomain: mImmediateContext->DSSetConstantBuffers(startSlot, numBuffers, dxBuffers);
			break;
		case UPO::EShaderType::EGeometry: mImmediateContext->GSSetConstantBuffers(startSlot, numBuffers, dxBuffers);
			break;
		case UPO::EShaderType::EPixel: mImmediateContext->PSSetConstantBuffers(startSlot, numBuffers, dxBuffers);
			break;
		case UPO::EShaderType::ECompute: mImmediateContext->CSSetConstantBuffers(startSlot, numBuffers, dxBuffers);
			break;
		}

	}

	void GFXDeviceDX::SetResourceView(GFXShaderResourceView** views, unsigned startSlot, unsigned numViews, EShaderType whichShder)
	{
		ID3D11ShaderResourceView* dxViews[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
		for (unsigned i = 0; i < numViews; i++)
		{
			dxViews[i] = nullptr;
			if (views[i])
				dxViews[i] = views[i]->HandleAs<ID3D11ShaderResourceView*>();
		}

		switch (whichShder)
		{
		case UPO::EShaderType::EVertex: mImmediateContext->VSSetShaderResources(startSlot, numViews, dxViews);
			break;
		case UPO::EShaderType::EHull: mImmediateContext->HSSetShaderResources(startSlot, numViews, dxViews);
			break;
		case UPO::EShaderType::EDomain: mImmediateContext->DSSetShaderResources(startSlot, numViews, dxViews);
			break;
		case UPO::EShaderType::EGeometry: mImmediateContext->GSSetShaderResources(startSlot, numViews, dxViews);
			break;
		case UPO::EShaderType::EPixel: mImmediateContext->PSSetShaderResources(startSlot, numViews, dxViews);
			break;
		case UPO::EShaderType::ECompute: mImmediateContext->CSSetShaderResources(startSlot, numViews, dxViews);
			break;
		}
	}

	void GFXDeviceDX::SetResourceView(GFXTexture2D** textures, unsigned startSlot, unsigned numTextures, EShaderType whichShader)
	{
		UASSERT(textures);

		ID3D11ShaderResourceView* dxViews[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
		for (unsigned i = 0; i < numTextures; i++)
		{
			dxViews[i] = nullptr;
			if (textures[i])
			{
				if(auto srv = textures[i]->GetShaderResourceView())
					dxViews[i] = srv->HandleAs<ID3D11ShaderResourceView*>();
			}
		}


		switch (whichShader)
		{
		case UPO::EShaderType::EVertex: mImmediateContext->VSSetShaderResources(startSlot, numTextures, dxViews);
			break;
		case UPO::EShaderType::EHull: mImmediateContext->HSSetShaderResources(startSlot, numTextures, dxViews);
			break;
		case UPO::EShaderType::EDomain:mImmediateContext->DSSetShaderResources(startSlot, numTextures, dxViews);
			break;
		case UPO::EShaderType::EGeometry: mImmediateContext->GSSetShaderResources(startSlot, numTextures, dxViews);
			break;
		case UPO::EShaderType::EPixel: mImmediateContext->PSSetShaderResources(startSlot, numTextures, dxViews);
			break;
		case UPO::EShaderType::ECompute: mImmediateContext->CSSetShaderResources(startSlot, numTextures, dxViews);
			break;
		}
	}

	void GFXDeviceDX::SetPrimitiveTopology(EPrimitiveTopology topology)
	{
		mImmediateContext->IASetPrimitiveTopology(ToDXType(topology));
	}

	void GFXDeviceDX::Draw(unsigned vertexCount, unsigned startVertexLocation, unsigned instanceCount, unsigned startInstanceLocation)
	{
		if (instanceCount > 1)
			mImmediateContext->DrawInstanced(vertexCount, instanceCount, startVertexLocation, startInstanceLocation);
		else
			mImmediateContext->Draw(vertexCount, startVertexLocation);
	}

	void GFXDeviceDX::DrawIndexed(unsigned indexCount, unsigned startIndexLocation, unsigned baseVertexLocation, unsigned instanceCount, unsigned startInstanceLocation)
	{
		if (instanceCount > 1)
			mImmediateContext->DrawIndexedInstanced(indexCount, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
		else
			mImmediateContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
	}

	GFXShaderResourceView* GFXDeviceDX::CreateShaderResourceView(GFXTexture2D* texture2D, unsigned mostDetailedMip, unsigned numMipLevels, EPixelFormat format)
	{
		UASSERT(texture2D);
		ID3D11Texture2D* dxTexture2D = texture2D->HandleAs<ID3D11Texture2D*>();
		D3D11_TEXTURE2D_DESC textureDesc;
		dxTexture2D->GetDesc(&textureDesc);
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		if (textureDesc.SampleDesc.Count > 1)	//is multi sample?
		{
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		}
		else
		{
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			//As an example, assuming MostDetailedMip = 6 and MipLevels = 2, 
			//the view will have access to 2 mipmap levels, 6 and 7, of the original texture
			desc.Texture2D.MostDetailedMip = mostDetailedMip;
			desc.Texture2D.MipLevels = numMipLevels;
		}
		desc.Format = GetFormatForShaderResourceView(format == EPixelFormat::UNKNOWN ? textureDesc.Format : ToDXType(format));
		ID3D11ShaderResourceView* srv;
		HRESULT result = mDevice->CreateShaderResourceView(dxTexture2D, &desc, &srv);
		if (FAILED(result))
		{
			ULOG_ERROR("failed to create shader resource view");
			return nullptr;
		}
		return new GFXShaderResourceViewDX(srv);
	}

	GFXShaderResourceView* GFXDeviceDX::CreateShaderResourceView(GFXTexture2D* texture2D)
	{
		UASSERT(texture2D);
		ID3D11ShaderResourceView* srv = nullptr;
		if (FAILED(mDevice->CreateShaderResourceView(texture2D->HandleAs<ID3D11Texture2D*>(), nullptr, &srv)))
		{
			ULOG_ERROR("failed to create shader resource view");
		}
		return new GFXShaderResourceViewDX(srv);
	}

	GFXTexture2D* GFXDeviceDX::CreateTexture2DFromMemory(const Buffer& memory)
	{
		unsigned flag = 0;
		DirectX::ScratchImage image;
		DirectX::TexMetadata metaData;
		HRESULT hr = S_OK;


		hr = DirectX::LoadFromDDSMemory(memory.Data(), memory.Size(), flag, &metaData, image);
		if (FAILED(hr))
		{
			hr = DirectX::LoadFromTGAMemory(memory.Data(), memory.Size(), &metaData, image);
		}
		if (FAILED(hr))
		{
			//TODO DirectX::LoadFromWICMemory doesn't work
			hr = DirectX::LoadFromWICMemory(memory.Data(), memory.Size(), flag, &metaData, image);
		}
		if (FAILED(hr))
		{
			ULOG_ERROR("failed to load texture");
			return nullptr;
		}

		ID3D11ShaderResourceView* dxSRV = nullptr;


		if (FAILED(DirectX::CreateShaderResourceView(mDevice, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &dxSRV)))
		{
			ULOG_ERROR("failed to create texture or shader resource view");
			return nullptr;
		}
		ID3D11Resource* texture2DResource = nullptr;
		dxSRV->GetResource(&texture2DResource);
		UASSERT(texture2DResource);
		ID3D11Texture2D* texture2D = nullptr;
		texture2DResource->QueryInterface<ID3D11Texture2D>(&texture2D);
		UASSERT(texture2D);
		D3D11_TEXTURE2D_DESC texture2DDesc;
		texture2D->GetDesc(&texture2DDesc);
		//texture2D->Release();

		{
			GFXTexture2DDesc desc;
			desc.mFlag = ETextureFlag::EImmutable | ETextureFlag::EShaderResourceView;
			desc.mFormat = ToGFXType(texture2DDesc.Format);
			desc.mWidth = texture2DDesc.Width;
			desc.mHeight = texture2DDesc.Height;
			desc.mSampleCount = texture2DDesc.SampleDesc.Count;
			desc.mMipLevels = texture2DDesc.MipLevels;

			auto srv = new GFXShaderResourceViewDX(dxSRV);
			return new GFXTexture2DDX(texture2D, srv, nullptr, nullptr, desc);
		}
	}

	GFXRenderTargetView* GFXDeviceDX::CreateRenderTargetView(GFXTexture2D* texture)
	{
		if (!texture) return nullptr;

		ID3D11Texture2D* dxTexture = texture->HandleAs<ID3D11Texture2D*>();
		if (!dxTexture) return nullptr;

		ULOG_WARN("DXT %", (size_t)dxTexture);

		UASSERT(mDevice);
		ID3D11RenderTargetView* rtv = nullptr;
		if (FAILED(mDevice->CreateRenderTargetView(dxTexture, nullptr, &rtv)))
		{
			ULOG_ERROR("Failed to crate render target view");
			return nullptr;
		}
		return new GFXRenderTargetViewDX(rtv);
	}

	GFXRenderTargetView* GFXDeviceDX::CreateRenderTargetView(GFXTexture2D* texture, unsigned mipSlice, EPixelFormat format /*= EPixelFormat::UNKNOWN*/)
	{
		if (!texture) return nullptr;

		ID3D11Texture2D* dxTexture = texture->HandleAs<ID3D11Texture2D*>();
		if (!dxTexture) return nullptr;

		ID3D11RenderTargetView* rtv = nullptr;
		D3D11_RENDER_TARGET_VIEW_DESC desc;

		D3D11_TEXTURE2D_DESC textureDesc;
		dxTexture->GetDesc(&textureDesc);
		if (textureDesc.SampleDesc.Count > 1)	//is multi sampler ?
		{
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		}
		else
		{
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = mipSlice;
		}

		if (format == EPixelFormat::UNKNOWN)
		{
			desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;	//the format of the resource will be used
		}
		else
		{
			desc.Format = GetFormatForRenderTargetView(ToDXType(format));
		}

		if (FAILED(mDevice->CreateRenderTargetView(dxTexture, &desc, &rtv)))
		{
			ULOG_ERROR("Failed to create render target view");
			return nullptr;
		}
		return new GFXRenderTargetViewDX(rtv);
	}
	//////////////////////////////////////////////////////////////////////////
	GFXDepthStencilView* GFXDeviceDX::CreateDepthStencilView(GFXTexture2D* texture)
	{
		if (!texture) return nullptr;

		ID3D11Texture2D* dxTexture = texture->HandleAs<ID3D11Texture2D*>();
		if (!dxTexture) return nullptr;

		ID3D11DepthStencilView* dsv = nullptr;
		if (FAILED(mDevice->CreateDepthStencilView(dxTexture, nullptr, &dsv)))
		{
			ULOG_ERROR("failed to create DepthStencilview");
			return nullptr;
		}
		return new GFXDepthStencilViewDX(dsv);
	}

	GFXDepthStencilView* GFXDeviceDX::CreateDepthStencilView(GFXTexture2D* texture, unsigned mipSlice, EPixelFormat format)
	{
		if (!texture) return nullptr;

		ID3D11Texture2D* dxTexture = texture->HandleAs<ID3D11Texture2D*>();
		if (!dxTexture) return nullptr;

		D3D11_TEXTURE2D_DESC textureDesc;
		dxTexture->GetDesc(&textureDesc);

		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		desc.Flags = 0;

		if (textureDesc.SampleDesc.Count > 1) // is multi sample
		{
			desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		}
		else
		{
			desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = mipSlice;
		}
		if (format == EPixelFormat::UNKNOWN)
			desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		else
			desc.Format = GetFormatForDepthStencilView(ToDXType(format));

		ID3D11DepthStencilView* dsv = nullptr;
		if (FAILED(mDevice->CreateDepthStencilView(dxTexture, &desc, &dsv)))
		{
			ULOG_ERROR("failed to create DepthStencilview");
			return nullptr;
		}
		return new GFXDepthStencilViewDX(dsv);
	}

	GFXTexture2D* GFXDeviceDX::CreateTexture2D(const GFXTexture2DDesc& param)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroType(textureDesc);

		textureDesc.CPUAccessFlags = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;

		bool gpuNeedWrite = false;
		if (param.mFlag.TestAny(ETextureFlag::ERenderTargatable | ETextureFlag::EDepthStencil))
			gpuNeedWrite = true;

		if (param.mInitialData)
		{
			ULOG_FATAL("not implemented");
			return nullptr;
		}
		if (param.mMipLevels != 1 && param.mInitialData)
		{
			ULOG_FATAL("currently only 1 miplevel is implemented");
			return nullptr;
		}

		if (param.mFlag.Test(ETextureFlag::EImmutable))
		{
			if (!param.mInitialData)
			{
				ULOG_ERROR("immutable needs Initial data");
				return nullptr;
			}
			textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
		}
		else if (param.mFlag.Test(ETextureFlag::ECPURead))
		{
			textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			if(param.mFlag.Test(ETextureFlag::ECPUWrite))
				textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			textureDesc.Usage = D3D11_USAGE_STAGING;
		}
		else if (param.mFlag.Test(ETextureFlag::ECPUWrite))
		{
			textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			textureDesc.Usage = gpuNeedWrite ? D3D11_USAGE_STAGING : D3D11_USAGE_DYNAMIC;
		}

		if (param.mFlag.Test(ETextureFlag::EShaderResourceView))
			textureDesc.BindFlags |= D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
		if (param.mFlag.Test(ETextureFlag::ERenderTargatable))
			textureDesc.BindFlags |= D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
		if (param.mFlag.Test(ETextureFlag::EDepthStencil))
			textureDesc.BindFlags |= D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;

		//All textures in a resource must have the same format, size, and number of mipmap levels.
		textureDesc.ArraySize = 1;
		textureDesc.Width = param.mWidth;
		textureDesc.Height = param.mHeight;
		textureDesc.MipLevels = param.mMipLevels;
		textureDesc.SampleDesc.Count = param.mSampleCount;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.MiscFlags = 0;
		DXGI_FORMAT pixelFormat = ToDXType(param.mFormat);
		textureDesc.Format = pixelFormat;

// 		size_t pitch = 0, slicePitch = 0;
// 		//DirectX::ComputePitch(pixelFormat, param.mWidth, param.mHeight, pitch, slicePitch);
// 		D3D11_SUBRESOURCE_DATA data;
// 		data.pSysMem = param.mInitialData;
// 		data.SysMemPitch = pitch;
// 		data.SysMemSlicePitch = 0;
// 		ID3D11Texture2D* handleTexture = nullptr;
// 		D3D11_SUBRESOURCE_DATA* pData = param.mInitialData == nullptr ? nullptr : &data;

		ID3D11Texture2D* texture = nullptr;
		ID3D11ShaderResourceView* shaderResourceView = nullptr;
		ID3D11RenderTargetView* renderTargetView = nullptr;
		ID3D11DepthStencilView* depthStencilView = nullptr;

		////////////////////////////create actual texture2D, currently initial data not implemented
		HRESULT result = mDevice->CreateTexture2D(&textureDesc, nullptr, &texture);
		DXERRORFATAL(result);

		///////////////////create shader resource view
		if (param.mFlag.Test(ETextureFlag::EShaderResourceView))
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC rvDesc;
			rvDesc.Format = GetFormatForShaderResourceView(pixelFormat);
			if (param.mSampleCount > 1)
			{
				rvDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D_SRV_DIMENSION_TEXTURE2DMS;
			}
			else
			{
				rvDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D_SRV_DIMENSION_TEXTURE2D;
				rvDesc.Texture2D.MostDetailedMip = 0;
				rvDesc.Texture2D.MipLevels = 1;
			}
			result = mDevice->CreateShaderResourceView(texture, &rvDesc, &shaderResourceView);
		}

		/////////////////////create render target view
		if (param.mFlag.Test(ETextureFlag::ERenderTargatable))
		{
			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
			rtvDesc.Format = GetFormatForRenderTargetView(pixelFormat);
			if (param.mSampleCount > 1)
			{
				rtvDesc.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2DMS;
			}
			else
			{
				rtvDesc.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2D;
				rtvDesc.Texture2D.MipSlice = 0;
			}
			result = mDevice->CreateRenderTargetView(texture, &rtvDesc, &renderTargetView);
		}


		//////////////////create depth stencil view
		if (param.mFlag.Test(ETextureFlag::EDepthStencil))
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC	dsvDesc;
			dsvDesc.Flags = 0;
			dsvDesc.Format = GetFormatForDepthStencilView(pixelFormat);
			if (param.mSampleCount > 1)
			{
				dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
			}
			else
			{
				dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				dsvDesc.Texture2D.MipSlice = 0;
			}

			result = mDevice->CreateDepthStencilView(texture, &dsvDesc, &depthStencilView);
		}

		GFXShaderResourceView* gfxShaderResourceView = nullptr;
		GFXRenderTargetView* gfxRenderTargetView = nullptr;
		GFXDepthStencilView* gfxDepthStencilView = nullptr;

		if (shaderResourceView) gfxShaderResourceView = new GFXShaderResourceViewDX(shaderResourceView);
		if (renderTargetView) gfxRenderTargetView = new GFXRenderTargetViewDX(renderTargetView);
		if (depthStencilView) gfxDepthStencilView = new GFXDepthStencilViewDX(depthStencilView);

		return new GFXTexture2DDX(texture, gfxShaderResourceView, gfxRenderTargetView, gfxDepthStencilView, param);

#if 0

		UASSERT(param.mWidth < MAX_TEXTURE2D_SIZE && param.mHeight < MAX_TEXTURE2D_SIZE);
		if (param.mUsage == EResourceUsage::EImmutable && param.mInitialData == nullptr)
		{
			ULOG_FATAL("immutable texture needs initial data");
			return nullptr;
		}
		if (param.mMipLevels != 1)
		{
			ULOG_FATAL("currently only 1 miplevel is implemented");
			return nullptr;
		}
		DXGI_FORMAT pixelFormat = ToDXType(param.mFormat);
		if (param.mCreateDepthStencilView && !DirectX::IsDepthStencil(pixelFormat))
		{
			ULOG_FATAL("invalid pixel format for DepthStencil");
			return nullptr;
		}

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = param.mWidth;
		desc.Height = param.mHeight;
		desc.MipLevels = param.mMipLevels;
		desc.ArraySize = 1;
		desc.MiscFlags = 0;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = ToDXType(param.mUsage);
		desc.Format = pixelFormat;
		desc.CPUAccessFlags = 0;

		desc.BindFlags = 0;
		if (param.mCreateShaderResourceView)
			desc.BindFlags |= D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
		if (param.mCreateRenderTargetView)
			desc.BindFlags |= D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
		if (param.mCreateDepthStencilView)
			desc.BindFlags |= D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;

		size_t pitch = 0, slicePitch = 0;
		// 			DirectX::ComputePitch(pixelFormat, param.mWidth, param.mHeight, pitch, slicePitch);
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = param.mInitialData;
		data.SysMemPitch = pitch;
		data.SysMemSlicePitch = slicePitch;
		ID3D11Texture2D* handleTexture = nullptr;
		D3D11_SUBRESOURCE_DATA* pData = param.mInitialData == nullptr ? nullptr : &data;
		if (SUCCEEDED(mDevice->CreateTexture2D(&desc, pData, &handleTexture)))
		{
			ID3D11ShaderResourceView* handleSRV = nullptr;
			if (param.mCreateShaderResourceView)
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC resViewDesc;
				resViewDesc.Format = pixelFormat;
				resViewDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D10_1_SRV_DIMENSION_TEXTURE2D;
				resViewDesc.Texture2D.MostDetailedMip = 0;
				resViewDesc.Texture2D.MipLevels = 1;
				if (FAILED(mDevice->CreateShaderResourceView(handleTexture, &resViewDesc, &handleSRV)))
				{
					ULOG_FATAL("Failed to create ShaderResourceView");
					handleTexture->Release();
					return nullptr;
				}
			}

			ID3D11RenderTargetView* handleRTV = nullptr;
			if (param.mCreateRenderTargetView)
			{
				D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
				renderTargetViewDesc.Format = pixelFormat;
				renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2D;
				renderTargetViewDesc.Texture2D.MipSlice = 0;
				if (FAILED(mDevice->CreateRenderTargetView(handleTexture, &renderTargetViewDesc, &handleRTV)))
				{
					ULOG_FATAL("failed to create RenderTargetView");
					if (handleSRV) handleSRV->Release();
					handleTexture->Release();
					return nullptr;
				}

			}
			ID3D11DepthStencilView* handleDSV = nullptr;
			if (param.mCreateDepthStencilView)
			{
				D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
				depthStencilViewDesc.Format = pixelFormat;
				depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
				depthStencilViewDesc.Texture2D.MipSlice = 0;
				depthStencilViewDesc.Flags = 0;
				if (FAILED(mDevice->CreateDepthStencilView(handleTexture, &depthStencilViewDesc, &handleDSV)))
				{
					ULOG_FATAL("failed to create DepthStencilView");
					if (handleSRV) handleSRV->Release();
					if (handleRTV) handleRTV->Release();
					handleTexture->Release();
					return nullptr;
				}
			}

			GFXTexture2DDX* ret = new GFXTexture2DDX;
			ret->mDesc = param;
			ret->mTexture = handleTexture;
			ret->mResourceView = handleSRV;
			ret->mRenderTargetView = handleRTV;
			ret->mDepthStencilView = handleDSV;

			ULOG_SUCCESS("Texture created");
			return ret;
		}
		ULOG_FATAL("texture creation failed");
		return nullptr;
#endif // 0
	}

	void GFXDeviceDX::SetDepthStencilState(GFXDepthStencilStateHandle state)
	{
		if (state)
			mImmediateContext->OMSetDepthStencilState(state.HandleAs<ID3D11DepthStencilState*>(), 1);
		else
			mImmediateContext->OMSetDepthStencilState(nullptr, 0);
	}


	//////////////////////////////////////////////////////////////////////////
	void GFXDeviceDX::SetRenderTarget(GFXRenderTargetView** renderTargets, unsigned numRenderTargets, GFXDepthStencilView* depthStencil)
	{
		ID3D11RenderTargetView* rtvs[GFX_MAX_RENDER_TARGET];
		for (unsigned i = 0; i < numRenderTargets; i++)
		{
			rtvs[i] = renderTargets[i]->HandleAs<ID3D11RenderTargetView*>();
		}

		ID3D11DepthStencilView* dsv = nullptr;
		if (depthStencil)
		{
			dsv = depthStencil->HandleAs<ID3D11DepthStencilView*>();
		}
		mImmediateContext->OMSetRenderTargets(numRenderTargets, rtvs, dsv);
	}

	//////////////////////////////////////////////////////////////////////////
	void GFXDeviceDX::SetRasterizerState(GFXRasterizerStateHandle state)
	{
		mImmediateContext->RSSetState(state ? state.HandleAs<ID3D11RasterizerState*>() : nullptr);
	}

	
	//////////////////////////////////////////////////////////////////////////
	GFXShader* GFXDeviceDX::CreateShader(const ShaderUniqueParam& param)
	{
		if (param.mFileName == nullptr || param.mEntryPoint == nullptr) return nullptr;

		Buffer byteCodes;

		if (!ShaderMgr::Get()->GetShaderCode(param, byteCodes))
		{
			ULOG_ERROR("failed to get shader [%] bytecodes", param.mFileName);
			return nullptr;
		}

		GFXShader* ret = CreateShader(byteCodes, param.mType, nullptr);
		if (ret == nullptr)
		{
			ULOG_ERROR("failed to create shader; name [%] entryPoint [%] type [%]", param.mFileName, param.mEntryPoint, EnumToStr(param.mType));
			return nullptr;
		}
		ULOG_SUCCESS("shader created. name [%] type [%]", param.mFileName, EnumToStr(param.mType));
		return ret;
	}

	GFXShader* GFXDeviceDX::CreateShader(const Buffer& bytecodes, EShaderType type, Name debugName)
	{
		void* handle = nullptr;
		HRESULT result = S_OK;
		GFXShader* shader = nullptr;
		switch (type)
		{
		case EShaderType::EVertex:
			result = mDevice->CreateVertexShader(bytecodes.Data(), bytecodes.Size(), nullptr, (ID3D11VertexShader**)&handle);
			shader = new GFXVertexShaderDX(bytecodes, handle, debugName);
			break;
		case EShaderType::EHull:
			result = mDevice->CreateHullShader(bytecodes.Data(), bytecodes.Size(), nullptr, (ID3D11HullShader**)&handle);
			shader = new GFXHullShaderDX(bytecodes, handle, debugName);
			break;
		case EShaderType::EDomain:
			result = mDevice->CreateDomainShader(bytecodes.Data(), bytecodes.Size(), nullptr, (ID3D11DomainShader**)&handle);
			shader = new GFXDomainShaderDX(bytecodes, handle, debugName);
			break;
		case EShaderType::EGeometry:
			result = mDevice->CreateGeometryShader(bytecodes.Data(), bytecodes.Size(), nullptr, (ID3D11GeometryShader**)&handle);
			shader = new GFXGeometryShaderDX(bytecodes, handle, debugName);
			break;
		case EShaderType::EPixel:
			result = mDevice->CreatePixelShader(bytecodes.Data(), bytecodes.Size(), nullptr, (ID3D11PixelShader**)&handle);
			shader = new GFXPixelShaderDX(bytecodes, handle, debugName);
			break;
		case EShaderType::ECompute:
			result = mDevice->CreateComputeShader(bytecodes.Data(), bytecodes.Size(), nullptr, (ID3D11ComputeShader**)&handle);
			shader = new GFXComputeShaderDX(bytecodes, handle, debugName);
			break;
		}

		if (SUCCEEDED(result))
		{
			return shader;
		}
		else
		{
			ULOG_ERROR("failed to create shader %", debugName);
			if (shader) delete shader;
			return nullptr;
		}
	}

	void GFXDeviceDX::SetShaders(GFXVertexShader* vertexShader, GFXPixelShader* pixelShader)
	{
		GFXShaderBound bound;
		bound.mVertexShader = vertexShader;
		bound.mPixelShader = pixelShader;
		SetShader(bound);
	}

	void GFXDeviceDX::SetShader(const GFXShaderBound& shaders)
	{
		ID3D11VertexShader* vs = nullptr;
		ID3D11DomainShader* ds = nullptr;
		ID3D11HullShader* hs = nullptr;
		ID3D11GeometryShader* gs = nullptr;
		ID3D11PixelShader* ps = nullptr;
		ID3D11ComputeShader* cs = nullptr;

		if (shaders.mVertexShader) vs = shaders.mVertexShader->HandleAs<ID3D11VertexShader*>();
		if (shaders.mHullShader)   hs = shaders.mHullShader->HandleAs<ID3D11HullShader*>();
		if (shaders.mDomainShader) ds = shaders.mDomainShader->HandleAs<ID3D11DomainShader*>();
		if (shaders.mGeometryShader) gs = shaders.mGeometryShader->HandleAs<ID3D11GeometryShader*>();
		if (shaders.mPixelShader) ps = shaders.mPixelShader->HandleAs<ID3D11PixelShader*>();
		if (shaders.mComputeShader) cs = shaders.mComputeShader->HandleAs<ID3D11ComputeShader*>();

		mImmediateContext->VSSetShader(vs, nullptr, 0);
		mImmediateContext->DSSetShader(ds, nullptr, 0);
		mImmediateContext->HSSetShader(hs, nullptr, 0);
		mImmediateContext->GSSetShader(gs, nullptr, 0);
		mImmediateContext->PSSetShader(ps, nullptr, 0);
		mImmediateContext->CSSetShader(cs, nullptr, 0);
	}



	//////////////////////////////////////////////////////////////////////////
	void GFXDeviceDX::SetSamplerState(GFXSamplerStateHandle* samplers, unsigned startSlot, unsigned numSamplers, EShaderType whichShader)
	{
		UASSERT(samplers);

		ID3D11SamplerState* dxSamplers[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
		for (unsigned i = 0; i < numSamplers; i++)
		{
			dxSamplers[i] = nullptr;
			if (samplers[i])
				dxSamplers[i] = samplers[i].HandleAs<ID3D11SamplerState*>();
		}

		switch (whichShader)
		{
		case UPO::EShaderType::EVertex: mImmediateContext->VSSetSamplers(startSlot, numSamplers, dxSamplers);
			break;
		case UPO::EShaderType::EHull: mImmediateContext->HSSetSamplers(startSlot, numSamplers, dxSamplers);
			break;
		case UPO::EShaderType::EDomain:mImmediateContext->DSSetSamplers(startSlot, numSamplers, dxSamplers);
			break;
		case UPO::EShaderType::EGeometry: mImmediateContext->GSSetSamplers(startSlot, numSamplers, dxSamplers);
			break;
		case UPO::EShaderType::EPixel: mImmediateContext->PSSetSamplers(startSlot, numSamplers, dxSamplers);
			break;
		case UPO::EShaderType::ECompute: mImmediateContext->CSSetSamplers(startSlot, numSamplers, dxSamplers);
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	GFXInputLayout* GFXDeviceDX::CreateInputLayout(const GFXInputLayoutDesc& param)
	{
		UASSERT(param.mVertexShader);

		unsigned numElement = param.GetNumElement();

		D3D11_INPUT_ELEMENT_DESC d3DElements[GFXInputLayoutDesc::MAX_ELEMENT];
		for (unsigned i = 0; i < numElement; i++)
		{
			auto elem = param.mElements[i];
			d3DElements[i].SemanticName = elem.mName;
			d3DElements[i].SemanticIndex = 0;
			d3DElements[i].InstanceDataStepRate = 0;
			d3DElements[i].InputSlotClass = elem.mPerInstance ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
			d3DElements[i].InputSlot = elem.mSlot;
			d3DElements[i].Format = ToDXType(elem.mFormat);
			d3DElements[i].AlignedByteOffset = elem.mOffset >= 0 ? elem.mOffset : D3D11_APPEND_ALIGNED_ELEMENT;
		}
		GFXVertexShaderDX* shader = param.mVertexShader->As<GFXVertexShaderDX>();

		ID3D11InputLayout* handle = nullptr;
		if (FAILED(mDevice->CreateInputLayout(d3DElements, numElement, shader->mShader.GetByteCode(), shader->mShader.GetByteCodeSize(), &handle)))
		{
			ULOG_ERROR("failed to create InputLayout");
			return nullptr;
		}
		return new GFXInputLayoutDX(param, handle);
	}
	//////////////////////////////////////////////////////////////////////////
	void GFXDeviceDX::SetInputLayout(GFXInputLayoutHandle layout)
	{
		mImmediateContext->IASetInputLayout(layout.HandleAs<ID3D11InputLayout*>());
	}
	//////////////////////////////////////////////////////////////////////////
	GFXBlendState* GFXDeviceDX::CreateBlendState(const GFXBlendStateDesc& param)
	{
		D3D11_BLEND_DESC desc;
		desc.AlphaToCoverageEnable = param.mAlphaToCoverage;
		desc.IndependentBlendEnable = param.mIndependentBlend;
		for (unsigned i = 0; i < GFX_MAX_RENDER_TARGET; i++)
		{
			desc.RenderTarget[i].BlendEnable = param.mRenderTargets[i].mBlendEnable;
			desc.RenderTarget[i].BlendOp = ToDXType(param.mRenderTargets[i].mBlendOp);
			desc.RenderTarget[i].BlendOpAlpha = ToDXType(param.mRenderTargets[i].mBlendOpAlpha);
			desc.RenderTarget[i].DestBlend = ToDXType(param.mRenderTargets[i].mDestBlend);
			desc.RenderTarget[i].DestBlendAlpha = ToDXType(param.mRenderTargets[i].mDestBlendAlpha);
			desc.RenderTarget[i].SrcBlend = ToDXType(param.mRenderTargets[i].mSrcBlend);
			desc.RenderTarget[i].SrcBlendAlpha = ToDXType(param.mRenderTargets[i].mSrcBlendAlpha);
			desc.RenderTarget[i].RenderTargetWriteMask = ToDXType(param.mRenderTargets[i].mWriteMask);
		}

		ID3D11BlendState* handle = nullptr;
		if (SUCCEEDED(mDevice->CreateBlendState(&desc, &handle)))
		{
			return new GFXBlendStateDX(param, handle);
		}
		ULOG_ERROR("failed to create blend state");
		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	void GFXDeviceDX::SetBlendState(GFXBlendStateHandle blendState, const Color& blendFactor, unsigned sampleMask /*= 0xFFffFFff*/)
	{
		mImmediateContext->OMSetBlendState(blendState.HandleAs<ID3D11BlendState*>(), blendFactor.mRGBA, sampleMask);
	}
	//////////////////////////////////////////////////////////////////////////
	void GFXDeviceDX::ClearState()
	{
		mImmediateContext->ClearState();
	}
	bool GFXDeviceDX::IsImmediate()
	{
		return mImmediateContext->GetType() == D3D11_DEVICE_CONTEXT_TYPE::D3D11_DEVICE_CONTEXT_IMMEDIATE;
	}




	void* GFXDeviceDX::Map(GFXBuffer* buffer, EMapFlag flag)
	{
		UASSERT(buffer);
		ID3D11Buffer* dxBuffer = buffer->HandleAs<ID3D11Buffer*>();
		D3D11_MAPPED_SUBRESOURCE mapped;
		DXERRORFATAL(mImmediateContext->Map(dxBuffer, 0, ToDXType(flag), 0, &mapped));
		return mapped.pData;
	}

	//////////////////////////////////////////////////////////////////////////
	void* GFXDeviceDX::Map(GFXTexture2D* texture, EMapFlag flag, unsigned mipIndex, unsigned& outRowPitch)
	{
		UASSERT(texture);
		ID3D11Texture2D* dxTexture = texture->HandleAs<ID3D11Texture2D*>();
		D3D11_MAPPED_SUBRESOURCE mapped;
		DXERRORFATAL(mImmediateContext->Map(dxTexture, mipIndex, ToDXType(flag), 0, &mapped));
		outRowPitch = mapped.RowPitch;
		return mapped.pData;
	}

	void GFXDeviceDX::Unmap(GFXBuffer* buffer)
	{
		UASSERT(buffer);
		mImmediateContext->Unmap(buffer->HandleAs<ID3D11Buffer*>(), 0);
	}
	//////////////////////////////////////////////////////////////////////////
	void GFXDeviceDX::Unmap(GFXTexture2D* texture, unsigned mipIndex)
	{
		UASSERT(texture);
		ID3D11Texture2D* dxTexture = texture->HandleAs<ID3D11Texture2D*>();
		mImmediateContext->Unmap(dxTexture, mipIndex);
	}
	//////////////////////////////////////////////////////////////////////////
	void GFXDeviceDX::CopyResource(GFXResource* dst, GFXResource* src)
	{
		UASSERT(dst && src && dst != src);
		UASSERT(typeid(dst) == typeid(src));

		ID3D11Resource* dstDX = dst->HandleAs<ID3D11Resource*>();
		ID3D11Resource* srcDX = src->HandleAs<ID3D11Resource*>();
		
		UASSERT(dstDX != srcDX);

		mImmediateContext->CopyResource(dstDX, srcDX);
	}
	//////////////////////////////////////////////////////////////////////////
	void GFXDeviceDX::CopySubresourceRegion(GFXTexture2D* dst, unsigned dstMipIndex, Vec2I dstXY, GFXTexture2D* src, unsigned srcMipIndex, Vec2I srcLeftTop, Vec2I srcRightBottom)
	{
		UASSERT(dst && src && dst != src);

		ID3D11Texture2D* dstDX = dst->HandleAs<ID3D11Texture2D*>();
		ID3D11Texture2D* srcDX = src->HandleAs<ID3D11Texture2D*>();

		UASSERT(dstDX && srcDX && dstDX != srcDX);

		D3D11_BOX dxBox = { srcLeftTop.mX, srcLeftTop.mY, 0, srcRightBottom.mX, srcRightBottom.mY, 1 };

		mImmediateContext->CopySubresourceRegion(dstDX, dstMipIndex, dstXY.mX, dstXY.mY, 0, srcDX, srcMipIndex, &dxBox);
	}
	//////////////////////////////////////////////////////////////////////////
	bool GFXSwapChainDX::Resize(const Vec2I& newSize)
	{
		ULOG_MESSAGE("Resizing... NewSize %", newSize);

		IDXGISwapChain* swapChain = (IDXGISwapChain*)mNativeHandle;
		UASSERT(swapChain);

		ID3D11Device* dxDevice = nullptr;
		ID3D11DeviceContext* immediateContext = nullptr;
		mBackBufferView->HandleAs<ID3D11RenderTargetView*>()->GetDevice(&dxDevice);
		UASSERT(dxDevice);
		dxDevice->GetImmediateContext(&immediateContext);
		UASSERT(immediateContext);
		immediateContext->OMSetRenderTargets(0, nullptr, nullptr);

		//You can use the IDXGISwapChain::ResizeBuffers method to handle window resizing.Before you call ResizeBuffers, 
		//you must release all outstanding references to the swap chain's buffers. The object that typically holds
		//a reference to a swap chain's buffer is a render - target - view.

		SafeDelete(mBackBufferView);

		// Preserve the existing buffer count and format.
		// Automatically choose the width and height to match the client rect for HWNDs.
		DXERRORFATAL(swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0));


		ID3D11Texture2D* backBuffer = nullptr;
		ID3D11RenderTargetView* rtv = nullptr;

		// Get buffer and create a render-target-view.
		DXERRORFATAL(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer));

		DXERRORFATAL(dxDevice->CreateRenderTargetView(backBuffer, nullptr, &rtv));
		
		D3D11_VIEWPORT vp;
		vp.Width = newSize.mX;
		vp.Height = newSize.mY;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		immediateContext->RSSetViewports(1, &vp);

		backBuffer->Release();
		immediateContext->Release();
		dxDevice->Release();

		mBackBufferSize = newSize;
		mBackBufferView = new GFXRenderTargetViewDX(rtv);

		return true;
	}

	bool GFXSwapChainDX::Present()
	{
		if (mDesc.mVSyncEnable)
		{
			// Lock to screen refresh rate.
			DXERRORFATAL(HandleAs<IDXGISwapChain*>()->Present(1, 0));
		}
		else
		{
			// Present as fast as possible.
			DXERRORFATAL(HandleAs<IDXGISwapChain*>()->Present(0, 0));
		}

		return true;
	}

	GFXSwapChainDX::~GFXSwapChainDX()
	{
		IDXGISwapChain* swapchain = (IDXGISwapChain*)mNativeHandle;
		if (swapchain)
		{
			swapchain->SetFullscreenState(false, nullptr);
		}

		SafeDelete(mBackBufferView);

		if (swapchain)
		{
			swapchain->Release();
			mNativeHandle = nullptr;
		}
	}

};