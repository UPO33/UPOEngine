#pragma once

#include "../Core/UBasic.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class Object;

	//////////////////////////////////////////////////////////////////////////
	class UAPI ObjectRefData
	{
	public:
		union
		{
			struct
			{
				size_t	mRefCount;
				Object*	mObject;
			};
		};
		void Inc()
		{
			mRefCount++;
		}
		void Dec();

		static ObjectRefData NullRef;
		static ObjectRefData* GetNew();
		static void Free(ObjectRefData* refdata);
	};


	//////////////////////////////////////////////////////////////////////////
	template<typename T> class TObjectPtr
	{
		static_assert(TT_IsObject<T>::value, "T must be a meta class derived from Object");

		ObjectRefData* mRefData;

	public:
		////type trait helper 
		void ZZZIsTObjectPtr() {}
		using ObjectType = T;

		TObjectPtr()
		{
			mRefData = &ObjectRefData::NullRef;
		}
		TObjectPtr(std::nullptr_t)
		{
			mRefData = &ObjectRefData::NullRef;
		}
		TObjectPtr(Object* object)
		{
			if (object)
			{
				mRefData = object->GetRefData();
				mRefData->Inc();
			}
			else
			{
				mRefData = &ObjectRefData::NullRef;
			}
		}
		TObjectPtr(const TObjectPtr& other)
		{
			mRefData = other->mRefData;
			mRefData->Inc();
		}
		TObjectPtr& operator = (const TObjectPtr& other)
		{
			other.mRefData->Inc();
			mRefData->Dec();
			mRefData = other->mRefData;
			return *this;
		}
		TObjectPtr& operator = (Object* object)
		{
			if (object)
			{
				ObjectRefData* tmp = object->GetRefData();
				tmp->Inc();
				mRefData->Dec();
				mRefData = tmp;
			}
			else
			{
				mRefData->Dec();
				mRefData = &ObjectRefData::NullRef;
			}
			return *this;
		}
		TObjectPtr& operator = (std::nullptr_t)
		{
			mRefData->Dec();
			mRefData = &ObjectRefData::NullRef;
			return *this;
		}
		~TObjectPtr()
		{
			mRefData->Dec();
		}
		operator bool() const { return mRefData->mObject != nullptr; }
		operator T* () const { return (T*)mRefData->mObject; }
		T* operator -> () const { return (T*)mRefData->mObject; }
		T* Get() const { return (T*)mRefData->mObject; }
		
		bool operator == (const Object* obj) const { return ((Object*)mRefData->mObject) == obj; }
		bool operator != (const Object* obj) const { return ((Object*)mRefData->mObject) != obj; }

		bool operator == (const TObjectPtr& objptr) const
		{
			return mRefData == objptr.mRefData;
		}
		bool operator != (const TObjectPtr& objptr) const
		{
			return mRefData != objptr.mRefData;
		}
	};

	typedef TObjectPtr<Object> ObjectPtr;
};