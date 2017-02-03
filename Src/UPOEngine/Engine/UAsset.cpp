#include "UAsset.h"
#include "../Meta/UMeta.h"
#include "UAssetSys.h"


namespace UPO
{
	UCLASS_BEGIN_IMPL(Asset)
		UPROPERTY(mTag)
	UCLASS_END_IMPL(Asset)

	
		


	UCLASS_BEGIN_IMPL(AssetID)
	UCLASS_END_IMPL(AssetID)



	bool Asset::NeedsRelease()
	{
		for (size_t i = 0; i < mRefs.Length(); i++)
			if (mRefs[i]) return false;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	Name Asset::GetName() const
	{
		return mEntry->GetName();
	}
	//////////////////////////////////////////////////////////////////////////
	AssetID Asset::GetID() const
	{
		return mEntry->GetID();
	}
	//////////////////////////////////////////////////////////////////////////
	bool Asset::Save()
	{
		FlagClear(EAssetFlag::EAF_Dirty);

		if (Stream* stream = mEntry->OpenStreamForSaving())
		{
			TArray<Object*>	objectsToSave;
			objectsToSave.Add(this);
			ObjectArchive::Save(objectsToSave, stream);

			mEntry->CloseStream();
			ULOG_SUCCESS("asset [%] saved", GetName());
			return true;
		}
		
		ULOG_ERROR("failed to open stream for saving asset [%]", GetName());
		return false;
	}

	bool Asset::SaveTo(Stream& stream)
	{
		AssetSys::WriteAssetHeader(stream, mEntry->mID, mEntry->mClassName);
		TArray<Object*> objectsToSave;
		objectsToSave.Add(this);
		ObjectArchive::Save(objectsToSave, &stream);
		return true;
	}



	void Asset::AddRef(Object* obj)
	{
		if (!IsAlive()) return;

		ObjectPtr objPtr = obj;
		if (objPtr)
		{
			mRefs.AddUnique(objPtr);
		}
	}

	void Asset::RemoveRef(Object* obj)
	{
		if (!IsAlive()) return;

		ObjectPtr objPtr = obj;
		if (objPtr)
		{
			mRefs.RemoveSwap(objPtr);
		}
	}

	void Asset::MetaBeforePropertyChange(const PropertyInfo* prp)
	{

	}

	void Asset::MetaAfterPropertyChange(const PropertyInfo*)
	{
		MarkDirty();
	}

	//////////////////////////////////////////////////////////////////////////
	AssetID AssetID::GetNewID()
	{
		static unsigned PreviousTick = 0;
		
		unsigned time = GetSecondsSince2000();
		unsigned tick = AppTickCount();

		if (tick == PreviousTick) Thread::Sleep(3);

		PreviousTick = tick;
		
		return AssetID(time, tick);
	}
	//////////////////////////////////////////////////////////////////////////
	void AssetID::MetaSerialize(Stream& stream)
	{
		stream.RW(mID0).RW(mID1);
	}



};