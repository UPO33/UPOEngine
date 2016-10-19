#include "UWorldSetting.h"
#include "../Meta/UMeta.h"

namespace UPO
{

	UCLASS_BEGIN_IMPL(AWorldSetting, UATTR_Instanceable())
		UPROPERTY(mSunDirection)
		UPROPERTY(mSunLight)
	UCLASS_END_IMPL(AWorldSetting)
};