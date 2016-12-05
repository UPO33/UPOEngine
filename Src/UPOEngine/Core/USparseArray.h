#pragma once

#include "UArray.h"
#include "UBitArray.h"


#define UBIT_SET(a,b) ((a) |= (1<<(b)))
#define UBIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define UBIT_FLIP(a,b) ((a) ^= (1<<(b)))
#define UBIT_CHECK(a,b) ((a) & (1<<(b)))


namespace UPO
{
	template<typename T> class TSparseArray2
	{
		static const size_t NUM_BITS = sizeof(size_t) * 8;
		static const size_t NUM_BYTES = sizeof(size_t);

		struct ElemPack
		{
			size_t mFreeMask;
			union
			{
				T			mData;
				int			mNextFree;

			} mElements[NUM_BITS];
			
			bool BitCheck(size_t index) { return UBIT_CHECK(mFreeMask, index); }
		};

		ElemPack*	mPacks;
		size_t		mNumElement;
		size_t		mCapacity;
		int			mFirstFreeIndex;
		int			mMaxFreeIndex;

		void PerformFreeIndex(size_t index, size_t count)
		{
			if (count == 0) return;

			for (size_t i = index + count - 1; i >= index; i--)
			{
				mPacks[i / NUM_BITS].mElements[i % NUM_BITS].mNextFree = mFirstFreeIndex;
				mFirstFreeIndex = i;
			}
		}
		void IncCapacity()
		{
			UASSERT(mCapacity % NUM_BITS == 0);

			size_t newCapacity = mCapacity + 128;

			if (mPacks)
			{
				size_t preNumPack = mCapacity / NUM_BITS;
				size_t numPack = newCapacity / NUM_BITS;
				mPacks = (ElemPack*)MemRealloc(mPacks, numPack * sizeof(ElemPack));
				UASSERT(mPacks);
				PerformPack(preNumPack, numPack - preNumPack);
				mCapacity = newCapacity;
			}
			else
			{
				size_t numPack = newCapacity / NUM_BITS;
				mPacks = (ElemPack*)MemAlloc(numPack * sizeof(ElemPack));
				UASSERT(mPacks);
				mCapacity = newCapacity;
				PerformPack(0, numPack);
			}
		}
		void PerformPack(size_t index, size_t count)
		{
			if (count == 0) return;

			for (int i = index + count - 1; i >= index; i--)
			{
				mPacks[i].mFreeMask = ~((size_t)0);

				for (int j = NUM_BITS - 1; j >= 0; j--)
				{
					mPacks[i].mElements[j].mNextFree = mFirstFreeIndex;
					mFirstFreeIndex = i * NUM_BITS + j;
				}
			}
		}
	public:
		TSparseArray2(size_t initialCapacity = 0)
		{
			mPacks = nullptr;
			mNumElement = 0;
			mCapacity = 0;
			mFirstFreeIndex = -1;
			mMaxFreeIndex = -1;

// 			if (initialCapacity) SetCapacity(initialCapacity);
		}

		~TSparseArray2()
		{
			Empty();
		}
		

// 		void SetCapacity(size_t newCapacity)
// 		{
// 			UASSERT(mCapacity % NUM_BITS == 0);
// 
// 			if (newCapacity == 0)
// 			{
// 				Empty();
// 				return;
// 			}
// 
// 			size_t newCapacityRouded = RoundUpToMulP2(newCapacity, NUM_BITS);
// 			if (newCapacityRouded == mCapacity) return;
// 
// 			if (mPacks)
// 			{
// 				size_t preNumPack = mCapacity / NUM_BITS;
// 				size_t numPack = newCapacityRouded / NUM_BITS;
// 				mPacks = (ElemPack*)MemRealloc(mPacks, numPack * sizeof(ElemPack));
// 				UASSERT(mPacks);
// 				PerformPack(preNumPack, numPack - preNumPack);
// // 				PerformFreeIndex(mNumElement, mCapacity, newCapacityRouded - mCapacity);
// 				mCapacity = newCapacityRouded;
// 			}
// 			else
// 			{
// 				size_t numPack = newCapacityRouded / NUM_BITS;
// 				mPacks = (ElemPack*)MemAlloc(numPack * sizeof(ElemPack));
// 				UASSERT(mPacks);
// 				mCapacity = newCapacityRouded;
// 				PerformFreeIndex(0, mCapacity);
// 				PerformPack(0, numPack);
// 			}
// 		}
		
		void Shrink()
		{
			if (mPacks)
			{

			}
		}
		size_t AddUnInit()
		{
			if (mFirstFreeIndex == -1)
			{
				IncCapacity();
			}
			else
			{
				const int index = mFirstFreeIndex;
				UBIT_CLEAR(mPacks[index / NUM_BITS].mFreeMask, index % NUM_BITS);
				mFirstFreeIndex = mPacks[index / NUM_BITS].mElements[index % NUM_BITS].mNextFree;
				return index;
			}
		}
		void RemoveAt(size_t index)
		{
			UASSERT(IsIndexValid(index));

			UBIT_SET(mPacks[index / NUM_BITS].mFreeMask, index % NUM_BITS);
			mPacks[index / NUM_BITS].mElements[index % NUM_BITS].mData.~T();
			mPacks[index / NUM_BITS].mElements[index % NUM_BITS].mNextFree = mFirstFreeIndex;
			mFirstFreeIndex = index;
			mMaxFreeIndex = Max(mMaxFreeIndex, mFirstFreeIndex);
			mNumElement--;
		}
		void RemoveAll()
		{
			ForEach([](T& elem) {
				elem.~T();
			});
			mNumElement = 0;
			mFirstFreeIndex = -1;
			mMaxFreeIndex = mCapacity - 1;
			PerformPack(0, mCapacity);
		}
		void Empty()
		{
			RemoveAll();
			if (mPacks) MemFree(mPacks);
			mPacks = nullptr;
			mCapacity = 0;
		}
		template<typename Lambda> void ForEach(Lambda proc)
		{
			const size_t numPack = mCapacity / NUM_BITS;
			const size_t numRest = mCapacity % NUM_BITS;
			UASSERT(numRest == 0);

			for (size_t iPack = 0; iPack < numPack; iPack++)
			{
				for (size_t iBit = 0; iBit < NUM_BITS; iBit++)
				{
					if (mPacks[iPack].BitCheck(iBit))
					{
						proc(mPacks[iPack].mElements[iBit].mData);
					}
				}
			}

// 			for (size_t iRest = 0; iRest < numRest; iRest++)
// 			{
// 				if (mPacks[numPack].BitCheck(iRest))
// 				{
// 					proc(mPacks[numPack].mElements[iRest].mData);
// 				}
// 			}
		}
		ElemPack& ElementAt(size_t index)
		{
			UASSERT(IsIndexValid(index));
			return mPacks[index / NUM_BITS].mElements[index % NUM_BITS].mData;
		}
		bool IsIndexValid(size_t index) const
		{
			return mPacks[index / NUM_BITS].BitCheck(index % NUM_BITS);
		}
	};

// 	template<typename T> class TSparseArray
// 	{
// 		union Element
// 		{
// 			T			mData;
// 			int			mNextFree;
// 		};
// 
// 		Element*			mElements;
// 		BitArray			mFreeBits;
// 		int					mFirstFreeIndex;
// 		int					mMaxFreeIndex;
// 		unsigned			mNumElement;
// 
// 	public:
// 		TSparseArray()
// 		{
// 			
// 		}
// 		~TSparseArray()
// 		{
// 
// 		}
// 
// 		size_t Capacity() const { return mFreeBits.Capacity(); }
// 		size_t NumElement() const { return mNumElement; }
// 
// 		void SetCapacity(size_t newCapacity)
// 		{
// 			if (newCapacity == 0)
// 			{
// 				Empty();
// 				return;
// 			}
// 
// 			if (Capacity() == newCapacity) return;
// 
// 			mFreeBits.SetCapacity(newCapacity);
// 			
// 			if (mElements)
// 			{
// 
// 			}
// 			else
// 			{
// 
// 			}
// 
// 		}
// 		void Shrink()
// 		{
// 
// 		}
// 		size_t AddUnInit()
// 		{
// 			if (mFirstFreeIndex == -1)
// 			{
// 
// 			}
// 			else
// 			{
// 				mFreeBits.SetBit(mFirstFreeIndex, true);
// 
// 				auto tmp = mFirstFreeIndex;
// 				mFirstFreeIndex = mElements[mFirstFreeIndex].mNextFree;
// 				return tmp;
// 			}
// 		}
// 		void RemoveAt(size_t index)
// 		{
// 			UASSERT(IsIndexValid(index));
// 
// 			mFreeBits.SetBit(index, true);
// 			Element& elem = mElements[index]
// 			elem.mData.~T();
// 			elem.mNextFree = mFirstFreeIndex;
// 			mFirstFreeIndex = index;
// 			mMaxFreeIndex = Max(mMaxFreeIndex, mFirstFreeIndex);
// 			mNumElement--;
// 		}
// 		void RemoveAll()
// 		{
// 			int* ffi = &mFirstFreeIndex;
// 			
// 			for (size_t i = 0; i < mFreeBits.Length(); i++)
// 			{
// 				if (!mFreeBits.GetBit(i)) mElements[i].~T();
// 				
// 				mFirstFreeIndex mElements[i].mNextFree = mFirstFreeIndex;
// 				mFirstFreeIndex = mElements[i];
// 			}
// 			mFreeBits.RemoveAll();
// 			mNumElement = 0;
// 			
// 		}
// 		void Empty()
// 		{
// 			RemoveAll();
// 			mFreeBits.Empty();
// 			if (mElements) MemFree(mElements);
// 			mElements = nullptr;
// 			mFirstFreeIndex = -1;
// 		}
// 		unsigned NumElement() const { return mNumElement; }
// 		unsigned Capacity() const { return mElements.Capacity(); }
// 
// 		bool IsIndexValid(size_t index) const
// 		{
// 			return mFreeBits.IsIndexValid(index) && !mFreeBits[index];
// 		}
// 		template<typename Lambda> void ForEach(Lambda proc)
// 		{
// 			const size_t numBits = mFreeBits.Length();
// 			for (size_t i = 0; i < numBits; i++)
// 			{
// 				if (!mFreeBits.GetBit(i))
// 					proc(mElements[i].mData);
// 			}
// 		}
// 	};
};