#pragma once

#include "UArray.h"
#include "UBitArray.h"

namespace UPO
{
	template<typename T> class TSparseArray
	{
		union Element
		{
			T			mData;
			int			mNextFree;
		};

		TArray<Element>		mElements;
		BitArray			mFreeBits;
		unsigned			mFirstFreeIndex;
		unsigned			mNumElement;

	public:
		TSparseArray()
		{
			
		}
		~TSparseArray()
		{

		}
		size_t AddUnInit()
		{
			if (mFirstFreeIndex == -1)
			{

			}
			else
			{
				mFreeBits.SetBit(mFirstFreeIndex, true);

				auto tmp = mFirstFreeIndex;
				mFirstFreeIndex = mElements[mFirstFreeIndex].mNextFree;
				return tmp;
			}
		}
		void RemoveAt(size_t index)
		{
			UASSERT(IsIndexValid(index));

			mFreeBits.SetBit(index, true);
			Element* elem = mElements.ElementAt(index);
			elem->mData.~T();
			elem->mNextFree = mFirstFreeIndex;
			mFirstFreeIndex = index;
		}
		void RemoveAll()
		{
			mElements.RemoveAll();
			mFreeBits.RemoveAll();
		}
		void Empty()
		{
			mElements.Empty();
			mFreeBits.Empty();
		}
		unsigned NumElement() const { return mNumElement; }
		unsigned Capacity() const { return mElements.Capacity(); }

		bool IsIndexValid(size_t index) const
		{
			return mFreeBits.IsIndexValid(index) && !mFreeBits[index];
		}
		template<typename Lambda> void ForEach(Lambda proc)
		{
			const size_t numBits = mFreeBits.Length();
			for (size_t i = 0; i < numBits; i++)
			{
				if (!mFreeBits.GetBit(i))
					proc(mElements[i].mData);
			}
		}
	};
};