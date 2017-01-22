#include "UPhysMaterial.h"
#include "../Meta/UMeta.h"

namespace UPO
{


	void APhysMaterial::CreatePxMaterial()
	{

	}




	UCLASS_BEGIN_IMPL(APhysMaterial, UATTR_Instanceable(), UATTR_Icon("PhysMaterial.png"))
		UPROPERTY(mStaticFriction)
		UPROPERTY(mDynamicFriction)
		UPROPERTY(mRestitution)
		UPROPERTY(mFrictionCombineMode)
		UPROPERTY(mRestitutionCombineMode)
	UCLASS_END_IMPL(APhysMaterial)


		UENUM(EPhysMaterialCombine, false, nullptr,
			EPhysMaterialCombine::EAverage,
			EPhysMaterialCombine::EMin,
			EPhysMaterialCombine::EMultiply,
			EPhysMaterialCombine::EMax);
};
