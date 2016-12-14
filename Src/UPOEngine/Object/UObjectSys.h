#pragma once

#include "../Core/UBasic.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class Object;
	class ClassInfo;

#if 0
	class ObjectPtr
	{
		unsigned	mIndex;
		unsigned	mID;

	public:
		ObjectPtr() : mID(0), mIndex(0) {}
		ObjectPtr(const Object* obj)
		{
			if (obj)
			{
				mIndex = obj->mObjectIndex;		mID = obj->mObjectID;
			}
			else
			{
				mIndex = mID = 0;
			}
		}
		Object* Get() const;
		bool IsValid() const;
		void Invalidate() { mID = mIndex = 0; }

		ObjectPtr& operator = (const Object* obj)
		{
			new (this) ObjectPtr(obj);
			return *this;
		}
	};
#endif

	class UAPI ObjectSys
	{
		Object* AllocObject(const ClassInfo*);
		void FreeObject(Object*);

	public:
		ObjectSys();
		~ObjectSys();
		static ObjectSys* Get();

		Object* NewObject(const ClassInfo* classInfo);
		void DeleteObject(Object* object);
	};
	inline ObjectSys* GObjectSys() { return ObjectSys::Get(); }

	//allocate new object and call ctor
	inline Object* NewObject(const ClassInfo* classInfo) { return GObjectSys()->NewObject(classInfo); }
	//call dtor and delete the object
	inline void DeleteObject(Object* object) { GObjectSys()->DeleteObject(object); }

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