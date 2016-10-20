#pragma once

#include "UGFXDevice.h"

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
		case UPO::EST_VERTEX:
			return "vs_5_0";
		case UPO::EST_PIXEL:
			return "ps_5_0";
		default:
			UASSERT(false);
			return nullptr;
		}
	}
	class ShaderIncludeer : public ID3DInclude
	{
	public:
		HRESULT __stdcall Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
		HRESULT __stdcall Close(LPCVOID pData);

	};

	struct UAPI ShaderMgr
	{
		static D3D_SHADER_MACRO* GetGlobalDefines()
		{
			return nullptr;
		}
		static void FixShaderPath(const char* shaderFilename, char outFullFilename[256])
		{
			sprintf_s(outFullFilename, 256, "..\\Content\\Shaders\\%s", shaderFilename);
		}
		static bool GetByteCode(const char* filename, const char* entryPoint, EShaderType type, Buffer& outByteCodes)
		{
			outByteCodes = Buffer();

			if (!filename || !entryPoint) return false;

			char fullFileName[256];
			FixShaderPath(filename, fullFileName);
			
			Buffer fileContent;
			if (!File::OpenReadFull(fullFileName, fileContent))
			{
				ULOG_ERROR("there is no such a shader file %s", filename);
				return false;
			}

			static const char* TypeToProfile[] = { "vs_5_0", "ps_5_0" };
			ShaderIncludeer inc;
			ID3D10Blob* shaderByteCode = nullptr;
			ID3D10Blob* shaderError = nullptr;
			if (SUCCEEDED(D3DCompile(fileContent.Data(), fileContent.Size(), fullFileName, GetGlobalDefines()
				, /*D3D_COMPILE_STANDARD_FILE_INCLUDE*/ &inc, entryPoint, ShaderTypeToDXProfile(type), D3D10_SHADER_ENABLE_STRICTNESS, 
				0, &shaderByteCode, &shaderError)))
			{
				outByteCodes = Buffer(shaderByteCode->GetBufferSize(), shaderByteCode->GetBufferPointer());
				return true;
			}
			else
			{
				if (shaderError)
					ULOG_ERROR("Compiling shader failed %s %s : %s", filename, entryPoint, ((char*)shaderError->GetBufferPointer()));
				else
					ULOG_ERROR("Mising Shader File : %s", filename);
				return false;
			}
		}

	};

	//////////////////////////////////////////////////////////////////////////
	inline DXGI_FORMAT ToDXType(EIndexBufferType in)
	{
		switch (in)
		{
		case UPO::EBT_USHORT: return DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
		case UPO::EBT_UINT: return DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		default: return DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	inline D3D11_USAGE ToDXType(EResourceUsage in)
	{
		switch (in)
		{
		case UPO::EBU_DEFAULT: return D3D11_USAGE_DEFAULT;
		case UPO::EBU_IMMUTABLE: return D3D11_USAGE_IMMUTABLE;
		case UPO::EBU_DYNAMIC: return D3D11_USAGE_DYNAMIC;
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
	inline D3D11_BLEND ToDXType(EBlendElement in)
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
	class GFXVertexBufferDX : public GFXBufferDX, GFXVertexBuffer
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
	class GFXIndexBufferDX : public GFXBufferDX, GFXIndexBuffer
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
	class GFXConstantBufferDX : public GFXBufferDX, GFXConstantBuffer
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
	class GFXDepthStencilStateDX : GFXDepthStencilState
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
	class GFXSamplerStateDX : GFXSamplerState
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
	class GFXRasterizerStateDX : GFXRasterizerState
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
	class GFXShaderDX : public GFXResourceDX, GFXShader
	{
		friend GFXDeviceDX;

		void* mHandle = nullptr;
		Buffer mByteCodes;


		~GFXShaderDX()
		{
			switch (mType)
			{
			case UPO::EST_VERTEX:
				if (mHandle) ((ID3D11VertexShader*)mHandle)->Release();
				break;
			case UPO::EST_PIXEL:
				if (mHandle) ((ID3D11PixelShader*)mHandle)->Release();
				break;
			}
			mByteCodes.Free();
		}
		void* GetByteCode() const { return mByteCodes.Data(); }
		size_t GetByteCodeSize() const { return mByteCodes.Size(); }

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
	//////////////////////////////////////////////////////////////////////////
	class GFXDeviceDX : public GFXDevice
	{
	public:
		ID3D11Device* mDevice = nullptr;
		ID3D11DeviceContext* mDeviceContext = nullptr;

		GFXDeviceDX()
		{}
		GFXDeviceDX(ID3D11Device* device, ID3D11DeviceContext* deviceContext) 
			: mDevice(device), mDeviceContext(deviceContext)
		{}

		ID3D11Device* GetDXDevice() const { return mDevice; }
		ID3D11DeviceContext* GetDXDeviceContext() const { return mDeviceContext; }
		//////////////////////////////////////////////////////////////////////////
		virtual void ClearRenderTarget(const GFXTexture2D* renderTarget, const Color& color) override
		{
			if (renderTarget == nullptr) return;
			ID3D11RenderTargetView* rtv = renderTarget->As<GFXTexture2DDX>()->mRenderTargetView;
			if (rtv == nullptr) return;
			mDeviceContext->ClearRenderTargetView(rtv, color.mRGBA);
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
			mDeviceContext->ClearDepthStencilView(dsv, clearFlag, depth, stencil);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual GFXIndexBuffer* CreateIndexBuffer(const GFXIndexBuffer_Desc& param) override
		{
			if (param.mInitialData == nullptr && param.mUsage == EResourceUsage::EBU_IMMUTABLE)
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

			if (param.mUsage == EResourceUsage::EBU_DYNAMIC)
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
				ret->mDeviceContext = mDeviceContext;
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
			if (param.mInitialData == nullptr && param.mUsage == EResourceUsage::EBU_IMMUTABLE)
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

			if (param.mUsage == EResourceUsage::EBU_DYNAMIC)
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
				ret->mDeviceContext = mDeviceContext;
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
				ret->mDeviceContext = mDeviceContext;
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
				mDeviceContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
				return;
			}

			ID3D11Buffer* buffers[1] = { buffer->As<GFXVertexBufferDX>()->mHandle };
			UINT strides[1] = { stride };
			UINT offsets[1] = { offset };
			mDeviceContext->IASetVertexBuffers(0, 1, buffers, strides, offsets);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void BinIndexBuffer(const GFXIndexBuffer* buffer, unsigned offset) override
		{
			if (buffer == nullptr)
			{
				mDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);
				return;
			}

			GFXIndexBufferDX* ib = buffer->As<GFXIndexBufferDX>();
			mDeviceContext->IASetIndexBuffer(ib->mHandle, ToDXType(ib->GetDesc().mType), offset);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void BindConstantBuffer(const GFXConstantBuffer* buffer, unsigned slot, EShaderType whichShader) override
		{
			ID3D11Buffer* d3dBuffer = buffer ? buffer->As<GFXConstantBufferDX>()->GetHandle() : nullptr;
			if (whichShader == EShaderType::EST_VERTEX)
				mDeviceContext->VSSetConstantBuffers(slot, 1, &d3dBuffer);
			else if (whichShader == EShaderType::EST_PIXEL)
				mDeviceContext->PSSetConstantBuffers(slot, 1, &d3dBuffer);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void SetPrimitiveTopology(EPrimitiveTopology topology) override
		{
			mDeviceContext->IASetPrimitiveTopology(ToDXType(topology));
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void Draw(unsigned vertexCount, unsigned startVertexLocation) override
		{
			mDeviceContext->Draw(vertexCount, startVertexLocation);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void DrawIndexed(unsigned indexCount, unsigned startIndexLocation, unsigned baseVertexLocation) override
		{
			mDeviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual GFXTexture2D* CreateTexture2D(const GFXTexture2D_Desc& param)
		{
			UASSERT(param.mWidth < MAX_TEXTURE2D_SIZE && param.mHeight < MAX_TEXTURE2D_SIZE);
			if (param.mUsage == EResourceUsage::EBU_IMMUTABLE && param.mInitialData == nullptr)
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
		virtual void SetDepthStencilState(const GFXDepthStencilState* state) override
		{
			if (state == nullptr)
				mDeviceContext->OMSetDepthStencilState(nullptr, 0);
			else
				mDeviceContext->OMSetDepthStencilState(state->As<GFXDepthStencilStateDX>()->mHandle, 1);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void SetRenderTarget(const GFXTexture2D* renderTarget, const GFXTexture2D* depthStencil) override
		{
			ID3D11RenderTargetView* rtv = nullptr;
			if (renderTarget)
			{
				rtv = renderTarget->As<GFXTexture2DDX>()->mRenderTargetView;
			}
			ID3D11DepthStencilView* dsv = nullptr;
			if (depthStencil)
			{
				dsv = depthStencil->As<GFXTexture2DDX>()->mDepthStencilView;
			}
			mDeviceContext->OMSetRenderTargets(1, &rtv, dsv);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void SetRasterizer(const GFXRasterizerState* state) override
		{
			mDeviceContext->RSSetState(state ? state->As<GFXRasterizerStateDX>()->mHandle : nullptr);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual GFXShader* GetShader(const char* filename, const char* entryPoint, EShaderType type) override
		{
			if (filename == nullptr || entryPoint == nullptr) return nullptr;

			Buffer byteCodes;
			if (!ShaderMgr::GetByteCode(filename, entryPoint, type, byteCodes))
			{
				ULOG_ERROR("failed to get shader bytecodes");
				return nullptr;
			}

			void* shader = nullptr; /*ID3D11VertexShader*   ID3D11PixelShader*/
			if (type == EShaderType::EST_VERTEX)
			{
				if (FAILED(mDevice->CreateVertexShader(byteCodes.Data(), byteCodes.Size(), nullptr, (ID3D11VertexShader**)&shader)))
				{
					ULOG_ERROR("Failed to create vertex shader");
					return nullptr;
				}
			}
			else if (type == EShaderType::EST_PIXEL)
			{
				if (FAILED(mDevice->CreatePixelShader(byteCodes.Data(), byteCodes.Size(), nullptr, (ID3D11PixelShader**)&shader)))
				{
					ULOG_ERROR("Failed to create pixel shader");
					return nullptr;
				}
			}
			GFXShaderDX* ret = new GFXShaderDX;
			ret->mDevice = mDevice;
			ret->mDeviceContext = mDeviceContext;
			ret->mType = type;
			ret->mHandle = shader;
			ret->mByteCodes = byteCodes;

			ULOG_SUCCESS("Shader Created");
			return ret;
#if 0
			char fullFileName[256];
			sprintf_s(fullFileName, "..\\Content\\Shaders\\%s", filename);
			static const char* TypeToProfile[] = { "vs_5_0", "ps_5_0" };

			ID3D10Blob* shaderByteCode = nullptr;
			ID3D10Blob* shaderError = nullptr;

			if (SUCCEEDED(D3DX11CompileFromFileA(fullFileName, nullptr, nullptr, functionName, TypeToProfile[type]
				, D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &shaderByteCode, &shaderError, nullptr)))
			{
				void* shader = nullptr; /*ID3D11VertexShader*   ID3D11PixelShader*/
				if (type == EShaderType::EST_VERTEX)
				{
					if (FAILED(mDevice->CreateVertexShader(shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), 
						nullptr, (ID3D11VertexShader**)&shader)))
					{
						ULOG_ERROR("Failed to create vertex shader");
						shaderByteCode->Release();
						if (shaderError) shaderError->Release();
						return nullptr;
					}

				}
				else if (type == EShaderType::EST_PIXEL)
				{
					if (FAILED(mDevice->CreatePixelShader(shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), 
						nullptr, (ID3D11PixelShader**)&shader)))
					{
						ULOG_ERROR("Failed to create pixel shader");
						shaderByteCode->Release();
						if (shaderError)shaderError->Release();
						return nullptr;
					}
				}
#endif
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void BindShaders(GFXShader* vertexShader, GFXShader* pixelShader)
		{
			ID3D11VertexShader* vs = nullptr;
			if (vertexShader)
			{
				vs = (ID3D11VertexShader*)vertexShader->As<GFXShaderDX>()->mHandle;
			}
			ID3D11PixelShader* ps = nullptr;
			if (pixelShader)
			{
				ps = (ID3D11PixelShader*)pixelShader->As<GFXShaderDX>()->mHandle;
			}
			mDeviceContext->VSSetShader(vs, nullptr, 0);
			mDeviceContext->PSSetShader(ps, nullptr, 0);
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
			if(whichShader == EShaderType::EST_VERTEX)
				mDeviceContext->VSSetShaderResources(slot, 1, &view);
			else if (whichShader == EShaderType::EST_PIXEL)
				mDeviceContext->PSSetShaderResources(slot, 1, &view);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void BindSamplerState(GFXSamplerState* sampler, unsigned slot, EShaderType shader) override
		{
			ID3D11SamplerState* samplerState = nullptr;
			if(sampler) samplerState = sampler->As<GFXSamplerStateDX>()->mHandle;
			
			if (shader == EShaderType::EST_VERTEX)
				mDeviceContext->VSSetSamplers(slot, 1, &samplerState);
			else if (shader == EShaderType::EST_PIXEL)
				mDeviceContext->PSSetSamplers(slot, 1, &samplerState);
		}
		//////////////////////////////////////////////////////////////////////////
		virtual GFXInputLayout* CreateInputLayout(const GFXVertexElement_Desc* elements, unsigned numElement, const GFXShader* vertexShader) override
		{
			if (elements == nullptr || numElement == 0 || vertexShader == nullptr) return nullptr;

			GFXShaderDX* shader = vertexShader->As<GFXShaderDX>();
			if (shader->GetType() != EST_VERTEX)
			{
				ULOG_ERROR("inpult layout needs vertex shader");
				return nullptr;
			}
			D3D11_INPUT_ELEMENT_DESC d3DElements[128];
			for (unsigned i = 0; i < numElement; i++)
			{
				d3DElements[i].SemanticName = elements[i].mName;
				d3DElements[i].SemanticIndex = 0;
				d3DElements[i].InstanceDataStepRate = 0;
				d3DElements[i].InputSlotClass = elements[i].mPerInstance ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
				d3DElements[i].InputSlot = elements[i].mSlot;
				d3DElements[i].Format = ToDXType(elements[i].mFormat);
				d3DElements[i].AlignedByteOffset = elements[i].mOffset >= 0 ? elements[i].mOffset : D3D11_APPEND_ALIGNED_ELEMENT;
			}
			ID3D11InputLayout* handle = nullptr;
			if (FAILED(mDevice->CreateInputLayout(d3DElements, numElement, shader->GetByteCode(), shader->GetByteCodeSize(), &handle)))
			{
				ULOG_ERROR("failed to create InputLayout");
				return nullptr;
			}
			GFXInputLayoutDX* ret = new GFXInputLayoutDX;
			ret->mHandle = handle;
			ULOG_SUCCESS("InputLayout Created");
			return ret;
		}
		//////////////////////////////////////////////////////////////////////////
		virtual void BinInputLayout(const GFXInputLayout* layout) override
		{
			mDeviceContext->IASetInputLayout(layout ? layout->As<GFXInputLayoutDX>()->mHandle : nullptr);
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
			mDeviceContext->OMSetBlendState(bs, blendFactor, sampleMask);
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