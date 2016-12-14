#include "UAsset.h"
#include "../Meta/UMeta.h"
#include "UAssetSys.h"


namespace UPO
{
	UCLASS_BEGIN_IMPL(Asset)
		UPROPERTY(mAssetFlag, UATTR_Hidden())
		UPROPERTY(mTag)
	UCLASS_END_IMPL(Asset)

	
		


	UCLASS_BEGIN_IMPL(AssetID)
	UCLASS_END_IMPL(AssetID)


	//////////////////////////////////////////////////////////////////////////
	void Asset::PostLoad()
	{
		OnInit();
		OnInitRS();
	}

	void Asset::Release()
	{
		if (mAssetFlag.TestAndClear(EAF_Alive))
		{

		}
	}
	void OnContruction()
	{
		Enqueue([]()
		{

		});
	}
	void OnDestruction()
	{

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
			ObjectArchive::Save(this, stream);
			mEntry->CloseStream();
			ULOG_SUCCESS("asset [%s] saved", GetName().CStr());
			return true;
		}
		
		ULOG_ERROR("failed to open stream for saving asset [%s]", GetName().CStr());
		return false;
	}

	bool Asset::SaveTo(Stream& stream)
	{
		AssetSys::WriteAssetHeader(stream, mEntry->mID, mEntry->mClassName);
		ObjectArchive::Save(this, &stream);
		return true;
	}

	void Asset::MetaPropertyChanged(PropertyInfo*)
	{
	}

	void Asset::AddRef(Object* obj)
	{
		ObjectPtr objPtr = obj;
		if (objPtr)
		{
			mRefs.AddUnique(objPtr);
		}
	}

	void Asset::RemoveRef(Object* obj)
	{
		ObjectPtr objPtr = obj;
		if (objPtr)
		{
			mRefs.RemoveSwap(objPtr);
		}


		//check remove
		for (size_t i = 0; i < mRefs.Length(); i++)
		{
		}

		mRefs.RemoveIf([](ObjectPtr& pobj) { 
			return pobj.Get() == nullptr;
		});

		if (mRefs.Length() == 0) // there is no ref
		{

		}
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