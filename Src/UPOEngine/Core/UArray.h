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
		friend class HelperArray;
		friend class String;

	public:
		class Iter
		{
		public:
			T* ptr;
			Iter(T* p) : ptr(p) {}
			bool operator !=(Iter rhs) const { return ptr != rhs.ptr; }
			T& operator*() { return *ptr; }
			void operator++() { ptr++; }
		};
		class ConstIter
		{
		public:
			T* ptr;
			ConstIter(T* p) : ptr(p) {}
			bool operator !=(ConstIter rhs) const { return ptr != rhs.ptr; }
			const T& operator*() const { return *ptr; }
			void operator++() { ptr++; }
		};

		Iter begin() { return Iter(mElements); }
		Iter end() { return Iter(mElements + mLength); }
		ConstIter begin() const { return ConstIter(mElements); }
		ConstIter end() const { return ConstIter(mElements + mLength); }
		ConstIter cbegin() const { return begin(); }
		ConstIter cend() const { return cend(); }

	private:
		T*			mElements;
		size_t		mLength;
		size_t		mCapacity;

		static T* Alloc(size_t size)
		{
// 			ULOG_MESSAGE("%zu bytes allocated", size);
			return (T*)MemAlloc(size);
		}
		static T* Realloc(T* memory, size_t newSize)
		{
// 			ULOG_SUCCESS("%zu bytes allocated ", newSize);
			return (T*)MemRealloc(memory, newSize);
		}
		static void Free(T* memory)
		{
			MemFree(memory);
		}
		static void CallDCTor(T* elements, size_t numElement)
		{
			for (size_t i = 0; i < numElement; i++)
				new (elements + i) T();
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
		static const size_t MININC = 1024 / sizeof(T);
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
			if(initialCapacity)
			{
				mLength = 0;
				mCapacity = initialCapacity;
				mElements = Alloc(initialCapacity * sizeof(T));
				UASSERT(mElements);
			}
			else
			{
				mElements = nullptr;
				mLength = mCapacity = 0;
			}
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
			if (mCapacity <= (mLength + numElement))
				IncCapacity(Max(MININC, numElement));
			CallCCTor(mElements + mLength, elements, numElement);
			mLength += numElement;
		}
		void Append(const std::initializer_list<T>& list)
		{
			Append(list.begin(), list.size());
		}
		/*
			Increasing capacity may perform Alloc or Realloc, it uses MemCopy not calling DTor CTor
		*/
		void IncCapacity(size_t numItem =  MININC)
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
		/*
		adds elements to the end of array with default constructor initialization and returns index of the first added element
		*/
		size_t AddDefault(size_t count = 1)
		{
			if (mCapacity < (mLength + count)) IncCapacity(count);
			auto tmp = mLength;
			mLength += count;
			CallDCTor(mElements + tmp, count);
			return tmp;
		}
		size_t Add(const std::initializer_list<T>& list)
		{
			auto tmp = mLength;
			Append(list);
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
		void Pop(T& out)
		{
			UASSERT(mLength > 0);
			mLength--;
			out = mElements[mLength];
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
		bool RemoveSwap(const T& element)
		{
			size_t index = Find(element);
			if (index != ~0)
			{
				RemoveAtSwap(index);
				return true;
			}
			return false;
		}
		void RemoveAll()
		{
			CallDTor(mElements, mLength);
			mLength = 0;
		}
		//POD only, doest call ctor dtor
		void Reverse()
		{
			int temp;
			int start = 0;
			int end = mLength - 1;
			while (start < end)
			{
				T temp = mElements[start];
				mElements[start] = mElements[end];
				mElements[end] = temp;
				start++;
				end--;
			}
// 			for (int i = 0, int j = mLength - 1; i < (mLength / 2); i++, j--)
// 			{
// 				T temp = mElements[i];
// 				mElements[i] = mElements[j];
// 				mElements[j] = temp;
// 			}
		}
		operator TArray<const T>& () { return *((TArray<const T>*)this); }

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

		//only calls destructor on removing elements
		void RemoveShift(size_t index, size_t count = 1)
		{
			UASSERT(index < mLength && count > 0);
			UASSERT(index + count <= mLength);
			size_t n = index + count;
			CallDTor(mElements + index, count);
			MemMove(mElements + index, mElements + n, (mLength - n) * sizeof(T));
			mLength -= count;
		}
		void Shrink()
		{
			UASSERT(mCapacity >= mLength);
			SetCapacity(mLength);
		}
		size_t Length() const { return mLength; }
		size_t Capacity() const { return mCapacity; }
		size_t Unused() const { return mCapacity - mLength; }

		T* Elements() const { return mElements; }
		T* ElementAt(size_t index) const { return mElements + index; }
		T* ElementAtSafe(size_t index) const { return (IsIndexValid(index) ? mElements + index : nullptr); }
		
		//checks whether 'ptr' is in array bound or not
		bool IsWithin(const void* ptr) const
		{
			return ((size_t)ptr) >= ((size_t)mElements) && ((size_t)ptr) < ((size_t)(mElements + mLength));
		}
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
		bool IsIndexValid(size_t index) const
		{
			return index < mLength;
		}
// 		size_t Find(const T element) const
// 		{
// 			for (size_t i = 0; i < mLength; i++)
// 				if (element == mElements[i]) return i;
// 			return ~0;
// 		}
		size_t Find(const T& element) const
		{
			for (size_t i = 0; i < mLength; i++)
				if (element == mElements[i]) return i;
			return ~0;
		}
		//lambda must return true if element found
		template<typename Lambda> size_t FindLambda(Lambda compareLambda) const
		{
			for (size_t i = 0; i < mLength; i++)
				if (compareLambda(mElements[i])) return i;
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
		template<typename Lambda> size_t FindLastLambda(Lambda compareLambda) const
		{
			size_t find = ~0;
			for (size_t i = 0; i < mLength; i++)
				if (compareLambda(mElements[i]))
					find = i;
			return find;
		}
		bool HasElement(const T& element) const { return Find(element) != ~0; }

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
	//////////////////////////////////////////////////////////////////////////
	class TypeInfo;
	enum EPropertyType;

	////cast TArray to this class to manipulate TArray for serialization or editor stuff, 
	//////////////////////////////////////////////////////////////////////////
	class UAPI SerArray
	{
	public:
		byte*	mElements;
		size_t	mLength;
		size_t  mCapacity;

		void CallDTor(size_t begin, size_t count, EPropertyType elementType, const TypeInfo* elementTypeInfo);
		void CallDCTor(size_t begin, size_t count, EPropertyType elementType, const TypeInfo* elementTypeInfo);

		void RemoveAll(EPropertyType elementType, const TypeInfo* elementTypeInfo);
		void Empty(EPropertyType elementType, const TypeInfo* elementTypeInfo);
		void* GetElement(size_t index, EPropertyType elementType, const TypeInfo* elementTypeInfo);
		void SetCapacity(size_t newCapacity, EPropertyType elementType, const TypeInfo* elementTypeInfo);
		size_t AddDefault(size_t count, EPropertyType elementType, const TypeInfo* elementTypeInfo);
	};
};