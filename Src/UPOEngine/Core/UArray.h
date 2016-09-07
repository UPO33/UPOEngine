#pragma once

#include "UMemory.h"

namespace UPO
{

	struct ArrayAlloctorDefault
	{
		void* Alloc(size_t size) { return MemAlloc(size); }
		void* Realloc(void* memory, size_t size) { return MemRealloc(memory, size); }
		void Free(void* memory) { MemFree(memory); }
	};

	template <size_t Size> struct ArrayAllocatorFixed
	{
		void* mMemory;

		ArrayAllocatorFixed()
		{
			mMemory = MemAlloc(Size);
		}
		~ArrayAllocatorFixed()
		{
			if (mMemory) MemFree(mMemory);
			mMemory = nullptr;
		}
		void* Alloc(size_t size)
		{
			UASSERT(size <= Size);
			return mMemory;
		}
		void* Realloc(void* ptr, size_t newSize)
		{
			UASSERT(newSize <= Size);
			return mMemory;
		}
		void Free(void* ptr)
		{

		}
	};

	//////////////////////////////////////////////////////////////////////////
	template<typename T> class TArray
	{
	private:
		T*			mElements;
		size_t		mLength;
		size_t		mCapacity;

		static T* Alloc(size_t size)
		{
			ULOG_MESSAGE("%zu bytes allocated", size);
			return (T*)MemAlloc(size);
		}
		static T* Realloc(T* memory, size_t newSize)
		{
			ULOG_SUCCESS("%zu bytes allocated ", newSize);
			return (T*)MemRealloc(memory, newSize);
		}
		static void Free(T* memory)
		{
			MemFree(memory);
		}
		static void CallCCTor(T* elements, const T* copy, size_t numELement)
		{
			for (size_t i = 0; i < numELement; i++)
				new (elements + i) T(copy[i]);
		}
		static void CallDTor(T* elements, size_t numElement)
		{
			for (size_t i = 0; i < numElement; i++)
				(elements + i)->~T();
		}
	public:
		//type trait helper
		void ZZZIsTArray() {}
		using ElementType = T;

		TArray()
		{
			mElements = nullptr;
			mLength = mCapacity = 0;
		}
		TArray(size_t initialCapacity)
		{
			UASSERT(initialCapacity);

			mLength = 0;
			mCapacity = initialCapacity;

			mElements = Alloc(initialCapacity * sizeof(T));
			UASSERT(mElements);
		}
		TArray(const T* elements, size_t numElement)
		{
			UASSERT(elements);
			UASSERT(elementCount > 0);

			mCapacity = mLength = elementCount;

			mElements = Alloc(elementCount * sizeof(T));
			UASSERT(mElements);
			CallCCTor(mElements, elements, numElement);
		}
		TArray(const TArray& copy)
		{
			if (copy.mElements && copy.mLength)
			{
				mLength = mCapacity = copy.mLength;
				mElements = Alloc(mLength * sizeof(T));
				UASSERT(mElements);
				CallCCTor(mElements, copy.mElements, copy.mLength);
			}
			else
			{
				mLength = mCapacity = 0;
				mElements = nullptr;
			}
		}
		~TArray()
		{
			Empty();
		}
		TArray& operator = (const TArray& other)
		{
			Empty();
			new (this) TArray(other);
			return *this;
		}
		TArray& operator += (const TArray& other)
		{
			Append(other.mElements, other.mLength);
			return *this;
		}
		void Append(const T* elements, size_t numElement)
		{
			if (elements == nullptr || numElement == 0) return;

			IncCapacity(numElement);
			CallCCTor(mElements + mLength, elements, numElement);
			mLength += numElement;
		}
		/*
			Increasing capacity may perform Alloc or Realloc, it uses MemCopy not calling DTor CTor
		*/
		void IncCapacity(size_t numItem = 64)
		{
			if (numItem == 0) return;

			if (mElements)
			{
				mCapacity += numItem;
				mElements = Realloc(mElements, mCapacity * sizeof(T));
				UASSERT(mElements);
			}
			else
			{
				UASSERT(mLength == 0);
				UASSERT(mCapacity == 0);
				mCapacity = numItem;
				mElements = Alloc(mCapacity * sizeof(T));
				UASSERT(mElements);
			}
		}
		//set the new capacity for array, if new capacity is less that current array length, destructor will be called on the waste elements
		void SetCapacity(size_t newCapacity)
		{
			if (newCapacity)
			{
				mCapacity = newCapacity;
				auto newLength = Min(mLength, mCapacity);

				if (mElements)
				{
					if (newLength < mLength)
						CallDTor(mElements + newLength, mLength - newLength);
					mLength = newLength;
					mElements = Realloc(mElements, newCapacity * sizeof(T));
					UASSERT(mElements);
				}
				else
				{
					mLength = newLength;
					mElements = Alloc(newCapacity * sizeof(T));
					UASSERT(mElements);
				}
			}
			else
			{
				Empty();
			}
		}
		void Empty()
		{
			if (mElements)
			{
				CallDTor(mElements, mLength);
				Free(mElements);
			}
			mElements = nullptr;
			mLength = mCapacity = 0;
		}
		T& LastElement()
		{
			UASSERT(mElements && mLength > 0);
			return mElements[mLength - 1];
		}
		const T& LastElement() const
		{
			UASSERT(mElements && mLength > 0);
			return mElements[mLength - 1];
		}
		/*
		adds elements to the end of array without any initialization and returns index of the first added element
		*/
		size_t AddUnInit(size_t count = 1)
		{
			if (mCapacity < (mLength + count)) IncCapacity(count);
			auto tmp = mLength;
			mLength += count;
			return tmp;
		}
		//adds an element at the end of array and calls default constructor
		size_t Add()
		{
			if (mCapacity <= mLength) IncCapacity();
			new (mElements + mLength) T();
			mLength++;
			return mLength - 1;
		}
		//adds an element at the end of array
		size_t Add(const T& element)
		{
			if (mCapacity <= mLength) IncCapacity();
			new (mElements + mLength) T(element);
			mLength++;
			return mLength - 1;
		}
		size_t AddUnique(const T& element)
		{
			size_t i = Find(element);
			if (i == ~0) return Add(element);
			else return i;
		}
		//remove an element from the end of array and calls dtor
		void Pop()
		{
			UASSERT(mLength > 0);
			mLength--;
			(mElements + mLength)->~T();
		}
		//remove an element at the specified index and move last element to it
		void RemoveAtSwap(size_t index)
		{
			UASSERT(index < mLength);
			mLength--;
			(mElements + index)->~T();
			if (index != mLength) // isn't last element?
			{
				new (mElements + index) T(mElements[mLength]); // move last element to removed index
				(mElements + mLength)->~T(); // destruct las element
			}
		}
// 		//doesn't call ctor dtor
// 		void RemoveSwap(size_t index, size_t count)
// 		{
// 			XASSERT(mLength > 0 && index < mLength && index + count <= mLength);
// 			const size_t n = Min(count, mLength - (index + count));
// #if 1
// 			MemCopy(mElements + index, mElements + (mLength - n), sizeof(T) * n);
// #else
// 			for (size_t i = 0; i < n; i++)
// 			{
// 				CopyObject<T>([index + i], mElements[mLength - n + i]);
// 			}
// #endif
// 			mLength -= count;
// 		}
		//doesn't call ctor dtor
// 		void RemoveShift(size_t index, size_t count = 1)
// 		{
// 			XASSERT(index < mLength && count > 0);
// 			XASSERT(index + count <= mLength);
// 			size_t n = index + count;
// 			MemMove(mElements + index, mElements + n, (mLength - n) * sizeof(T));
// 			mLength -= count;
// 		}
		void Shrink()
		{
			UASSERT(mCapacity >= mLength);
			SetCapacity(mLength);
		}
		size_t Length() const { return mLength; }
		size_t Capacity() const { return mCapacity; }
		size_t Unused() const { return mCapacity - mLength; }

		T* Elements() const { return mElements; }

		const T& operator [] (size_t index) const
		{
			UASSERT(mElements && index < mLength);
			return mElements[index];
		}
		T& operator [] (size_t index)
		{
			UASSERT(mElements && index < mLength);
			return mElements[index];
		}


		size_t Find(const T& element) const
		{
			for (size_t i = 0; i < mLength; i++)
				if (element == mElements[i]) return i;
			return ~0;
		}
		size_t FindLast(const T& element) const
		{
			size_t find = ~0;
			for (size_t i = 0; i < mLength; i++)
				if (element == mElements[i])
					find = i;
			return find;
		}


		template < typename Lambda> void ForEach(Lambda proc)
		{
			for (size_t i = 0; i < mLength; i++)
				proc(mElements[i]);
		}
		//POD only
		template<typename Lambda> void RemoveIf(Lambda proc)
		{
			size_t j = 0;
			for (size_t i = 0; i < mLength; i++)
			{
				if (proc(mElements[i]))
				{
					continue;
				}
				else
				{
					mElements[j] = mElements[i];
					
					j++;
				}
			}
			mLength = j;
		}

	};
};