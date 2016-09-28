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
	void AssetNameToRelativePath(const String& assetName, String& out)
	{
		char buffer[256];
		sprintf_s(buffer, "../Content/%s", assetName.CStr());
		out = assetName;
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
	}

	AssetSys::~AssetSys()
	{

	}
	//////////////////////////////////////////////////////////////////////////
	AssetEntry* AssetSys::CreateAsset(const ClassInfo* assetClass, Name assetName)
	{
		if (assetClass)
		{
			if (!assetClass->IsBaseOf(Asset::GetClassInfoStatic())) return nullptr;

			if (FindAsset(assetName)) return nullptr;

			Asset* newAsset = (Asset*)NewObject(assetClass);

			AssetEntry* newEntry = new AssetEntry;
			newEntry->mName = assetName;
			newEntry->mClassName = assetClass->GetName();
			newEntry->mID = AssetID::GetNewID();
			newEntry->mInstance = newAsset;

			mAssets.Add(newEntry);

			newAsset->mEntry = newEntry;
			newAsset->FlagSet(EAssetFlag::EAF_Dirty);
			newAsset->Save();

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
	bool AssetSys::SaveAsset(Asset* asset)
	{
		UASSERT(asset);

		if (Stream* stream = asset->mEntry->OpenStreamForSaving())
		{
			WriteAssetHeader(*stream, asset->GetID(), asset->GetClassInfo()->GetName());
			ObjectArchive::Save(asset, stream);
			ULOG_SUCCESS("asset [%s] saved", asset->GetName().CStr());
			return true;
		}
		ULOG_ERROR("failed to save asset [%s]", asset->GetName().CStr());
		return false;
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
			AssetNameToRelativePath(assetNamesString[i], assetFileName);

			AssetID assetID;
			Name assetClassName;

			if (CheckFile(assetFileName, &assetID, &assetClassName))
			{
				AssetEntry newAsset;
				newAsset.mID = assetID;
				newAsset.mClassName = assetClassName;
				newAsset.mName = assetNamesString[i];

				mAssets.Add(newAsset);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	bool AssetSys::CheckFile(const String& filename, AssetID* outAssetID, Name* outClassName) const
	{
		if (filename.IsEmpty()) return false;

		auto fStream = StreamWriterFile(filename);
		
		ReadAssetHeader(fStream, outAssetID, outClassName);

		return false;
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

			TArray<Object*> objectsLoaded;
			ObjectArchive::Load(objectsLoaded, OpenStreamForLoading());
			CloseStream();
			if (objectsLoaded.Length() == 1)
			{
				Asset* loadedAsset = objectsLoaded[0]->Cast<Asset>();
				UASSERT(loadedAsset);

				loadedAsset->AddOwner(owner);
				loadedAsset->mEntry = this;

				mInstance = loadedAsset;
				
				loadedAsset->PostLoad();
				ULOG_SUCCESS("asset loaded");
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