#pragma once

#include "UBasic.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class RefCountable
	{
		template<typename T> friend class TSmartPtr;

		unsigned mRefCount;
	public:
		RefCountable() : mRefCount(0) {}
		~RefCountable() { UASSERT(mRefCount == 0); }
		void IncRefCount(int num = 1) { mRefCount += num; }
	};


	//////////////////////////////////////////////////////////////////////////
	template<typename T> class TSmartPtr
	{
		T* mPtr;
	public:
		TSmartPtr()
		{
			mPtr = nullptr;
		}
		~TSmartPtr()
		{
			if (mPtr)
			{
				mPtr->mRefCount--;
				if (mPtr->mRefCount == 0)
				{
					delete mPtr;
				}
			}

			mPtr = nullptr;
		}
		TSmartPtr(T* ptr)
		{
			mPtr = ptr;
			if (mPtr) mPtr->mRefCount++;
		}
		TSmartPtr(const TSmartPtr& copy)
		{
			mPtr = copy.mPtr;
			if (mPtr) mPtr->mRefCount++;
		}

		TSmartPtr& operator = (const TSmartPtr& other)
		{
			if (other.mPtr)
			{
				other.mPtr->mRefCount++;
				this->~TSmartPtr();
				mPtr = other.mPtr;
			}
			else
			{
				this->~TSmartPtr();
			}
			return *this;
		}
		TSmartPtr& operator = (T* ptr)
		{
			if (ptr)
			{
				ptr->mRefCount++;
				this->~TSmartPtr();
				mPtr = ptr;
			}
			else
			{
				this->~TSmartPtr();
			}
			return *this;
		}


		operator bool() const { return mPtr != nullptr; }
		operator T* () const { return mPtr; }
		T* operator -> () const { return mPtr; }

		bool operator == (const TSmartPtr& other) const { return mPtr == other.mPtr; }
		bool operator != (const TSmartPtr& other) const { return mPtr != other.mPtr; }
	};

};