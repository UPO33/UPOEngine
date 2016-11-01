#pragma once

#include "UBasic.h"

#include "UString.h"
#include "UName.h"
#include "UBuffer.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	struct UAPI CRC32
	{
		typedef uint32 HashT;

		static uint32 GetPrime();
		static uint32 HashBuffer(const void* buffer, size_t length, uint32 hash = GetPrime());
		static uint32 HashStr(const char* str, uint32 hash = GetPrime());
	};
	//////////////////////////////////////////////////////////////////////////
	struct UAPI FNV32
	{
		typedef uint32 HashT;

		static const uint32 HASH_PRIME = 2166136261;

		inline static uint32 GetPrime() { return HASH_PRIME; }
		static uint32 HashBuffer(const void* buffer, size_t length, uint32 hash = GetPrime());
		static uint32 HashStr(const char* str, uint32 hash = GetPrime());
	};
	//////////////////////////////////////////////////////////////////////////
	struct UAPI FNV64
	{
		typedef uint64 HashT;

		static const uint64 HASH_PRIME = 14695981039346656037ULL;

		inline static uint64 GetPrime() { return HASH_PRIME; }
		static uint64 HashBuffer(const void* buffer, size_t length, uint64 hash = GetPrime());
		static uint64 HashStr(const char* str, uint64 hash = GetPrime());
	};

	

	//////////////////////////////////////////////////////////////////////////
	template<typename HashClass> struct THasher
	{
		typedef typename HashClass::HashT HashT;

		HashT mHash;

		THasher()
		{
			mHash = HashClass::GetPrime();
		}
		THasher& Add(const void* buffer, size_t length)
		{
			mHash = HashClass::HashBuffer(buffer, length, mHash);
			return *this;
		}
		template<typename T> THasher& Add(const T& item)
		{
			Add(&item, sizeof(T));
			return *this;
		}
		template<typename T> THasher& operator << (const T& item)
		{
			return Add(item);
		}
		THasher& Add(const char* cstr)
		{
			Add(cstr, StrLen(cstr));
			return *this;
		}
		THasher& Add(const String& str)
		{
			Add(str.CStr(), str.Length());
			return *this;
		}
		THasher& Add(const Name& name)
		{
			Add(name.CStr(), name.Length());
			return *this;
		}
		THasher& Add(const Buffer& buffer)
		{
			Add(buffer.Data(), buffer.Size());
			return *this;
		}

		auto GetHash() { return mHash; }
	};

	typedef THasher<FNV32> HasherFNV32;
	typedef THasher<FNV64> HasherFNV64;
	typedef THasher<CRC32> HasherCRC32;
};