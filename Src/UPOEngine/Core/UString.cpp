#include "UString.h"

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
};