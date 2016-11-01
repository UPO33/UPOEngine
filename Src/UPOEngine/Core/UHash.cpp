#include "UHash.h"

#include <zlib/zlib.h>

namespace UPO
{
	/*
	namespace Hash
	{
		uint32 FNV32(const void* data, size_t len, uint32 hash)
		{
			UASSERT(data);
			unsigned char* p = (unsigned  char*)data;
			for (size_t i = 0; i < len; i++)
			{
				hash = (hash * 16777619) ^ p[i];
			}
			return hash;
		}

		uint64 FNV64(const void* data, size_t len, uint64 hash)
		{
			UASSERT(data);
			unsigned char* p = (unsigned char*)data;
			for (size_t i = 0; i < len; i++)
			{
				hash = (hash * 1099511628211ULL) ^ p[i];
			}
			return hash;
		}

		uint32 FNV32Str(const char * str, uint32 hash)
		{
			UASSERT(str);
			unsigned char* p = (unsigned  char*)str;
			while (*p)
			{
				hash = (hash * 16777619) ^ *p;
				p++;
			}
			return hash;
		}

		uint64 FNV64Str(const char* str, uint64 hash)
		{
			UASSERT(str);
			unsigned char* p = (unsigned char*)str;
			while (*p)
			{
				hash = (hash * 1099511628211ULL) ^ *p;
				p++;
			}
			return hash;
		}
		
		uint32 CRC32GetPrime()
		{
			static uint32 CRCPrime = ::crc32(0, nullptr, 0);
			return CRCPrime;
		}
		uint32 CRC32(const void* buffer, size_t length, uint32 crc)
		{
			return ::crc32(crc, (const Bytef*)buffer, (uInt)length);
		}
	}
	*/




	uint32 CRC32::GetPrime()
	{
		static uint32 CRCPrime = ::crc32(0, nullptr, 0);
		return CRCPrime;
	}

	uint32 CRC32::HashBuffer(const void* buffer, size_t length, uint32 hash)
	{
		return ::crc32(hash, (const Bytef*)buffer, (uInt)length);
	}

	uint32 CRC32::HashStr(const char* str, uint32 hash /*= GetPrime()*/)
	{
		return HashBuffer(str, StrLen(str), hash);
	}

	uint32 FNV32::HashBuffer(const void* buffer, size_t length, uint32 hash)
	{
		UASSERT(buffer);
		unsigned char* p = (unsigned  char*)buffer;
		for (size_t i = 0; i < length; i++)
		{
			hash = (hash * 16777619) ^ p[i];
		}
		return hash;
	}

	uint32 FNV32::HashStr(const char* str, uint32 hash /*= GetPrime()*/)
	{
		return HashBuffer(str, StrLen(str), hash);
	}

	uint64 FNV64::HashBuffer(const void* buffer, size_t length, uint64 hash)
	{
		UASSERT(buffer);
		unsigned char* p = (unsigned char*)buffer;
		for (size_t i = 0; i < length; i++)
		{
			hash = (hash * 1099511628211ULL) ^ p[i];
		}
		return hash;
	}

	uint64 FNV64::HashStr(const char* str, uint64 hash)
	{
		return HashBuffer(str, StrLen(str), hash);
	}

}
