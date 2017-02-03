#include "UObjectPtr.h"
#include "UObject.h"
#include "../Core/UFreeListAllocator.h"

namespace UPO
{
#if 0
	//////////////////////////////////////////////////////////////////////////
	struct ObjectPtrDataPack
	{
		static const int NUM_INSTANCE_IN_PACK = 8192;

		ObjectRefData			mInstances[NUM_INSTANCE_IN_PACK];
		ObjectRefData*			mFirstFree;
		ObjectPtrDataPack*		mNextPack;

		ObjectPtrDataPack()
		{
			mNextPack = nullptr;
			mFirstFree = mInstances + 0;
			for (int i = 0; i < NUM_INSTANCE_IN_PACK - 1; i++)
			{
				mInstances[i].mObject = nullptr;
				mInstances[i].mRefCount = 0;
				mInstances[i].mNextFree = mInstances + (i + 1);
			}

			mInstances[NUM_INSTANCE_IN_PACK - 1].mObject = nullptr;
			mInstances[NUM_INSTANCE_IN_PACK - 1].mRefCount = 0;
			mInstances[NUM_INSTANCE_IN_PACK - 1].mNextFree = nullptr;

		}
	};

	/*
	only grows and allocates
	*/
	struct ObjectPtrContext
	{
		ObjectRefData*			mFirstFreeInstance = nullptr;
		ObjectPtrDataPack*		mPackHead = nullptr;

		~ObjectPtrContext()
		{
			ShutDown();
		}
		ObjectRefData* GetFreeInstance()
		{
			if (mFirstFreeInstance)
			{
				ObjectRefData* ret = mFirstFreeInstance;
				mFirstFreeInstance = mFirstFreeInstance->mNextFree;
				return ret;
			}
			else
			{
				ObjectPtrDataPack* newPack = new ObjectPtrDataPack;
				newPack->mNextPack = mPackHead;
				mPackHead = newPack;
				ObjectRefData* ret = newPack->mFirstFree;
				mFirstFreeInstance = ret->mNextFree;
				return ret;
			}
		}
		void FreeInstance(ObjectRefData* instance)
		{
			instance->mNextFree = mFirstFreeInstance;
			mFirstFreeInstance = instance;
		}
		void ShutDown()
		{
			ObjectPtrDataPack* iter = mPackHead;
			while (iter)
			{
				auto tmp = iter;
				iter = iter->mNextPack;
				delete iter;
			}
			mPackHead = nullptr;
			mFirstFreeInstance = nullptr;
		}
	};

	ObjectPtrContext gObjectPtrContext;
#endif


	//////////////////////////////////////////////////////////////////////////
	ObjectRefData ObjectRefData::NullRef = { ~0U, nullptr };

	FreeListAllocator gObjPtrAllocator{ sizeof(ObjectRefData) + 8, 256 };


	void ObjectRefData::Dec()
	{
		mRefCount--;
		if (mRefCount == 0)
		{
			if (this == &NullRef) return;

			gObjPtrAllocator.Free(this);
		}
	}



	//////////////////////////////////////////////////////////////////////////
	ObjectRefData* ObjectRefData::GetNew()
	{
		return new (gObjPtrAllocator.Alloc()) ObjectRefData;

// 		ObjectRefData* ret = gObjectPtrContext.GetFreeInstance();
// 		ret->mObject = owner;
// 		ret->mRefCount = 0;
// 		return ret;
	}
	//////////////////////////////////////////////////////////////////////////
	void ObjectRefData::Free(ObjectRefData* ptrData)
	{
		gObjPtrAllocator.Free(ptrData);

// 		gObjectPtrContext.FreeInstance(ptrData);
	}

};