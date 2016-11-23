#include "UTexture2D.h"

#include "../Meta/UMeta.h"

namespace UPO
{
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