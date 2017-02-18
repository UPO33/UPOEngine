#pragma once

#include "UObjectPtr.h"
#include "UObjectSys.h"
#include "UObjectArchive.h"



#ifdef GetClassInfo
#undef  GetClassInfo
#endif

// #define USEOBJREF
// #define USEOBJLUT

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class Object;
	class ClassInfo;
	class ObjectRefData;
	class ObjectSys;
	class MetaSys;
	class PropertyInfo;

	//////////////////////////////////////////////////////////////////////////
	class UAPI Object
	{
		UCLASS(Object, void)

		template<typename T> friend class TObjectPtr;
		friend ObjectSys;
		friend MetaSys;
#ifdef USEOBJLUT
		friend class ObjectPtr;
		friend class ObjectContext;
		unsigned	mObjectIndex;
		unsigned	mObjectID;
#endif
	private:
		ClassInfo*					mClassInfo;
		mutable ObjectRefData*		mRefData;
		
	public:

#ifndef UENDUSER
		size_t	mNeedsPropertyBrowserRefresh : 1;
		Object() :
			mNeedsPropertyBrowserRefresh(false)
		{}
#else
		Object(){}
#endif
		
		virtual ~Object();

		ClassInfo* GetClassInfo() const { return mClassInfo; }
		ObjectRefData* GetRefData();
		template<typename T> T* Cast() const
		{
			if (IsBaseOf(T::GetClassInfoStatic())) return (T*) this;
			return nullptr;
		}
		bool IsBaseOf(const ClassInfo* baseClass) const;

		void MetaBeforePropertyChange(const PropertyInfo*) {}
		void MetaAfterPropertyChange(const PropertyInfo*) {}
	};

	template<typename T> T* UCast(Object* object)
	{
		if (object) return object->Cast<T>();
		return nullptr;
	}
};