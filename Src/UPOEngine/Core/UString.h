#pragma once

#include "UBasic.h"
#include "UMemory.h"
#include "UArray.h"

namespace UPO
{
#if UPLATFORM_WIN
	static const char PATH_SEPARATOR_CHAR = '\\';
#else
	static const char PATH_SEPARATOR_CHAR = '/';
#endif

	//////////////////////////////////////////////////////////////////////////
	class Stream;
	class Name;

	//////////////////////////////////////////////////////////////////////////
	inline size_t StrLen(const char* str) { return strlen(str); }
	inline bool StrBeginWith(const char* str, const char* begin) { return strstr(str, begin) == str; }
	inline bool StrCopy(char* dst, const char* src, size_t bufferSize) { return strcpy_s(dst, bufferSize, src) == 0; }
	UAPI bool StrTakeBetweenFirstLast(const char* str, char* out, char first, char last);
	UAPI size_t StrCountChar(const char* str, char chr);
	UAPI const char* StrFindNChar(const char* str, char chr, unsigned n);
	UAPI const char* StrFindRNChar(const char* str, char chr, unsigned n);


	//////////////////////////////////////////////////////////////////////////
	class UAPI String
	{
		UCLASS(String, void)
	private:
		struct UAPI Chunk
		{
			size_t		mLen;
			char		mChars[0xFF]; // fake array size

			static Chunk Empty;
		};
		//allocate a Str and set length
		static Chunk* AllocChunk(size_t length)
		{
			Chunk* newIns = (Chunk*)MemAlloc(sizeof(size_t) + (length * sizeof(char)) + sizeof(char));
			newIns->mLen = length;
			return newIns;
		}
	public:
		Chunk* mStr;
	public:
		static String Empty;

		String()
		{
			mStr = nullptr;
		}
		String(const char* str)
		{
			if (str && str[0])
			{
				size_t len = StrLen(str);
				new (this) String(str, len);
			}
			else
			{
				new (this) String();
			}
		}
		String(const char* str, size_t length)
		{
			if (length)
			{
				UASSERT(str);
				mStr = AllocChunk(length);
				MemCopy(mStr->mChars, str, length);
				mStr->mChars[length] = 0;
			}
			else
			{
				new (this) String();
			}
		}
		String(const String& other)
		{
			if (other.mStr)
			{
				mStr = AllocChunk(other.mStr->mLen);
				MemCopy(mStr->mChars, other.mStr->mChars, other.mStr->mLen * sizeof(char) + sizeof(char));
			}
			else
			{
				new (this) String();
			}
		}
		String(const Name& name);

		String& operator = (const UPO::String& other)
		{
			if (other.mStr)
			{
				if (mStr == nullptr || mStr->mLen < other.mStr->mLen)
				{
					this->~String();
					mStr = AllocChunk(other.mStr->mLen);
				}
				mStr->mLen = other.mStr->mLen;
				MemCopy(mStr->mChars, other.mStr->mChars, mStr->mLen * sizeof(char) + sizeof(char));
			}
			else
			{
				this->~String();
			}
			return *this;
		}
		void SetEmpty()
		{
			if (mStr)
			{
				MemFree(mStr);
				mStr = nullptr;
			}
		}
		~String()
		{
			SetEmpty();
		}
		bool BeginsWith(const String& other) const
		{
// 			if (mStr == nullptr) return false;
// 			if (mStr->mLen < other.mStr->mLen) return false;
// 			for (unsigned i = 0; i < other.mStr->mLen; i++)
// 			{
// 				if (mStr->mChars[i] != other.mStr->mChars[i]) return false;
// 			}
			return true;
		}
		bool Equal(const String& other) const;
		bool operator == (const String& other) const
		{
			return Equal(other);
		}
		bool operator != (const String& other) const
		{
			return !Equal(other);
		}
		char& operator [] (unsigned index)
		{
			UASSERT(mStr && index < Length());
			return mStr->mChars[index];
		}		
		char* CStr() const
		{
			if(mStr) return mStr->mChars;
			return nullptr;
		}
		operator const char* () const
		{
			if(mStr) return mStr->mChars;
			return nullptr;
		}
		unsigned Length() const
		{
			if(mStr) return mStr->mLen;
			return 0;
		}
		bool IsEmpty() const
		{
			return mStr == nullptr;
		}

		unsigned FindNChar(char chr, unsigned n = 0) const;
		unsigned FindNCharReverse(char chr, unsigned n = 0) const;
		void Split(char seprator, TArray<String>& out) const;
		void Trim();
		
		String SubStr(unsigned index, unsigned count = ~0) const;

		bool ToFloat(float& out) const;
		void SetFormatted(const char* format, ...);
		void MetaSerialize(Stream&);
	};
};