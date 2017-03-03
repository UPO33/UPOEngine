#include "UAssetConverter.h"

#include "../UPOEngine/Engine/USound.h"
#include "../UPOEngine/Engine/UStaticMesh.h"
#include "../UPOEngine/Engine/UTexture2D.h"

namespace UPOEd
{
	String gStaticMeshExts[] = { "obj", "fbx", "3ds", };
	String gTexture2DExts[] = { "png", "bmp", "jpg", "jpeg", "dds", "tiff", "tga" };
	String gSoundExts[] = { "wav", "ogg" };

	bool UExtIsTexture2D(const String& ext) 
	{
		for (auto item : gTexture2DExts)
			if (item.Equal(ext, false)) return true;
		return false;
	}
	bool UExtIsStaticMesh(const String& ext)
	{
		for (auto item : gStaticMeshExts)
			if (item.Equal(ext, false)) return true;
		return false;
	}
	bool UExtIsSound(const String& ext)
	{
		for (auto item : gSoundExts)
			if (item.Equal(ext, false)) return true;
		return false;
	}
	bool AssetConverter::ExtIsSupported(const String& ext)
	{
		return UExtIsTexture2D(ext) || UExtIsStaticMesh(ext) || UExtIsSound(ext);
	}




	bool AssetConverter::Convert(const String& fullfilename, class AssetEntry* folderToStoreAssetIn)
	{
		String ext = PathGetExt(fullfilename);

		if (!ExtIsSupported(ext))
		{
			ULOG_ERROR("unsupported asset file");
			return false;
		}

		Buffer fileContent;
		if (!File::OpenReadFull(fullfilename, fileContent))
		{
			ULOG_ERROR("reading file filed");
			return false;
		}
		String name = PathGetFileName(fullfilename, false);
		//get a unique name for the new asset
		{
			bool validName = true;
			int iSuffix = 1;
			do
			{
				validName = true;
				for (auto item : folderToStoreAssetIn->mChildren)
				{
					if (item->mFilename == name) validName = false;
				}
				if (!validName)
					name += String::MakeFrom(iSuffix);

			} while (!validName);
		}

		if (UExtIsTexture2D(ext)) return Convert<ATexture2D>(fileContent, name, folderToStoreAssetIn);
		if (UExtIsStaticMesh(ext)) return Convert<AStaticMesh>(fileContent, name, folderToStoreAssetIn);
		if (UExtIsSound(ext)) return Convert<ASound>(fileContent, name, folderToStoreAssetIn);
	}
	/*
	bool AssetConverter::Convert_Texture(Buffer& fileContent, String name, AssetEntry* folderToStoreAssetIn)
	{
		AssetEntry* entry = folderToStoreAssetIn->CreateChild(name, false, AssetID::GetNewID(), ATexture2D::GetClassInfoStatic()->GetName());
		ATexture2D* asset = NewObject<ATexture2D>();
		entry->mInstance = asset;
		asset->mEntry = entry;

		{
			asset->mContent = fileContent;

			if (asset->Save())
			{
				DeleteObject(asset);
				return true;
			}
			else
			{
				DeleteObject(asset);
				return false;
			}
		}
	}

	bool AssetConverter::Convert_SMesh(Buffer& fileContent, String name, AssetEntry* folderToStoreAssetIn)
	{
		AssetEntry* entry = folderToStoreAssetIn->CreateChild(name, false, AssetID::GetNewID(), AStaticMesh::GetClassInfoStatic()->GetName());
		AStaticMesh* asset = NewObject<AStaticMesh>();
		entry->mInstance = asset;
		asset->mEntry = entry;

		{
			asset->mContent = fileContent;

			if (asset->Save())
			{
				DeleteObject(asset);
				return true;
			}
			else
			{
				DeleteObject(asset);
				return false;
			}
		}
	}
*/


};