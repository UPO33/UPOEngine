#include "UAssetSys.h"
#include "../Meta/UMeta.h"
#include "../Object/UObject.h"
#include "../Misc/UApp.h"

namespace UPO
{
	static const char UASSET_FILE_HEADER[] = "#UPOASSET#";
	
	/*
		asset files structure
		UASSET_FILE_HEADER, AssetID assetID, Name className, ObjectArchive....
	
	*/

	//////////////////////////////////////////////////////////////////////////
	//relative
	void AssetNameToRelativePath(Name assetName, String& out)
	{
		out.SetFormatted("..//Content//%s", assetName.CStr());
	}

	//////////////////////////////////////////////////////////////////////////
	bool ReadAssetHeader(Stream& assetFile, AssetID* outID = nullptr, Name* outClassName = nullptr)
	{
		UASSERT(!assetFile.IsReader());

		char header[64];
		size_t len = UARRAYLEN(UASSET_FILE_HEADER);
		assetFile.Bytes(header, len);

		if (assetFile.HasError()) return false;

		if (MemEqual(header, UASSET_FILE_HEADER, len))
		{
			AssetID assetID;
			Name className;

			assetID.MetaSerialize(assetFile);
			className.MetaSerialize(assetFile);

			if (outID) *outID = assetID;
			if (outClassName) *outClassName = className;

			return !assetFile.HasError();
		}
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	bool WriteAssetHeader(Stream& assetFile, AssetID id, Name className)
	{
		UASSERT(assetFile.IsReader());

		size_t len = UARRAYLEN(UASSET_FILE_HEADER);
		assetFile.Bytes((void*)UASSET_FILE_HEADER, len);

		id.MetaSerialize(assetFile);
		className.MetaSerialize(assetFile);
		return !assetFile.HasError();
	}
	//////////////////////////////////////////////////////////////////////////
	AssetSys* AssetSys::Get()
	{
		static TInstance<AssetSys> Ins;
		return Ins;
	}
	//////////////////////////////////////////////////////////////////////////
	AssetSys::AssetSys()
	{
		mAssetEntries.SetCapacity(1024);
		CollectAssetEntries();
	}
	//////////////////////////////////////////////////////////////////////////
	void DeleteAssetEntry(AssetEntry* entry)
	{
		for (size_t i = 0; i < entry->mChildren.Length(); i++)
		{
			DeleteAssetEntry(entry->mChildren[i]);
		}
		delete entry;
	}
	//////////////////////////////////////////////////////////////////////////
	AssetSys::~AssetSys()
	{
		if (mEngineFolder) DeleteAssetEntry(mEngineFolder);
		if (mProjectFolder) DeleteAssetEntry(mProjectFolder);

		mEngineFolder = mProjectFolder = nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	AssetEntry* AssetSys::CreateAsset(const ClassInfo* assetClass, Name assetName)
	{
		if (assetClass)
		{
			if (!assetClass->IsBaseOf(Asset::GetClassInfoStatic()))
			{
				ULOG_MESSAGE("Invalid asset class");
				return nullptr;
			}

			if (FindAsset(assetName))
			{
				ULOG_MESSAGE("There is a asset with the same name");
				return nullptr;
			}

			Asset* newAsset = NewObject<Asset>(assetClass);

			AssetEntry* newEntry = new AssetEntry;
			newEntry->mAssetName = assetName;
			newEntry->mClassName = assetClass->GetName();
			newEntry->mID = AssetID::GetNewID();
			newEntry->mInstance = newAsset;

			AddEntryToList(newEntry);

			newAsset->mEntry = newEntry;

			//newAsset->OnConstruct();
			
			newAsset->FlagSet(EAssetFlag::EAF_Dirty);
			if (newAsset->Save())
			{
				DeleteObject(newAsset);
				newEntry->mInstance = nullptr;
				return newEntry;
			}
			else
			{
				DeleteObject(newAsset);
				delete newEntry;
				return nullptr;
			}
		}
		return nullptr;
	}

	AssetEntry* AssetSys::CreateDefaulAssetFile(AssetEntry* folderToCreateIn, const String& assetFilename, ClassInfo* assetClass)
	{
		String strAssetName = String(folderToCreateIn->mAssetName) + '/' + assetFilename;
		Name nameAssetName = strAssetName;
		if (FindAsset(nameAssetName))
		{
			ULOG_ERROR("failed to create asset file [%], currently exist", strAssetName);
			return nullptr;
		}
		AssetEntry* newAssetEntry = new AssetEntry;
		newAssetEntry->mIsFolder = false;
		newAssetEntry->mAssetName = strAssetName;
		newAssetEntry->mID = AssetID::GetNewID();
		newAssetEntry->mClassName = assetClass->GetName();
		newAssetEntry->mFilename = assetFilename;
		newAssetEntry->mFullFilename = folderToCreateIn->mFullFilename + '/' + assetFilename;

		Asset* newAssetInstance = NewObject<Asset>(assetClass);
		newAssetEntry->mInstance = newAssetInstance;
		newAssetInstance->mEntry = newAssetEntry;

		newAssetInstance->MarkDirty();
		bool bSaved = newAssetInstance->Save();

		newAssetEntry->mInstance = nullptr;
		DeleteObject(newAssetInstance);
		newAssetInstance = nullptr;

		if (bSaved)
		{
			folderToCreateIn->mChildren.Add(newAssetEntry);
			AddEntryToList(newAssetEntry);

			return newAssetEntry;
		}
		else
		{
			delete newAssetEntry;
			newAssetEntry = nullptr;
			return nullptr;
		}
	}

	bool AssetSys::ReadFileFull(const String& filename, Buffer& outFileContent)
	{
		outFileContent = Buffer();

		if (filename.IsEmpty()) return false;

		String fullFileName = GetEngineAssetsPath() + PATH_SEPARATOR_CHAR + filename;

		if (File::OpenReadFull(fullFileName, outFileContent))
			return true;

		if (GetProjectAssetsPath().IsEmpty())	//has not project?
			return false;	

		fullFileName = GetProjectAssetsPath() + PATH_SEPARATOR_CHAR + filename;
		if (File::OpenReadFull(fullFileName, outFileContent))
			return true;
		
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	Asset* AssetSys::LoadAsset(Name name, Object* ref)
	{
		if (AssetEntry* assetItem = FindAsset(name))
		{
			assetItem->LoadNow(ref);
			return assetItem->GetInstance();
		}
		else
		{
			ULOG_ERROR("asset [%] not found", name);
		}
		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	Asset* AssetSys::LoadAsset(AssetID id, Object* ref)
	{
		if (AssetEntry* assetItem = FindAsset(id))
		{
			assetItem->LoadNow(ref);
			return assetItem->GetInstance();
		}
		else
		{
			ULOG_ERROR("asset not found");
		}
		return nullptr;
	}

	//////////////////////////////////////////////////////////////////////////
	Asset* AssetSys::FindLoadedAsset(Name assetName)
	{
		if (auto find = FindAsset(assetName))  return find->GetInstance();
		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	Asset* AssetSys::FindLoadedAsset(AssetID assetID)
	{
		if (auto find = FindAsset(assetID))  return find->GetInstance();
		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	AssetEntry* AssetSys::FindAsset(Name name)
	{
		for (size_t i = 0; i < mAssetEntries.Length(); i++)
		{
			if (mAssetEntries[i]->GetName() == name) return mAssetEntries[i];
		}
		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	AssetEntry* AssetSys::FindAsset(AssetID id)
	{
		for (size_t i = 0; i < mAssetEntries.Length(); i++)
		{
			if (mAssetEntries[i]->GetID() == id) return mAssetEntries[i];
		}
		return nullptr;
	}
	

// 	struct FileInfo
// 	{
// 		String mName;
// 		bool mIsDirectory;
// 	};
// 	void PathGetEntries(const String& str, TArray<FileInfo>& out)
// 	{
// 	}

	void AssetSys::CollectAssetEntries(AssetEntry* parent)
	{
		{
			TArray<String> files;
			PathGetFiles(parent->mFullFilename, files);
			for (size_t i = 0; i < files.Length(); i++)
			{
				String fullFileName = parent->mFullFilename + '/' + files[i];
				AssetID assetID;
				Name className;
				if (CheckFile(fullFileName, &assetID, &className))
				{
					AssetEntry* newEntry = new AssetEntry;
					newEntry->mIsFolder = false;
					newEntry->mParent = parent;
					newEntry->mClassName = className;
					newEntry->mID = assetID;
					newEntry->mFilename = files[i];
					newEntry->mFullFilename = fullFileName;
					newEntry->mAssetName = parent->mAssetName.IsEmpty() ? files[i] : (String(parent->mAssetName) + '/' + files[i]);

					parent->mChildren.Add(newEntry);

					mAssetEntries.Add(newEntry);

				}
			}
		}

		{
			TArray<String> dirs;
			PathGetFolders(parent->mFullFilename, dirs);
			for (size_t i = 0; i < dirs.Length(); i++)
			{
				AssetEntry* entryDir = new AssetEntry;
				parent->mChildren.Add(entryDir);

				entryDir->mIsFolder = true;
				entryDir->mParent = parent;
				entryDir->mFilename = dirs[i];
				entryDir->mFullFilename = parent->mFullFilename + '/' + dirs[i];
				entryDir->mAssetName = parent->mAssetName.IsEmpty() ? dirs[i] : (String(parent->mAssetName) + '/' + dirs[i]);

				

				mAssetEntries.Add(entryDir);

				CollectAssetEntries(entryDir);
			}
		}
	}


	//////////////////////////////////////////////////////////////////////////
	void AssetSys::CollectAssetEntries()
	{
#if 0
		mAssets.RemoveAll();

		auto collectAssets = [&](const String& path){
			TArray<String> assetNamesString;
			PathGetFiles(path, assetNamesString, true);
			for (size_t i = 0; i < assetNamesString.Length(); i++)
			{
				String assetFileName = path + assetNamesString[i];
			

				AssetID assetID;
				Name assetClassName;

				if (CheckFile(assetFileName, &assetID, &assetClassName))
				{
					AssetEntry* newAsset = new AssetEntry;
					newAsset->mID = assetID;
					newAsset->mClassName = assetClassName;
					newAsset->mAssetName = assetNamesString[i];
					//newAsset->mAbsFilename = assetFileName;

					AddEntryToList(newAsset);
				}
			}
		};

		collectAssets(GetEngineContentPath());
		if(String projContent = GetProjectContentPath())
			collectAssets(projContent);
#endif // 0

		mEngineFolder = new AssetEntry;
		mEngineFolder->mIsFolder = nullptr;
		mEngineFolder->mFullFilename = GetEngineAssetsPath();
		
		mProjectFolder = new AssetEntry;
		mProjectFolder->mIsFolder = true;
		mProjectFolder->mFullFilename = GetProjectAssetsPath();

		CollectAssetEntries(mEngineFolder);
		CollectAssetEntries(mProjectFolder);

		//log asset entries
		{
			
		}
// 		TArray<String> assetNamesString;
// 		String engineContentPath = GetEngineContentPath();
// 		PathGetFiles(engineContentPath, assetNamesString, true);
// 		for (size_t i = 0; i < assetNamesString.Length(); i++)
// 		{
// 			String assetFileName = engineContentPath + assetNamesString[i];
// 			//AssetNameToRelativePath(Name(assetNamesString[i]), assetFileName);
// 
// 			AssetID assetID;
// 			Name assetClassName;
// 
// 			if (CheckFile(assetFileName, &assetID, &assetClassName))
// 			{
// 				ULOG_MESSAGE("assest found [%s]", assetFileName);
// 				AssetEntry* newAsset = new AssetEntry;
// 				newAsset->mID = assetID;
// 				newAsset->mClassName = assetClassName;
// 				newAsset->mName = assetNamesString[i];
// 
// 				mAssets.Add(newAsset);
// 			}
// 		}
	}
	//get absolute engine's assets directory
	String AssetSys::GetEngineAssetsPath()
	{
		UASSERT(GApp()->mEngineAssetsPath);
		return  GApp()->mEngineAssetsPath;
	}
	//get absolute project's assets directory, returns empty if no project
	String AssetSys::GetProjectAssetsPath()
	{
		return  GApp()->mProjectAssetsPath;
	}

	//////////////////////////////////////////////////////////////////////////
	bool AssetSys::CheckFile(const String& filename, AssetID* outAssetID, Name* outClassName) const
	{
		if (filename.IsEmpty()) return false;

		auto fStream = StreamWriterFile(filename);
		
		return ReadAssetHeader(fStream, outAssetID, outClassName);
	}
	//////////////////////////////////////////////////////////////////////////
	void AssetSys::SaveAllDirtyAssets()
	{
		for (AssetEntry* item : mAssetEntries)
		{
			if (Asset* instance = item->GetInstance())
			{
				if (instance->IsDirty()) instance->Save();
			}
		}
	}

	bool AssetSys::ReadAssetHeader(Stream& assetFile, AssetID* outID /*= nullptr*/, Name* outClassName /*= nullptr*/)
	{
		UASSERT(!assetFile.IsReader());

		char header[64];
		size_t len = UARRAYLEN(UASSET_FILE_HEADER);
		assetFile.Bytes(header, len);

		if (assetFile.HasError()) return false;

		if (MemEqual(header, UASSET_FILE_HEADER, len))
		{
			AssetID assetID;
			Name className;

			assetID.MetaSerialize(assetFile);
			className.MetaSerialize(assetFile);

			if (outID) *outID = assetID;
			if (outClassName) *outClassName = className;

			return !assetFile.HasError();
		}
		return false;
	}

	bool AssetSys::WriteAssetHeader(Stream& assetFile, AssetID id, Name className)
	{
		UASSERT(assetFile.IsReader());

		size_t len = UARRAYLEN(UASSET_FILE_HEADER);
		assetFile.Bytes((void*)UASSET_FILE_HEADER, len);

		id.MetaSerialize(assetFile);
		className.MetaSerialize(assetFile);
		return !assetFile.HasError();
	}


	void AssetSys::Tick(float delta)
	{
		///////////////////cheeking release
		{
			static const unsigned NUM_RELEASECHECK = 8;

			unsigned i = mReleaseCheckIndex;
			unsigned num = mReleaseCheckIndex + NUM_RELEASECHECK;
			mReleaseCheckIndex += NUM_RELEASECHECK;
			for (; i < num; i++)
			{
				if (mLoadedAssets.IsIndexValid(i))
				{
					Asset* asset = mLoadedAssets[i];
					if (asset->IsAlive() && asset->NeedsRelease())
					{
						mTickSinceLastKill = 0;
						mPendingKillAssets.Add(asset);
						ULOG_MESSAGE("destroying asset [%]", asset->GetName());
						asset->FlagClear(EAF_Alive);
						asset->OnDestroy();
					}
				}
				else
				{
					mReleaseCheckIndex = 0;
					break;
				}
			}
		}

		/////////////////////kill released entities
		static const unsigned NUM_TICK_REQUIRED_TO_KILL = 60;

		if (mTickSinceLastKill > NUM_TICK_REQUIRED_TO_KILL);
		{
			mTickSinceLastKill = 0;

			for (Asset* pendingKill : mPendingKillAssets)
			{
				mLoadedAssets.LastElement()->mPrivateIndex = pendingKill->mPrivateIndex;
				mLoadedAssets.RemoveAtSwap(pendingKill->mPrivateIndex);
			}

			for (Asset* pendingKill : mPendingKillAssets)
			{
				DeleteObject(pendingKill);
			}

			mPendingKillAssets.RemoveAll();
		}
		mTickSinceLastKill++;
	}

	void AssetSys::Frame(float delta)
	{

	}

// 	bool AssetSys::ForceUnloadAsset(Asset* asset)
// 	{
// 		if (asset->FlagTestAndClear(EAF_Alive))
// 		{
// 			mLoadedAssets.LastElement()->mPrivateIndex = asset->mPrivateIndex;
// 			mLoadedAssets.RemoveAtSwap(asset->mPrivateIndex);
// 
// 			asset->mRefs.RemoveAll();
// 			asset->OnRelease();
// 		}
// 	}

	ClassInfo* AssetEntry::GetClassInfo() const
	{
		return MetaSys::Get()->FindClass(mClassName);
	}

	//////////////////////////////////////////////////////////////////////////
	Stream* AssetEntry::OpenStreamForLoading()
	{
		//currently is reading or writing
		if (mStream) return nullptr;

		mStream = new StreamWriterFile(mFullFilename);

		if (ReadAssetHeader(*mStream))
			return mStream;
		
		delete mStream;
		mStream = nullptr;
		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	Stream* AssetEntry::OpenStreamForSaving()
	{
		if (mStream) return nullptr;


		mStream = new StreamReaderFile(mFullFilename);

		if (WriteAssetHeader(*mStream, mID, mClassName))
			return mStream;

		delete mStream;
		mStream = nullptr;
		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	void AssetEntry::CloseStream()
	{
		if (mStream)
		{
			delete mStream;
			mStream = nullptr;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool AssetEntry::LoadNow(Object* ref)
	{
		if (Asset* asset = GetInstance()) // is loaded?
		{
			asset->AddRef(ref);
			return true;
		}

		if (const ClassInfo* assetClass = GMetaSys()->FindClass(mClassName))
		{
			ULOG_MESSAGE("start loading asset [%] ...", mAssetName);

			TArray<Object*> loadedObjects;
			ObjectArchive::Load(loadedObjects, OpenStreamForLoading());

			CloseStream();

			Asset* loadedAsset = nullptr;
			for (Object* obj : loadedObjects)
			{
				if (obj && obj->Cast<Asset>())
				{
					

					UASSERT(loadedAsset == nullptr);
					loadedAsset = obj->Cast<Asset>();
				}
			}

			if (loadedAsset)
			{
				loadedAsset->AddRef(ref);
				loadedAsset->mEntry = this;
				loadedAsset->mPrivateIndex = GAssetSys()->mLoadedAssets.Add(loadedAsset);

				mInstance = loadedAsset;
			
				ULOG_SUCCESS("asset [%] loaded", GetName());

				loadedAsset->OnCreate();

				
				return true;
			}
			else
			{
				ULOG_ERROR("failed to load asset");
				return false;
			}
		}
		else
		{
			ULOG_ERROR("failed to load asset [%], class [%] is unknown", mAssetName, mClassName);
			return false;
		}

		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	bool AssetEntry::Rename(const String& newName)
	{
		if (newName == mFilename) return true;

		String newFullFilename = mParent->mFullFilename + '/' + newName;

		if (File::Rename(mFullFilename, newFullFilename))
		{
			mFilename = newName;
			Renamed();
			return true;
		}
		else
		{
			ULOG_ERROR("failed to rename asset [%]", mFullFilename);
			return false;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void AssetEntry::Renamed()
	{
		mAssetName = String(mParent->mAssetName) + '/' + mFilename;
		mFullFilename = mParent->mFullFilename + '/' + mFilename;

		for (auto child : mChildren)
			child->Renamed();
	}

	AssetEntry* AssetEntry::CreateChild(const String& name, bool isFolder, AssetID id /*= AssetID()*/, Name className /*= Name()*/)
	{
		AssetEntry* newEntry = new AssetEntry;
		newEntry->mFilename = name;
		newEntry->mAssetName = String(mAssetName) + '/' + name;
		newEntry->mFullFilename = mFullFilename + '/' + name;
		newEntry->mID = id;
		newEntry->mClassName = className;
		newEntry->mIsFolder = isFolder;
		newEntry->mParent = this;
		mChildren.Add(newEntry);
		GAssetSys()->AddEntryToList(newEntry);
		return newEntry;
	}

};