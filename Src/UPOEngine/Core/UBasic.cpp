#include "UBasic.h"
#include "../Meta/UMeta.h"

#ifdef UPLATFORM_WIN
#include <windows.h>
#endif

UAPI void UOutputDebugString(const char* str)
{
#ifdef UCOMPILER_MSVC
	::OutputDebugStringA(str);
#endif
}


namespace UPO
{
	void AppCrash()
	{
		abort();
	}



	UCLASS_BEGIN_IMPL(Flag)
	UPROPERTY(mFlag, UATTR_Hidden())
	UCLASS_END_IMPL(Flag)


};

