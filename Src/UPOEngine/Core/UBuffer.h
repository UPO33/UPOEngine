#pragma once

#include "UBasic.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class UAPI Buffer
	{
		UCLASS(Buffer, void)

		void*	mBuffer;
		size_t	mSize;

	public:
		Buffer() : mBuffer(nullptr), mSize(0) {}
		Buffer(size_t size, void* initialData = nullptr);
		Buffer(const Buffer& copy);
		Buffer& operator = (const Buffer& other);
		~Buffer();
		void Free();
		void* Data() const { return mBuffer; }
		size_t Size() const { return mSize; }

		
		void MetaSerialize(Stream&);
	};
};