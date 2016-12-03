#pragma once

#include "UBasic.h"

namespace UPO
{
	class UAPI BitArray
	{
	public:
		struct Element
		{
			BitArray& mArray;
			size_t mIndex;

			Element(BitArray& array, size_t index) : mArray(array), mIndex(index) {}

			Element& operator = (bool b)
			{
				mArray.SetBit(mIndex, b);
				return *this;
			}
			operator bool() const
			{
				return mArray.GetBit(mIndex);
			}
		};

	private:
		static size_t const NUM_BITS = sizeof(size_t) * 8;
		static size_t const NUM_BYTE = sizeof(size_t);

		size_t*				mBits;
		size_t				mNumBit;
		size_t				mCapacity;

	public:
		BitArray(size_t capacity = 0);
		~BitArray();
		void Empty();
		void SetCapacity(size_t newCapacity);
		size_t Add(bool bit);
		void RemoveAtSwap(size_t index);
		void RemoveAll();
		bool IsIndexValid(size_t index) const;
		bool GetBit(size_t index) const;
		void SetBit(size_t index, bool bit);
		void ToggleBit(size_t index);
		void Shrink();

		bool operator [] (size_t index) const { return GetBit(index); }
		Element operator [] (size_t index) { return Element(*this, index); }

		size_t Length() const { return mNumBit; }
		size_t Capacity() const { return mCapacity; }
		size_t* Bits() const { return mBits; }

		template<typename Lambda> void ForEach(Lambda proc)
		{
			for (size_t i = 0; i < mNumBit; i++)
				proc(GetBit(i));
		}
	};
};