#include "UHash.h"


namespace UPO
{
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
	}
}
