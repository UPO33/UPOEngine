#pragma once

#include "UGFXDevice.h"
#include "UShaderMgr.h"

#include <DirectXTex.h>
#include <D3DX11.h>
#include <D3DX11tex.h>
#include <d3dcommon.h>
#include <D3DX10math.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <SimpleMath.h>
#include <D3Dcompiler.h>

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
	inline D3D11_USAGE ToDXType(EResourceUsage in)
	{
		switch (in)
		{
		case EResourceUsage::EDefault: return D3D11_USAGE_DEFAULT;
		case EResourceUsage::EImmutable: return D3D11_USAGE_IMMUTABLE;
		case EResourceUsage::EDynamic: return D3D11_USAGE_DYNAMIC;
		default: return D3D11_USAGE_DEFAULT;
		}
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
	inline DXGI_FORMAT ToDXType(EVertexFormat in)
	{
		switch (in)
		{
		case EVertexFormat::EFloat1: return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		case EVertexFormat::EFloat2: return DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
		case EVertexFormat::EFloat3: return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
		case EVertexFormat::EFloat4: return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		}
		return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
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
				ULOG_ERROR("Failed to map vertex buffer");
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
	class GFXVertexBufferDX : public GFXBufferDX, public GFXVertexBuffer
	{
		friend GFXDeviceDX;

		~GFXVertexBufferDX()
		{
		}
		virtual void* Map(EMapFlag flag) override
		{
			return GFXBufferDX::Map(flag);
		}
		virtual void Unmap() override
		{
			GFXBufferDX::Unmap();
		}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXIndexBufferDX : public GFXBufferDX, public GFXIndexBuffer
	{
		friend GFXDeviceDX;

		~GFXIndexBufferDX()
		{
		}
		virtual void* Map(EMapFlag flag) override
		{
			return GFXBufferDX::Map(flag);
		}
		virtual void Unmap() override
		{
			GFXBufferDX::Unmap();
		}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXConstantBufferDX : public GFXBufferDX, public GFXConstantBuffer
	{
		friend GFXDeviceDX;

		~GFXConstantBufferDX()
		{
		}
		virtual void* Map(EMapFlag flag) override
		{
			return GFXBufferDX::Map(flag);
		}
		virtual void Unmap() override
		{
			GFXBufferDX::Unmap();
		}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXDepthStencilStateDX : public GFXDepthStencilState
	{
		friend GFXDeviceDX;
	public:
		ID3D11DepthStencilState* mHandle;

		~GFXDepthStencilStateDX()
		{
			if (mHandle) mHandle->Release();
			mHandle = nullptr;
		}
	};
	
	//////////////////////////////////////////////////////////////////////////
	class GFXSamplerStateDX : public GFXSamplerState
	{
		friend class GFXDeviceDX;
	public:
		ID3D11SamplerState* mHandle;

		~GFXSamplerStateDX()
		{
			if (mHandle) mHandle->Release();
			mHandle = nullptr;
		}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXRasterizerStateDX : public GFXRasterizerState
	{
		friend GFXDeviceDX;
	public:
		ID3D11RasterizerState* mHandle;

		~GFXRasterizerStateDX()
		{
			if (mHandle) mHandle->Release();
			mHandle = nullptr;
		}
	};
	
	static const unsigned MAX_TEXTURE2D_SIZE = D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;


	
	//////////////////////////////////////////////////////////////////////////
	class GFXTexture2DDX : public GFXTexture2D
	{
		friend GFXDeviceDX;
	public:
		ID3D11Texture2D*			mTexture = nullptr;
		ID3D11ShaderResourceView*	mResourceView = nullptr;
		ID3D11RenderTargetView*		mRenderTargetView = nullptr;
		ID3D11DepthStencilView*		mDepthStencilView = nullptr;

		~GFXTexture2DDX()
		{
			if (mResourceView) mResourceView->Release();
			mResourceView = nullptr;
			if (mRenderTargetView) mRenderTargetView->Release();
			mRenderTargetView = nullptr;
			if (mDepthStencilView) mDepthStencilView->Release();
			mDepthStencilView = nullptr;
			if (mTexture) mTexture->Release();
			mTexture = nullptr;
		}
	};
	//////////////////////////////////////////////////////////////////////////
	struct GFXShaderDXBase
	{
		void* mHandle = nullptr;
		Buffer mByteCodes;

		void* GetByteCode() const { return mByteCodes.Data(); }
		size_t GetByteCodeSize() const { return mByteCodes.Size(); }

		void Release(EShaderType type)
		{
			if(mHandle)
			{
				switch (type)
				{
				case UPO::EShaderType::EVertex: reinterpret_cast<ID3D11VertexShader*>(mHandle)->Release();
					break;
				case UPO::EShaderType::EHull: reinterpret_cast<ID3D11HullShader*>(mHandle)->Release();
					break;
				case UPO::EShaderType::EDomain: reinterpret_cast<ID3D11DomainShader*>(mHandle)->Release();
					break;
				case UPO::EShaderType::EGeometry: reinterpret_cast<ID3D11GeometryShader*>(mHandle)->Release();
					break;
				case UPO::EShaderType::EPixel: reinterpret_cast<ID3D11PixelShader*>(mHandle)->Release();
					break;
				case UPO::EShaderType::ECompute: reinterpret_cast<ID3D11ComputeShader*>(mHandle)->Release();
					break;
				}
			}
			mHandle = nullptr;
		}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXVertexShaderDX : public GFXResourceDX, public GFXVertexShader
	{
	public:
		GFXShaderDXBase mShader;

		GFXVertexShaderDX(const GFXShaderDXBase& shader) : mShader(shader) {}

		~GFXVertexShaderDX()
		{
			mShader.Release(EShaderType::EVertex);
		}
		ID3D11VertexShader* Handle() { return reinterpret_cast<ID3D11VertexShader*>(mShader.mHandle); }
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXHullShaderDX : public GFXResourceDX, public GFXHullShader
	{
	public:
		GFXShaderDXBase mShader;

		GFXHullShaderDX(const GFXShaderDXBase& shader) : mShader(shader) {}

		~GFXHullShaderDX()
		{
			mShader.Release(EShaderType::EHull);
		}
		ID3D11HullShader* Handle() { return reinterpret_cast<ID3D11HullShader*>(mShader.mHandle); }
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXDomainShaderDX : public GFXResourceDX, public GFXDomainShader
	{
	public:
		GFXShaderDXBase mShader;

		GFXDomainShaderDX(const GFXShaderDXBase& shader) : mShader(shader) {}

		~GFXDomainShaderDX()
		{
			mShader.Release(EShaderType::EDomain);
		}
		ID3D11DomainShader* Handle() { return reinterpret_cast<ID3D11DomainShader*>(mShader.mHandle); }
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXGeometryShaderDX : public GFXResourceDX, public GFXGeometryShader
	{
	public:
		GFXShaderDXBase mShader;

		GFXGeometryShaderDX(const GFXShaderDXBase& shader) : mShader(shader) {}
		~GFXGeometryShaderDX()
		{
			mShader.Release(EShaderType::EGeometry);
		}
		ID3D11GeometryShader* Handle() { return reinterpret_cast<ID3D11GeometryShader*>(mShader.mHandle); }
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXPixelShaderDX : public GFXResourceDX, public GFXPixelShader
	{
	public:
		GFXShaderDXBase mShader;

		GFXPixelShaderDX(const GFXShaderDXBase& shader) : mShader(shader) {}
		~GFXPixelShaderDX()
		{
			mShader.Release(EShaderType::EPixel);
		}
		ID3D11PixelShader* Handle() { return reinterpret_cast<ID3D11PixelShader*>(mShader.mHandle); }
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXComputeShaderDX : public GFXResourceDX, public GFXComputeShader
	{
	public:
		GFXShaderDXBase mShader;

		GFXComputeShaderDX(const GFXShaderDXBase& shader) : mShader(shader) {}
		~GFXComputeShaderDX()
		{
			mShader.Release(EShaderType::ECompute);
		}
		ID3D11ComputeShader* Handle() { return reinterpret_cast<ID3D11ComputeShader*>(mShader.mHandle); }
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXInputLayoutDX : public GFXInputLayout
	{
		friend GFXDeviceDX;
	public:
		ID3D11InputLayout* mHandle = nullptr;
		~GFXInputLayoutDX()
		{
			if (mHandle) mHandle->Release();
			mHandle = nullptr;
		}
	};
	

	//////////////////////////////////////////////////////////////////////////
	class GFXBlendStateDX : public GFXBlendState
	{
		friend GFXDeviceDX;

		ID3D11BlendState* mHandle;

		~GFXBlendStateDX()
		{
			if (mHandle) mHandle->Release();
			mHandle = nullptr;
		}
	};
	struct GFXDevice_Desc
	{

	};
	//////////////////////////////////////////////////////////////////////////
	class GFXRenderTargerView
	{

	};
	class GFXDepthStencilView
	{

	};
	class GFXTexture2DView
	{

	};
	class GFXSwapChainDX
	{

	};
	//////////////////////////////////////////////////////////////////////////
	class GFXDeviceDX : public GFXDevice
	{
	public:
		ID3D11Device* mDevice = nullptr;
		ID3D11DeviceContext* mImmediateContext = nullptr;
		D3D_FEATURE_LEVEL	mFeatureLevel;

		static GFXDeviceDX* Create();
		
		GFXDeviceDX(){}

		GFXDeviceDX(ID3D11Device* device, ID3D11DeviceContext* immediateContext, D3D_FEATURE_LEVEL featureLevel)
			: mDevice(device), mImmediateContext(immediateContext), mFeatureLevel(featureLevel)
		{}

		ID3D11Device* GetDXDevice() const { return mDevice; }
		ID3D11DeviceContext* GetDXDeviceContext() const { return mImmediateContext; }

		//////////////////////////////////////////////////////////////////////////
		GFXSwapChain* CreateSwapChain(GameWindow* wnd) override;

		//////////////////////////////////////////////////////////////////////////
		virtual void ClearRenderTarget(const GFXTexture2D* renderTarget, const Color& color) override
		{
			if (renderTarget == nullptr) return;
			ID3D11RenderTargetView* rtv = renderTarget->As<GFXTexture2DDX>()->mRenderTargetView;
			if (rtv == nullptr) return;
			mImmediateContext->ClearRenderTargetView(rtv, color.mRGBA);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void ClearDepthStencil(const GFXTexture2D* depthTexture, bool clearDepth, bool clearStencil, float depth, char stencil) override
		{
			if (depthTexture == nullptr) return;
			ID3D11DepthStencilView* dsv = depthTexture->As<GFXTexture2DDX>()->mDepthStencilView;
			if (dsv == nullptr) return;
			UINT clearFlag = 0;
			if (clearDepth) clearFlag |= D3D11_CLEAR_DEPTH;
			if (clearStencil) clearFlag |= D3D11_CLEAR_STENCIL;
			mImmediateContext->ClearDepthStencilView(dsv, clearFlag, depth, stencil);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual GFXIndexBuffer* CreateIndexBuffer(const GFXIndexBuffer_Desc& param) override
		{
			if (param.mInitialData == nullptr && param.mUsage == EResourceUsage::EImmutable)
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
			if (param.mCPUWriteAccess) desc.CPUAccessFlags |= D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
			if (param.mCPUReadAccess) desc.CPUAccessFlags |= D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;

			if (param.mUsage == EResourceUsage::EDynamic)
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

			desc.Usage = ToDXType(param.mUsage);

			D3D11_SUBRESOURCE_DATA data;
			data.pSysMem = param.mInitialData;
			data.SysMemPitch = 0;
			data.SysMemSlicePitch = 0;

			ID3D11Buffer* handle = nullptr;
			if (SUCCEEDED(mDevice->CreateBuffer(&desc, &data, &handle)))
			{
				GFXIndexBufferDX* ret = new GFXIndexBufferDX;
				ret->mDevice = mDevice;
				ret->mDeviceContext = mImmediateContext;
				ret->mHandle = handle;
				ret->mDesc = param;
				ret->mDesc.mInitialData = nullptr;
				ULOG_SUCCESS("index buffer created.");
				return ret;
			}
			ULOG_ERROR("Failed to create index buffer");
			return nullptr;
		}
		//////////////////////////////////////////////////////////////////////////
		virtual GFXVertexBuffer* CreateVertexBuffer(const GFXVertexBuffer_Desc& param) override
		{
			if (param.mInitialData == nullptr && param.mUsage == EResourceUsage::EImmutable)
			{
				ULOG_ERROR("immutable buffer needs initial data");
				return nullptr;
			}
			D3D11_BUFFER_DESC desc;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
			desc.ByteWidth = param.mSize;

			desc.CPUAccessFlags = 0;
			if (param.mCPUWriteAccess) desc.CPUAccessFlags |= D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
			if (param.mCPUReadAccess) desc.CPUAccessFlags |= D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;

			if (param.mUsage == EResourceUsage::EDynamic)
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

			desc.Usage = ToDXType(param.mUsage);

			D3D11_SUBRESOURCE_DATA data;
			data.pSysMem = param.mInitialData;
			data.SysMemPitch = 0;
			data.SysMemSlicePitch = 0;

			D3D11_SUBRESOURCE_DATA* pData = nullptr;
			if (param.mInitialData) pData = &data;

			ID3D11Buffer* handle = nullptr;
			if (SUCCEEDED(mDevice->CreateBuffer(&desc, pData, &handle)))
			{
				GFXVertexBufferDX* ret = new GFXVertexBufferDX;
				ret->mDevice = mDevice;
				ret->mDeviceContext = mImmediateContext;
				ret->mHandle = handle;
				ret->mDesc = param;
				ret->mDesc.mInitialData = nullptr;
				ULOG_SUCCESS("Vertex buffer created");
				return ret;
			}
			ULOG_ERROR("failed to create vertex buffer");
			return nullptr;
		}
		//////////////////////////////////////////////////////////////////////////
		virtual GFXConstantBuffer* CreateConstantBuffer(const GFXConstantBuffer_Desc& param) override
		{
			UASSERT(param.mSize % 16 == 0);

			D3D11_BUFFER_DESC desc;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
			desc.ByteWidth = param.mSize;
			desc.CPUAccessFlags =  D3D11_CPU_ACCESS_WRITE;
			desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

			D3D11_SUBRESOURCE_DATA data;
			data.pSysMem = param.mInitialData;
			data.SysMemPitch = 0;
			data.SysMemSlicePitch = 0;

			D3D11_SUBRESOURCE_DATA* pData = nullptr;
			if (param.mInitialData) pData = &data;

			ID3D11Buffer* handle = nullptr;
			if (SUCCEEDED(mDevice->CreateBuffer(&desc, pData, &handle)))
			{
				GFXConstantBufferDX* ret = new GFXConstantBufferDX;
				ret->mDevice = mDevice;
				ret->mDeviceContext = mImmediateContext;
				ret->mHandle = handle;
				ret->mDesc = param;
				ret->mDesc.mInitialData = nullptr;
				ULOG_SUCCESS("constant buffer created");
				return ret;
			}
			ULOG_ERROR("failed to create constant buffer");
			return nullptr;
		}
		//////////////////////////////////////////////////////////////////////////
		virtual GFXDepthStencilState* CreateDepthStencilState(const GFXDepthStencilState_Desc& param) override
		{
			D3D11_DEPTH_STENCIL_DESC desc;
			desc.DepthEnable = param.mDepthEnable;
			desc.DepthWriteMask = param.mDepthWriteAll ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
			desc.DepthFunc = (D3D11_COMPARISON_FUNC)param.mDepthFunc;

			desc.StencilEnable = param.mStencilEnable;
			desc.StencilReadMask = param.mStencilReadMask;
			desc.StencilWriteMask = param.mStencilWriteMask;

			desc.FrontFace.StencilFailOp = (D3D11_STENCIL_OP)param.mFrontFaceStencilFailOp;
			desc.FrontFace.StencilDepthFailOp = (D3D11_STENCIL_OP)param.mFrontFaceStencilDepthFailOp;
			desc.FrontFace.StencilPassOp = (D3D11_STENCIL_OP)param.mFrontFaceStencilPassOp;
			desc.FrontFace.StencilFunc = (D3D11_COMPARISON_FUNC)param.mFrontFaceStencilFunc;

			desc.BackFace.StencilFailOp = (D3D11_STENCIL_OP)param.mBackFaceStencilFailOp;
			desc.BackFace.StencilDepthFailOp = (D3D11_STENCIL_OP)param.mBackFaceStencilDepthFailOp;
			desc.BackFace.StencilPassOp = (D3D11_STENCIL_OP)param.mBackFaceStencilPassOp;
			desc.BackFace.StencilFunc = (D3D11_COMPARISON_FUNC)param.mBackFaceStencilFunc;

			ID3D11DepthStencilState* handle = nullptr;
			if (SUCCEEDED(mDevice->CreateDepthStencilState(&desc, &handle)))
			{
				GFXDepthStencilStateDX* ret = new GFXDepthStencilStateDX;
				ret->mHandle = handle;
				ret->mDesc = param;
				ULOG_SUCCESS("depth stencil state crated");
				return ret;
			}
			ULOG_ERROR("Failed to create depth stencil state");
			return nullptr;
		}
		//////////////////////////////////////////////////////////////////////////
		virtual GFXSamplerState* CreateSamplerState(const GFXSamplerState_Desc& param) override
		{
			D3D11_SAMPLER_DESC desc;
			desc.Filter = (D3D11_FILTER)param.mFilter;
			desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)param.mAddressU;
			desc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)param.mAddressV;
			desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)param.mAddressW;
			desc.MipLODBias = param.mMipLODBias;
			desc.MaxAnisotropy = param.mMaxAnisotropy;
			desc.ComparisonFunc = (D3D11_COMPARISON_FUNC)param.mComparisonFunc;
			param.mBorderColor.StoreTo(desc.BorderColor);
			desc.MinLOD = param.mMinLOD;
			desc.MaxLOD = param.mMaxLOD;
			
			ID3D11SamplerState* handle = nullptr;
			if (SUCCEEDED(mDevice->CreateSamplerState(&desc, &handle)))
			{
				GFXSamplerStateDX* ret = new GFXSamplerStateDX;
				ret->mHandle = handle;
				ret->mDesc = param;
				ULOG_SUCCESS("Sampler state crated");
				return ret;
			}
			ULOG_ERROR("Failed to create sampler state");
			return nullptr;
		}
		//////////////////////////////////////////////////////////////////////////
		virtual GFXRasterizerState* CreateRasterizerState(const GFXRasterizerState_Desc& param) override
		{
			D3D11_RASTERIZER_DESC desc;
			desc.FillMode = param.mWireframe ? D3D11_FILL_MODE::D3D11_FILL_WIREFRAME : D3D11_FILL_MODE::D3D11_FILL_SOLID;
			desc.CullMode = (D3D11_CULL_MODE)param.mCullMode;
			desc.DepthBias = param.mDepthBias;
			desc.DepthBiasClamp = param.mDepthBiasClamp;
			desc.DepthClipEnable = param.mDepthClipEnable;
			desc.AntialiasedLineEnable = param.mAntialiasedLineEnable;
			desc.FrontCounterClockwise = param.mFrontCounterClockWise;
			desc.MultisampleEnable = param.mMultisampleEnable;
			desc.ScissorEnable = param.mScissorEnable;
			desc.SlopeScaledDepthBias = param.mSlopeScaledDepthBias;
			
			ID3D11RasterizerState* handle = nullptr;
			if(SUCCEEDED(mDevice->CreateRasterizerState(&desc, &handle)))
			{
				GFXRasterizerStateDX* ret = new GFXRasterizerStateDX;
				ret->mHandle = handle;
				ret->mDesc = param;
				ULOG_SUCCESS("rasterizer created");
				return ret;
			}
			ULOG_ERROR("Failde to create rasterizer");
			return nullptr;
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void BinVertexBuffer(const GFXVertexBuffer* buffer, unsigned stride, unsigned offset) override
		{
			if (buffer == nullptr)
			{
				mImmediateContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
				return;
			}

			ID3D11Buffer* buffers[1] = { buffer->As<GFXVertexBufferDX>()->mHandle };
			UINT strides[1] = { stride };
			UINT offsets[1] = { offset };
			mImmediateContext->IASetVertexBuffers(0, 1, buffers, strides, offsets);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void BinIndexBuffer(const GFXIndexBuffer* buffer, unsigned offset) override
		{
			if (buffer == nullptr)
			{
				mImmediateContext->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);
				return;
			}

			GFXIndexBufferDX* ib = buffer->As<GFXIndexBufferDX>();
			mImmediateContext->IASetIndexBuffer(ib->mHandle, ToDXType(ib->GetDesc().mType), offset);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void BindConstantBuffer(const GFXConstantBuffer* buffer, unsigned slot, EShaderType whichShader) override
		{
			ID3D11Buffer* d3dBuffer = buffer ? buffer->As<GFXConstantBufferDX>()->GetHandle() : nullptr;
			if (whichShader == EShaderType::EVertex)
				mImmediateContext->VSSetConstantBuffers(slot, 1, &d3dBuffer);
			else if (whichShader == EShaderType::EHull)
				mImmediateContext->PSSetConstantBuffers(slot, 1, &d3dBuffer);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void SetPrimitiveTopology(EPrimitiveTopology topology) override
		{
			mImmediateContext->IASetPrimitiveTopology(ToDXType(topology));
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void Draw(unsigned vertexCount, unsigned startVertexLocation, unsigned instanceCount, unsigned startInstanceLocation) override
		{
			if(instanceCount > 1)
				mImmediateContext->DrawInstanced(vertexCount, instanceCount, startVertexLocation, startInstanceLocation);
			else
				mImmediateContext->Draw(vertexCount, startVertexLocation);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void DrawIndexed(unsigned indexCount, unsigned startIndexLocation, unsigned baseVertexLocation, unsigned instanceCount, unsigned startInstanceLocation) override
		{
			if(instanceCount > 1)
				mImmediateContext->DrawIndexedInstanced(indexCount, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
			else
				mImmediateContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual GFXTexture2D* CreateTexture2D(const GFXTexture2D_Desc& param)
		{
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
			if (param.mDepthStencil && !DirectX::IsDepthStencil(pixelFormat))
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
			if(param.mResourceView)
				desc.BindFlags |= D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
			if(param.mRenderTargetable)
				desc.BindFlags |= D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
			if (param.mDepthStencil)
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
				if(param.mResourceView)
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
				if (param.mRenderTargetable)
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
				if (param.mDepthStencil)
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
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void BindDepthStencilState(const GFXDepthStencilState* state) override
		{
			if (state == nullptr)
				mImmediateContext->OMSetDepthStencilState(nullptr, 0);
			else
				mImmediateContext->OMSetDepthStencilState(state->As<GFXDepthStencilStateDX>()->mHandle, 1);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void BindRenderTarget(const GFXTexture2D* renderTarget, const GFXTexture2D* depthStencil) override
		{
			ID3D11RenderTargetView* rtv = nullptr;
			if (renderTarget)
			{
				rtv = renderTarget->As<GFXTexture2DDX>()->mRenderTargetView;
				UASSERT(rtv);
			}
			ID3D11DepthStencilView* dsv = nullptr;
			if (depthStencil)
			{
				dsv = depthStencil->As<GFXTexture2DDX>()->mDepthStencilView;
				UASSERT(dsv);
			}
			mImmediateContext->OMSetRenderTargets(1, &rtv, dsv);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void BindRasterizer(const GFXRasterizerState* state) override
		{
			mImmediateContext->RSSetState(state ? state->As<GFXRasterizerStateDX>()->mHandle : nullptr);
		}
		
		GFXShader* CreateShaderFromBytecode(const Buffer& bytecodes, EShaderType type)
		{
			HRESULT result = S_OK;
			GFXShader* shader = nullptr;
			GFXShaderDXBase shaderDX;

			switch (type)
			{
			case EShaderType::EVertex:
				result = mDevice->CreateVertexShader(bytecodes.Data(), bytecodes.Size(), nullptr, (ID3D11VertexShader**)&shaderDX.mHandle);
				shader = new GFXVertexShaderDX(shaderDX);
				break;
			case EShaderType::EHull:
				result = mDevice->CreateHullShader(bytecodes.Data(), bytecodes.Size(), nullptr, (ID3D11HullShader**)&shaderDX.mHandle);
				shader = new GFXHullShaderDX(shaderDX);
				break;
			case EShaderType::EDomain:
				result = mDevice->CreateDomainShader(bytecodes.Data(), bytecodes.Size(), nullptr, (ID3D11DomainShader**)&shaderDX.mHandle);
				shader = new GFXDomainShaderDX(shaderDX);
				break;
			case EShaderType::EGeometry:
				result = mDevice->CreateGeometryShader(bytecodes.Data(), bytecodes.Size(), nullptr, (ID3D11GeometryShader**)&shaderDX.mHandle);
				shader = new GFXGeometryShaderDX(shaderDX);
				break;
			case EShaderType::EPixel:
				result = mDevice->CreatePixelShader(bytecodes.Data(), bytecodes.Size(), nullptr, (ID3D11PixelShader**)&shaderDX.mHandle);
				shader = new GFXPixelShaderDX(shaderDX);
				break;
			case EShaderType::ECompute:
				result = mDevice->CreateComputeShader(bytecodes.Data(), bytecodes.Size(), nullptr, (ID3D11ComputeShader**)&shaderDX.mHandle);
				shader = new GFXComputeShaderDX(shaderDX);
				break;
			}
			
			if (SUCCEEDED(result))
			{
				return shader;
			}
			else
			{
				if (shader) delete shader;
				return nullptr;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		virtual GFXShader* CreateShader(const ShaderUniqueParam& param) override
		{
			if (param.mFileName == nullptr || param.mEntryPoint == nullptr) return nullptr;

			Buffer byteCodes;

			if (!ShaderMgr::Get()->GetShaderCode(param, byteCodes))
			{
				ULOG_ERROR("failed to get shader [%s] bytecodes", param.mFileName);
				return nullptr;
			}

			GFXShader* ret = CreateShaderFromBytecode(byteCodes, param.mType);
			if (ret == nullptr)
			{
				ULOG_ERROR("failed to create shader; name: [%s], entryPoint: [%s], type: [%s]", param.mFileName, param.mEntryPoint, EnumToStr(param.mType));
				return nullptr;
			}
			ULOG_SUCCESS("shader created. name: [%s], type: [%s]", param.mFileName, EnumToStr(param.mType));
			return ret;
		}

		//////////////////////////////////////////////////////////////////////////
		virtual void BindShaders(GFXVertexShader* vertexShader, GFXPixelShader* pixelShader) override
		{
			if(auto vs = vertexShader->As<GFXVertexShaderDX>())
				mImmediateContext->VSSetShader(vs->Handle(), nullptr, 0);
			if (auto ps = pixelShader->As<GFXPixelShaderDX>())
				mImmediateContext->PSSetShader(ps->Handle(), nullptr, 0);
		}

		virtual void BindShader(const GFXShaderBound& shaders) override
		{

		}
		//////////////////////////////////////////////////////////////////////////
		virtual void BindTexture(GFXTexture2D* texture, unsigned slot, EShaderType whichShader) override
		{
			if (texture == nullptr) return;
			ID3D11ShaderResourceView* view = nullptr;
			if (texture)
			{
				view = texture->As<GFXTexture2DDX>()->mResourceView;
				if (view == nullptr)
				{
					ULOG_ERROR("texture has no view");
					return;
				}
			}
			if(whichShader == EShaderType::EVertex)
				mImmediateContext->VSSetShaderResources(slot, 1, &view);
			else if (whichShader == EShaderType::EHull)
				mImmediateContext->PSSetShaderResources(slot, 1, &view);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void BindSamplerState(GFXSamplerState* sampler, unsigned slot, EShaderType shader) override
		{
			ID3D11SamplerState* samplerState = nullptr;
			if(sampler) samplerState = sampler->As<GFXSamplerStateDX>()->mHandle;
			
			if (shader == EShaderType::EVertex)
				mImmediateContext->VSSetSamplers(slot, 1, &samplerState);
			else if (shader == EShaderType::EHull)
				mImmediateContext->PSSetSamplers(slot, 1, &samplerState);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual GFXInputLayout* CreateInputLayout(const GFXInputLayoutDesc& param) override
		{
			UASSERT(param.mVertexShader);
			
			unsigned numElement = 0;
			for (;numElement < param.MAX_ELEMENT; numElement++)
			{
				if(param.mElements[numElement].mName == nullptr) break;
			}

			UASSERT(numElement != 0);

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
			GFXInputLayoutDX* ret = new GFXInputLayoutDX;
			ret->mDesc = param;
			ret->mHandle = handle;
			ULOG_SUCCESS("InputLayout Created");
			return ret;
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void BinInputLayout(const GFXInputLayout* layout) override
		{
			mImmediateContext->IASetInputLayout(layout ? layout->As<GFXInputLayoutDX>()->mHandle : nullptr);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual GFXBlendState* CreateBlendState(const GFXBlendState_Desc& param) override
		{
			D3D11_BLEND_DESC desc;
			desc.AlphaToCoverageEnable = param.mAlphaToCoverageEnable;
			desc.IndependentBlendEnable = param.mIndependentBlendEnable;
			for (size_t i = 0; i < GFX_MAX_RENDER_TARGET; i++)
			{
				desc.RenderTarget[0].BlendEnable = param.mRenderTargets[0].mBlendEnable;
				desc.RenderTarget[0].BlendOp = ToDXType(param.mRenderTargets[0].mBlendOp);
				desc.RenderTarget[0].BlendOpAlpha = ToDXType(param.mRenderTargets[0].mBlendOpAlpha);
				desc.RenderTarget[0].DestBlend = ToDXType(param.mRenderTargets[0].mDestBlend);
				desc.RenderTarget[0].DestBlendAlpha = ToDXType(param.mRenderTargets[0].mDestBlendAlpha);
				desc.RenderTarget[0].SrcBlend = ToDXType(param.mRenderTargets[0].mSrcBlend);
				desc.RenderTarget[0].SrcBlendAlpha = ToDXType(param.mRenderTargets[0].mSrcBlendAlpha);
				desc.RenderTarget[0].RenderTargetWriteMask = ToDXType(param.mRenderTargets[0].mWriteMask);
			}
			ID3D11BlendState* handle = nullptr;
			if (FAILED(mDevice->CreateBlendState(&desc, &handle)))
			{
				ULOG_ERROR("failed to create BlendState");
				return nullptr;
			}
			GFXBlendStateDX* ret = new GFXBlendStateDX;
			ret->mDesc = param;
			ret->mHandle = handle;
			return ret;
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void BindBlendState(const GFXBlendState* state, float blendFactor[4], unsigned sampleMask = 0xFFffFFff) override
		{
			ID3D11BlendState* bs = state ? state->As<GFXBlendStateDX>()->mHandle : nullptr;
			mImmediateContext->OMSetBlendState(bs, blendFactor, sampleMask);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual GFXTexture2D* LoadTextureFromFile(const char* filename)
		{
			if (filename == nullptr) return nullptr;

			char fullFileName[256];
			sprintf_s(fullFileName, "..//Content//%s", filename);
			wchar_t fullFileNameW[256];
			size_t ncc = 0;
			mbstowcs_s(&ncc, fullFileNameW, 256, fullFileName, 256);
			const char* ext = StrFindRNChar(filename, '.', 0);
			if (ext == nullptr) return nullptr;
			unsigned flag = 0;
			DirectX::ScratchImage image;
			HRESULT hr = S_OK;
			if (_strcmpi(ext, ".dds") == 0)
				hr = DirectX::LoadFromDDSFile(fullFileNameW, flag, nullptr, image);
			else if(_strcmpi(ext, ".tga") == 0)
				hr = DirectX::LoadFromTGAFile(fullFileNameW, nullptr, image);
			else
				hr = DirectX::LoadFromWICFile(fullFileNameW, flag, nullptr, image);
			if (SUCCEEDED(hr))
			{
				ID3D11ShaderResourceView* srv = nullptr;
				ID3D11Resource* handle = nullptr;
				if (FAILED(DirectX::CreateShaderResourceView(mDevice, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &srv)))
				{
					ULOG_ERROR("failed to create texture");
					return nullptr;
				}
				GFXTexture2DDX* ret = new GFXTexture2DDX;
				ret->mResourceView = srv;
				srv->GetResource((ID3D11Resource**)&(ret->mTexture));
				ULOG_SUCCESS("Texture loaded");
				return ret;

			}
			ULOG_ERROR("failed to load texture");
			return nullptr;
		}
	};
	//////////////////////////////////////////////////////////////////////////
	extern UAPI GFXDeviceDX* gGFXDX;
};