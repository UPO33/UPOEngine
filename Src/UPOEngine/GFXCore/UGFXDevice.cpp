#pragma once

#include "../Meta/UMeta.h"

#include "UGFXDeviceDX.h"

namespace UPO
{

	UAPI const char* EnumToStr(EShaderType in)
	{
		static const char* LUT[] =
		{
			"Vertex",
			"Hull",
			"Domain",
			"Geometry",
			"Pixel",
			"Compute",
		};
		return LUT[(unsigned)in];
	}

	TArray<GFXResource*>	gGFXResources;

	UAPI GFXResource* UGetGFXResourceByNativeHandle(void* nativeHandle)
	{
		//TODO : optimize this function 

		for (GFXResource* iter : gGFXResources)
		{
			if (iter->mNativeHandle == nativeHandle)
				return iter;
		}
		return nullptr;
	}

	UAPI GFXDevice* gGFX = nullptr;





	GFXDevice* GFXDevice::Create()
	{
	 	 return GFXDeviceDX::Create();
	}

	UENUM(ECullMode, false, nullptr,
		ECullMode::ENone,
		ECullMode::EFront,
		ECullMode::EBack);

	UENUM(EComparisonFunc, false, nullptr,
		EComparisonFunc::ENever,
		EComparisonFunc::ELess,
		EComparisonFunc::EEqual,
		EComparisonFunc::ELessEqual,
		EComparisonFunc::EGreater,
		EComparisonFunc::ENotEqual,
		EComparisonFunc::EGreaterEqual,
		EComparisonFunc::EAlways);
	
	UENUM(ETextureAddress, false, nullptr, 
		ETextureAddress::EWrap,
		ETextureAddress::EMirror,
		ETextureAddress::EClamp,
		ETextureAddress::EBorder);

	UENUM(ETextureFilter, false, nullptr,
		ETextureFilter::EMinMagMipPoint,
		ETextureFilter::EMinMagPointMipLinear,
		ETextureFilter::EMinPointMagLinearMipPoint,
		ETextureFilter::EMinPointMagMipLinear,
		ETextureFilter::EMinLinearMagMipPoint,
		ETextureFilter::EMinLinearMagPointMipLinear,
		ETextureFilter::EMinMagLinearMipPoint,
		ETextureFilter::EMinMagMipLinear,
		ETextureFilter::EAnisotropic,
		ETextureFilter::EComparisonMinMagMipPoint,
		ETextureFilter::EComparisonMinMagPointMipLinear,
		ETextureFilter::EComparisonMinPointMagLinearMipPoint,
		ETextureFilter::EComparisonMinPointMagMipLinear,
		ETextureFilter::EComparisonMinLinearMagMipPoint,
		ETextureFilter::EComparisonMinLinearMagPointMipLinear,
		ETextureFilter::EComparisonMinMagLinearMipPoint,
		ETextureFilter::EComparisonMinMagMipLinear,
		ETextureFilter::EComparisonAnisotropic);

	bool GFXDepthStencilStateDesc::operator==(const GFXDepthStencilStateDesc& other) const
	{
		return mDepthEnable == other.mDepthEnable &&
			mDepthWriteAll == other.mDepthWriteAll &&
			mDepthFunc == other.mDepthFunc &&
			mStencilEnable == other.mStencilEnable &&
			mStencilReadMask == other.mStencilReadMask &&
			mStencilWriteMask == other.mStencilWriteMask &&
			mFrontFaceStencilFailOp == other.mFrontFaceStencilFailOp &&
			mFrontFaceStencilDepthFailOp == other.mFrontFaceStencilDepthFailOp &&
			mFrontFaceStencilPassOp == other.mFrontFaceStencilPassOp &&
			mFrontFaceStencilFunc == other.mFrontFaceStencilFunc &&
			mBackFaceStencilFailOp == other.mBackFaceStencilFailOp &&
			mBackFaceStencilDepthFailOp == other.mBackFaceStencilDepthFailOp &&
			mBackFaceStencilPassOp == other.mBackFaceStencilPassOp &&
			mBackFaceStencilFunc == other.mBackFaceStencilFunc;
	}

	bool GFXRasterizerStateDesc::operator==(const GFXRasterizerStateDesc& other) const
	{
		return mWireframe == other.mWireframe &&
			mCullMode == other.mCullMode &&
			mFrontCounterClockWise == other.mFrontCounterClockWise &&
			mDepthBias == other.mDepthBias &&
			mDepthBiasClamp == other.mDepthBiasClamp &&
			mSlopeScaledDepthBias == other.mSlopeScaledDepthBias &&
			mDepthClipEnable == other.mDepthClipEnable &&
			mScissorEnable == other.mScissorEnable &&
			mMultisampleEnable == mMultisampleEnable &&
			mAntialiasedLineEnable == mAntialiasedLineEnable;
	}

	bool GFXSamplerStateDesc::operator==(const GFXSamplerStateDesc& other) const
	{
		return mFilter == other.mFilter &&
			mAddressU == other.mAddressU &&
			mAddressV == other.mAddressV &&
			mAddressW == other.mAddressW &&
			mMipLODBias == other.mMipLODBias &&
			mMaxAnisotropy == other.mMaxAnisotropy &&
			mComparisonFunc == other.mComparisonFunc &&
			mBorderColor == other.mBorderColor &&
			mMinLOD == other.mMinLOD &&
			mMaxLOD == other.mMaxLOD;
	}

	bool GFXBlendStateDesc::operator==(const GFXBlendStateDesc& other) const
	{
		if (mAlphaToCoverage != other.mAlphaToCoverage || mIndependentBlend != other.mIndependentBlend)
			return false;

		for (unsigned i = 0; i < GFX_MAX_RENDER_TARGET; i++)
		{
			if (mRenderTargets[i].mBlendEnable != other.mRenderTargets[i].mBlendEnable ||
				mRenderTargets[i].mSrcBlend != other.mRenderTargets[i].mSrcBlend ||
				mRenderTargets[i].mDestBlend != other.mRenderTargets[i].mDestBlend ||
				mRenderTargets[i].mBlendOp != other.mRenderTargets[i].mBlendOp ||
				mRenderTargets[i].mSrcBlendAlpha != other.mRenderTargets[i].mSrcBlendAlpha ||
				mRenderTargets[i].mDestBlendAlpha != other.mRenderTargets[i].mDestBlendAlpha ||
				mRenderTargets[i].mBlendOpAlpha != other.mRenderTargets[i].mBlendOpAlpha ||
				mRenderTargets[i].mWriteMask != other.mRenderTargets[i].mWriteMask)
			{
				return false;
			}
		}

		return true;
	}

	bool GFXInputLayoutDesc::operator==(const GFXInputLayoutDesc& other) const
	{
		if (mVertexShader != other.mVertexShader) return false;
		if (GetNumElement() != other.GetNumElement()) return false;

		unsigned numElement = GetNumElement();
		for (unsigned i = 0; i < numElement; i++)
		{
			UASSERT(mElements[i].mName && other.mElements[i].mName);

			if ((strcmp(mElements[i].mName, other.mElements[i].mName) != 0) ||
				mElements[i].mFormat != other.mElements[i].mFormat ||
				mElements[i].mOffset != other.mElements[i].mOffset ||
				mElements[i].mPerInstance != other.mElements[i].mPerInstance ||
				mElements[i].mSlot != other.mElements[i].mSlot);
			return false;

		}
	}

	GFXResource::GFXResource(void* nativeHandle, Name debugName) : mNativeHandle(nativeHandle), mDebugName(debugName)
	{
		gGFXResources.Add(this);
	}

	GFXResource::~GFXResource()
	{
		gGFXResources.RemoveSwap(this);
	}

};