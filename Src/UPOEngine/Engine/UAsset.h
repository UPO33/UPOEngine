#pragma once

#include "../Core/UCore.h"
#include "../Object/UObject.h"
#include "../Core/UCommandQueue.h"
#include "../Core/UBound.h"

#include "UEngineBase.h"

namespace UPOEd
{
	class AssetConverter;
};

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
		EAF_Alive = 1,
		EAF_Dirty = 1 << 1,
		EAF_ReceiveTick = 1 << 2,
		EAF_RecieveFrame = 1 << 3,

		EAF_Defaul = EAF_Alive,
	};
	//////////////////////////////////////////////////////////////////////////
	class UAPI Asset : public Object
	{
		UCLASS(Asset, Object)

		friend AssetSys;
		friend AssetEntry;
		friend UPOEd::AssetConverter;

	private:
		Flag				mAssetFlag = EAssetFlag::EAF_Defaul;
		AssetEntry*			mEntry = nullptr;
		TArray<ObjectPtr>	mRefs;
		Name				mTag;
		unsigned			mPrivateIndex;
		AssetSys*			mAssetSys;

	public:
		void*				mEdData = nullptr;
		void*				mUserData = nullptr;


	private:
		bool NeedsRelease();

	public:
		bool FlagTest(unsigned flag) const { return mAssetFlag.Test(flag); }
		bool FlagTestAndClear(unsigned flag) { return mAssetFlag.TestAndClear(flag); }
		void FlagSet(unsigned flag) { mAssetFlag.Set(flag); }
		void FlagClear(unsigned flag) { mAssetFlag.Clear(flag); }

		Name GetName() const;
		AssetID GetID() const;



		bool Save();
		//save asset to
		bool SaveTo(Stream&);

		bool IsDirty() const { return FlagTest(EAssetFlag::EAF_Dirty); }
		bool IsAlive() const { return FlagTest(EAssetFlag::EAF_Alive); }

		void MarkDirty() { FlagSet(EAssetFlag::EAF_Dirty); }
		


		void AddRef(Object*);
		void RemoveRef(Object*);


		virtual void OnCreate() {};
		virtual void OnDestroy() {};


		virtual void OnTick(float delta) {}
		virtual void OnFrame(float delta) {}

		void MetaBeforePropertyChange(const PropertyInfo*);
		void MetaAfterPropertyChange(const PropertyInfo*);
	};


};