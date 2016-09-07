#pragma once

#include "UBasic.h"
#include <memory.h>

namespace UPO
{
	inline void MemCopy(void* dst, const void* src, size_t size) { ::memcpy(dst, src, size); }
	inline void MemMove(void* dst, const void* src, size_t size) { ::memmove(dst, src, size); }
	inline void MemZero(void* dst, size_t size) { ::memset(dst, 0, size); }


	inline void* MemAlloc(size_t size) { return ::malloc(size); }
	inline void* MemRealloc(void* memory, size_t newSize) { return ::realloc(memory, newSize); }
	inline void  MemFree(void* memory) { ::free(memory); }

	//MemNew and MemDelete should be paired
	//allocate memory for T, doesn't call constructor
	template < typename T> T* MemNew(const bool zeromemory = false)
	{
		T* ptr = (T*)MemAlloc(sizeof(T));
		if (zeromemory) MemZero(ptr, sizeof(T));
		return ptr;
	}
	//free allocated memory of T, doesn't call destructor
	template< typename T> void MemDelete(T* ptr)
	{
		MemFree(ptr);
	}






	template<typename T> void ZeroType(T& object)
	{
		::memset(&object, 0, sizeof(T));
	};
	template<typename T> void CopyType(T& dst, const T& src)
	{
		::memcpy(&dst, &src, sizeof(T));
	}

};