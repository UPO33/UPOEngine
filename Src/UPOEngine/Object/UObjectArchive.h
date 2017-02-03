#pragma once

#include "../Core/UBasic.h"
#include "../Core/UArray.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class Object;



	//////////////////////////////////////////////////////////////////////////
	class UAPI ObjectArchive
	{
	public:
		static void Save(TArray<Object*>& inObjects, Stream* stream);
		static void Load(TArray<Object*>& outObjects, Stream* stream, Object* additionalRefForAssets = nullptr);
	};
};