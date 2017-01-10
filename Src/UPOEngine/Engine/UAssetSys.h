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

	/************************************************************************
	an AssetEntry actually keeps the information about an asst file in Engine's directory/Assets/ or Project's directory/Assets/
	************************************************************************/
	class UAPI AssetEntry
	{
		friend AssetSys;
	public:
		Name				mAssetName;
		AssetID				mID;
		Name				mClassName;
		TObjectPtr<Asset>	mInstance; //null if not loaded
		Stream*				mStream = nullptr;

		AssetEntry*				mParent = nullptr;
		TArray<AssetEntry*>		mChildren;
		String					mFilename;		// MyAsset.asset
		String					mFullFilename;	// C:\MyProject\Assets\MyAsset.asset
		bool					mIsFolder;

	public:
		Name GetName() const { return mAssetName; }
		Name GetClassName() const { return mClassName; }
		ClassInfo* GetClassInfo() const;
		AssetID GetID() const { return mID; }
		//get an instance of this asset , returns null if not loaded
		Asset* GetInstance() const { return mInstance.Get(); }
		Stream* OpenStreamForLoading();
		Stream* OpenStreamForSaving();
		void CloseStream();
		bool LoadNow(Object* ref);
		String GetFileName() const { return mFilename; }
		String GetFullFileName() const { return mFullFilename; }
		bool IsFolder() const { return mIsFolder; }

		bool Rename(const String& newName);
		AssetEntry* CreateChild(const String& name, bool isFolder, AssetID id = AssetID(), Name className = Name());

	private:
		void Renamed();
	};


	/************************************************************************
	manages loading, creating and releasing assets
	************************************************************************/
	class UAPI AssetSys
	{
		friend AssetEntry;

		TArray<AssetEntry*>	mAssetEntries;
		AssetEntry* mEngineFolder = nullptr;
		AssetEntry* mProjectFolder = nullptr;

		TArray<Asset*>	mLoadedAssets;
		TArray<Asset*>	mPendingKillAssets;

		TArray<Asset*>	mTickEnableAssets;
		TArray<Asset*>	mFrameEnableAssets;

		unsigned			mReleaseCheckIndex = 0;
		unsigned			mTickSinceLastKill = 0;

	public:
		static AssetSys* Get();

		AssetSys();
		~AssetSys();

		//create a asset with default properties 
		AssetEntry* CreateAsset(const ClassInfo* assetClass, Name assetName);
		AssetEntry* CreateDefaulAssetFile(AssetEntry* folderToCreateIn, const String& assetFilename, ClassInfo* assetClass);

		bool ReadFileFull(const String& filename, Buffer& outFileContent);

		Asset* LoadAsset(Name nanme, Object* ref);
		Asset* FindLoadedAsset(Name assetName);
		Asset* FindLoadedAsset(AssetID id);
		AssetEntry* FindAsset(Name name);
		AssetEntry* FindAsset(AssetID id);

		//collect assets from directories
		void CollectAssetEntries();
		void CollectAssetEntries(AssetEntry* parent);

		AssetEntry* EngineEntry() const { return mEngineFolder; }
		AssetEntry* ProjectEntry() const { return mProjectFolder; }
		
		String GetEngineAssetsPath();
		String GetProjectAssetsPath();
		//return true if a file is asset
		bool CheckFile(const String& filename, AssetID* outAssetID = nullptr, Name* outClassName = nullptr) const;

		void AddEntryToList(AssetEntry* newEntry)
		{
			mAssetEntries.Add(newEntry);
		}
		const TArray<AssetEntry*>& GetAssets() const { return mAssetEntries; }
		bool DeleteAsset(AssetEntry* asset);
		void SaveAllDirtyAssets();

		static bool ReadAssetHeader(Stream& assetFile, AssetID* outID = nullptr, Name* outClassName = nullptr);
		static bool WriteAssetHeader(Stream& assetFile, AssetID id, Name className);

		void Tick(float delta);
		void Frame(float delta);

// 		bool ForceUnloadAsset(Asset* asset);
	};
	inline AssetSys* GAssetSys() { return AssetSys::Get(); }

};