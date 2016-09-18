#include "UAsset.h"
#include "../Meta/UMeta.h"

namespace UPO
{
	UCLASS_BEGIN_IMPL(Asset)
		UPROPERTY(mName)
		UPROPERTY(mAssetFlag)
		UPROPERTY(mTags)
	UCLASS_END_IMPL(Asset)


	//////////////////////////////////////////////////////////////////////////
	Asset* Asset::Load(const char* name, World* owner)
	{
		return nullptr;
	}

};