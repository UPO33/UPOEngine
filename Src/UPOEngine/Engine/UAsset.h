#pragma once

#include "../Core/UCore.h"
#include "../Object/UObject.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class World;
	class Entity;
	class AssetSys;
	class AssetEntry;

	//////////////////////////////////////////////////////////////////////////
	class UAPI AssetID
	{
		UCLASS(AssetID, void)

		unsigned mID0;
		unsigned mID1;

	public:
		AssetID() : mID0(0), mID1(0) {}
		AssetID(unsigned i, unsigned j) : mID0(i), mID1(j) {}

		bool IsNull() const { return mID0 == 0 && mID1 == 0; }
		
		bool operator == (const AssetID& other) const { return mID0 == other.mID0 && mID1 == other.mID1; }

		static AssetID GetNewID();

		void MetaSerialize(Stream&);
	};
	//////////////////////////////////////////////////////////////////////////
	enum EAssetFlag
	{
		EAF_Dirty,
		EAF_Defaul,
	};
	//////////////////////////////////////////////////////////////////////////
	class UAPI Asset : public Object
	{
		UCLASS(Asset, Object)

		friend AssetSys;
		friend AssetEntry;

	private:
		Flag				mAssetFlag = EAssetFlag::EAF_Defaul;
		AssetEntry*			mEntry = nullptr;
		TArray<World*>		mWorldOwners;	//the worlds that r using this asset
		Name				mTag;
		

		void PostLoad();

	public:
		bool FlagTest(unsigned flag) const { return mAssetFlag.Test(flag); }
		bool FlagTestAndClear(unsigned flag) { return mAssetFlag.TestAndClear(flag); }
		void FlagSet(unsigned flag) { mAssetFlag.Set(flag); }
		void FlagClear(unsigned flag) { mAssetFlag.Clear(flag); }

		Name GetName() const;
		AssetID GetID() const;



		bool Save();
		bool IsDirty() const { return FlagTest(EAssetFlag::EAF_Dirty); }
		
		void IsOwnedBy(World*) {}
		void AddOwner(World*) {}
		void RemoveOwner(World*) {}
		
		virtual void OnConstruct() {};

		virtual void OnInit() {};
		virtual void OnInitRS() {};
		virtual void OnRelease() {};
		virtual void OnReleaseRS() {};
	};


};