#include "UPhysMaterial.h"
#include "../Meta/UMeta.h"

namespace UPO
{


	void APhysMaterial::CreatePxMaterial()
	{

	}




	UCLASS_BEGIN_IMPL(APhysMaterial, UATTR_Instanceable())
		UPROPERTY(mStaticFriction)
		UPROPERTY(mDynamicFriction)
		UPROPERTY(mRestitution)
		UPROPERTY(mFrictionCombineMode)
		UPROPERTY(mRestitutionCombineMode)
	UCLASS_END_IMPL(APhysMaterial)
};
