#pragma once

#include "UGFXDevice.h"
#include "UShaderMgr.h"

#include <DirectXTex.h>
#include <D3DX11.h>
#include <D3DX11tex.h>
#include <d3dcommon.h>
#include <D3DX10math.h>
// #include <DirectXMath.h>
// #include <DirectXCollision.h>
#include <SimpleMath.h>
#include <D3Dcompiler.h>
// #include <SpriteFont.h>
// #include <SpriteBatch.h>

using namespace DirectX::SimpleMath;
using namespace DirectX;

namespace UPO
{

};

#ifndef D3D_COMPILE_STANDARD_FILE_INCLUDE
#define D3D_COMPILE_STANDARD_FILE_INCLUDE ((ID3DInclude*)(UINT_PTR)1)
#endif

namespace UPO
{
	/*
	D3D11_USAGE

	Usage		Default		Dynamic		Immutable	Staging
	GPU-Read	yes			yes				yes			yes
	GPU-Write	yes										yes
	CPU-Read											yes
	CPU-Write				yes							yes
	*/


	//////////////////////////////////////////////////////////////////////////
	class GFXDeviceDX;

	inline const char* ShaderTypeToDXProfile(EShaderType type)
	{
		switch (type)
		{
		case EShaderType::EVertex:
			return "vs_5_0";
		case EShaderType::EPixel:
			return "ps_5_0";
		}

		ULOG_FATAL("invalid type");
		return nullptr;
	}

	//////////////////////////////////////////////////////////////////////////
	inline DXGI_FORMAT ToDXType(EIndexBufferType in)
	{
		switch (in)
		{
		case EIndexBufferType::EUShort: return DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
		case EIndexBufferType::EUInt: return DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		}

		UASSERT(false);
		return DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
	}

	//////////////////////////////////////////////////////////////////////////
	inline D3D11_PRIMITIVE_TOPOLOGY ToDXType(EPrimitiveTopology in)
	{
		return (D3D11_PRIMITIVE_TOPOLOGY)in;
	}
	//////////////////////////////////////////////////////////////////////////
	inline DXGI_FORMAT ToDXType(EPixelFormat in)
	{
		return (DXGI_FORMAT)in;
	}
	//////////////////////////////////////////////////////////////////////////
	inline D3D11_MAP ToDXType(EMapFlag in)
	{
		return (D3D11_MAP)in;
	}
	//////////////////////////////////////////////////////////////////////////
	inline D3D11_BLEND ToDXType(EBlendFactor in)
	{
		return (D3D11_BLEND)in;
	}
	//////////////////////////////////////////////////////////////////////////
	inline D3D11_BLEND_OP ToDXType(EBlendOP in)
	{
		return (D3D11_BLEND_OP)in;
	}
	//////////////////////////////////////////////////////////////////////////
	inline UINT8 ToDXType(EBlendColorWrite in)
	{
		return (UINT8)in;
	}
	//////////////////////////////////////////////////////////////////////////
	inline D3D11_TEXTURE_ADDRESS_MODE ToDXType(ETextureAddress in)
	{
		return (D3D11_TEXTURE_ADDRESS_MODE)in;
	}
	//////////////////////////////////////////////////////////////////////////
	inline D3D11_FILTER ToDXType(ETextureFilter in)
	{
		return (D3D11_FILTER)in;
	}
	//////////////////////////////////////////////////////////////////////////
	inline D3D11_COMPARISON_FUNC ToDXType(EComparisonFunc in)
	{
		return (D3D11_COMPARISON_FUNC)in;
	}
	//////////////////////////////////////////////////////////////////////////
	inline D3D11_CULL_MODE ToDXType(ECullMode in)
	{
		return (D3D11_CULL_MODE)in;
	}
	//////////////////////////////////////////////////////////////////////////
	inline D3D11_STENCIL_OP ToDXType(EStencilOP in)
	{
		return (D3D11_STENCIL_OP)in;
	}
	//////////////////////////////////////////////////////////////////////////
	inline DXGI_FORMAT ToDXType(EVertexFormat in)
	{
		switch (in)
		{
		case EVertexFormat::EFloat1: return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		case EVertexFormat::EFloat2: return DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
		case EVertexFormat::EFloat3: return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
		case EVertexFormat::EFloat4: return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
		case EVertexFormat::EColor32: return DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		}
		return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	}
	inline EPixelFormat ToGFXType(DXGI_FORMAT in)
	{
		//TODO
		return (EPixelFormat)in;
	}
	//////////////////////////////////////////////////////////////////////////
	class GFXResourceDX
	{
		friend GFXDeviceDX;
	protected:
		ID3D11Device*			mDevice = nullptr;
		ID3D11DeviceContext*	mDeviceContext = nullptr;

	public:
// 		GFXResourceDX(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
// 			: mDevice(device), mDeviceContext(deviceContext) {}

		auto GetDevice() const { return mDevice; }
		auto GetDeviceContext() const { return mDeviceContext; }
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXBufferDX : public GFXResourceDX
	{
		friend GFXDeviceDX;

		ID3D11Buffer*			mHandle;

	public:
		ID3D11Buffer* GetHandle() const { return mHandle; }
		~GFXBufferDX()
		{
			if (mHandle) mHandle->Release();
			mHandle = nullptr;
		}
		void* Map(EMapFlag flag)
		{
			D3D11_MAPPED_SUBRESOURCE mappedData;
			if (FAILED(mDeviceContext->Map(mHandle, 0, ToDXType(flag), 0, &mappedData)))
			{
				ULOG_ERROR("Failed to map buffer");
				return nullptr;
			}
			return mappedData.pData;
		}
		void Unmap()
		{
			mDeviceContext->Unmap(mHandle, 0);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXVertexBufferDX : public GFXVertexBuffer
	{
	public:
		GFXVertexBufferDX(ID3D11Buffer* vbuffer, const GFXVertexBufferDesc& desc, Name debugName = nullptr) : GFXVertexBuffer(vbuffer)
		{
			mDesc = desc;
		}
		~GFXVertexBufferDX()
		{
			if (mNativeHandle)
				HandleAs<ID3D11Buffer*>()->Release();
			mNativeHandle = nullptr;
		}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXIndexBufferDX : public GFXIndexBuffer
	{
	public:
		GFXIndexBufferDX(ID3D11Buffer* vbuffer, const GFXIndexBufferDesc& desc, Name debugName = nullptr) : GFXIndexBuffer(vbuffer)
		{
			mDesc = desc;
		}
		~GFXIndexBufferDX()
		{
			if (mNativeHandle)
				HandleAs<ID3D11Buffer*>()->Release();
			mNativeHandle = nullptr;
		}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXConstantBufferDX : public GFXConstantBuffer
	{
	public:
		GFXConstantBufferDX(ID3D11Buffer* vbuffer, const GFXConstantBufferDesc& desc, Name debugName = nullptr) : GFXConstantBuffer(vbuffer) 
		{
			mDesc = desc;
		}
		~GFXConstantBufferDX()
		{
			if (mNativeHandle)
				HandleAs<ID3D11Buffer*>()->Release();
			mNativeHandle = nullptr;
		}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXDepthStencilStateDX : public GFXDepthStencilState
	{
		friend GFXDeviceDX;
	public:
		GFXDepthStencilStateDX(const GFXDepthStencilStateDesc& desc, ID3D11DepthStencilState* handle, Name name = nullptr) : GFXDepthStencilState(handle, name)
		{
			mDesc = desc;
		}
		~GFXDepthStencilStateDX()
		{
			if (mNativeHandle)
				HandleAs<ID3D11DepthStencilState*>()->Release();
			mNativeHandle = nullptr;
		}
	};
	
	//////////////////////////////////////////////////////////////////////////
	class GFXSamplerStateDX : public GFXSamplerState
	{
		friend class GFXDeviceDX;
	public:
		GFXSamplerStateDX(const GFXSamplerStateDesc& desc, ID3D11SamplerState* sampler, Name debugName = nullptr) : GFXSamplerState(sampler, debugName) 
		{
			mDesc = desc;
		}
		~GFXSamplerStateDX()
		{
			if (mNativeHandle) 
				HandleAs<ID3D11SamplerState*>()->Release();
			mNativeHandle = nullptr;
		}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXRasterizerStateDX : public GFXRasterizerState
	{
		friend GFXDeviceDX;
	public:
		GFXRasterizerStateDX(const GFXRasterizerStateDesc& desc, ID3D11RasterizerState* state, Name debugName = nullptr) : GFXRasterizerState(state, debugName)
		{
			mDesc = desc;
		}
		~GFXRasterizerStateDX()
		{
			if (mNativeHandle) 
				HandleAs<ID3D11RasterizerState*>()->Release();
			mNativeHandle = nullptr;
		}
	};
	
	static const unsigned MAX_TEXTURE2D_SIZE = D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;


	
	//////////////////////////////////////////////////////////////////////////
	class GFXTexture2DDX : public GFXTexture2D
	{
	public:


		GFXTexture2DDX(ID3D11Texture2D* handle, GFXShaderResourceView* srv, GFXRenderTargetView* rtv, GFXDepthStencilView* dsv,  const GFXTexture2DDesc& param = GFXTexture2DDesc(), Name debugName = nullptr)
			: GFXTexture2D(handle, debugName)
		{
			mShaderResourceView = srv;
			mRenderTargetView = rtv;
			mDepthStencilView = dsv;
			mDesc = param;
		}

		~GFXTexture2DDX()
		{
			SafeDelete(mShaderResourceView);
			SafeDelete(mRenderTargetView);
			SafeDelete(mDepthStencilView);
			if (mNativeHandle)
				HandleAs<ID3D11Texture2D*>()->Release();
		}
	};
	//////////////////////////////////////////////////////////////////////////
	struct GFXShaderDXBase
	{
		Buffer mByteCodes;

		GFXShaderDXBase(const Buffer& bytecodes) : mByteCodes(bytecodes) {}

		void* GetByteCode() const { return mByteCodes.Data(); }
		size_t GetByteCodeSize() const { return mByteCodes.Size(); }


		void Release(void*& handle, EShaderType type)
		{
			if(handle)
			{
				switch (type)
				{
				case UPO::EShaderType::EVertex: ((ID3D11VertexShader*)(handle))->Release();
					break;
				case UPO::EShaderType::EHull: ((ID3D11HullShader*)(handle))->Release();
					break;
				case UPO::EShaderType::EDomain: ((ID3D11DomainShader*)(handle))->Release();
					break;
				case UPO::EShaderType::EGeometry: ((ID3D11GeometryShader*)(handle))->Release();
					break;
				case UPO::EShaderType::EPixel: ((ID3D11PixelShader*)(handle))->Release();
					break;
				case UPO::EShaderType::ECompute: ((ID3D11ComputeShader*)(handle))->Release();
					break;
				}
			}
			handle = nullptr;
		}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXVertexShaderDX : public GFXVertexShader
	{
	public:
		GFXShaderDXBase mShader;

		GFXVertexShaderDX(const Buffer& byteCodes, void* handle , Name debugName) : GFXVertexShader(handle, debugName), mShader(byteCodes) {}

		~GFXVertexShaderDX()
		{
			mShader.Release(mNativeHandle, mType);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXHullShaderDX : public GFXHullShader
	{
	public:
		GFXShaderDXBase mShader;

		GFXHullShaderDX(const Buffer& byteCodes, void* handle, Name debugName) : GFXHullShader(handle, debugName), mShader(byteCodes) {}

		~GFXHullShaderDX()
		{
			mShader.Release(mNativeHandle, mType);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXDomainShaderDX : public GFXDomainShader
	{
	public:
		GFXShaderDXBase mShader;

		GFXDomainShaderDX(const Buffer& byteCodes, void* handle, Name debugName) : GFXDomainShader(handle, debugName), mShader(byteCodes) {}

		~GFXDomainShaderDX()
		{
			mShader.Release(mNativeHandle, mType);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXGeometryShaderDX : public GFXGeometryShader
	{
	public:
		GFXShaderDXBase mShader;

		GFXGeometryShaderDX(const Buffer& byteCodes, void* handle, Name debugName) : GFXGeometryShader(handle, debugName), mShader(byteCodes) {}

		~GFXGeometryShaderDX()
		{
			mShader.Release(mNativeHandle, mType);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXPixelShaderDX : public GFXPixelShader
	{
	public:
		GFXShaderDXBase mShader;

		GFXPixelShaderDX(const Buffer& byteCodes, void* handle, Name debugName) : GFXPixelShader(handle, debugName), mShader(byteCodes) {}

		~GFXPixelShaderDX()
		{
			mShader.Release(mNativeHandle, mType);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXComputeShaderDX : public GFXComputeShader
	{
	public:
		GFXShaderDXBase mShader;

		GFXComputeShaderDX(const Buffer& byteCodes, void* handle, Name debugName) : GFXComputeShader(handle, debugName), mShader(byteCodes) {}

		~GFXComputeShaderDX()
		{
			mShader.Release(mNativeHandle, mType);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXInputLayoutDX : public GFXInputLayout
	{
		friend GFXDeviceDX;
	public:
		GFXInputLayoutDX(const GFXInputLayoutDesc& desc, ID3D11InputLayout* handle, Name debugName = nullptr) : GFXInputLayout(handle, debugName)
		{
			mDesc = desc;
		}
		~GFXInputLayoutDX()
		{
			if (mNativeHandle)
				HandleAs<ID3D11InputLayout*>()->Release();
			mNativeHandle = nullptr;
		}
	};
	

	//////////////////////////////////////////////////////////////////////////
	class GFXBlendStateDX : public GFXBlendState
	{
		friend GFXDeviceDX;

		GFXBlendStateDX(const GFXBlendStateDesc& desc, ID3D11BlendState* handle, Name debugName = nullptr) : GFXBlendState(handle, debugName)
		{
			mDesc = desc;
		}
		~GFXBlendStateDX()
		{
			if (mNativeHandle)
				HandleAs<ID3D11BlendState*>()->Release();
			mNativeHandle = nullptr;
		}
	};
	struct GFXDevice_Desc
	{

	};

	class UAPI GFXSwapChainDX : public GFXSwapChain
	{
		friend GFXDeviceDX;

		GFXRenderTargetView* mBackBufferView = nullptr;
		

	public:
		GFXSwapChainDX(void* handle, Name debugName = nullptr) : GFXSwapChain(handle, debugName) {}
		~GFXSwapChainDX();

		bool Resize(const Vec2I& newSize) override;
		bool Present() override;

		GFXRenderTargetView* GetBackBufferView() override { return mBackBufferView; }
		GameWindow* GetWindow() override { return mDesc.mGameWindow; }
	};

	class GFXShaderResourceViewDX : public GFXShaderResourceView
	{
	public:
		GFXShaderResourceViewDX(ID3D11ShaderResourceView* srv) : GFXShaderResourceView(srv)
		{}
		~GFXShaderResourceViewDX()
		{
			if (mNativeHandle)
				HandleAs<ID3D11ShaderResourceView*>()->Release();
			mNativeHandle = nullptr;
		}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXRenderTargetViewDX : public GFXRenderTargetView
	{
	public:
		GFXRenderTargetViewDX(ID3D11RenderTargetView* handle) : GFXRenderTargetView(handle)
		{ }
		~GFXRenderTargetViewDX()
		{
			if (mNativeHandle)
				HandleAs<ID3D11RenderTargetView*>()->Release();
			mNativeHandle = nullptr;
		}
		
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXDepthStencilViewDX : public GFXDepthStencilView
	{
	public:
		GFXDepthStencilViewDX(ID3D11DepthStencilView* handle) : GFXDepthStencilView(handle)
		{ }
		~GFXDepthStencilViewDX()
		{
			if (mNativeHandle)
				HandleAs<ID3D11DepthStencilView*>()->Release();
			mNativeHandle = nullptr;
		}

	};

	//////////////////////////////////////////////////////////////////////////
	inline DXGI_FORMAT GetFormatForShaderResourceView(DXGI_FORMAT format)
	{
		return format;
	}
	inline DXGI_FORMAT GetFormatForRenderTargetView(DXGI_FORMAT format)
	{
		return format;
	}
	inline DXGI_FORMAT GetFormatForDepthStencilView(DXGI_FORMAT format)
	{
		if (DirectX::IsDepthStencil(format))
			return format;
		switch (DirectX::MakeTypeless(format))
		{
		case DXGI_FORMAT_R32_TYPELESS:
			return DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
		case DXGI_FORMAT_R24G8_TYPELESS:
			return DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		case DXGI_FORMAT_R32G8X24_TYPELESS:
			return DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		case DXGI_FORMAT_R16_TYPELESS:
			return DXGI_FORMAT::DXGI_FORMAT_D16_UNORM;
		}
		return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	}

	//////////////////////////////////////////////////////////////////////////
	class GFXDeviceDX : public GFXDevice
	{
	public:
		ID3D11Device* mDevice = nullptr;
		ID3D11DeviceContext* mImmediateContext = nullptr;
		D3D_FEATURE_LEVEL	mFeatureLevel;
		IDXGIFactory*	mGIFactory = nullptr;
		IDXGIAdapter*	mAdapter = nullptr;

		static GFXDeviceDX* Create();
		


		ID3D11Device* GetDXDevice() const { return mDevice; }
		ID3D11DeviceContext* GetDXDeviceContext() const { return mImmediateContext; }

		//////////////////////////////////////////////////////////////////////////
		GFXSwapChain* CreateSwapChain(const GFXSwapChainDesc& param) override;

		//////////////////////////////////////////////////////////////////////////
		void SetViewport(const GFXViewport& viewport) override;
		//////////////////////////////////////////////////////////////////////////
		virtual void ClearRenderTarget(GFXRenderTargetView* view, const Color& color) override;
		virtual void ClearDepthStencil(GFXDepthStencilView* view, bool clearDepth, bool clearStencil, float depth, char stencil) override;


		//////////////////////////////////////////////////////////////////////////
		GFXIndexBuffer* CreateIndexBuffer(const GFXIndexBufferDesc& param) override;
		//////////////////////////////////////////////////////////////////////////
		GFXVertexBuffer* CreateVertexBuffer(const GFXVertexBufferDesc& param) override;
		//////////////////////////////////////////////////////////////////////////
		GFXConstantBuffer* CreateConstantBuffer(const GFXConstantBufferDesc& param) override;
		GFXConstantBuffer* CreateConstantBuffer(unsigned size) override;

		//////////////////////////////////////////////////////////////////////////
		GFXDepthStencilState* CreateDepthStencilState(const GFXDepthStencilStateDesc& param) override;
		//////////////////////////////////////////////////////////////////////////
		GFXSamplerState* CreateSamplerState(const GFXSamplerStateDesc& param) override;
		//////////////////////////////////////////////////////////////////////////
		GFXRasterizerState* CreateRasterizerState(const GFXRasterizerStateDesc& param) override;
		//////////////////////////////////////////////////////////////////////////
		void SetVertexBuffer(GFXVertexBuffer* buffer, unsigned slot, unsigned stride, unsigned offset = 0) override;

		//////////////////////////////////////////////////////////////////////////
		void SetIndexBuffer(GFXIndexBuffer* buffer, unsigned offset = 0) override;
		//////////////////////////////////////////////////////////////////////////
		void SetConstentBuffer(GFXConstantBuffer** buffers, unsigned startSlot, unsigned numBuffers, EShaderType whichShader) override;
		void SetResourceView(GFXShaderResourceView** views, unsigned startSlot, unsigned numViews, EShaderType whichShder) override;
		void SetResourceView(GFXTexture2D** textures, unsigned startSlot, unsigned numTextures, EShaderType whichShader) override;

		//////////////////////////////////////////////////////////////////////////
		void SetPrimitiveTopology(EPrimitiveTopology topology) override;
		//////////////////////////////////////////////////////////////////////////
		void Draw(unsigned vertexCount, unsigned startVertexLocation, unsigned instanceCount, unsigned startInstanceLocation) override;
		//////////////////////////////////////////////////////////////////////////
		void DrawIndexed(unsigned indexCount, unsigned startIndexLocation, unsigned baseVertexLocation, unsigned instanceCount, unsigned startInstanceLocation) override;
		//////////////////////////////////////////////////////////////////////////
		GFXRenderTargetView* CreateRenderTargetView(GFXTexture2D* texture) override;
		GFXRenderTargetView* CreateRenderTargetView(GFXTexture2D* texture, unsigned mipSlice, EPixelFormat format) override;

		GFXDepthStencilView* CreateDepthStencilView(GFXTexture2D* texture) override;
		GFXDepthStencilView* CreateDepthStencilView(GFXTexture2D* texture, unsigned mipSlice, EPixelFormat format) override;

		GFXShaderResourceView* CreateShaderResourceView(GFXTexture2D* texture2D) override;
		GFXShaderResourceView* CreateShaderResourceView(GFXTexture2D* texture2D, unsigned mostDetailedMip, unsigned numMipLevels, EPixelFormat format) override;

		//////////////////////////////////////////////////////////////////////////
		GFXTexture2D* CreateTexture2DFromMemory(const Buffer& memory) override;



		//////////////////////////////////////////////////////////////////////////
		GFXTexture2D* CreateTexture2D(const GFXTexture2DDesc& param);
		//////////////////////////////////////////////////////////////////////////
		void SetDepthStencilState(GFXDepthStencilStateHandle state) override;
		//////////////////////////////////////////////////////////////////////////
		void SetRenderTarget(GFXRenderTargetView** renderTargets, unsigned numRenderTargets, GFXDepthStencilView* depthStencil) override;
		//////////////////////////////////////////////////////////////////////////
		void SetRasterizerState(GFXRasterizerStateHandle state) override;
		//////////////////////////////////////////////////////////////////////////
		GFXShader* CreateShaderFromBytecode(const Buffer& bytecodes, EShaderType type);
		//////////////////////////////////////////////////////////////////////////
		GFXShader* CreateShader(const ShaderUniqueParam& param) override;
		GFXShader* CreateShader(const Buffer& bytesCode, EShaderType type, Name debugName) override;

		//////////////////////////////////////////////////////////////////////////
		void SetShaders(GFXVertexShader* vertexShader, GFXPixelShader* pixelShader) override;
		//////////////////////////////////////////////////////////////////////////
		void SetShader(const GFXShaderBound& shaders) override;
		//////////////////////////////////////////////////////////////////////////
		
		//////////////////////////////////////////////////////////////////////////
		void SetSamplerState(GFXSamplerStateHandle* samplers, unsigned startSlot, unsigned numSamplers, EShaderType whichShader) override;
		//////////////////////////////////////////////////////////////////////////
		GFXInputLayout* CreateInputLayout(const GFXInputLayoutDesc& param) override;
		void SetInputLayout(GFXInputLayoutHandle layout) override;

		//////////////////////////////////////////////////////////////////////////
		GFXBlendState* CreateBlendState(const GFXBlendStateDesc& param) override;
		//////////////////////////////////////////////////////////////////////////
		void SetBlendState(GFXBlendStateHandle blendState, const Color& blendFactor, unsigned sampleMask = 0xFFffFFff) override;
		void ClearState() override;
		bool IsImmediate() override;


		virtual void* Map(GFXBuffer*, EMapFlag) override;
		virtual void Unmap(GFXBuffer*) override;

		void CopyResource(GFXResource* dst, GFXResource* src) override;

		void CopySubresourceRegion(GFXTexture2D* dst, unsigned dstMipIndex, Vec2I dstXY, GFXTexture2D* src, unsigned srcMipIndex,
			Vec2I srcLeftTop, Vec2I srcRightBottom) override;


		void* Map(GFXTexture2D* texture, EMapFlag flag, unsigned mipIndex, unsigned& outRowPitch) override;
		void Unmap(GFXTexture2D* texture, unsigned mipIndex) override;

		//////////////////////////////////////////////////////////////////////////
// 		GFXTexture2D* LoadTextureFromFile(const char* filename)
// 		{
// 			if (filename == nullptr) return nullptr;
// 
// 			
// 			char fullFileName[256];
// 			sprintf_s(fullFileName, "..//Content//%s", filename);
// 			wchar_t fullFileNameW[256];
// 			size_t ncc = 0;
// 			mbstowcs_s(&ncc, fullFileNameW, 256, fullFileName, 256);
// 			const char* ext = StrFindRNChar(filename, '.', 0);
// 			if (ext == nullptr) return nullptr;
// 			unsigned flag = 0;
// 			DirectX::ScratchImage image;
// 			HRESULT hr = S_OK;
// 			if (_strcmpi(ext, ".dds") == 0)
// 				hr = DirectX::LoadFromDDSFile(fullFileNameW, flag, nullptr, image);
// 			else if(_strcmpi(ext, ".tga") == 0)
// 				hr = DirectX::LoadFromTGAFile(fullFileNameW, nullptr, image);
// 			else
// 				hr = DirectX::LoadFromWICFile(fullFileNameW, flag, nullptr, image);
// 			if (SUCCEEDED(hr))
// 			{
// 				ID3D11ShaderResourceView* srv = nullptr;
// 				ID3D11Resource* handle = nullptr;
// 				if (FAILED(DirectX::CreateShaderResourceView(mDevice, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &srv)))
// 				{
// 					ULOG_ERROR("failed to create texture");
// 					return nullptr;
// 				}
// 				GFXTexture2DDX* ret = new GFXTexture2DDX;
// 				ret->mShaderResourceView = srv;
// 				srv->GetResource((ID3D11Resource**)&(ret->mTexture));
// 				ULOG_SUCCESS("Texture loaded");
// 				return ret;
// 
// 			}
// 			ULOG_ERROR("failed to load texture");
// 			return nullptr;
// 		}
	};
};