#include "UEntity.h"
#include "../Meta/UMeta.h"

namespace UPO
{
	UCLASS_BEGIN_IMPL(Entity)
	UCLASS_END_IMPL(Entity)

	//////////////////////////////////////////////////////////////////////////
	void Entity::Destroy()
	{
		if (FlagTestAnClear(EEF_Alive))
		{
			if (FlagTest(EEF_BeginPlayWasCalled))
				OnEndPlay();

			OnDestroy();

			
		}
	}

};