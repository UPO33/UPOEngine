#pragma once

#include "UBasic.h"
#include "UArray.h"
#include "UFileSys.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////fwd decl
	class String;
	class Name;

	/*
	Base class for streaming data, reader means this class only read data
	*/
	class UAPI Stream
	{
	protected:
		bool mIsReader;
		bool mHasError;

	public:
		//read/write a variable
		template<typename T> Stream& RW(T& value)
		{
			if (std::is_arithmetic<T>::value)
			{
				Bytes(&value, sizeof(T));
			}
// 			else if (const bool isMeta = XCLASS_ISMETA(T))
// 			{
// 				std::conditional<isMeta, T, T>::type::GetClassInfoStatic()
// 			}
			//static_assert(std::is_arithmetic<T>::value, "Only arithmetic types are allowed");
			Bytes(&value, sizeof(T));
			return *this;
		}
		
		virtual ~Stream() {}

		//indicates whether this stream reads data or writes
		bool IsReader() const { return mIsReader; }
		//if the stream has error it doesn't read/write
		bool HasError() const { return mHasError; }
		//number of bytes read or written
		virtual unsigned GetSeek() const = 0;

		//ignore numByte if is writer stream, nothing do on reading
		virtual Stream& Ignore(unsigned numByte) { return *this; }
		//read/write bytes
		virtual Stream& Bytes(void* bytes, unsigned length) = 0;

// 		virtual unsigned PushSizeToken() {};
// 		virtual void PopSizeToken() {};
	};


	/*
	a simple stream which writes the taken data to memory
	*/
	class UAPI StreamReaderMemory : public Stream
	{
		TArray<char> mBuffer;
	public:
		StreamReaderMemory(unsigned initialCapacity = 0);
		~StreamReaderMemory();

		virtual unsigned GetSeek() const override;
		virtual Stream& Bytes(void* bytes, unsigned length) override;
		void* GetHead() { return mBuffer.Elements(); }
		
	};

	/*
	a simple stream which takes data from memory and writes
	*/
	class UAPI StreamWriterMemory : public Stream
	{
		char* mData;
		size_t mSize;
		size_t mPos;
	public:
		StreamWriterMemory(void* data, size_t size);
		~StreamWriterMemory();

		virtual Stream& Bytes(void* bytes, unsigned length) override;
		virtual unsigned GetSeek() const override { return mPos;}
		void* GetHead() const { return mData; }
	};

	/*
		a stream which writes the taken data to a file
	*/
	class StreamReaderFile : public Stream
	{
		File mFile;
		size_t mPos;

	public:
		StreamReaderFile(const String& filename);
		~StreamReaderFile();

		virtual Stream& Bytes(void* bytes, unsigned length) override;
		virtual unsigned GetSeek() const override { return mPos; }
	};


	/*
		the stream which writes data from a file
	*/
	class UAPI StreamWriterFile : public Stream
	{
		File mFile;
		size_t mPos;

	public:
		StreamWriterFile(const String& filename);
		~StreamWriterFile();

		virtual Stream& Bytes(void* bytes, unsigned length) override;
		virtual unsigned GetSeek() const override { return mPos; }

	};
};