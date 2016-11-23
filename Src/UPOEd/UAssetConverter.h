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
		static bool Convert_Texture(Buffer& fileContent, String name, AssetEntry* folderToStoreAssetIn);
		static bool Convert_SMesh(Buffer& fileContent, String name, AssetEntry* folderToStoreAssetIn);
	};
};