#include "UMaterial.h"
#include "UPhysMaterial.h"
#include "../Meta/UMeta.h"

#include "UTexture2D.h"

namespace UPO
{
	UCLASS_BEGIN_IMPL(AMaterial, UATTR_Instanceable())
		UPROPERTY(mPhysMaterial)
	UCLASS_END_IMPL(AMaterial)

	
		
		
	UCLASS_BEGIN_IMPL(AMaterialTest, UATTR_Instanceable())
		UPROPERTY(mDiffuseTexture)
		UPROPERTY(mDiffuseColor)
	UCLASS_END_IMPL(AMaterialTest)

	AMaterialTest::AMaterialTest()
	{
		mDiffuseTexture = nullptr;
		mDiffuseColor = Color::WHITE;
	}

	AMaterialTest::~AMaterialTest()
	{

	}

	void AMaterialTest::OnCreate()
	{

	}

	void AMaterialTest::OnDestroy()
	{

	}

	void AMaterialTest::MetaAfterPropertyChange(const PropertyInfo*)
	{
		
	}

};