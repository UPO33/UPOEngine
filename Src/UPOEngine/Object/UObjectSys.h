#pragma once

#include "../Core/UBasic.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class Object;
	class ClassInfo;

	class UAPI ObjectSys
	{
	public:
		static Object* NewObject(const ClassInfo* classInfo);
		static void DeleteObject(Object* object);
	};

	//allocate new object and call ctor
	inline Object* NewObject(const ClassInfo* classInfo) { return ObjectSys::NewObject(classInfo); }
	//call dtor and delete the object
	inline void DeleteObject(Object* object) { ObjectSys::DeleteObject(object); }

	template<typename T> T* NewObject() 
	{
		static_assert(TT_IsObject<T>::value, "T must be a meta class derived from Object");
		return (T*)NewObject(T::GetClassInfoStatic());
	}
	template<typename T> T* NewObject(const ClassInfo* classInfo)
	{
		static_assert(TT_IsObject<T>::value, "T must be a meta class derived from Object");
		return (T*)NewObject(classInfo);
	}

};