#include "UTestAsset.h"
#include "../Meta/UMeta.h"

namespace UPO
{
	ATest::ATest()
	{
		mFloat = 1;
		ULOG_MESSAGE("%p", this);
	}

	ATest::~ATest()
	{
		ULOG_MESSAGE("");
	}

	void ATest::OnInit()
	{
		ULOG_MESSAGE("");
	}

	void ATest::OnInitRS()
	{
		ULOG_MESSAGE("");
	}

	void ATest::OnRelease()
	{
		ULOG_MESSAGE("");
	}

	void ATest::OnReleaseRS()
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