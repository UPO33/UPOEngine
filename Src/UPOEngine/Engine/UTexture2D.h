#pragma once

#include "UAsset.h"

#include "../GFXCore/UGFXDevice.h"


namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class ATexture2D;
	class ATexture2DRS;
	

	//////////////////////////////////////////////////////////////////////////
	struct Texture2DSamplerInfo
	{
		UCLASS(Texture2DSamplerInfo, void)

		ETextureAddress		mUAddress = ETextureAddress::EClamp;
		ETextureAddress		mVAddress = ETextureAddress::EClamp;
		ETextureFilter		mFilter = ETextureFilter::EMinMagMipLinear;
		Color				mBorderColor = Color(1,1,1,1);
		float				mMaxAnisotropy = 16;
		float				mMipLODBias = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI ATexture2DRS
	{
	public:
		class GFXSamplerState*	mSampler = nullptr;
		class GFXTexture2D*		mTexture = nullptr;
		class ATexture2D*		mGS = nullptr;

		
		void ReCreateSampler();
		void RecreatreTexture();

		
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI ATexture2D : public Asset
	{
		friend UPOEd::AssetConverter;

		UCLASS(ATexture2D, Asset)

	private:
		Buffer		mContent;
		Texture2DSamplerInfo mSampler;
		ATexture2DRS*	mRS;


		virtual void OnCreate() override;
		virtual void OnDestroy() override;

	public:
		ATexture2DRS* GetRS() const { return mRS; }

		void MetaBeforePropertyChange(const PropertyInfo*);
		void MetaAfterPropertyChange(const PropertyInfo*);
	};
};