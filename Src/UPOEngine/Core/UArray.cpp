#include "UArray.h"

#include "../Meta/UClassInfo.h"

namespace UPO
{

	void SerArray::CallDTor(size_t begin, size_t count, EPropertyType elementType, const TypeInfo* elementTypeInfo)
	{
		for (size_t i = 0; i < count; i++)
		{
			if (void* element = GetElement(begin + i, elementType, elementTypeInfo))
			{
				if (elementType == EPropertyType::EPT_MetaClass)
				{
					UASSERT(elementTypeInfo);
					UASSERT(elementTypeInfo->Cast<ClassInfo>());
					elementTypeInfo->Cast<ClassInfo>()->Call_Destructor(element);
				}
			}
		}
	}
	void SerArray::CallDCTor(size_t begin, size_t count, EPropertyType elementType, const TypeInfo* elementTypeInfo)
	{
		//array element type should not be TArray or unknown 
		UASSERT(elementType != EPropertyType::EPT_TArray && elementType != EPropertyType::EPT_Unknown);

		char* start = (char*)GetElement(begin, elementType, elementTypeInfo);
		char* end = (char*)GetElement(begin + count, elementType, elementTypeInfo);
		//zero initialization first
		MemZero(start, (size_t)(end - start));

		for (size_t i = 0; i < count; i++)
		{
			if (void* element = GetElement(begin + i, elementType, elementTypeInfo))
			{
				if (elementType == EPropertyType::EPT_MetaClass)
				{
					UASSERT(elementTypeInfo);
					UASSERT(elementTypeInfo->Cast<ClassInfo>());
					elementTypeInfo->Cast<ClassInfo>()->Call_DefaultConstructor(element);
				}
				else if (elementType == EPropertyType::EPT_TObjectPtr)
				{
					new (element) TObjectPtr<Object>();
				}
				//other types don't need constructor
			}
		}
	}

	void SerArray::RemoveAll(EPropertyType elementType, const TypeInfo* elementTypeInfo)
	{
		CallDTor(0, mLength, elementType, elementTypeInfo);
		mLength = 0;
	}

	void SerArray::Empty(EPropertyType elementType, const TypeInfo* elementTypeInfo)
	{
		RemoveAll(elementType, elementTypeInfo);
		if (mElements) MemFree(mElements);
		mElements = nullptr;
		mCapacity = 0;
	}

	void* SerArray::GetElement(size_t index, EPropertyType elementType, const TypeInfo* elementTypeInfo)
	{
		if (index > mLength) return nullptr;

		if (elementType == EPropertyType::EPT_MetaClass)
		{
			UASSERT(elementTypeInfo);
			return mElements + elementTypeInfo->GetSize() * index;
		}

		return mElements + PropertyType_GetTypeSize(elementType) * index;
	}

	void SerArray::SetCapacity(size_t newCapacity, EPropertyType elementType, const TypeInfo* elementTypeInfo)
	{
		size_t typeSize = PropertyType_GetTypeSize(elementType);
		if (elementType == EPT_MetaClass)
		{
			UASSERT(elementTypeInfo);
			typeSize = elementTypeInfo->GetSize();
		}

		if (newCapacity)
		{
			mCapacity = newCapacity;
			auto newLength = Min(mLength, mCapacity);

			if (mElements)
			{
				if (newLength < mLength)
					CallDTor(newLength, mLength - newLength, elementType, elementTypeInfo);
				mLength = newLength;
				mElements = (byte*)MemRealloc(mElements, newCapacity * typeSize);
				UASSERT(mElements);
			}
			else
			{
				mLength = newLength;
				mElements = (byte*)MemAlloc(newCapacity * typeSize);
				UASSERT(mElements);
			}
		}
		else
		{
			Empty(elementType, elementTypeInfo);
		}
	}

	size_t SerArray::AddDefault(size_t count, EPropertyType elementType, const TypeInfo* elementTypeInfo)
	{
		if (mCapacity <= (mLength + count))
			SetCapacity(mCapacity + count, elementType, elementTypeInfo);
		auto tmp = mLength;
		mLength += count;
		CallDCTor(tmp, count, elementType, elementTypeInfo);
		return tmp;
	}

};