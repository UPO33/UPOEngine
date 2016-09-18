#pragma once

#include "../Core/UCore.h"
#include "../Object/UObject.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class World;
	class Entity;
	class AssetSys;

	//////////////////////////////////////////////////////////////////////////
	struct AssetID
	{
		unsigned	mID0, mID1;
		AssetID()
		{
			
		}
		~AssetID()
		{

		}
	};
	//////////////////////////////////////////////////////////////////////////
	enum EAssetFlag
	{
		EAF_Dirty,
		EAF_Defaul,
	};
	//////////////////////////////////////////////////////////////////////////
	class Asset : public Object
	{
		UCLASS(Asset, Object)

		friend AssetSys;

	private:
		Name				mName;
		TArray<World*>		mWorldOwners;	//the worlds that r using this asset
		Flag				mAssetFlag;
		AssetID				mCreationID;
		TArray<Name>		mTags;


	public:
		virtual void OnInit() {};
		virtual void OnInitRS() {};
		virtual void OnRelease() {};
		virtual void OnReleaseRS() {};



		static Asset* Load(const char* name, World* owner);
	};


};