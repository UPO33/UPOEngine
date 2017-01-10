#include "UTexture2D.h"

#include "../Meta/UMeta.h"

namespace UPO
{
	UCLASS_BEGIN_IMPL(Texture2DSamplerInfo)
		UPROPERTY(mUAddress)
		UPROPERTY(mVAddress)
		UPROPERTY(mFilter)
		UPROPERTY(mBorderColor)
		UPROPERTY(mMaxAnisotropy, UATTR_Range(0, 16))
		UPROPERTY(mMipLODBias, UATTR_Range(0, 128))
	UCLASS_END_IMPL(Texture2DSamplerInfo)


	UCLASS_BEGIN_IMPL(ATexture2D)
		UPROPERTY(mContent, UATTR_Hidden())
		UPROPERTY(mSampler)
	UCLASS_END_IMPL(ATexture2D)

	//////////////////////////////////////////////////////////////////////////
	GFXSamplerStateDesc ToGFX(const Texture2DSamplerInfo& in)
	{
		GFXSamplerStateDesc desc;
		desc.mAddressU = in.mUAddress;
		desc.mAddressV = in.mVAddress;
		desc.mBorderColor = in.mBorderColor;
		desc.mFilter = in.mFilter;
		desc.mMaxAnisotropy = in.mMaxAnisotropy;
		desc.mMipLODBias = in.mMipLODBias;
		return desc;
	}

	void ATexture2D::OnCreate()
	{
		EnqueueRenderCommandAndWait([this]()
		{
			ATexture2DRS* rs = new ATexture2DRS;
			
			rs->mSampler = gGFX->CreateSamplerState(ToGFX(this->mSampler));
			if(!rs->mSampler) 
				ULOG_ERROR("failed to create sampler for asset [%]", this->GetName());

			rs->mTexture = gGFX->CreateTexture2DFromMemory(mContent);
			if(!rs->mTexture)
				ULOG_ERROR("failed to create texture for asset [%]", this->GetName());

			mRS = rs;
		});
	}
	
	void ATexture2D::MetaBeforePropertyChange(const PropertyInfo* prp)
	{

	}

	void ATexture2D::MetaAfterPropertyChange(const PropertyInfo* prp)
	{
		if (UPROPERTY_NAME_EQUAL(prp, mSampler))
		{
			EnqueueRenderCommandAndWait([this]()
			{
				if (mRS->mSampler) delete mRS->mSampler;
				mRS->mSampler = gGFX->CreateSamplerState(ToGFX(this->mSampler));

			});
		}
	}

	void ATexture2D::OnDestroy()
	{
		EnqueueRenderCommandAndWait([this]() {
			if (!mRS)return;

			if (mRS->mSampler) delete mRS->mSampler;
			if (mRS->mTexture) delete mRS->mTexture;
		});
	}

	void ATexture2DRS::ReCreateSampler()
	{

	}

	void ATexture2DRS::RecreatreTexture()
	{

	}

};