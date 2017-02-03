#include "UStream.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	StreamReaderMemory::StreamReaderMemory(unsigned initialCapacity)
		: mBuffer(initialCapacity)
	{
		mIsReader = true;
		mHasError = false;
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
		if (length && !mHasError)
		{
			UASSERT(bytes);
			mBuffer.Append((const char*)bytes, length);
		}
		return *this;
	}




	//////////////////////////////////////////////////////////////////////////
	StreamWriterMemory::StreamWriterMemory(void* data, size_t size)
	{
		mIsReader = false;
		mHasError = false;
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
		if (length && !mHasError)
		{
			UASSERT(bytes);
			size_t nFree = mSize - mPos;
			if (length <= nFree)
			{
				MemCopy(bytes, mData + mPos, length);
				mPos += length;
			}
			else
			{
				mHasError = true;
				UASSERT(!mHasError);
			}
		}
		
		return *this;
	}


	//////////////////////////////////////////////////////////////////////////
	StreamWriterFile::StreamWriterFile(const String& filename)
	{
		mIsReader = false;
		mPos = 0;
		mFile.Open(filename, EFileOpenMode::Read);
		mHasError = !mFile.IsOpen();
	}
	//////////////////////////////////////////////////////////////////////////
	StreamWriterFile::~StreamWriterFile()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	Stream& StreamWriterFile::Bytes(void* bytes, unsigned length)
	{
		if (!mHasError && length)
		{
			UASSERT(bytes);
			size_t nRead = mFile.ReadBytes(bytes, length);
			mPos += nRead;
			mHasError = nRead != length;
			UASSERT(!mHasError);
		}
		
		return *this;
	}
	//////////////////////////////////////////////////////////////////////////
	StreamReaderFile::StreamReaderFile(const String& filename)
	{
		mIsReader = true;
		mPos = 0;
		mHasError = !mFile.Open(filename, EFileOpenMode::Write);
	}
	//////////////////////////////////////////////////////////////////////////
	StreamReaderFile::~StreamReaderFile()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	Stream& StreamReaderFile::Bytes(void* bytes, unsigned length)
	{
		if (!mHasError && length)
		{
			UASSERT(bytes);
			size_t nWrite = mFile.WriteBytes(bytes, length);
			mPos += nWrite;
			mHasError = nWrite != length;
			UASSERT(!mHasError);
		}
		return *this;
	}

}