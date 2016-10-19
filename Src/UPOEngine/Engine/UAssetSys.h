#pragma once

#include "../Core/UBasic.h"
#include "../Core/UName.h"

#include "UAsset.h"

// #define UASSET_FILE_HEADER	"UPOASSET"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class Asset;
	class Object;
	class World;
	class AssetSys;

	//////////////////////////////////////////////////////////////////////////
	class UAPI AssetEntry
	{
		friend AssetSys;

		Name				mName;
		AssetID				mID;
		Name				mClassName;
		TObjectPtr<Asset>	mInstance; //null if not loaded
		Stream*				mStream = nullptr;

	public:
		Name GetName() const { return mName; }
		Name GetClassName() const { return mClassName; }
		AssetID GetID() const { return mID; }
		Asset* GetInstance() const { return mInstance.Get(); }
		Stream* OpenStreamForLoading();
		Stream* OpenStreamForSaving();
		void CloseStream();
		bool LoadNow(World* owner);
	};


	struct AssetFileHeader
	{
		char	mTag[32];
	};
	//////////////////////////////////////////////////////////////////////////
	class UAPI AssetSys
	{
		TArray<AssetEntry*>	mAssets;
		
	public:
		static AssetSys* Get();

		AssetSys();
		~AssetSys();

		//create a asset with default properties 
		AssetEntry* CreateAsset(const ClassInfo* assetClass, Name assetName);
		Asset* LoadAsset(Name nanme, World* owner);
		Asset* FindLoadedAsset(Name assetName);
		Asset* FindLoadedAsset(AssetID id);
		AssetEntry* FindAsset(Name name);
		AssetEntry* FindAsset(AssetID id);

		void CollectAssetsInContent();
		//return true if a file is asset
		bool CheckFile(const String& filename, AssetID* outAssetID = nullptr, Name* outClassName = nullptr) const;

		void AddEntryToList(AssetEntry* newEntry)
		{
			mAssets.Add(newEntry);
		}
		const TArray<AssetEntry*>& GetAssets() const { return mAssets; }
		bool DeleteAsset(AssetEntry* asset);
	};


};