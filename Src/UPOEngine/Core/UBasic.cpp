#include "UBasic.h"
#include "../Meta/UMeta.h"

namespace UPO
{
	UCLASS_BEGIN_IMPL(Flag)
		UPROPERTY(mFlag, UATTR_Hidden())
	UCLASS_END_IMPL(Flag)

	void AppCrash()
	{
		abort();
	}

};