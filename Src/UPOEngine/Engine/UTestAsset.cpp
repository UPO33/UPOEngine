#include "UTestAsset.h"
#include "../Meta/UMeta.h"

namespace UPO
{
	ATest::ATest()
	{
		mFloat = 1;
	}

	ATest::~ATest()
	{
		ULOG_MESSAGE("");
	}

	




	UCLASS_BEGIN_IMPL(ATest, UATTR_Instanceable(), UATTR_Icon("AssetBrowserAsset.png"))
		UPROPERTY(mVec3)
		UPROPERTY(mVec2)
		UPROPERTY(mFloat)
		UPROPERTY(mString)
	UCLASS_END_IMPL(ATest)
};