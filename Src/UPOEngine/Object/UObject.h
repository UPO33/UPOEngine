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
		ClassInfo*			mClassInfo;
		ObjectRefData*		mRefData;
		
	public:
		Object() {}
		virtual ~Object();

		ClassInfo* GetClassInfo() const { return mClassInfo; }
		ObjectRefData* GetRefData();
		template<typename T> T* Cast() const
		{
			if (GetClassInfo()->IsBaseOf(T::GetClassInfoStatic())) return (T*) this;
			return nullptr;
		}

		bool IsAsset() const;
		bool IsEntity() const;
	};

};