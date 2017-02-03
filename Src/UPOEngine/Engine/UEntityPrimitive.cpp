#include "UEntityPrimitive.h"
#include "../Meta/UMeta.h"

namespace UPO
{


	
	UCLASS_BEGIN_IMPL(EntityPrimitive)
	UPROPERTY(mVisible)
	UPROPERTY(mCastShadow)
	UPROPERTY(mReceiveShadow)
	UPROPERTY(mMainPassEnable)
	UPROPERTY(mCutomDepthStencilEnable)
	UPROPERTY(mCustomStencilValue)
	UCLASS_END_IMPL(EntityPrimitive);






	void EntityPrimitiveRS::Fetch()
	{
		mRSFlag.Clear(ERS_Visible | ERS_CastShadow | ERS_RecieveShadow | ERS_MainPassEnable | ERS_CustomDepthEnable | ERS_Selected);

		UASSERT(mGS);
		EntityPrimitive* gs = (EntityPrimitive*)mGS;


		if (gs->mVisible) mRSFlag.Set(ERS_Visible);
		if (gs->mCastShadow) mRSFlag.Set(ERS_CastShadow);
		if (gs->mReceiveShadow) mRSFlag.Set(ERS_RecieveShadow);
		if (gs->mMainPassEnable) mRSFlag.Set(ERS_MainPassEnable);
		if (gs->mCutomDepthStencilEnable) mRSFlag.Set(ERS_CustomDepthEnable);
		if (gs->mSelected) mRSFlag.Set(ERS_Selected);

		mCustomStencilValue = gs->mCustomStencilValue;
	}

};