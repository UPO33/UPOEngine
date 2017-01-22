#include "UString.h"
#include "../Meta/UMeta.h"

#include <string>
#include <sstream>
#include <stdarg.h>

namespace UPO
{
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
	String::String(const Name& name) : String(name.CStr(), name.Length())
	{
	}
	bool String::BeginsWith(const String& other, bool caseSensitive) const
	{
		if (Length() < other.Length()) return false;
		if (caseSensitive)
		{
			for (unsigned i = 0; i < other.Length(); i++)
				if (CStr()[i] != other.CStr()[i]) return false;
		}
		else
		{
			for (unsigned i = 0; i < other.Length(); i++)
				if (tolower(CStr()[i]) != tolower(other.CStr()[i])) return false;
		}

		return true;
	}
	bool String::Equal(const String& other, bool caseSensitive) const
	{
		if (Length() == other.Length())
		{
			if (caseSensitive)
			{
				for (unsigned i = 0; i < Length(); i++)
					if (CStr()[i] != other.CStr()[i]) return false;
			}
			else
			{
				for (unsigned i = 0; i < Length(); i++)
					if (tolower(CStr()[i]) != tolower(CStr()[i])) return false;
			}

			return true;
		}
		return false;
	}
	unsigned String::FindN(char chr, unsigned n, bool caseSensitive) const
	{
		unsigned numFound = 0;
		if(caseSensitive)
		{
			for (unsigned i = 0; i < Length(); i++)
			{
				if (CStr()[i] == chr)
				{
					if (numFound == n) return i;
					numFound++;
				}
			}
		}
		else
		{
			for (unsigned i = 0; i < Length(); i++)
			{
				if (tolower(CStr()[i]) == chr)
				{
					if (numFound == n) return i;
					numFound++;
				}
			}
		}
		return ~0;
	}
	unsigned String::FindRN(char chr, unsigned n, bool caseSensitive) const
	{
		if (Length() == 0) return ~0;

		unsigned numFound = 0;
		if (caseSensitive)
		{
			for (unsigned i = Length() - 1; i >= 0; i--)
			{
				if (CStr()[i] == chr)
				{
					if (numFound == n) return i;
					numFound++;
				}
			}
		}
		else
		{
			for (unsigned i = Length() - 1; i >= 0; i--)
			{
				if (tolower(CStr()[i]) == chr)
				{
					if (numFound == n) return i;
					numFound++;
				}
			}
		}
		return ~0;
	}
	//////////////////////////////////////////////////////////////////////////
	void String::Split(char seprator, TArray<String>& out, bool caseSensitive) const
	{
		out.RemoveAll();

		auto len = Length();
		if (len == 0) return;

		const char* str = CStr();
		size_t last = 0;

		for (size_t i = 0; i < len; i++)
		{
			if ((caseSensitive ? CStr()[i] : tolower(CStr()[i])) == seprator)
			{
				if (last == i) //there is no string between separators
				{
				}
				else
				{
					out.AddDefault();
					out.LastElement() = String(str + last, i - last);
				}

				last = i + 1;
			}
		}

		if (last == 0) // nothing found
		{
		}
		else if (last == len) //token is last char
		{
		}
		else
		{
			out.AddDefault();
			out.LastElement() = String(str + last, len - last);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void String::Trim()
	{
		int len = Length();
		if (len == 0) return;
		const char* str = CStr();

		int firstNonSpace = 0;
		for (; firstNonSpace < len; firstNonSpace++)
		{
			if (!isspace(str[firstNonSpace])) break;
		}

		if (firstNonSpace == len) // all space?
		{
			SetEmpty();
			return;
		}

		int lastNonSpace = len - 1;
		for (; lastNonSpace >= 0; lastNonSpace--)
		{
			if (!isspace(str[lastNonSpace])) break;
		}

		int newLen = lastNonSpace - firstNonSpace + 1;

		if (firstNonSpace == 0) // there is no space on left so memcpy not required
		{
		}
		else
		{
			MemCopy(CStr(), CStr() + firstNonSpace, newLen);
		}
		mChars.mLength = newLen;
		mChars.mElements[newLen] = '\0';
	}
	//////////////////////////////////////////////////////////////////////////
	String String::operator+(char chr) const
	{
		String ret = *this;
		ret += chr;
		return ret;
	}
	//////////////////////////////////////////////////////////////////////////
	String& String::operator+=(char chr)
	{
		mChars.IncCapacity(1);
		mChars.mElements[mChars.mLength++] = chr;
		return *this;
	}


	String& String::Replace(char x, char y, bool caseSensitive)
	{
		if (caseSensitive)
		{
			for (size_t i = 0; i < Length(); i++)
			{
				if (CStr()[i] == x) CStr()[i] = y;
			}
		}
		else
		{
			for (size_t i = 0; i < Length(); i++)
			{
				if (tolower(CStr()[i]) == x) CStr()[i] = y;
			}
		}
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	String String::SubStr(unsigned index, unsigned count /*= ~0*/) const
	{
		UASSERT(index < Length());

		if (count == ~0)
		{
			count = Length() - index;
			UASSERT((index + count) <= Length());
		}

		return String(CStr() + index, count);
	}



	bool String::ToFloat(float& out) const
	{
		out = 0;
		out = strtof(CStr(), nullptr);
		return true;

		// 		if(out == 0)
		// 		{
		// 			std::istringstream iss(std::string(mStr->mChars));
		// 			float f;
		// 			iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
		// 			out = f;
		// 			return iss.eof() && !iss.fail();
		// 		}
	}
	//////////////////////////////////////////////////////////////////////////
	void String::SetFormatted(const char* format, ...)
	{
		char buffer[2048];
		va_list args;
		va_start(args, format);
		vsprintf_s(buffer, sizeof(buffer), format, args);
		va_end(args);

		*this = buffer;
	}

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
			
			if (len)
			{
				mChars.SetCapacity(len + 1);
				mChars.mLength = len;
				stream.Bytes(mChars.mElements, len);
				mChars.mElements[len] = '\0';
			}
		}
	}

	String String::MakeFrom(int n)
	{
		char buffer[128] = { '\0' };
		_itoa_s(n, buffer, 10);
		return String(buffer);
	}

	//////////////////////////////////////////////////////////////////////////
	String String::operator+(const String& str) const
	{
		String ret = *this;
		ret += str;
		return ret;
	}
	//////////////////////////////////////////////////////////////////////////
	String& String::operator+=(const String& str)
	{
		if (str.Length())
		{
			mChars.IncCapacity(str.Length() + 1);
			MemCopy(CStr() + Length(), str.CStr(), str.Length() + 1);
			mChars.mLength += str.Length();
		}
		return *this;
	}




#if 0
	bool String::BeginsWith(const String& other, bool caseSensitive) const
	{
		if (mStr == nullptr) return false;
		if (mStr->mLen < other.mStr->mLen) return false;
		if (caseSensitive)
		{
			for (unsigned i = 0; i < other.mStr->mLen; i++)
				if (mStr->mChars[i] != other.mStr->mChars[i]) return false;
		}
		else
		{
			for (unsigned i = 0; i < other.mStr->mLen; i++)
				if (tolower(mStr->mChars[i]) != tolower(other.mStr->mChars[i])) return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool String::Equal(const String& other, bool caseSensitive) const
	{
		if (mStr->mLen == other.mStr->mLen)
		{
			if(caseSensitive)
			{
				for (unsigned i = 0; i < mStr->mLen; i++)
					if (mStr->mChars[i] != other.mStr->mChars[i]) return false;
			}
			else
			{
				for (unsigned i = 0; i < mStr->mLen; i++)
					if (tolower(mStr->mChars[i]) != tolower(other.mStr->mChars[i])) return false;
			}

			return true;
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	unsigned String::FindNChar(char chr, unsigned n /*= 0*/) const
	{
		if(mStr)
		{
			unsigned numFound = 0;
			for (unsigned i = 0; i < mStr->mLen; i++)
			{
				if (mStr->mChars[i] == chr)
				{
					if (numFound == n) return i;
					numFound++;
				}
			}
		}
		return ~0;
	}
	//////////////////////////////////////////////////////////////////////////
	unsigned String::FindNCharReverse(char chr, unsigned n /*= 0*/) const
	{
		if(mStr)
		{
			unsigned numFound = 0;
			for (unsigned i = mStr->mLen - 1; i >= 0; i--)
			{
				if (mStr->mChars[i] == chr)
				{
					if (numFound == n) return i;
					numFound++;
				}
			}
		}
		return ~0;
	}
	//////////////////////////////////////////////////////////////////////////
	void String::Split(char seprator, TArray<String>& out) const
	{
			out.RemoveAll();

			auto len = Length();
			if (len == 0) return;

			const char* str = CStr();
			size_t last = 0;

			for (size_t i = 0; i < len; i++)
			{
				if (str[i] == seprator)
				{
					if (last == i) //there is no string between separators
					{
					}
					else
					{
						out.AddDefault();
						out.LastElement() = String(str + last, i - last);
					}

					last = i + 1;
				}
			}

			if (last == 0) // nothing found
			{
			}
			else if (last == len) //token is last char
			{
			}
			else
			{
				out.AddDefault();
				out.LastElement() = String(str + last, len - last);
			}
	}
	

	void String::Trim()
	{
		if (mStr)
		{
			int len = Length();
			if (len == 0) return;
			const char* str = CStr();

			int firstNonSpace = 0;
			for (; firstNonSpace < len; firstNonSpace++)
			{
				if (!isspace(str[firstNonSpace])) break;
			}

			if (firstNonSpace == len ) // all space?
			{
				SetEmpty();
				return;
			}

			int lastNonSpace = len - 1;
			for (; lastNonSpace >= 0; lastNonSpace--)
			{
				if (!isspace(str[lastNonSpace])) break;
			}

			int newLen = lastNonSpace - firstNonSpace + 1;

			if (firstNonSpace == 0) // there is no space on left so memcpy not required
			{ }
			else
			{
				MemCopy(mStr->mChars, mStr->mChars + firstNonSpace, newLen);
			}
			mStr->mLen = newLen;
			mStr->mChars[newLen] = 0;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	String String::SubStr(unsigned index, unsigned count /*= ~0*/) const
	{
		UASSERT(index < Length());

		if (count == ~0)
		{
			count = Length() - index;
			UASSERT((index + count) <= Length());
		}

		return String(CStr() + index, count);
	}

	bool String::ToFloat(float& out) const
	{
		out = 0;

		if (mStr == nullptr) return false;

		out = strtof(mStr->mChars, nullptr);
		return true;

		// 		if(out == 0)
		// 		{
		// 			std::istringstream iss(std::string(mStr->mChars));
		// 			float f;
		// 			iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
		// 			out = f;
		// 			return iss.eof() && !iss.fail();
		// 		}


	}
	//////////////////////////////////////////////////////////////////////////
	void String::SetFormatted(const char* format, ...)
	{
		char buffer[2048];
		va_list args;
		va_start(args, format);
		vsprintf_s(buffer, sizeof(buffer), format, args);
		va_end(args);

		*this = buffer;
	}

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

	String::Chunk String::Chunk::Empty = { 0,{ 0,0,0,0,0,0,0,0 } };
#endif




	UCLASS_BEGIN_IMPL(String)
	UCLASS_END_IMPL(String)

};