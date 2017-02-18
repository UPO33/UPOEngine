#pragma once

#include "../Core/UCore.h"

namespace UPO
{
	static const unsigned GFX_MAX_RENDER_TARGET = 8;

	//////////////////////////////////////////////////////////////////////////
	class GameWindow;

	//////////////////////////////////////////////////////////////////////////
	class GFXDevice;
	class GFXDepthStencilState;
	class GFXBlendState;
	class GFXRasterizerState;
	class GFXInputLayout;
	class GFXVertexBuffer;
	class GFXIndexBuffer;
	class GFXShaderResourceView;
	class GFXRenderTargetView;
	class GFXDepthStencilView;
	

	//////////////////////////////////////////////////////////////////////////
	enum class EResourceUsage
	{
		EDefault, ECPURead, ECPUWrite, ECPUReadWrite, EImmutable,
	};

	//////////////////////////////////////////////////////////////////////////
	enum class EIndexBufferType
	{
		EUShort,
		EUInt,
	};
	//////////////////////////////////////////////////////////////////////////
	enum class EComparisonFunc
	{
		ENever = 1,
		ELess = 2,
		EEqual = 3,
		ELessEqual = 4,
		EGreater = 5,
		ENotEqual = 6,
		EGreaterEqual = 7,
		EAlways = 8
	};

	//////////////////////////////////////////////////////////////////////////
	enum class EStencilOP
	{
		EKeep = 1,
		EZero = 2,
		EReplace = 3,
		EIncClamp = 4,	//Increment the stencil value by 1, and clamp the result.
		EDecClamp = 5,	//Decrement the stencil value by 1, and clamp the result.
		EInvert = 6,
		EInc = 7,
		EDec = 8
	};
	//////////////////////////////////////////////////////////////////////////
	enum class ETextureFilter
	{
		EMinMagMipPoint = 0,
		EMinMagPointMipLinear = 0x1,
		EMinPointMagLinearMipPoint = 0x4,
		EMinPointMagMipLinear = 0x5,
		EMinLinearMagMipPoint = 0x10,
		EMinLinearMagPointMipLinear = 0x11,
		EMinMagLinearMipPoint = 0x14,
		EMinMagMipLinear = 0x15,
		EAnisotropic = 0x55,
		EComparisonMinMagMipPoint = 0x80,
		EComparisonMinMagPointMipLinear = 0x81,
		EComparisonMinPointMagLinearMipPoint = 0x84,
		EComparisonMinPointMagMipLinear = 0x85,
		EComparisonMinLinearMagMipPoint = 0x90,
		EComparisonMinLinearMagPointMipLinear = 0x91,
		EComparisonMinMagLinearMipPoint = 0x94,
		EComparisonMinMagMipLinear = 0x95,
		EComparisonAnisotropic = 0xd5
	};
	//////////////////////////////////////////////////////////////////////////
	enum class ETextureAddress
	{
		EWrap = 1,
		EMirror = 2,
		EClamp = 3,
		EBorder = 4,
	};
	//////////////////////////////////////////////////////////////////////////
	enum class ECullMode
	{
		ENone = 1,
		EFront = 2,
		EBack = 3
	};
	//////////////////////////////////////////////////////////////////////////
	enum class EPrimitiveTopology
	{
		EUndefined = 0,

		EPointList = 1,

		ELineList = 2,
		ELineStrip = 3,
		ETriangleList = 4,
		ETriangleStrip = 5,

		ELineListAdj = 10,
		ELineStripAdj = 11,
		ETriangleListAdj = 12,
		ETriangleStripAdj = 13,
	};
	//////////////////////////////////////////////////////////////////////////
	enum class EPixelFormat
	{
		UNKNOWN = 0,

		R32G32B32A32_TYPELESS = 1,
		R32G32B32A32_FLOAT = 2,
		R32G32B32A32_UINT = 3,
		R32G32B32A32_SINT = 4,

		R32G32B32_TYPELESS = 5,
		R32G32B32_FLOAT = 6,
		R32G32B32_UINT = 7,
		R32G32B32_SINT = 8,

		R16G16B16A16_TYPELESS = 9,
		R16G16B16A16_FLOAT = 10,
		R16G16B16A16_UNORM = 11,
		R16G16B16A16_UINT = 12,
		R16G16B16A16_SNORM = 13,
		R16G16B16A16_SINT = 14,

		R32G32_TYPELESS = 15,
		R32G32_FLOAT = 16,
		R32G32_UINT = 17,
		R32G32_SINT = 18,

		R32G8X24_TYPELESS = 19,	//32 bits for the red channel, 8 bits for the green channel, and 24 bits are unused.
		D32_FLOAT_S8X24_UINT = 20,	//his format supports 32-bit depth, 8-bit stencil, and 24 bits are unused.⁵
		R32_FLOAT_X8X24_TYPELESS = 21, //32-bit red channel, 8 bits are unused, and 24 bits are unused.⁵
		X32_TYPELESS_G8X24_UINT = 22, //32 bits unused, 8 bits for green channel, and 24 bits are unused.
		R10G10B10A2_TYPELESS = 23,
		R10G10B10A2_UNORM = 24,
		R10G10B10A2_UINT = 25,
		R11G11B10_FLOAT = 26,

		R8G8B8A8_TYPELESS = 27,
		R8G8B8A8_UNORM = 28,
		R8G8B8A8_UNORM_SRGB = 29,
		R8G8B8A8_UINT = 30,
		R8G8B8A8_SNORM = 31,
		R8G8B8A8_SINT = 32,

		R16G16_TYPELESS = 33,
		R16G16_FLOAT = 34,
		R16G16_UNORM = 35,
		R16G16_UINT = 36,
		R16G16_SNORM = 37,
		R16G16_SINT = 38,

		R32_TYPELESS = 39,
		D32_FLOAT = 40,	//32 bits for depth
		R32_FLOAT = 41,
		R32_UINT = 42,
		R32_SINT = 43,

		R24G8_TYPELESS = 44,
		D24_UNORM_S8_UINT = 45,	//A 32-bit z-buffer format that supports 24 bits for depth and 8 bits for stencil.
		R24_UNORM_X8_TYPELESS = 46, //24 bits red channel and 8 bits unused
		X24_TYPELESS_G8_UINT = 47,// 24 bits unused and 8 bits green channel

		R8G8_TYPELESS = 48,
		R8G8_UNORM = 49,
		R8G8_UINT = 50,
		R8G8_SNORM = 51,
		R8G8_SINT = 52,

		R16_TYPELESS = 53,
		R16_FLOAT = 54,
		D16_UNORM = 55, //valid for depth
		R16_UNORM = 56,
		R16_UINT = 57,
		R16_SNORM = 58,
		R16_SINT = 59,

		R8_TYPELESS = 60,
		R8_UNORM = 61,
		R8_UINT = 62,
		R8_SNORM = 63,
		R8_SINT = 64,

		A8_UNORM = 65,
		R1_UNORM = 66,
		R9G9B9E5_SHAREDEXP = 67,
		R8G8_B8G8_UNORM = 68,
		G8R8_G8B8_UNORM = 69,
		BC1_TYPELESS = 70,
		BC1_UNORM = 71,
		BC1_UNORM_SRGB = 72,
		BC2_TYPELESS = 73,
		BC2_UNORM = 74,
		BC2_UNORM_SRGB = 75,
		BC3_TYPELESS = 76,
		BC3_UNORM = 77,
		BC3_UNORM_SRGB = 78,
		BC4_TYPELESS = 79,
		BC4_UNORM = 80,
		BC4_SNORM = 81,
		BC5_TYPELESS = 82,
		BC5_UNORM = 83,
		BC5_SNORM = 84,
		B5G6R5_UNORM = 85,
		B5G5R5A1_UNORM = 86,
		B8G8R8A8_UNORM = 87,
		B8G8R8X8_UNORM = 88,
		R10G10B10_XR_BIAS_A2_UNORM = 89,

		B8G8R8A8_TYPELESS = 90,
		B8G8R8A8_UNORM_SRGB = 91,
		B8G8R8X8_TYPELESS = 92,
		B8G8R8X8_UNORM_SRGB = 93,

		BC6H_TYPELESS = 94,
		BC6H_UF16 = 95,
		BC6H_SF16 = 96,
		BC7_TYPELESS = 97,
		BC7_UNORM = 98,
		BC7_UNORM_SRGB = 99,
		FORCE_UINT = 0xffffffff
	};
	//////////////////////////////////////////////////////////////////////////
	enum class EShaderType
	{
		EVertex,
		EHull,
		EDomain,
		EGeometry,
		EPixel,
		ECompute,
	};
	UAPI const char* EnumToStr(EShaderType in);


	//////////////////////////////////////////////////////////////////////////
	enum class EMapFlag
	{
		ERead = 1,
		EWrite = 2,
		EReadWrite = 3,
		//indicates that the app doesn't need to keep the old data in the buffer. If the GPU is still using the buffer when
		//you call, the runtime returns a pointer to a new region of memory instead of the old buffer data. 
		EWriteDiscard = 4,	
		EWriteNoOverwrite = 5
	};
	//////////////////////////////////////////////////////////////////////////
	enum class EBlendFactor
	{
		EZero = 1,
		EOne = 2,
		ESrcColor = 3,
		EInvSrcColor = 4,
		ESrcAlpha = 5,
		EInvSrcAlpha = 6,

		EDstAlpha = 7,
		EInvDstApha = 8,
		EDstColor = 9,
		EInvDstColor = 10,

		EFactor = 14,
		EInvFactor = 15,
	};
	//////////////////////////////////////////////////////////////////////////
	enum class EBlendOP
	{
		EAdd = 1,
		//Subtract source 1 from source 2.
		ESub = 2,
		//Subtract source 2 from source 1.
		ERevSub = 3,
		EMin = 4,
		EMax = 5
	};
	//////////////////////////////////////////////////////////////////////////
	enum  EBlendColorWrite
	{
		ECW_ENABLE_RED = 1,
		ECW_ENABLE_GREEN = 2,
		ECW_ENABLE_BLUE = 4,
		ECW_ENABLE_ALPHA = 8,
		ECW_ENABLE_ALL = (((ECW_ENABLE_RED | ECW_ENABLE_GREEN) | ECW_ENABLE_BLUE) | ECW_ENABLE_ALPHA)
	};
	enum class EVertexFormat
	{
		EUnknown, EFloat1, EFloat2, EFloat3, EFloat4, EColor32, EUInt32
	};
	//////////////////////////////////////////////////////////////////////////
	struct GFXResourceDesc
	{

	};
	//////////////////////////////////////////////////////////////////////////
	struct GFXIndexBufferDesc : GFXResourceDesc
	{
		void*				mInitialData = nullptr;
		size_t				mSize = 0;
		EIndexBufferType	mType = EIndexBufferType::EUShort;
		bool				mImmutable = false;
		bool				mDynamic = false;
		bool				mCPUReadAccess = false;

	};
	//////////////////////////////////////////////////////////////////////////
	struct GFXVertexBufferDesc : GFXResourceDesc
	{
		void*				mInitialData = nullptr;
		size_t				mSize = 0;
		EResourceUsage		mUsage = EResourceUsage::EDefault;
		bool				mImmutable = false;
		bool				mDynamic = false;
		bool				mCPUReadAccess = false;
	};
	//////////////////////////////////////////////////////////////////////////
	struct UAPI GFXDepthStencilStateDesc
	{
		bool				mDepthEnable = true;
		bool				mDepthWriteAll = true;
		EComparisonFunc		mDepthFunc = EComparisonFunc::ELess;

		bool				mStencilEnable = false;
		unsigned char		mStencilReadMask = 0xFF;
		unsigned char		mStencilWriteMask = 0xFF;

		EStencilOP			mFrontFaceStencilFailOp = EStencilOP::EKeep;
		EStencilOP			mFrontFaceStencilDepthFailOp = EStencilOP::EKeep;
		EStencilOP			mFrontFaceStencilPassOp = EStencilOP::EKeep;
		EComparisonFunc		mFrontFaceStencilFunc = EComparisonFunc::EAlways;

		EStencilOP			mBackFaceStencilFailOp = EStencilOP::EKeep;
		EStencilOP			mBackFaceStencilDepthFailOp = EStencilOP::EKeep;
		EStencilOP			mBackFaceStencilPassOp = EStencilOP::EKeep;
		EComparisonFunc		mBackFaceStencilFunc = EComparisonFunc::EAlways;


		bool operator == (const GFXDepthStencilStateDesc& other) const;
		bool operator != (const GFXDepthStencilStateDesc& other) const { return !this->operator==(other); }
	};
	
	//////////////////////////////////////////////////////////////////////////
	struct UAPI GFXSamplerStateDesc
	{
		//Filtering method to use when sampling a texture 
		ETextureFilter		mFilter = ETextureFilter::EMinLinearMagPointMipLinear;
		ETextureAddress		mAddressU = ETextureAddress::EClamp;
		ETextureAddress		mAddressV = ETextureAddress::EClamp;
		ETextureAddress		mAddressW = ETextureAddress::EClamp;
		//Offset from the calculated mipmap level. For example, if Direct3D calculates that a texture should be sampled
		//at mipmap level 3 and MipLODBias is 2, then the texture will be sampled at mipmap level 5.
		float				mMipLODBias = 0;
		//Clamping value used if ETF_ANISOTROPIC or ETF_COMPARISON_ANISOTROPIC is specified in Filter.Valid values are between 1 and 16.
		unsigned			mMaxAnisotropy = 1;
		//A function that compares sampled data against existing sampled data
		EComparisonFunc		mComparisonFunc = EComparisonFunc::ENever;
		//Border color to use if ETA_ADDRESS_BORDER is specified for AddressU, AddressV, or AddressW.
		Color				mBorderColor = Color(1);
		//Lower end of the mipmap range to clamp access to, where 0 is the largest and most 
		//detailed mipmap level and any level higher than that is less detailed.
		float				mMinLOD = -FLT_MAX;
		//Upper end of the mipmap range to clamp access to, where 0 is the largest and most detailed mipmap level
		//and any level higher than that is less detailed. This value must be greater than or equal to MinLOD.
		//To have no upper limit on LOD set this to a large value such as D3D11_FLOAT32_MAX.
		float				mMaxLOD = FLT_MAX;

		bool operator == (const GFXSamplerStateDesc& other) const;
		bool operator != (const GFXSamplerStateDesc& other) const { return !this->operator==(other); }
	};
	//////////////////////////////////////////////////////////////////////////
	struct UAPI GFXRasterizerStateDesc
	{
		bool			mWireframe = false;
		//Indicates triangles facing the specified direction are not drawn
		ECullMode		mCullMode = ECullMode::EBack;
		//Determines if a triangle is front- or back-facing. If this parameter is true, a triangle will be considered front-facing
		//if its vertices are counter-clockwise on the render target and considered back-facing if they are clockwise.
		bool			mFrontCounterClockWise = false;
		int				mDepthBias = 0;
		float			mDepthBiasClamp = 0;
		float			mSlopeScaledDepthBias = 0;
		bool			mDepthClipEnable = true;
		bool			mScissorEnable = false;
		bool			mMultisampleEnable = false;
		bool			mAntialiasedLineEnable = false;

		bool operator == (const GFXRasterizerStateDesc& other) const;
		bool operator != (const GFXRasterizerStateDesc& other) const { return !this->operator ==(other); }
	};


	/************************************************************************
		if (mBlendEnable)
		{
			finalColor.rgb = (mSrcBlend * newColor.rgb) < mBlendOp > (mDestBlend * oldColor.rgb);
			finalColor.a = (mSrcBlendAlpha * newColor.a) < mBlendOpAlpha > (mDestBlendAlpha * oldColor.a);
		}
		else
		{
			finalColor = newColor;
		}

		for alpha blending :
		finalColor.rgb = newAlpha * newColor + (1 - newAlpha) * oldColor;
		finalColor.a = newAlpha.a;
	************************************************************************/
	struct UAPI GFXBlendStateDesc
	{
		bool mAlphaToCoverage = false;
		//true enables independent blending. If set to false, only the mRenderTargets[0] members are used, rest are ignored. 
		bool mIndependentBlend = false;
		struct
		{
			bool mBlendEnable = false;
			EBlendFactor mSrcBlend = EBlendFactor::EOne;
			EBlendFactor mDestBlend = EBlendFactor::EZero;
			EBlendOP mBlendOp = EBlendOP::EAdd;
			EBlendFactor mSrcBlendAlpha = EBlendFactor::EOne;
			EBlendFactor mDestBlendAlpha = EBlendFactor::EZero;
			EBlendOP mBlendOpAlpha = EBlendOP::EAdd;
			EBlendColorWrite mWriteMask = EBlendColorWrite::ECW_ENABLE_ALL;

			void MakeAlphaBlending()
			{
				//src is pixel shader output
				//dest is current value on render target
				mBlendEnable = true;

				mSrcBlend = EBlendFactor::ESrcAlpha;
				mBlendOp = EBlendOP::EAdd;
				mDestBlend = EBlendFactor::EInvSrcAlpha;

				mSrcBlendAlpha = EBlendFactor::EOne;
				mBlendOpAlpha = EBlendOP::EAdd;
				mDestBlendAlpha = EBlendFactor::EZero;
			}

		} mRenderTargets[GFX_MAX_RENDER_TARGET];

		bool operator == (const GFXBlendStateDesc& other) const;
		bool operator != (const GFXBlendStateDesc& other) const { return !this->operator==(other); }
	};

	//////////////////////////////////////////////////////////////////////////
	struct GFXConstantBufferDesc
	{
		void*				mInitialData = nullptr;
		size_t				mSize = 0;	//should be multiple of 16
		EResourceUsage		mUsage = EResourceUsage::ECPUWrite;
	};




	//////////////////////////////////////////////////////////////////////////
	class UAPI GFXResource : public RefCountable
	{
	public:
		void*	mNativeHandle;
		Name	mDebugName;

		GFXResource(void* nativeHandle, Name debugName = nullptr);
		virtual ~GFXResource();

		template<typename T> T* As() const { return dynamic_cast<T*>(const_cast<GFXResource*>(this)); }
		template<typename T> bool Is() const { return dynamic_cast<T*>(this) != nullptr; }
		template<typename T> T& HandleAs() { return *((T*)(&mNativeHandle)); }

		operator bool () const { return mNativeHandle != nullptr; }
	};


	//////////////////////////////////////////////////////////////////////////
	typedef unsigned long long ShaderHash_t;
	//////////////////////////////////////////////////////////////////////////
	class GFXShader : public GFXResource
	{
	protected:
		EShaderType		mType;
	public:
		GFXShader(void* handle, EShaderType type, Name debugName) : GFXResource(handle, debugName) , mType(type) {}
		EShaderType GetType() const { return mType; }
	};

	typedef TSmartPtr<GFXShader> GFXShaderRef;

	//////////////////////////////////////////////////////////////////////////
	class GFXVertexShader : public GFXShader 
	{ 
	public:
		GFXVertexShader(void* handle, Name debugName) : GFXShader(handle, EShaderType::EVertex, debugName) {}

		static const EShaderType EnumType = EShaderType::EVertex;
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXHullShader : public GFXShader 
	{
	public:
		GFXHullShader(void* handle, Name debugName) : GFXShader(handle, EShaderType::EHull, debugName) {}
		static const EShaderType EnumType = EShaderType::EHull;
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXDomainShader : public GFXShader 
	{ 
	public:
		GFXDomainShader(void* handle, Name debugName) : GFXShader(handle, EShaderType::EDomain, debugName) {}
		static const EShaderType EnumType = EShaderType::EDomain;
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXGeometryShader : public GFXShader 
	{
	public:
		GFXGeometryShader(void* handle, Name debugName) : GFXShader(handle, EShaderType::EGeometry, debugName) {}
		static const EShaderType EnumType = EShaderType::EGeometry;
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXPixelShader : public GFXShader 
	{
	public: 
		GFXPixelShader(void* handle, Name debugName) : GFXShader(handle, EShaderType::EPixel, debugName) {}
		static const EShaderType EnumType = EShaderType::EPixel;
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXComputeShader : public GFXShader 
	{
	public: 
		GFXComputeShader(void* handle, Name debugName) : GFXShader(handle, EShaderType::ECompute, debugName) {}
		static const EShaderType EnumType = EShaderType::ECompute; 
	};


	typedef TSmartPtr<GFXVertexShader> GFXVertexShaderRef;
	typedef TSmartPtr<GFXHullShader> GFXHullShaderRef;
	typedef TSmartPtr<GFXDomainShader> GFXDomainShaderRef;
	typedef TSmartPtr<GFXGeometryShader> GFXGeometryShaderRef;
	typedef TSmartPtr<GFXPixelShader> GFXPixelShaderRef;
	typedef TSmartPtr<GFXComputeShader> GFXComputeShaderRef;

	//////////////////////////////////////////////////////////////////////////
	template<EShaderType type> struct TShaderClassFromType {  };

	template<> struct TShaderClassFromType<EShaderType::EVertex> : public GFXVertexShader { typedef GFXVertexShader T; };
	template<> struct TShaderClassFromType<EShaderType::EHull> : public GFXHullShader { typedef GFXHullShader T; };
	template<> struct TShaderClassFromType<EShaderType::EDomain> : public GFXDomainShader { typedef GFXDomainShader T; };
	template<> struct TShaderClassFromType<EShaderType::EGeometry> : public GFXGeometryShader { typedef GFXGeometryShader T; };
	template<> struct TShaderClassFromType<EShaderType::EPixel> : public GFXPixelShader { typedef GFXPixelShader T; };
	template<> struct TShaderClassFromType<EShaderType::ECompute> : public GFXComputeShader { typedef GFXComputeShader T; };

	//////////////////////////////////////////////////////////////////////////
	struct UAPI ShaderMacroDefinition
	{
		ShaderMacroDefinition() {}
		ShaderMacroDefinition(const char* name, const char* definition) : mName(name), mDefinition(definition) {}

		String mName;
		String mDefinition;

		template<typename HasherClass = HasherCRC32> auto GetHash(HasherClass& hasher) const
		{
			hasher << mName;
			hasher << mDefinition;
			return hasher.GetHash();
		}
	};
	//////////////////////////////////////////////////////////////////////////
	struct UAPI ShaderUniqueParam
	{
		static const unsigned MAX_DEFINITION = 32;

		const char* mFileName;
		const char* mEntryPoint;
		EShaderType mType;
		ShaderMacroDefinition mCustomDefinitions[MAX_DEFINITION];
		
		template<typename HasherClass = HasherCRC32> auto GetHash() const
		{
			UASSERT(mFileName && mEntryPoint);
			
			HasherClass hasher;
			hasher << mFileName << mEntryPoint << mType;
			for (size_t i = 0; i < MAX_DEFINITION; i++)
			{
				if (!mCustomDefinitions[i].mName) break;

				UASSERT(mCustomDefinitions[i].mName && mCustomDefinitions[i].mDefinition);

				hasher << mCustomDefinitions[i].mName << mCustomDefinitions[i].mDefinition;
			}

			return hasher.GetHash();
		}
	};

	//////////////////////////////////////////////////////////////////////////
	class GFXRasterizerState : public GFXResource
	{
	protected:
		GFXRasterizerStateDesc mDesc;
	public:
		GFXRasterizerState(void* handle, Name debugName = nullptr) : GFXResource(handle, debugName) {}

		const GFXRasterizerStateDesc& GetDesc() const { return mDesc; }
	};
	typedef TSmartPtr<GFXRasterizerState> GFXRasterizerStateRef;

	//////////////////////////////////////////////////////////////////////////
	class GFXSamplerState : public GFXResource
	{
	protected:
		GFXSamplerStateDesc mDesc;
	public:
		GFXSamplerState(void* handle, Name debugName = nullptr) : GFXResource(handle, debugName) {}
		const GFXSamplerStateDesc& GetDesc() const { return mDesc; }
	};
	typedef TSmartPtr<GFXSamplerState> GFXSamplerStateRef;

	//////////////////////////////////////////////////////////////////////////
	class GFXBuffer : public GFXResource
	{
	public:
		GFXBuffer(void* handle, Name debugName = nullptr) : GFXResource(handle, debugName) {}
	};
	typedef TSmartPtr<GFXBuffer> GFXBufferRef;

	//////////////////////////////////////////////////////////////////////////
	class GFXVertexBuffer : public GFXBuffer
	{
	protected:
		GFXVertexBufferDesc	mDesc;
	public:
		GFXVertexBuffer(void* handle, Name debugName = nullptr) : GFXBuffer(handle, debugName) {}
		const GFXVertexBufferDesc& GetDesc() const { return mDesc; }
	};
	typedef TSmartPtr<GFXVertexBuffer> GFXVertexBufferRef;

	//////////////////////////////////////////////////////////////////////////
	class GFXIndexBuffer : public GFXBuffer
	{
	protected:
		GFXIndexBufferDesc		mDesc;
	public:
		GFXIndexBuffer(void* handle, Name debugName = nullptr) : GFXBuffer(handle, debugName) {}
		const GFXIndexBufferDesc& GetDesc() const { return mDesc; }
	};
	typedef TSmartPtr<GFXIndexBuffer> GFXIndexBufferRef;

	//////////////////////////////////////////////////////////////////////////
	class GFXConstantBuffer : public GFXBuffer
	{
	protected:
		GFXConstantBufferDesc mDesc;
	public:
		GFXConstantBuffer(void* handle, Name debugName = nullptr) : GFXBuffer(handle, debugName) {}
		const GFXConstantBufferDesc& GetDesc() const { return mDesc; }
	};
	typedef TSmartPtr<GFXConstantBuffer> GFXConstantBufferRef;

	//////////////////////////////////////////////////////////////////////////
	class GFXDepthStencilState : public GFXResource
	{
	protected:
		GFXDepthStencilStateDesc	mDesc;
	public:
		GFXDepthStencilState(void* handle, Name debugName = nullptr) : GFXResource(handle, debugName) {}

		const GFXDepthStencilStateDesc& GetDesc() const { return mDesc; }
	};
	typedef TSmartPtr<GFXDepthStencilState> GFXDepthStencilStateRef;


	enum ETextureFlag
	{
		EDefault = 0,
		EShaderResourceView = 1,
		ERenderTargatable = 2,
		EDepthStencil = 4,
		ECPURead = 8,
		ECPUWrite = 16,
		EImmutable = 32,
	};
	enum BufferFlag
	{

	};
	//////////////////////////////////////////////////////////////////////////
	struct GFXTexture2DDesc
	{
		void*			mInitialData = nullptr;
		unsigned		mWidth = 0;
		unsigned		mHeight = 0;
		EPixelFormat	mFormat = EPixelFormat::UNKNOWN;
		//Use 1 for a multisampled texture; or 0 to generate a full set of subtextures.
		unsigned		mMipLevels = 1;	//currently only 1 mipLevel is implemented
		unsigned		mSampleCount = 1;
// 		bool			mShaderResourceView = true;
// 		bool			mRenderTargetView = false;
// 		bool			mDepthStencilView = false;
// 		EResourceUsage	mUsage = EResourceUsage::EDefault;
		Flag		mFlag = 0; //ETextureFlag
	};

	//////////////////////////////////////////////////////////////////////////
	class GFXTexture2D : public GFXResource
	{
	protected:
		GFXTexture2DDesc mDesc;
		GFXShaderResourceView*	mShaderResourceView = nullptr;
		GFXRenderTargetView*	mRenderTargetView = nullptr;
		GFXDepthStencilView*	mDepthStencilView = nullptr;
	public:
		GFXShaderResourceView* GetShaderResourceView() { return mShaderResourceView; }
		GFXRenderTargetView* GetRenderTargetView() { return mRenderTargetView; }
		GFXDepthStencilView* GetDepthStencilView() { return mDepthStencilView; }

		GFXTexture2D(void* handle , Name debugName = nullptr) : GFXResource(handle, debugName) {}
		const GFXTexture2DDesc& GetDesc() const { return mDesc; }

	};
	typedef TSmartPtr<GFXTexture2D> GFXTexture2DRef;

	//////////////////////////////////////////////////////////////////////////
	struct GFXVertexElementDesc
	{
		const char*		mName = nullptr;
		EVertexFormat	mFormat = EVertexFormat::EUnknown;
		int				mOffset = -1;	//-1 to auto detect
		unsigned		mSlot = 0;
		bool			mPerInstance = false;

		GFXVertexElementDesc() {}

		GFXVertexElementDesc(const char* name, EVertexFormat format, int offset = -1, unsigned slot = 0, bool perInstance = false)
		{
			UASSERT(name && format != EVertexFormat::EUnknown);
			mName = name;
			mFormat = format;
			mOffset = offset;
			mSlot = slot;
			mPerInstance = perInstance;
		}
	};
	//////////////////////////////////////////////////////////////////////////
	struct UAPI GFXInputLayoutDesc
	{
		static const unsigned MAX_ELEMENT = 32;

		GFXVertexShader* mVertexShader;
		GFXVertexElementDesc mElements[MAX_ELEMENT];
		
		unsigned GetNumElement() const
		{
			for (unsigned i = 0; i < MAX_ELEMENT; i++)
				if(mElements[i].mName == nullptr) return i;

			return MAX_ELEMENT;
		}
		bool operator == (const GFXInputLayoutDesc& other) const;
		bool operator != (const GFXInputLayoutDesc& other) const { return !this->operator==(other); }
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXInputLayout : public GFXResource
	{
	protected:
		GFXInputLayoutDesc mDesc;
	public:
		GFXInputLayout(void* handle, Name debugName) : GFXResource(handle, debugName) {}
		const GFXInputLayoutDesc& GetDesc() const { return mDesc; }
	};
	typedef TSmartPtr<GFXInputLayout> GFXInputLayoutRef;

	


	//////////////////////////////////////////////////////////////////////////
	class GFXBlendState : public GFXResource
	{
	protected:
		GFXBlendStateDesc mDesc;
	public:
		GFXBlendState(void* handle, Name debugName = nullptr) : GFXResource(handle, debugName) {}

		const GFXBlendStateDesc& GetDesc() const { return mDesc; }
	};
	typedef TSmartPtr<GFXBlendState> GFXBlendStateRef;


	//////////////////////////////////////////////////////////////////////////
	struct GFXShaderBound
	{
		GFXVertexShader* mVertexShader = nullptr;
		GFXHullShader* mHullShader = nullptr;
		GFXDomainShader* mDomainShader = nullptr;
		GFXGeometryShader* mGeometryShader = nullptr;
		GFXPixelShader*	mPixelShader = nullptr;
		GFXPixelShader*	mComputeShader = nullptr;
	};
	//////////////////////////////////////////////////////////////////////////
	struct GFXPipelineStage_Desc
	{
		GFXVertexShader* mVertexShader = nullptr;
		GFXHullShader* mHullShader = nullptr;
		GFXDomainShader* mDomainShader = nullptr;
		GFXGeometryShader* mGeometryShader = nullptr;
		GFXPixelShader*	mPixelShader = nullptr;

		GFXInputLayout* mInputLayout = nullptr;

		GFXVertexBuffer* mVertexBuffers[4] = {};
		unsigned mNumVertexBuffers = 0;

		GFXIndexBuffer* mIndexBuffer = nullptr;

		GFXRasterizerState* mRasterizerState = nullptr;
		GFXBlendState* mBlendState = nullptr;
		GFXDepthStencilState* mDepthStencilState = nullptr;
	};

	struct GFXSwapChainDesc
	{
		bool	mVSyncEnable = true;
		bool	mFullScreem = false;
		int		mSampleCount = 1;
		GameWindow*	mGameWindow = nullptr;

		GFXSwapChainDesc(){}

		GFXSwapChainDesc(InitConfig)
		{
			mVSyncEnable = GEngineConfig()->AsBool("GFX.VSync");
			mSampleCount = Max(GEngineConfig()->AsNumber("GFX.MultiSample"), 1.0f);
			mFullScreem = GEngineConfig()->AsBool("Window.FullScreen", false);
		}
	};

	

	//////////////////////////////////////////////////////////////////////////
	class GFXShaderResourceView : public GFXResource
	{
	public:
		GFXShaderResourceView(void* handle, Name debugName = nullptr) : GFXResource(handle, debugName) {}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXRenderTargetView : public GFXResource
	{
	public:
		GFXRenderTargetView(void* handle, Name debugName = nullptr) : GFXResource(handle, debugName) {}
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXDepthStencilView : public GFXResource
	{
	public:
		GFXDepthStencilView(void* handle, Name debugName = nullptr) : GFXResource(handle, debugName) {}
	};

	UAPI GFXResource* UGetGFXResourceByNativeHandle(void* nativeHandle);

	//////////////////////////////////////////////////////////////////////////
	class UAPI GFXSwapChain : public GFXResource
	{
	protected:
		GFXSwapChainDesc mDesc;
		Vec2I mBackBufferSize;
	public:
		GFXSwapChain(void* handle, Name debugName) : GFXResource(handle, debugName) {}

		const GFXSwapChainDesc& GetDesc() { return mDesc; }

		virtual bool Present() { return false; };
		virtual GFXRenderTargetView* GetBackBufferView() { return nullptr; }
		virtual void GetBackBufferSize(Vec2I& out) { out = mBackBufferSize; }
		virtual bool Resize(const Vec2I& newSize) { return false; }
		virtual GameWindow* GetWindow() { return nullptr; }
	};
	//////////////////////////////////////////////////////////////////////////
	template<class TGFXResource> class TGFXResourceHandle
	{
	public:
		void* mNativeHandle;

		TGFXResourceHandle() : mNativeHandle(nullptr) {}
		TGFXResourceHandle(const TGFXResource* resource)
		{
			mNativeHandle = resource ? resource->mNativeHandle : nullptr;
		}
		~TGFXResourceHandle() { mNativeHandle = nullptr; }

		TGFXResourceHandle& operator = (const TGFXResource* resource)
		{
			mNativeHandle = resource ? resource->mNativeHandle : nullptr;
			return *this;
		}
		template<typename T> T& HandleAs() { return *(reinterpret_cast<T*>(&mNativeHandle)); }

		TGFXResource* GetResource() const
		{
			return (TGFXResource*)UGetGFXResourceByNativeHandle(mNativeHandle);
		}

		template<typename To> explicit operator TGFXResourceHandle<To>() const
		{
			return TGFXResourceHandle<To>((To*)(mNativeHandle));
		}
		operator bool() const { return mNativeHandle != nullptr; }
	};

	typedef TGFXResourceHandle<GFXTexture2D>				GFXTexture2DHandle;
	typedef TGFXResourceHandle<GFXShader>					GFXShaderHandle;
	typedef TGFXResourceHandle<GFXVertexShader>				GFXVertexShaderHandle;
	typedef TGFXResourceHandle<GFXHullShader>				GFXHullShaderHandle;
	typedef TGFXResourceHandle<GFXDomainShader>				GFXDomainShaderHandle;
	typedef TGFXResourceHandle<GFXGeometryShader>			GFXGeometryShaderHandle;
	typedef TGFXResourceHandle<GFXPixelShader>				GFXPixelShaderHandle;
	typedef TGFXResourceHandle<GFXComputeShader>			GFXComputeShaderHandle;

	typedef TGFXResourceHandle<GFXRasterizerState>			GFXRasterizerStateHandle;
	typedef TGFXResourceHandle<GFXDepthStencilState>		GFXDepthStencilStateHandle;
	typedef TGFXResourceHandle<GFXBlendState>				GFXBlendStateHandle;
	typedef TGFXResourceHandle<GFXSamplerState>				GFXSamplerStateHandle;

	typedef TGFXResourceHandle<GFXShaderResourceView>		GFXShaderResourceViewHandle;
	typedef TGFXResourceHandle<GFXDepthStencilView>			GFXDepthStencilViewHandle;
	typedef TGFXResourceHandle<GFXRenderTargetView>			GFXRenderTargetViewHandle;

	typedef TGFXResourceHandle<GFXVertexBuffer>				GFXVertexBufferHandle;
	typedef TGFXResourceHandle<GFXIndexBuffer>				GFXIndexBufferHandle;
	typedef TGFXResourceHandle<GFXConstantBuffer>			GFXConstantBufferHandle;
	typedef TGFXResourceHandle<GFXInputLayout>				GFXInputLayoutHandle;


	struct GFXViewport
	{
		float mTopLeftX;
		float mTopLeftY;
		float mWidth;
		float mHeight;
		float mMinDepth;	//Minimum depth of the viewport. Ranges between 0 and 1.
		float mMaxDepth;	//Maximum depth of the viewport. Ranges between 0 and 1.

		GFXViewport(){}
		GFXViewport(Vec2 topLeft, Vec2 size, float minDepth, float maxDepth)
		{
			mTopLeftX = topLeft.mX;
			mTopLeftY = topLeft.mY;
			mWidth = size.mX;
			mHeight = size.mY;
			mMinDepth = minDepth;
			mMaxDepth = maxDepth;
		}
	};
	inline Vec3 UNDCToScreenSpace(const Vec3 ndc, const GFXViewport& viewport)
	{
		//https://msdn.microsoft.com/en-us/library/windows/desktop/bb205126(v=vs.85).aspx
		float X = (ndc.mX + 1) * viewport.mWidth * 0.5f + viewport.mTopLeftX;
		float Y = (1 - ndc.mY) * viewport.mHeight * 0.5f + viewport.mTopLeftY;
		float Z = viewport.mMinDepth + ndc.mZ * (viewport.mMaxDepth - viewport.mMinDepth);
		return Vec3(X, Y, Z);
	}
	//////////////////////////////////////////////////////////////////////////
	class UAPI GFXDevice
	{
	public:

		static GFXDevice* Create();

		//////////////////////////////////////////////////////////////////////////
		virtual void SetViewport(const GFXViewport& viewport) = 0;

		//////////////////////////////////////////////////////////////////////////
		virtual GFXShaderResourceView* CreateShaderResourceView(GFXTexture2D* texture2D) = 0;
		virtual GFXShaderResourceView* CreateShaderResourceView(GFXTexture2D* texture2D, unsigned mostDetailedMip, unsigned numMipLevels, EPixelFormat format) = 0;

		virtual GFXRenderTargetView* CreateRenderTargetView(GFXTexture2D* texture) = 0;
		virtual GFXRenderTargetView* CreateRenderTargetView(GFXTexture2D* texture, unsigned mipSlice, EPixelFormat format = EPixelFormat::UNKNOWN) = 0;

		virtual GFXDepthStencilView* CreateDepthStencilView(GFXTexture2D* texture) = 0;
		virtual GFXDepthStencilView* CreateDepthStencilView(GFXTexture2D* texture, unsigned mipSlice, EPixelFormat format = EPixelFormat::UNKNOWN) = 0;

		//////////////////////////////////////////////////////////////////////////
		//creates a swap chain from a game window
		virtual GFXSwapChain* CreateSwapChain(const GFXSwapChainDesc& param) = 0;

		//////////////////////////////////////////////////////////////////////////buffers
		virtual GFXIndexBuffer* CreateIndexBuffer(const GFXIndexBufferDesc&) = 0;
		virtual GFXVertexBuffer* CreateVertexBuffer(const GFXVertexBufferDesc&) = 0;
		//DEPRECATED
		virtual GFXConstantBuffer* CreateConstantBuffer(const GFXConstantBufferDesc&) = 0;
		virtual GFXConstantBuffer* CreateConstantBuffer(unsigned size) = 0;

		//////////////////////////////////////////////////////////////////////////states
		virtual GFXDepthStencilState* CreateDepthStencilState(const GFXDepthStencilStateDesc&) = 0;
		virtual GFXSamplerState* CreateSamplerState(const GFXSamplerStateDesc&) = 0;
		virtual GFXRasterizerState* CreateRasterizerState(const GFXRasterizerStateDesc&) = 0;


		virtual GFXInputLayout* CreateInputLayout(const GFXInputLayoutDesc& param) = 0;
		virtual void SetInputLayout(GFXInputLayoutHandle layout) = 0;

		virtual void SetVertexBuffer(GFXVertexBuffer* buffer, unsigned slot, unsigned stride, unsigned offset = 0) = 0;
		virtual void SetIndexBuffer(GFXIndexBuffer* buffer, unsigned offset = 0) = 0;
		virtual void SetPrimitiveTopology(EPrimitiveTopology topology) = 0;
		//@vertexCount				Number of vertices to draw.
		//@startVertexLocation		Index of the first vertex, which is usually an offset in a vertex buffer.
		virtual void Draw(unsigned vertexCount, unsigned startVertexLocation = 0, unsigned instanceCount = 0, unsigned startInstanceLocation = 0) = 0;
		//@indexCount				Number of indices to draw.
		//@startIndexLocation		The location of the first index read by the GPU from the index buffer.
		//@baseVertexLocation		A value added to each index before reading a vertex from the vertex buffer.
		virtual void DrawIndexed(unsigned indexCount, unsigned startIndexLocation = 0, unsigned baseVertexLocation = 0, unsigned instanceCount = 0, unsigned startInstanceLocation = 0) = 0;

		virtual GFXTexture2D* CreateTexture2DFromMemory(const Buffer& memory) = 0;

		virtual GFXTexture2D* CreateTexture2D(const GFXTexture2DDesc& param) = 0;


		virtual void SetDepthStencilState(GFXDepthStencilStateHandle state) = 0;
		virtual void SetRenderTarget(GFXRenderTargetView** renderTargets, unsigned numRenderTargets, GFXDepthStencilView* depthStencil) = 0;
		template<unsigned N> void SetRenderTarget(GFXRenderTargetView*(&renderTargets)[N], GFXDepthStencilView* depthStencil)
		{
			SetRenderTarget(renderTargets, N, depthStencil);
		}
		virtual void SetRasterizerState(GFXRasterizerStateHandle state) = 0;

		virtual GFXShader* CreateShader(const Buffer& bytesCode, EShaderType type, Name debugName = nullptr) = 0;
		virtual GFXShader* CreateShader(const ShaderUniqueParam& param) = 0;
		template<typename TShaderClass> TShaderClass* CreateShader(const Buffer& bytesCode, Name debugName = nullptr)
		{
			return (TShaderClass*)CreateShader(bytesCode, TShaderClass::EnumType, debugName);
		}
		// 		template<typename ShaderClass> ShaderClass* CreateShader(const char* filename, const char* entryPoint, const ShaderMacroDefinition* customDefinitionsNullTerminated = nullptr)
		// 		{
		// 			static_assert(std::is_base_of<GFXShader, ShaderClass>::value, "invalid shader class");
		// 			ShaderUniqueParam param;
		// 			param.mFileName = filename;
		// 			param.mEntryPoint = entryPoint;
		// 			param.mType = ShaderClass::EnumType;
		// 			//TODO: definitions
		// 			return CreateShader(filename, functionName, ShaderClass::EnumType, customDefinitionsNullTerminated)->As<ShaderClass>();
		// 		}

		virtual void SetShader(const GFXShaderBound& shaders) = 0;

		virtual void SetShaders(GFXVertexShader* vs, GFXHullShader* hs, GFXDomainShader* ds, GFXGeometryShader* gs, GFXPixelShader* ps) = 0;


		virtual void SetResourceView(GFXTexture2D** textures, unsigned startSlot, unsigned numTextures, EShaderType whichShader) = 0;
		template<unsigned N> void SetResourceView(GFXTexture2D*(&textures)[N], unsigned startSlot, EShaderType whichShader)
		{
			SetResourceView(textures, startSlot, N, whichShader);
		};
		void SetResourceView(GFXTexture2D* texture, unsigned slot, EShaderType wichShader)
		{
			SetResourceView(&texture, slot, 1, wichShader);
		}



		virtual void SetResourceView(GFXShaderResourceView** views, unsigned startSlot, unsigned numViews, EShaderType whichShder) = 0;
		template<unsigned N> void SetResourceView(GFXShaderResourceView*(&views)[N], unsigned startSlot, EShaderType whichShader)
		{
			SetResourceView(views, startSlot, N, whichShader);
		}
		void SetResourceView(GFXShaderResourceView* view, unsigned slot, EShaderType whichShader)
		{
			SetResourceView(&view, slot, 1, whichShader);
		}




		virtual void SetSamplerState(GFXSamplerStateHandle* samplers, unsigned startSlot, unsigned numSamplers, EShaderType whichShader) = 0;
		template<unsigned N> void SetSamplerState(GFXSamplerStateHandle(&samplers)[N], unsigned startSlot, EShaderType whichShader)
		{
			SetSamplerState(samplers, startSlot, N, whichShader);
		}
		void SetSamplerState(GFXSamplerStateHandle sampler, unsigned slot, EShaderType whichShader)
		{
			SetSamplerState(&sampler, slot, 1, whichShader);
		}



		virtual void SetConstentBuffer(GFXConstantBuffer** buffers, unsigned startSlot, unsigned numBuffers, EShaderType whichShader) = 0;
		template<unsigned N> void SetConstentBuffer(GFXConstantBuffer*(&buffers)[N], unsigned startSlot, EShaderType whichShader)
		{
			SetConstentBuffer(buffers, startSlot, N, whichShader);
		}
		void SetConstentBuffer(GFXConstantBuffer* buffer, unsigned slot, EShaderType whichShader)
		{
			SetConstentBuffer(&buffer, slot, 1, whichShader);
		}




		
		virtual GFXBlendState* CreateBlendState(const GFXBlendStateDesc& param) = 0;
		virtual void SetBlendState(GFXBlendStateHandle blendState, const Color& blendFactor = Color::WHITE, unsigned sampleMask = 0xFFffFFff) = 0;

		virtual void ClearRenderTarget(GFXRenderTargetView* view, const Color& color) = 0;
		virtual void ClearDepthStencil(GFXDepthStencilView* view, bool clearDepth, bool clearStencil, float depth, char stencil) = 0;

		virtual void Dispatch(unsigned groupCount[3]) {}

		/*
		This method resets any device context to the default settings. This sets all input/output resource slots, shaders,
		input layouts, predications, scissor rectangles, depth-stencil state, rasterizer state, 
		blend state, sampler state, and viewports to NULL. The primitive topology is set to UNDEFINED.
		*/
		virtual void ClearState() = 0;
		virtual bool IsImmediate() = 0;

		virtual void* Map(GFXBuffer*, EMapFlag) = 0;
		virtual void Unmap(GFXBuffer*) = 0;
		template<typename T> T* Map(GFXBuffer* buffer, EMapFlag flag) { return (T*)Map(buffer, flag); }

		virtual void CopyResource(GFXResource* dst, GFXResource* src) = 0;
		virtual void CopySubresourceRegion(GFXTexture2D* dst, unsigned dstMipIndex, Vec2I dstXY, GFXTexture2D* src, unsigned srcMipIndex, Vec2I srcLeftTop, Vec2I srcRightBottom) = 0;


		virtual void* Map(GFXTexture2D*, EMapFlag, unsigned mipIndex, unsigned& outRowPitch) = 0;
		virtual void Unmap(GFXTexture2D*, unsigned mipIndex) = 0;
	};
	//////////////////////////////////////////////////////////////////////////
	extern UAPI GFXDevice* gGFX;
};