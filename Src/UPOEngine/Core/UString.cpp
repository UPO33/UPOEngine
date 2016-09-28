#include "UString.h"
#include "../Meta/UMeta.h"

namespace UPO
{
	UCLASS_BEGIN_IMPL(String)
	UCLASS_END_IMPL(String)

	//////////////////////////////////////////////////////////////////////////
	bool StrTakeBetweenFirstLast(const char* str, char* out, char first, char last)
	{
		UASSERT(str && out);
		const char* iter = str;
		const char* firstFound = nullptr;
		const char* lastFound = nullptr;

		while (*iter)
		{
			if (*iter == first && firstFound == nullptr)
				firstFound = iter;
			if (*iter == last)
				lastFound = iter;
		}
		if (firstFound < lastFound)
		{
			unsigned iter = 1;

			while (firstFound + iter != lastFound)
			{
				out[iter] = firstFound[iter];
			}
			out[iter] = 0;
			return true;
		}
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	size_t StrCountChar(const char* str, char chr)
	{
		UASSERT(str);
		size_t n = 0;
		while (*str)
		{
			if (*str == chr) n++;
			str++;
		}
		return n;
	}
	//////////////////////////////////////////////////////////////////////////
	const char* StrFindNChar(const char* str, char chr, unsigned n)
	{
		UASSERT(str);
		while (*str)
		{
			if (*str == chr)
			{
				if (n == 0) return str;
				n--;
			}
			str++;
		}
		return nullptr;
	}
	const char* StrFindRNChar(const char* str, char chr, unsigned n)
	{
		UASSERT(str);
		auto len = StrLen(str);
		for (int i = len - 1; i >= 0; i--)
		{
			if (str[i] == chr)
			{
				if (n == 0) return str + i;
				n--;
			}
		}
		return nullptr;
	}



	UPO::String String::Empty;

	//////////////////////////////////////////////////////////////////////////
	void String::MetaSerialize(Stream& stream)
	{
		if (stream.IsReader())
		{
			uint16 len = (uint16)Length();
			stream.RW(len);
			if (len != 0) stream.Bytes(CStr(), len);
		}
		else
		{
			//by default we suppose that default constructor was called before stream writes
			uint16 len = 0;
			stream.RW(len);
			SetEmpty();
			if (len)
			{
				mStr = AllocChunk(len);
				UASSERT(mStr);
				stream.Bytes(mStr->mChars, len);
				mStr->mChars[len] = 0;
			}
		}
	}

	String::Chunk String::Chunk::Empty = { 0, {0,0,0,0,0,0,0,0} };

};