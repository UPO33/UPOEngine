#pragma once

#include "UCommonEd.h"

namespace UPOEd
{
	//////////////////////////////////////////////////////////////////////////
	class AssetConverter
	{
	public:
		static bool ExtIsSupported(const String& ext);
		static bool Convert(const String& fullfilename, AssetEntry* folderToStoreAssetIn);
	private:
// 		static bool Convert_Texture(Buffer& fileContent, String name, AssetEntry* folderToStoreAssetIn);
// 		static bool Convert_SMesh(Buffer& fileContent, String name, AssetEntry* folderToStoreAssetIn);
// 		static bool Convert_Sound(Buffer& fileContent, String name, AssetEntry* folderToStoreAssetIn);

		template<typename TAssetClass> static bool Convert(Buffer& fileContent, String name, AssetEntry* folderToStoreAssetIn)
		{
			AssetEntry* entry = folderToStoreAssetIn->CreateChild(name, false, AssetID::GetNewID(), TAssetClass::GetClassInfoStatic()->GetName());
			TAssetClass* asset = NewObject<TAssetClass>();
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
	};
};