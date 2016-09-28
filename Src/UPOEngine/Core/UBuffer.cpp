#include "UBuffer.h"
#include "UMemory.h"
#include "../Meta/UMeta.h"
#include "UStream.h"

namespace UPO
{
	UCLASS_BEGIN_IMPL(Buffer)
	UCLASS_END_IMPL(Buffer)

	//////////////////////////////////////////////////////////////////////////
	Buffer::Buffer(size_t size, void* initialData)
	{
		if (size)
		{
			mSize = size;
			mBuffer = MemAlloc(size);
			UASSERT(mBuffer);
			if (initialData)
			{
				MemCopy(mBuffer, initialData, size);
			}
		}
		else
		{
			mBuffer = nullptr;
			mSize = 0;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	Buffer::Buffer(const Buffer& copy)
	{
		if (copy.mBuffer)
		{
			mSize = copy.mSize;
			mBuffer = MemAlloc(copy.mSize);
			UASSERT(mBuffer);
			MemCopy(mBuffer, copy.mBuffer, mSize);
		}
		else
		{
			mBuffer = nullptr;
			mSize = 0;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	Buffer& Buffer::operator=(const Buffer& other)
	{
		Free();
		new (this) Buffer(other);
		return *this;
	}
	//////////////////////////////////////////////////////////////////////////
	Buffer::~Buffer()
	{
		Free();
	}
	//////////////////////////////////////////////////////////////////////////
	void Buffer::Free()
	{
		if (mBuffer) MemFree(mBuffer);
		mBuffer = nullptr;
		mSize = 0;
	}
	//////////////////////////////////////////////////////////////////////////
	void Buffer::MetaSerialize(Stream& stream)
	{
		uint32 size = (uint32)mSize;
		if (stream.IsReader())
		{
			stream.RW(size);
			if (size) stream.Bytes(mBuffer, size);
		}
		else
		{
			stream.RW(size);
			if (size)
			{
				mSize = size;
				mBuffer = MemAlloc(size);
				UASSERT(mBuffer);
				stream.Bytes(mBuffer, size);
			}
			else
			{
				mSize = 0;
				mBuffer = nullptr;
			}
		}
	}

};