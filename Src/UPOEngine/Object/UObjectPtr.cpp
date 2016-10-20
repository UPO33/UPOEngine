#include "UObjectPtr.h"
#include "UObject.h"

namespace UPO
{

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

	//////////////////////////////////////////////////////////////////////////
	ObjectRefData ObjectRefData::NullRef = { nullptr, 0xFFff };

	//////////////////////////////////////////////////////////////////////////
	ObjectRefData* ObjectRefData::GetNew(Object* owner)
	{
		ObjectRefData* ret = gObjectPtrContext.GetFreeInstance();
		ret->mObject = owner;
		ret->mRefCount = 0;
		return ret;
	}
	//////////////////////////////////////////////////////////////////////////
	void ObjectRefData::Free(ObjectRefData* ptrData)
	{
		gObjectPtrContext.FreeInstance(ptrData);
	}

};