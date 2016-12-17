#include "UTexture2D.h"

#include "../Meta/UMeta.h"

namespace UPO
{


	void ATexture2D::OnInit()
	{
		EnqueueRenderCommend([this]() {
			mEditingLock.Enter();
			ATexture2DRS* rs = new ATexture2DRS;
			rs->mSampler = nullptr; //create sampler
			rs->mTexture = nullptr; // load texture from memory
			mRS = rs;
			mEditingLock.Leave();
		});
	}
	void ATexture2D::OnRelease()
	{
		EnqueueRenderCommend([this]() {
			if (mRS) delete mRS;
		});
	}

	void ATexture2D::MetaPropertyChanged(PropertyInfo* pi)
	{
		if (pi->GetName() == "mSample")
		{
			EnqueueRenderCommend([this]()
			{
				mEditingLock.Enter();
				delete mRS->mSampler;
				GFXSamplerState_Desc desc;
				GFXSamplerState* newSampler = gGFX->CreateSamplerState(desc);
				mEditingLock.Leave();
			});
		}
	}

		UCLASS_BEGIN_IMPL(Texture2DSamplerInfo)
			UPROPERTY(mUAddress)
			UPROPERTY(mVAddress)
			UPROPERTY(mFilter)
			UPROPERTY(mBorderColor)
			UCLASS_END_IMPL(Texture2DSamplerInfo)


			UCLASS_BEGIN_IMPL(ATexture2D)
			UPROPERTY(mContent, UATTR_Hidden())
			UPROPERTY(mSampler)
		UCLASS_END_IMPL(ATexture2D)

};