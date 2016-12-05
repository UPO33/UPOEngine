#pragma once

#include "UBasic.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class UAPI FreeListAllocatorStatic
	{
		static const int POOL_ALIGN = 64;

		struct Elem
		{
			Elem*	mNextFree;
		};

		Elem*	mFirstFree;
		void*	mMemoryBlock;
		int		mElementSize;
		int		mElementCount;
		
	public:
		FreeListAllocatorStatic(const FreeListAllocatorStatic&) = delete;
		FreeListAllocatorStatic& operator = (const FreeListAllocatorStatic&) = delete;

		FreeListAllocatorStatic()
		{
			mFirstFree = nullptr;
			mMemoryBlock = nullptr;
			mElementCount = mElementSize = 0;
		}
		FreeListAllocatorStatic(int elementSize, int numElement)
		{
			mFirstFree = nullptr;
			mMemoryBlock = nullptr;
			mElementCount = mElementSize = 0;
			Reset(elementSize, numElement);
		}
		~FreeListAllocatorStatic() { Destroy(); }

		int ElementSize() const { return mElementSize; }
		
		void Reset(int elementSize, int numElement);
		//free all elements but not memory
		void FreeAll();
		//free all elements and memory
		void Destroy();
		bool IsFull() const { return mFirstFree == nullptr; }
		//checks whether or not a pointer is valid memory address (whether free or allocated)
		bool IsValidMemory(const void* ptr) const;
		//allocate an element, return null if its full
		void* Alloc();
		void Free(void* ptr);
	};


	//////////////////////////////////////////////////////////////////////////
	class UAPI FreeListAllocator
	{
		static const int POOL_ALIGN = 64;

		struct Elem
		{
			Elem*	mNextFree;
		};

		/*
		struct Chunk
		{
			Elem			mElements[mNumElementInChunck];
			Chunk*			mNextChunk;
		};
		*/

		Elem*	mFirstFree;
		void*	mChunkHead;
		int		mElementSize;
		int		mNumElementInChunk;
		
		
		void*& ChunkGetNext(const void* chunk) const;
		void* AllocChunk() const;
		void FreeChunkElements(byte* chunkElements);	
		void AddNewChunk();
	public:
		FreeListAllocator(const FreeListAllocator&) = delete;
		FreeListAllocator& operator = (const FreeListAllocator&) = delete;

		FreeListAllocator()
		{
			mChunkHead = nullptr;
			mFirstFree = nullptr;
			mElementSize = mNumElementInChunk = 0;
		}
		FreeListAllocator(int elementSize, int numElementInChunk)
		{
			mChunkHead = nullptr;
			mFirstFree = nullptr;
			mElementSize = mNumElementInChunk = 0;
			Reset(elementSize, numElementInChunk);
		}
		~FreeListAllocator() { Destroy(); }
		
		void Reset(int elementSize, int numElementInChunk);
		void* Alloc();
		void Free(void* ptr);
		//free all elements but not memory
		void FreeAll();
		//free all elements and memory
		void Destroy();

		int ElementSize() const { return mElementSize; }
		int NumElementInChunk() const { return mNumElementInChunk; }
	};

};