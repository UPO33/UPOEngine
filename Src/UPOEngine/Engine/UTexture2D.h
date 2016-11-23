#pragma once

#include "UAsset.h"

#include "../GFXCore/UGFXDevice.h"


namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	struct Texture2DSamplerInfo
	{
		UCLASS(Texture2DSamplerInfo, void)

		ETextureAddress		mUAddress = ETextureAddress::EClamp;
		ETextureAddress		mVAddress = ETextureAddress::EClamp;
		ETextureFilter		mFilter = ETextureFilter::ETF_MIN_POINT_MAG_MIP_LINEAR;
		Color				mBorderColor = Color(1,1,1,1);
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI ATexture2D : public Asset
	{
		friend UPOEd::AssetConverter;

		UCLASS(ATexture2D, Asset)

	private:
		Buffer		mContent;
		unsigned	mWidth;
		unsigned	mHeight;
		Texture2DSamplerInfo mSampler;
	};
};