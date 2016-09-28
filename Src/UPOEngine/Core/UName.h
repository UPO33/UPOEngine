#pragma once

#include "UString.h"
#include "UHash.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class UAPI NameContext
	{
		friend class Name;

		static const unsigned CONTEXT_TABLE_SIZE = 8192;	//must be pow2
		static const unsigned NAME_MAX_LENGTH = 255;	//must be pow2 - 1

		struct Instance
		{
			Instance*	mNext = nullptr;
			uint32		mHash = Hash::FNV32_HASH_PRIME;
			uint32		mRefCount = 0;
			char		mString[NAME_MAX_LENGTH + 1] = { 0 };
			uint32		mLength = 0;
		};


		Instance*	mTable[CONTEXT_TABLE_SIZE];
		size_t		mNumInstance = 0;

	public:
		static Instance EmptyInstance;

		static NameContext* Get();

		NameContext();
		~NameContext();

		Instance* GetOrSet(const char* str);
		void FreeUnuseds();

		size_t GetNumInstance() const { return mNumInstance; }
	};


	//////////////////////////////////////////////////////////////////////////
	class UAPI Name
	{
		UCLASS(Name, void)

	public:
		static const unsigned NAME_MAX_LENGTH = NameContext::NAME_MAX_LENGTH;
		static const Name Empty;

	private:
		NameContext::Instance*	mInstance;


	public:
		Name()
		{
			mInstance = &NameContext::EmptyInstance;
		}
		~Name()
		{
			mInstance->mRefCount--;
		}
		Name(const char* str)
		{
			mInstance = NameContext::Get()->GetOrSet(str);
		}
		Name(const Name& other)
		{
			mInstance = other.mInstance;
			mInstance->mRefCount++;
		}

		Name& operator = (const Name& name)
		{
			name.mInstance->mRefCount++;
			mInstance->mRefCount--;
			mInstance = name.mInstance;
			return *this;
		}
		Name& operator = (const char* str)
		{
			mInstance->mRefCount--;
			mInstance = NameContext::Get()->GetOrSet(str);
			return *this;
		}
		bool operator == (const Name& other) const
		{
			if (mInstance == other.mInstance)
			{
				UASSERT(mInstance->mHash == other.mInstance->mHash);
				return true;
			}
			return false;
		}
		bool operator != (const Name& other) const
		{
			if (mInstance != other.mInstance)
			{
				UASSERT(mInstance->mHash != other.mInstance->mHash);
				return true;
			}
			return false;
		}
		bool operator == (const char* str) const
		{
			if (str)
			{
				return mInstance->mHash == Hash::FNV32Str(str);
			}
			else
			{
				return mInstance == &NameContext::EmptyInstance;
			}
		}
		bool operator != (const char* str) const
		{
			if (str)
			{
				return mInstance->mHash != Hash::FNV32Str(str);
			}
			else
			{
				return mInstance != &NameContext::EmptyInstance;
			}
		}
		operator const char*() const { return mInstance->mString; }

		bool IsEmpty() const { return mInstance == &NameContext::EmptyInstance; }
		auto Length() const { return mInstance->mLength; }
		const char* CStr() const { return mInstance->mString; }
		auto GetHash() const { return mInstance->mHash; }

		void MetaSerialize(Stream&);
	};

	inline uint32 GetHash32(const Name& name) { return (uint32)name.GetHash(); }
};