#pragma once

#include "UBasic.h"

namespace UPO
{
	typedef uint32 HashFNV32;
	typedef uint64 HashFNV64;

	namespace Hash
	{
		static const uint32 FNV32_HASH_PRIME = 2166136261;
		static const uint64 FNV64_HASH_PRIME = 14695981039346656037ULL;

		UAPI uint32 FNV32(const void* data, size_t len, uint32 hash = FNV32_HASH_PRIME);
		UAPI uint64 FNV64(const void* data, size_t len, uint64 hash = FNV64_HASH_PRIME);

		UAPI uint32 FNV32Str(const char* str, uint32 hash = FNV32_HASH_PRIME);
		UAPI uint64 FNV64Str(const char* str, uint64 hash = FNV64_HASH_PRIME);

	};
};