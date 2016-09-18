#include "UObject.h"
#include "UObjectPtr.h"

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
			ObjectRefData::Free(mRefData);
			mRefData = nullptr;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	ObjectRefData* Object::GetRefData()
	{
		if (mRefData == nullptr)
		{
			mRefData = ObjectRefData::GetNew(this);
		}
		return mRefData;
	}


	void Object::Serialize(Stream&)
	{

	}

};