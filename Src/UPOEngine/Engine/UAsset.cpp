#include "UAsset.h"
#include "../Meta/UMeta.h"
#include "UAssetSys.h"


namespace UPO
{
	UCLASS_BEGIN_IMPL(Asset)
		UPROPERTY(mAssetFlag)
		UPROPERTY(mTags)
	UCLASS_END_IMPL(Asset)

	
		


	UCLASS_BEGIN_IMPL(AssetID)
	UCLASS_END_IMPL(AssetID)


	//////////////////////////////////////////////////////////////////////////
	void Asset::PostLoad()
	{
		OnInit();
		OnInitRS();
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
	void Asset::Save()
	{
		if (AssetSys::Get()->SaveAsset(this))
			FlagClear(EAssetFlag::EAF_Dirty);
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