#pragma once

#include "../Core/UCore.h"

namespace UPO
{
	static const unsigned GFX_MAX_RENDER_TARGET = 8;

	//////////////////////////////////////////////////////////////////////////
	class GameWindow;

	//////////////////////////////////////////////////////////////////////////
	class GFXDevice;

	//////////////////////////////////////////////////////////////////////////
	enum class EResourceUsage
	{
		EDefault, EImmutable, EDynamic,
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
	//TODO
	enum class EStencilOP
	{
		EKeep = 1,
		EZero = 2,
		EReplace = 3,
		ESO_INCR_SAT = 4,
		ESO_DECR_SAT = 5,
		ESO_INVERT = 6,
		ESO_INCR = 7,
		ESO_DECR = 8
	};
	//////////////////////////////////////////////////////////////////////////
	enum ETextureFilter
	{
		ETF_MIN_MAG_MIP_POINT = 0,
		ETF_MIN_MAG_POINT_MIP_LINEAR = 0x1,
		ETF_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x4,
		ETF_MIN_POINT_MAG_MIP_LINEAR = 0x5,
		ETF_MIN_LINEAR_MAG_MIP_POINT = 0x10,
		ETF_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x11,
		ETF_MIN_MAG_LINEAR_MIP_POINT = 0x14,
		ETF_MIN_MAG_MIP_LINEAR = 0x15,
		ETF_ANISOTROPIC = 0x55,
		ETF_COMPARISON_MIN_MAG_MIP_POINT = 0x80,
		ETF_COMPARISON_MIN_MAG_POINT_MIP_LINEAR = 0x81,
		ETF_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x84,
		ETF_COMPARISON_MIN_POINT_MAG_MIP_LINEAR = 0x85,
		ETF_COMPARISON_MIN_LINEAR_MAG_MIP_POINT = 0x90,
		ETF_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
		ETF_COMPARISON_MIN_MAG_LINEAR_MIP_POINT = 0x94,
		ETF_COMPARISON_MIN_MAG_MIP_LINEAR = 0x95,
		ETF_COMPARISON_ANISOTROPIC = 0xd5
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
	enum EPrimitiveTopology
	{
		EPT_UNDEFINED = 0,
		EPT_POINTLIST = 1,
		EPT_LINELIST = 2,
		EPT_LINESTRIP = 3,
		EPT_TRIANGLELIST = 4,
		EPT_TRIANGLESTRIP = 5,
		EPT_LINELIST_ADJ = 10,
		EPT_LINESTRIP_ADJ = 11,
		EPT_TRIANGLELIST_ADJ = 12,
		EPT_TRIANGLESTRIP_ADJ = 13,
	};
	//////////////////////////////////////////////////////////////////////////
	enum EPixelFormat
	{
		EPT_UNKNOWN = 0,

		EPT_R32G32B32A32_TYPELESS = 1,
		EPT_R32G32B32A32_FLOAT = 2,
		EPT_R32G32B32A32_UINT = 3,
		EPT_R32G32B32A32_SINT = 4,

		EPT_R32G32B32_TYPELESS = 5,
		EPT_R32G32B32_FLOAT = 6,
		EPT_R32G32B32_UINT = 7,
		EPT_R32G32B32_SINT = 8,

		EPT_R16G16B16A16_TYPELESS = 9,
		EPT_R16G16B16A16_FLOAT = 10,
		EPT_R16G16B16A16_UNORM = 11,
		EPT_R16G16B16A16_UINT = 12,
		EPT_R16G16B16A16_SNORM = 13,
		EPT_R16G16B16A16_SINT = 14,

		EPT_R32G32_TYPELESS = 15,
		EPT_R32G32_FLOAT = 16,
		EPT_R32G32_UINT = 17,
		EPT_R32G32_SINT = 18,

		EPT_R32G8X24_TYPELESS = 19,
		EPT_D32_FLOAT_S8X24_UINT = 20,
		EPT_R32_FLOAT_X8X24_TYPELESS = 21,
		EPT_X32_TYPELESS_G8X24_UINT = 22,
		EPT_R10G10B10A2_TYPELESS = 23,
		EPT_R10G10B10A2_UNORM = 24,
		EPT_R10G10B10A2_UINT = 25,
		EPT_R11G11B10_FLOAT = 26,

		EPT_R8G8B8A8_TYPELESS = 27,
		EPT_R8G8B8A8_UNORM = 28,
		EPT_R8G8B8A8_UNORM_SRGB = 29,
		EPT_R8G8B8A8_UINT = 30,
		EPT_R8G8B8A8_SNORM = 31,
		EPT_R8G8B8A8_SINT = 32,

		EPT_R16G16_TYPELESS = 33,
		EPT_R16G16_FLOAT = 34,
		EPT_R16G16_UNORM = 35,
		EPT_R16G16_UINT = 36,
		EPT_R16G16_SNORM = 37,
		EPT_R16G16_SINT = 38,

		EPT_R32_TYPELESS = 39,
		EPT_D32_FLOAT = 40,	//valid for depth
		EPT_R32_FLOAT = 41,
		EPT_R32_UINT = 42,
		EPT_R32_SINT = 43,

		EPT_R24G8_TYPELESS = 44,
		EPT_D24_UNORM_S8_UINT = 45,	//valid for depth
		EPT_R24_UNORM_X8_TYPELESS = 46,
		EPT_X24_TYPELESS_G8_UINT = 47,

		EPT_R8G8_TYPELESS = 48,
		EPT_R8G8_UNORM = 49,
		EPT_R8G8_UINT = 50,
		EPT_R8G8_SNORM = 51,
		EPT_R8G8_SINT = 52,

		EPT_R16_TYPELESS = 53,
		EPT_R16_FLOAT = 54,
		EPT_D16_UNORM = 55, //valid for depth
		EPT_R16_UNORM = 56,
		EPT_R16_UINT = 57,
		EPT_R16_SNORM = 58,
		EPT_R16_SINT = 59,

		EPT_R8_TYPELESS = 60,
		EPT_R8_UNORM = 61,
		EPT_R8_UINT = 62,
		EPT_R8_SNORM = 63,
		EPT_R8_SINT = 64,

		EPT_A8_UNORM = 65,
		EPT_R1_UNORM = 66,
		EPT_R9G9B9E5_SHAREDEXP = 67,
		EPT_R8G8_B8G8_UNORM = 68,
		EPT_G8R8_G8B8_UNORM = 69,
		EPT_BC1_TYPELESS = 70,
		EPT_BC1_UNORM = 71,
		EPT_BC1_UNORM_SRGB = 72,
		EPT_BC2_TYPELESS = 73,
		EPT_BC2_UNORM = 74,
		EPT_BC2_UNORM_SRGB = 75,
		EPT_BC3_TYPELESS = 76,
		EPT_BC3_UNORM = 77,
		EPT_BC3_UNORM_SRGB = 78,
		EPT_BC4_TYPELESS = 79,
		EPT_BC4_UNORM = 80,
		EPT_BC4_SNORM = 81,
		EPT_BC5_TYPELESS = 82,
		EPT_BC5_UNORM = 83,
		EPT_BC5_SNORM = 84,
		EPT_B5G6R5_UNORM = 85,
		EPT_B5G5R5A1_UNORM = 86,
		EPT_B8G8R8A8_UNORM = 87,
		EPT_B8G8R8X8_UNORM = 88,
		EPT_R10G10B10_XR_BIAS_A2_UNORM = 89,

		EPT_B8G8R8A8_TYPELESS = 90,
		EPT_B8G8R8A8_UNORM_SRGB = 91,
		EPT_B8G8R8X8_TYPELESS = 92,
		EPT_B8G8R8X8_UNORM_SRGB = 93,

		EPT_BC6H_TYPELESS = 94,
		EPT_BC6H_UF16 = 95,
		EPT_BC6H_SF16 = 96,
		EPT_BC7_TYPELESS = 97,
		EPT_BC7_UNORM = 98,
		EPT_BC7_UNORM_SRGB = 99,
		EPT_FORCE_UINT = 0xffffffff
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


		EBlendFactor = 14,
		EInvBlendFactor = 15,
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
		EUnknown, EFloat1, EFloat2, EFloat3, EFloat4,
	};
	//////////////////////////////////////////////////////////////////////////
	struct GFXResource_Desc
	{
		bool				mCPUReadAccess = false;
		bool				mCPUWriteAccess = false;
	};
	//////////////////////////////////////////////////////////////////////////
	struct GFXIndexBuffer_Desc : GFXResource_Desc
	{
		void*				mInitialData = nullptr;
		size_t				mSize = 0;
		EResourceUsage		mUsage = EResourceUsage::EDefault;
		EIndexBufferType	mType = EIndexBufferType::EUShort;

	};
	//////////////////////////////////////////////////////////////////////////
	struct GFXVertexBuffer_Desc : GFXResource_Desc
	{
		void*				mInitialData = nullptr;
		size_t				mSize = 0;
		EResourceUsage		mUsage = EResourceUsage::EDefault;
	};
	//////////////////////////////////////////////////////////////////////////
	struct GFXDepthStencilState_Desc
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
	};
	
	//////////////////////////////////////////////////////////////////////////
	struct GFXSamplerState_Desc
	{
		//Filtering method to use when sampling a texture 
		ETextureFilter		mFilter = ETextureFilter::ETF_MIN_MAG_MIP_LINEAR;
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
	};
	//////////////////////////////////////////////////////////////////////////
	struct GFXRasterizerState_Desc
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
	struct GFXBlendState_Desc
	{
		bool mAlphaToCoverageEnable = false;
		bool mIndependentBlendEnable = false;
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

		} mRenderTargets[GFX_MAX_RENDER_TARGET];
	};

	//////////////////////////////////////////////////////////////////////////
	struct GFXConstantBuffer_Desc
	{
		void*				mInitialData = nullptr;
		size_t				mSize = 0;	//should be multiple of 16
		EResourceUsage		mUsage = EResourceUsage::EDynamic;
	};



	enum class EGFXResource
	{
		EResource,
		EShader,
		ERasterizerState,
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXResource : public RefCountable
	{
		void* mNativeHandle = nullptr;
		EGFXResource mType = EGFXResource::EResource;
		

	public:
		virtual ~GFXResource(){}
		template<typename T> T* As() const { return dynamic_cast<T*>(const_cast<GFXResource*>(this)); }
		template<typename T> bool Is() const { return dynamic_cast<T*>(this) != nullptr; }
		EGFXResource GetType() const { return mType; }
		template<typename T> T& HandleAs() { return *(reinterpret_cast<T*>(mNativeHandle)); }
	};
	//////////////////////////////////////////////////////////////////////////
	typedef unsigned long long ShaderHash_t;
	//////////////////////////////////////////////////////////////////////////
	class GFXShader : public GFXResource
	{
	protected:
		EShaderType mType;
		ShaderHash_t mHash;
	public:
		EShaderType GetType() const { return mType; }
		ShaderHash_t GetHash() const { return mHash; }
	};

	typedef TSmartPtr<GFXShader> GFXShaderRef;


	class GFXVertexShader : public GFXShader { public: static const EShaderType EnumType = EShaderType::EVertex; };
	class GFXHullShader : public GFXShader { public: static const EShaderType EnumType = EShaderType::EHull; };
	class GFXDomainShader : public GFXShader { public: static const EShaderType EnumType = EShaderType::EDomain; };
	class GFXGeometryShader : public GFXShader { public: static const EShaderType EnumType = EShaderType::EGeometry; };
	class GFXPixelShader : public GFXShader { public: static const EShaderType EnumType = EShaderType::EPixel; };
	class GFXComputeShader : public GFXShader { public: static const EShaderType EnumType = EShaderType::ECompute; };


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
	public:
		GFXRasterizerState_Desc mDesc;
		const GFXRasterizerState_Desc& GetDesc() const { return mDesc; }
	};
	typedef TSmartPtr<GFXRasterizerState> GFXRasterizerStateRef;

	//////////////////////////////////////////////////////////////////////////
	class GFXSamplerState : public GFXResource
	{
	protected:
		GFXSamplerState_Desc mDesc;
	public:
		const GFXSamplerState_Desc& GetDesc() const { return mDesc; }
	};
	typedef TSmartPtr<GFXSamplerState> GFXSamplerStateRef;

	//////////////////////////////////////////////////////////////////////////
	class GFXBuffer : public GFXResource
	{
	public:
		virtual void* Map(EMapFlag) = 0;
		virtual void Unmap() = 0;

		template<typename T> T* Map(EMapFlag flag) { return (T*)Map(flag); }
	};
	typedef TSmartPtr<GFXBuffer> GFXBufferRef;

	//////////////////////////////////////////////////////////////////////////
	class GFXVertexBuffer : public GFXBuffer
	{
	protected:
		GFXVertexBuffer_Desc	mDesc;
	public:
		const GFXVertexBuffer_Desc& GetDesc() const { return mDesc; }
	};
	typedef TSmartPtr<GFXVertexBuffer> GFXVertexBufferRef;

	//////////////////////////////////////////////////////////////////////////
	class GFXIndexBuffer : public GFXBuffer
	{
	protected:
		GFXIndexBuffer_Desc		mDesc;
	public:
		const GFXIndexBuffer_Desc& GetDesc() const { return mDesc; }
	};
	typedef TSmartPtr<GFXIndexBuffer> GFXIndexBufferRef;

	//////////////////////////////////////////////////////////////////////////
	class GFXConstantBuffer : public GFXBuffer
	{
	protected:
		GFXConstantBuffer_Desc mDesc;
	public:
		const GFXConstantBuffer_Desc& GetDesc() const { return mDesc; }
	};
	typedef TSmartPtr<GFXConstantBuffer> GFXConstantBufferRef;

	//////////////////////////////////////////////////////////////////////////
	class GFXDepthStencilState : public GFXResource
	{
	protected:
		GFXDepthStencilState_Desc	mDesc;
	public:
		const GFXDepthStencilState_Desc& GetDesc() const { return mDesc; }
	};
	typedef TSmartPtr<GFXDepthStencilState> GFXDepthStencilStateRef;

	//////////////////////////////////////////////////////////////////////////
	struct GFXTexture2D_Desc
	{
		void*			mInitialData = nullptr;
		unsigned		mWidth = 0;
		unsigned		mHeight = 0;
		EPixelFormat	mFormat = EPixelFormat::EPT_UNKNOWN;
		unsigned		mMipLevels = 1;	//currently only 1 mipLevel is implemented
		bool			mResourceView = true;
		bool			mRenderTargetable = false;
		bool			mDepthStencil = false;
		EResourceUsage	mUsage = EResourceUsage::EDefault;
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXTexture2D : public GFXResource
	{
	protected:
		GFXTexture2D_Desc mDesc;
	public:
		const GFXTexture2D_Desc& GetDesc() const { return mDesc; }
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
	struct GFXInputLayoutDesc
	{
		static const unsigned MAX_ELEMENT = 32;

		GFXVertexShader* mVertexShader;
		GFXVertexElementDesc mElements[MAX_ELEMENT];
		
	};
	//////////////////////////////////////////////////////////////////////////
	class GFXInputLayout : public GFXResource
	{
	protected:
		GFXInputLayoutDesc mDesc;
	public:
		const GFXInputLayoutDesc& GetDesc() const { return mDesc; }
	};
	typedef TSmartPtr<GFXInputLayout> GFXInputLayoutRef;

	


	//////////////////////////////////////////////////////////////////////////
	class GFXBlendState : public GFXResource
	{
	protected:
		GFXBlendState_Desc mDesc;
	public:
		const GFXBlendState_Desc& GetDesc() const { return mDesc; }
	};
	typedef TSmartPtr<GFXBlendState> GFXBlendStateRef;

	//////////////////////////////////////////////////////////////////////////
	class GFXProgram : public GFXResource
	{
	};
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
	class GFXPipleLine : public GFXResource
	{

	};
	//////////////////////////////////////////////////////////////////////////
	class UAPI GFXSwapChain : public GFXResource
	{
	public:
		virtual bool Present() { return false; };
		virtual GFXTexture2D* GetBackBuffer() { return nullptr; }
		virtual void GetBackBufferSize(Vec2I& out) {}
		virtual bool Resize(const Vec2I& newSize) { return false; }
		virtual GameWindow* GetGameWindow() { return nullptr; }
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI GFXDevice : public GFXResource
	{
	public:

		static GFXDevice* Create();

		//////////////////////////////////////////////////////////////////////////
		//creates a swap chain from a game window
		virtual GFXSwapChain* CreateSwapChain(GameWindow*) = 0;

		//////////////////////////////////////////////////////////////////////////buffers
		virtual GFXIndexBuffer* CreateIndexBuffer(const GFXIndexBuffer_Desc&) = 0;
		virtual GFXVertexBuffer* CreateVertexBuffer(const GFXVertexBuffer_Desc&) = 0;
		virtual GFXConstantBuffer* CreateConstantBuffer(const GFXConstantBuffer_Desc&) = 0;

		//////////////////////////////////////////////////////////////////////////states
		virtual GFXDepthStencilState* CreateDepthStencilState(const GFXDepthStencilState_Desc&) = 0;
		virtual GFXSamplerState* CreateSamplerState(const GFXSamplerState_Desc&) = 0;
		virtual GFXRasterizerState* CreateRasterizerState(const GFXRasterizerState_Desc&) = 0;


		virtual GFXInputLayout* CreateInputLayout(const GFXInputLayoutDesc& param) = 0;

// 		virtual void Bind(GFXDepthStencilState*) = 0;
// 		virtual void Bind(GFXRasterizerState*) = 0;
// 		virtual void Bind(GFXInputLayout*) = 0;

		virtual void BinVertexBuffer(const GFXVertexBuffer* buffer, unsigned stride, unsigned offset = 0) = 0;
		virtual void BinIndexBuffer(const GFXIndexBuffer* buffer, unsigned offset = 0) = 0;
		virtual void BindConstantBuffer(const GFXConstantBuffer* buffer, unsigned slot, EShaderType whichShader) = 0;
		virtual void SetPrimitiveTopology(EPrimitiveTopology topology) = 0;
		//@vertexCount				Number of vertices to draw.
		//@startVertexLocation		Index of the first vertex, which is usually an offset in a vertex buffer.
		virtual void Draw(unsigned vertexCount, unsigned startVertexLocation = 0, unsigned instanceCount = 0, unsigned startInstanceLocation = 0) = 0;
		//@indexCount				Number of indices to draw.
		//@startIndexLocation		The location of the first index read by the GPU from the index buffer.
		//@baseVertexLocation		A value added to each index before reading a vertex from the vertex buffer.
		virtual void DrawIndexed(unsigned indexCount, unsigned startIndexLocation = 0, unsigned baseVertexLocation = 0, unsigned instanceCount = 0, unsigned startInstanceLocation = 0) = 0;


		virtual GFXTexture2D* CreateTexture2D(const GFXTexture2D_Desc& param) = 0;
		virtual void BindDepthStencilState(const GFXDepthStencilState* state) = 0;
		virtual void BindRenderTarget(const GFXTexture2D* renderTarget, const GFXTexture2D* depthStencil) = 0;
		virtual void BindRasterizer(const GFXRasterizerState* state) = 0;
		
		virtual GFXShader* CreateShader(const ShaderUniqueParam& param) = 0;
		
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

		virtual void BindShader(const GFXShaderBound& shaders) = 0;

		virtual void BindShaders(GFXVertexShader* vertexShader, GFXPixelShader* pixelShader) = 0;

		

		virtual void BindTexture(GFXTexture2D* texture, unsigned slot, EShaderType whichShader) = 0;
		virtual void BindSamplerState(GFXSamplerState* sampler, unsigned slot, EShaderType whichShader) = 0;

		
		virtual void BinInputLayout(const GFXInputLayout* layout) = 0;
		virtual GFXBlendState* CreateBlendState(const GFXBlendState_Desc& param) = 0;
		virtual void BindBlendState(const GFXBlendState* state, float blendFactor[4], unsigned sampleMask = 0xFFffFFff) = 0;

		virtual void ClearRenderTarget(const GFXTexture2D* renderTarget, const Color& color) = 0;
		virtual void ClearDepthStencil(const GFXTexture2D* depthTexture, bool clearDepth, bool clearStencil, float depth, char stencil) = 0;

		virtual void Dispatch(unsigned groupCount[3]) {}

	};
	//////////////////////////////////////////////////////////////////////////
	extern UAPI GFXDevice* gGFX;
};