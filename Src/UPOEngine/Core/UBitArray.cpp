#include "UBitArray.h"

#include "UMemory.h"

namespace UPO
{
	BitArray::BitArray(size_t capacity /*= 0*/)
	{
		mBits = nullptr;
		mNumBit = mCapacity = 0;
		if (capacity)
			SetCapacity(capacity);
	}

	BitArray::~BitArray()
	{
		Empty();
	}

	void BitArray::SetCapacity(size_t newCapacity)
	{
		if (newCapacity == 0)
		{
			Empty();
			return;
		}

		if (mBits)
		{
			mBits = (size_t*)MemRealloc(mBits, newCapacity / 8 + NUM_BYTE);
			UASSERT(mBits);
			mCapacity = newCapacity;
			mNumBit = Min(mNumBit, mCapacity * NUM_BITS);
		}
		else
		{
			UASSERT(mNumBit == 0);
			mBits = (size_t*)MemAlloc(newCapacity / 8 + NUM_BYTE);
			UASSERT(mBits);
			mCapacity = newCapacity;
		}
	}

	size_t BitArray::Add(bool bit)
	{
		if (mCapacity <= mNumBit)
		{
			SetCapacity(mCapacity + 1024);
		}

		auto tmp = mNumBit;
		SetBit(mNumBit++, bit);
		return tmp;
	}

	void BitArray::RemoveAtSwap(size_t index)
	{
		UASSERT(IsIndexValid(index));
		SetBit(index, GetBit(mNumBit - 1));
		mNumBit--;
	}

	void BitArray::RemoveAll()
	{
		mNumBit = 0;
	}

	bool BitArray::IsIndexValid(size_t index) const
	{
		return index < mNumBit;
	}

	void BitArray::Empty()
	{
		if (mBits) MemFree(mBits);
		mBits = nullptr;
		mCapacity = 0;
		mNumBit = 0;
	}

	bool BitArray::GetBit(size_t index) const
	{
		UASSERT(IsIndexValid(index));
		return mBits[index / NUM_BITS] & (1 << (index % NUM_BITS));
	}

	void BitArray::SetBit(size_t index, bool bit)
	{
		UASSERT(IsIndexValid(index));
		if (bit)
			mBits[index / NUM_BITS] |= 1 << (index % NUM_BITS);
		else
			mBits[index / NUM_BITS] &= ~(1 << (index % NUM_BITS));
	}

	void BitArray::ToggleBit(size_t index)
	{
		UASSERT(IsIndexValid(index));
		mBits[index / NUM_BITS] ^= 1 << (index % NUM_BITS);
	}

	void BitArray::Shrink()
	{
		SetCapacity(mNumBit);
	}

};