#include "UEntityLight.h"
#include "UEntityLightVisualizer.h"
#include "UWorldRS.h"

#include "../Meta/UMeta.h"


namespace UPO
{
	UCLASS_BEGIN_IMPL(EntityLight, UATTR_Instanceable(), UATTR_Icon("PointLight.png"))
		UPROPERTY(mActive)
		UPROPERTY(mCastShadow)
		UPROPERTY(mRadius, UATTR_Range(0, 100000))
		UPROPERTY(mColor)
		UPROPERTY(mIsSpot)
		UPROPERTY(mSpotInnerAngle, UATTR_Range(1, 180))
		UPROPERTY(mSpotOuterAngle, UATTR_Range(1, 180))
	UCLASS_END_IMPL(EntityLight)

	
		
	EntityLight::EntityLight()
	{
#ifndef UENDUSER
		mVisualizer = new EntityLightVisualizer(this);
#endif
	}

	EntityLight::~EntityLight()
	{

	}

	EntityLightRS::EntityLightRS(EntityLight* gs, WorldRS* wrs) : EntityRS(gs, wrs)
	{
		mPrivateIndex = mOwner->mLights.Add(this);
		OnFetch(EEF_RenderDataTransformDirty | EEF_RenderDataMiscDirty);
	}

	EntityLightRS::~EntityLightRS()
	{
		mOwner->mLights.LastElement()->mPrivateIndex = mPrivateIndex;
		mOwner->mLights.RemoveAtSwap(mPrivateIndex);
	}

	void EntityLightRS::OnFetch(unsigned flag)
	{
		EntityRS::OnFetch(flag);

		EntityLight* gs = (EntityLight*)mGS;
		
		if (flag & EEF_RenderDataTransformDirty)
		{
			mWorldPosition = gs->GetWorldPosition();
		}
		if (flag & EEF_RenderDataMiscDirty)
		{
			mRadius = gs->mRadius;
			mColor = gs->mColor;

			mRSFlag.Clear(ERS_CastShadow | ERS_Visible);

			if (gs->mCastShadow) mRSFlag.Set(ERS_CastShadow);
			if (gs->mActive) mRSFlag.Set(ERS_Visible);
		}
	}

}