#pragma once

#include "UGFXDevice.h"

namespace UPO
{
	namespace GlobalResources
	{

		UAPI void LoadGFXResources();
		UAPI void UnloadGFXResources();

		UAPI GFXDepthStencilState* GetDepthStencilState(const GFXDepthStencilStateDesc&);
		UAPI GFXRasterizerState* GetRasterizerState(const GFXRasterizerStateDesc&);
		UAPI GFXBlendState* GetBlendState(const GFXBlendStateDesc&);
		UAPI GFXSamplerState* GetSamplerState(const GFXSamplerStateDesc&);
		UAPI GFXInputLayout* GetInputLayout(const GFXInputLayoutDesc&);
		UAPI GFXTexture2D* GetDefaultTexture2D();
		UAPI GFXSamplerState* GetDefaultSampler();

	}


	//////////////////////////////////////////////////////////////////////////
	template<
		bool				DepthEnable = true,
		bool				DepthWriteAll = true,
		EComparisonFunc		DepthFunc = EComparisonFunc::ELess,

		bool				StencilEnable = false,
		unsigned char		StencilReadMask = 0xFF,
		unsigned char		StencilWriteMask = 0xFF,

		EStencilOP			FrontFaceStencilFailOp = EStencilOP::EKeep,
		EStencilOP			FrontFaceStencilDepthFailOp = EStencilOP::EKeep,
		EStencilOP			FrontFaceStencilPassOp = EStencilOP::EKeep,
		EComparisonFunc		FrontFaceStencilFunc = EComparisonFunc::EAlways,

		EStencilOP			BackFaceStencilFailOp = EStencilOP::EKeep,
		EStencilOP			BackFaceStencilDepthFailOp = EStencilOP::EKeep,
		EStencilOP			BackFaceStencilPassOp = EStencilOP::EKeep,
		EComparisonFunc		BackFaceStencilFunc = EComparisonFunc::EAlways

	> struct TDepthStencilState
	{
		static GFXDepthStencilState* Get()
		{
			static GFXDepthStencilState* State = nullptr;
			if (State == nullptr)
			{
				GFXDepthStencilStateDesc desc;

				desc.mDepthEnable = DepthEnable;
				desc.mDepthWriteAll = DepthWriteAll;
				desc.mDepthFunc = DepthFunc;

				desc.mStencilEnable = StencilEnable;
				desc.mStencilReadMask = StencilReadMask;
				desc.mStencilWriteMask = StencilWriteMask;

				desc.mFrontFaceStencilFailOp = FrontFaceStencilFailOp;
				desc.mFrontFaceStencilDepthFailOp = FrontFaceStencilDepthFailOp;
				desc.mFrontFaceStencilPassOp = FrontFaceStencilPassOp;
				desc.mFrontFaceStencilFunc = FrontFaceStencilFunc;

				desc.mBackFaceStencilFailOp = BackFaceStencilFailOp;
				desc.mBackFaceStencilDepthFailOp = BackFaceStencilDepthFailOp;
				desc.mBackFaceStencilPassOp = BackFaceStencilPassOp;
				desc.mBackFaceStencilFunc = BackFaceStencilFunc;

				State = GlobalResources::GetDepthStencilState(desc);
			}

			return State;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	template<
		bool		Wireframe = false,
		ECullMode	CullMode = ECullMode::EBack,
		bool		FrontCounterClockWise = false,
		int			DepthBias = 0,
		bool		DepthClipEnable = true,
		bool		ScissorEnable = false,
		bool		MultisampleEnable = false,
		bool		AntialiasedLineEnable = false

	> struct TRasterizerState
	{
		static GFXRasterizerState* Get()
		{
			static GFXRasterizerState* State = nullptr;
			if (State == nullptr)
			{
				GFXRasterizerStateDesc desc;
				desc.mWireframe = Wireframe;
				desc.mCullMode = CullMode;
				desc.mFrontCounterClockWise = FrontCounterClockWise;
				desc.mDepthBias = DepthBias;
				desc.mDepthBiasClamp = 0;
				desc.mSlopeScaledDepthBias = 0;
				desc.mDepthClipEnable = DepthClipEnable;
				desc.mScissorEnable = ScissorEnable;
				desc.mMultisampleEnable = MultisampleEnable;
				desc.mAntialiasedLineEnable = AntialiasedLineEnable;
				State = GlobalResources::GetRasterizerState(desc);
			}
			return State;
		}
	};
};