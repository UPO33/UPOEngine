#pragma once

#include "../Core/UCore.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class Object;
	class Asset;
	class Entity;
	class ClassInfo;
	class PropertyInfo;
	

	//////////////////////////////////////////////////////////////////////////
	class ObjectArchive
	{
	public:
		static void Save(Object* inObject, Stream* stream)
		{
			TArray<Object*> objs;
			objs.Add(inObject);
			Save(objs, stream);
		}
		static Object* Load(Stream* stream)
		{
			TArray<Object*> objs;
			Load(objs, stream);
			UASSERT(objs.Length() <= 1);
			if (objs.Length())
				return objs[0];
			else
				return nullptr;
		}
		static void Save(TArray<Object*>& inObjects, Stream* stream);
		static void Load(TArray<Object*>& outObjects, Stream* stream);
	};
};