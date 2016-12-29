#include "UFreeListAllocator.h"
#include "UMemory.h"

namespace UPO
{

	void FreeListAllocatorStatic::Reset(int elementSize, int numElement)
	{
		UASSERT(elementSize > sizeof(void*) && numElement > 0);

		Destroy();
		mElementSize = elementSize;
		mElementCount = numElement;
		mMemoryBlock = MemAlloc(elementSize * numElement + POOL_ALIGN);
		FreeAll();
	}


	void FreeListAllocatorStatic::FreeAll()
	{
		mFirstFree = nullptr;
		byte* elements = (byte*) RoundUpToMulP2((size_t)mMemoryBlock, POOL_ALIGN);
		for (int i = mElementCount - 1; i >= 0; i--)
		{
			Elem* elem = reinterpret_cast<Elem*>(elements + (i * mElementSize));
			elem->mNextFree = mFirstFree;
			mFirstFree = elem;
		}
	}

	void FreeListAllocatorStatic::Destroy()
	{
		if (mMemoryBlock) MemFree(mMemoryBlock);
		mMemoryBlock = nullptr;
		mFirstFree = nullptr;
	}

	bool FreeListAllocatorStatic::IsValidMemory(const void* p) const
	{
		size_t iPtr = (size_t)p;
		size_t iHead = RoundUpToMulP2((size_t)mMemoryBlock, POOL_ALIGN);
		size_t iEnd = iHead + (mElementSize * mElementCount); // end of memory
		if(iPtr >= iHead && iPtr < iEnd)
		{
			return (iPtr - iHead) % mElementSize == 0;
		}
		return false;
	}

	void* FreeListAllocatorStatic::Alloc()
	{
		if (mFirstFree == nullptr) return nullptr;

		Elem* tmp = mFirstFree;
		mFirstFree = mFirstFree->mNextFree;
		return tmp;
	}

	void FreeListAllocatorStatic::Free(void* ptr)
	{
		UASSERT(ptr);
		Elem* elem = reinterpret_cast<Elem*>(ptr);
		elem->mNextFree = mFirstFree;
		mFirstFree = elem;
	}

	//////////////////////////////////////////////////////////////////////////
	void*& FreeListAllocator::ChunkGetNext(const void* chunk) const
	{
		size_t elementStart = RoundUpToMulP2((size_t)chunk, POOL_ALIGN);
		return *((void**)(elementStart + mElementSize * mNumElementInChunk + sizeof(void*)));
	}

	void* FreeListAllocator::AllocChunk() const
	{
		return MemAlloc(mElementSize * mNumElementInChunk + sizeof(void*) + POOL_ALIGN);
	}

	void FreeListAllocator::FreeChunkElements(byte* chunkElements)
	{
		for (int i = mNumElementInChunk - 1; i >= 0; i--)
		{
			Elem* elem = reinterpret_cast<Elem*>(chunkElements + (i * mElementSize));
			elem->mNextFree = mFirstFree;
			mFirstFree = elem;
		}
	}

	void FreeListAllocator::AddNewChunk()
	{
		void* newChunk = AllocChunk();
		UASSERT(newChunk);
		byte* elements = (byte*)RoundUpToMulP2((size_t)newChunk, POOL_ALIGN);
		FreeChunkElements(elements);
		ChunkGetNext(newChunk) = mChunkHead;
		mChunkHead = newChunk;
	}

	void FreeListAllocator::Reset(int elementSize, int numElementInChunk)
	{
		UASSERT(elementSize > sizeof(void*) && numElementInChunk > 8);
		Destroy();
		mElementSize = elementSize;
		mNumElementInChunk = numElementInChunk;
	}

	void* FreeListAllocator::Alloc()
	{
		if (mFirstFree == nullptr) AddNewChunk();

		auto tmp = mFirstFree;
		mFirstFree = mFirstFree->mNextFree;
		return tmp;


	}

	void FreeListAllocator::Free(void* ptr)
	{
		UASSERT(ptr);
		Elem* elem = reinterpret_cast<Elem*>(ptr);
		elem->mNextFree = mFirstFree;
		mFirstFree = elem;
	}

	void FreeListAllocator::FreeAll()
	{
		mFirstFree = nullptr;

		void* chunk = mChunkHead;
		while (chunk)
		{
			byte* elements = (byte*)RoundUpToMulP2((size_t)chunk, POOL_ALIGN);
			FreeChunkElements(elements);
			chunk = ChunkGetNext(chunk);
		}
	}

	void FreeListAllocator::Destroy()
	{
		void* chunk = mChunkHead;
		while (chunk)
		{
			auto tmp = chunk;
			chunk = ChunkGetNext(chunk);
			MemFree(tmp);
		}

		mChunkHead = nullptr;
		mFirstFree = nullptr;
	}

};