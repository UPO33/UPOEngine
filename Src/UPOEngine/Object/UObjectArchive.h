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
		static void Save(Object* inObject, Stream* stream)
		{
			TArray<Object*> objs;
			objs.Add(inObject);
			Save(objs, stream);
		}
		static Object* Load(Stream* stream, Object* additionalRefForAssets = nullptr)
		{
			TArray<Object*> objs;
			Load(objs, stream, additionalRefForAssets);
			UASSERT(objs.Length() <= 1);
			if (objs.Length())
				return objs[0];
			else
				return nullptr;
		}
		static void Save(TArray<Object*>& inObjects, Stream* stream);
		static void Load(TArray<Object*>& outObjects, Stream* stream, Object* additionalRefForAssets = nullptr);
	};
};