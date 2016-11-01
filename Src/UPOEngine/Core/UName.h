#pragma once

#include "UString.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class UAPI NameContext
	{
		friend class Name;
	public:
		static const unsigned NAME_MAX_LENGTH = 255;	//must be pow2 - 1
		static const unsigned TABLE_SIZE = 4096;	//must be power of two
	private:
		struct UAPI Instance
		{
			Instance*	mNext;
			uint32		mHash;
			char		mString[NAME_MAX_LENGTH + 1];
			uint32		mLength;

			Instance();
		};

		size_t		mNumInstance = 0;
		Instance*	mTable[TABLE_SIZE];
		


	public:
		static Instance EmptyInstance;

		static NameContext* Get();

		NameContext();
		~NameContext();

		Instance* GetOrSet(const char* str);

		size_t GetNumInstance() const { return mNumInstance; }
	};


	//////////////////////////////////////////////////////////////////////////
	class UAPI Name
	{
		UCLASS(Name, void)

	public:
		static const unsigned NAME_MAX_LENGTH = NameContext::NAME_MAX_LENGTH;
		static const Name Empty;

		typedef decltype(NameContext::Instance::mHash) HashT;

	private:
		NameContext::Instance*	mInstance;


	public:
		Name()
		{
			mInstance = &NameContext::EmptyInstance;
		}
		~Name()
		{
		}
		Name(const char* str)
		{
			mInstance = NameContext::Get()->GetOrSet(str);
		}
		Name(const Name& other)
		{
			mInstance = other.mInstance;
		}

		Name& operator = (const Name& name)
		{
			mInstance = name.mInstance;
			return *this;
		}
		Name& operator = (const char* str)
		{
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
		bool operator == (const char* str) const;
		bool operator != (const char* str) const
		{
			return !this->operator==(str);
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