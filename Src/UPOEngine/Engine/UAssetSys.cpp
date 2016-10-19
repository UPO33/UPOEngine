#include "UAssetSys.h"
#include "../Meta/UMeta.h"
#include "../Object/UObject.h"

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
		out.SetFormatted("../Content/%s", assetName.CStr());
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

	AssetSys::AssetSys()
	{
		mAssets.SetCapacity(1024);
		CollectAssetsInContent();
	}

	AssetSys::~AssetSys()
	{

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
			newEntry->mName = assetName;
			newEntry->mClassName = assetClass->GetName();
			newEntry->mID = AssetID::GetNewID();
			newEntry->mInstance = newAsset;

			AddEntryToList(newEntry);

			newAsset->mEntry = newEntry;

			newAsset->OnConstruct();
			
			newAsset->FlagSet(EAssetFlag::EAF_Dirty);
			newAsset->Save();

			DeleteObject(newAsset);

			newEntry->mInstance = nullptr;

			return newEntry;
		}
		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	Asset* AssetSys::LoadAsset(Name nanme, World* owner)
	{
		if (AssetEntry* assetItem = FindAsset(nanme))
		{
			assetItem->LoadNow(owner);
			return assetItem->GetInstance();
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
		for (size_t i = 0; i < mAssets.Length(); i++)
		{
			if (mAssets[i]->GetName() == name) return mAssets[i];
		}
		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	AssetEntry* AssetSys::FindAsset(AssetID id)
	{
		for (size_t i = 0; i < mAssets.Length(); i++)
		{
			if (mAssets[i]->GetID() == id) return mAssets[i];
		}
		return nullptr;
	}
	


	//////////////////////////////////////////////////////////////////////////
	void AssetSys::CollectAssetsInContent()
	{
		mAssets.RemoveAll();

		TArray<String> assetNamesString;
		PathGetFiles("../Content/", assetNamesString, true);
		for (size_t i = 0; i < assetNamesString.Length(); i++)
		{
			String assetFileName;
			AssetNameToRelativePath(Name(assetNamesString[i]), assetFileName);

			AssetID assetID;
			Name assetClassName;

			if (CheckFile(assetFileName, &assetID, &assetClassName))
			{
				AssetEntry* newAsset = new AssetEntry;
				newAsset->mID = assetID;
				newAsset->mClassName = assetClassName;
				newAsset->mName = assetNamesString[i];

				mAssets.Add(newAsset);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	bool AssetSys::CheckFile(const String& filename, AssetID* outAssetID, Name* outClassName) const
	{
		if (filename.IsEmpty()) return false;

		auto fStream = StreamWriterFile(filename);
		
		return ReadAssetHeader(fStream, outAssetID, outClassName);
	}
	
	//////////////////////////////////////////////////////////////////////////
	Stream* AssetEntry::OpenStreamForLoading()
	{
		//currently is reading or writing
		if (mStream) return nullptr;

		String assetPath;
		AssetNameToRelativePath(GetName().CStr(), assetPath);
		mStream = new StreamWriterFile(assetPath);

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

		String assetPath;
		AssetNameToRelativePath(mName.CStr(), assetPath);
		mStream = new StreamReaderFile(assetPath);

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
	bool AssetEntry::LoadNow(World* owner)
	{
		if (Asset* asset = GetInstance()) // is loaded?
		{
			asset->AddOwner(owner);
			return true;
		}

		if (const ClassInfo* assetClass = MetaSys::Get()->FindClass(mClassName))
		{
			ULOG_MESSAGE("start loading asset [%s] ...", mName.CStr());

			Object* assetObject = ObjectArchive::Load(OpenStreamForLoading());
			CloseStream();
			if (assetObject)
			{
				Asset* asset = assetObject->Cast<Asset>();
				UASSERT(asset);
				
				asset->AddOwner(owner);
				asset->mEntry = this;

				mInstance = asset;
			
				asset->PostLoad();

				ULOG_SUCCESS("asset [%s] loaded", GetName());
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
			ULOG_ERROR("failed to load asset [%s], class [%s] is unknown", mName.CStr(), mClassName.CStr());
			return false;
		}

		return false;
	}

};