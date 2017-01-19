#include "UAssetConverter.h"

namespace UPOEd
{
	String gStaticMeshExts[] = { "obj", "fbx", "3ds", };
	String gTexture2DExts[] = { "png", "bmp", "jpg", "jpeg", "dds", "tiff", "tga" };


	bool UExtIsTexture2D(const String& ext) 
	{
		for (auto item : gTexture2DExts)
			if (ext == item) return true;
		return false;
	}
	bool UExtIsStaticMesh(const String& ext)
	{
		for (auto item : gStaticMeshExts)
			if (ext == item) return true;
		return false;
	}

	bool AssetConverter::ExtIsSupported(const String& ext)
	{
		return UExtIsTexture2D(ext) || UExtIsStaticMesh(ext);
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

		if (UExtIsTexture2D(ext)) return Convert_Texture(fileContent, name, folderToStoreAssetIn);
		if (UExtIsStaticMesh(ext)) return Convert_SMesh(fileContent, name, folderToStoreAssetIn);
	}

	bool AssetConverter::Convert_Texture(Buffer& fileContent, String name, AssetEntry* folderToStoreAssetIn)
	{
		AssetEntry* entry = folderToStoreAssetIn->CreateChild(name, false, AssetID::GetNewID(), ATexture2D::GetClassInfoStatic()->GetName());
		ATexture2D* assetTexture2D = NewObject<ATexture2D>();
		entry->mInstance = assetTexture2D;
		assetTexture2D->mEntry = entry;

		{
			assetTexture2D->mContent = fileContent;

			if (assetTexture2D->Save())
			{
				DeleteObject(assetTexture2D);
				return true;
			}
			else
			{
				DeleteObject(assetTexture2D);
				return false;
			}
		}
	}

	bool AssetConverter::Convert_SMesh(Buffer& fileContent, String name, AssetEntry* folderToStoreAssetIn)
	{
		AssetEntry* entry = folderToStoreAssetIn->CreateChild(name, false, AssetID::GetNewID(), AStaticMesh::GetClassInfoStatic()->GetName());
		AStaticMesh* assetStaticMesh = NewObject<AStaticMesh>();
		entry->mInstance = assetStaticMesh;
		assetStaticMesh->mEntry = entry;

		{
			assetStaticMesh->mContent = fileContent;

			if (assetStaticMesh->Save())
			{
				DeleteObject(assetStaticMesh);
				return true;
			}
			else
			{
				DeleteObject(assetStaticMesh);
				return false;
			}
		}
	}

};