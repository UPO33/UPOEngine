#include "UStream.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	StreamReaderMemory::StreamReaderMemory(unsigned initialCapacity)
		: mBuffer(initialCapacity)
	{
		mIsReader = true;
	}
	//////////////////////////////////////////////////////////////////////////
	StreamReaderMemory::~StreamReaderMemory()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	unsigned StreamReaderMemory::GetSeek() const
	{
		return mBuffer.Length();
	}
	//////////////////////////////////////////////////////////////////////////
	Stream& StreamReaderMemory::Bytes(void* bytes, unsigned length)
	{
		mBuffer.Append((const char*)bytes, length);
		return *this;
	}




	//////////////////////////////////////////////////////////////////////////
	StreamWriterMemory::StreamWriterMemory(void* data, size_t size)
	{
		mIsReader = false;
		mData = (char*)data;
		mSize = size;
		mPos = 0;
	}
	//////////////////////////////////////////////////////////////////////////
	StreamWriterMemory::~StreamWriterMemory()
	{
		mData = nullptr;
		mSize = 0;
		mPos = 0;
	}
	//////////////////////////////////////////////////////////////////////////
	Stream& StreamWriterMemory::Bytes(void* bytes, unsigned length)
	{
		MemCopy(bytes, mData + mPos, length);
		mPos += length;
		return *this;
	}

}