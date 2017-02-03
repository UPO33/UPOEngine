#include "UObject.h"
#include "UObjectPtr.h"

#include "../Engine/UAsset.h"
#include "../Engine/UEntity.h"

#include "../Meta/UMeta.h"

namespace UPO
{
	UCLASS_BEGIN_IMPL(Object)
	UCLASS_END_IMPL(Object)

	//////////////////////////////////////////////////////////////////////////
	Object::~Object()
	{
		mClassInfo = nullptr;
		if (mRefData)
		{
			mRefData->mObject = nullptr;
			mRefData->Dec();
			mRefData = nullptr;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	ObjectRefData* Object::GetRefData()
	{
		if (mRefData == nullptr)
		{
			mRefData = ObjectRefData::GetNew();
			mRefData->mObject = this;
			mRefData->mRefCount = 1;
		}
		return mRefData;
	}



	bool Object::IsBaseOf(const ClassInfo* baseClass) const
	{
		return mClassInfo->IsBaseOf(baseClass);
	}
};