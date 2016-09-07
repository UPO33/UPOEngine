#pragma once

#include "UBasic.h"

namespace UPO
{
#if UPLATFORM_WIN
	static const char PATH_SEPARATOR_CHAR = '\\';
#else
	static const char PATH_SEPARATOR_CHAR = '/';
#endif

	inline size_t StrLen(const char* str) { return strlen(str); }
	inline bool StrBeginWith(const char* str, const char* begin) { return strstr(str, begin) == str; }
	inline bool StrCopy(char* dst, const char* src, size_t bufferSize) { return strcpy_s(dst, bufferSize, src) == 0; }
	UAPI bool StrTakeBetweenFirstLast(const char* str, char* out, char first, char last);
	UAPI size_t StrCountChar(const char* str, char chr);
	UAPI const char* StrFindNChar(const char* str, char chr, unsigned n);
	UAPI const char* StrFindRNChar(const char* str, char chr, unsigned n);
};