#pragma once

#include "UBasic.h"
#include <memory.h>

namespace UPO
{
	static const size_t MALLOC_MAX_ALIGN = 8;

	inline void MemCopy(void* dst, const void* src, size_t size) { ::memcpy(dst, src, size); }
	inline void MemMove(void* dst, const void* src, size_t size) { ::memmove(dst, src, size); }
	inline void MemZero(void* dst, size_t size) { ::memset(dst, 0, size); }
	inline int  MemCmp(const void* a, const void* b, size_t size) { return ::memcmp(a, b, size); }
	inline bool MemEqual(const void* a, const void* b, size_t size) { return ::memcmp(a, b, size) == 0; }

	inline void* MemAlloc(size_t size) { return ::malloc(size); }
	inline void* MemRealloc(void* memory, size_t newSize) { return ::realloc(memory, newSize); }
	inline void  MemFree(void* memory) { ::free(memory); }
	template<typename T> void SafeMemFree(T*& memory)
	{
		if (memory) MemFree(memory);
		memory = nullptr;
	}
	inline void* MemAllocAligned(size_t size, size_t align) { return ::_aligned_malloc(size, align); }
	inline void* MemReallocAligned(void* memory, size_t newSize, size_t newAlignment) { return ::_aligned_realloc(memory, newSize, newAlignment); }
	inline void  MemFreeAligned(void* memory) { ::_aligned_free(memory); }
	template<typename T> void  SafeMemFreeAligned(T*& memory) 
	{ 
		if (memory) MemFreeAligned(memory);
		memory = nullptr;
	}

	//MemNew and MemDelete should be paired
	//allocate memory sizeof(T) and alifnof(T), doesn't call constructor
	template < typename T> T* MemNew(const bool zeromemory = false)
	{
		T* ptr = (T*)MemAllocAligned(sizeof(T), alignof(T));
		if (zeromemory) MemZero(ptr, sizeof(T));
		return ptr;
	}
	//MemNew and MemDelete should be paired
	//free allocated memory of T, doesn't call destructor
	template< typename T> void MemDelete(T* ptr)
	{
		MemFreeAligned(ptr);
	}
	template< typename T> void SafeMemDelete(T*& ptr)
	{
		SafeMemFreeAligned(ptr);
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