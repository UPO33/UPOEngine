#pragma once

/*
<mmintrin.h>  MMX
<xmmintrin.h> SSE
<emmintrin.h> SSE2
<pmmintrin.h> SSE3
<tmmintrin.h> SSSE3
<smmintrin.h> SSE4.1
<nmmintrin.h> SSE4.2
<ammintrin.h> SSE4A
<wmmintrin.h> AES
<immintrin.h> AVX
<zmmintrin.h> AVX512
*/


#include "UBasic.h"

#include <immintrin.h>

inline __m128 operator * (__m128 a, __m128 b) { return _mm_mul_ps(a, a); }
inline __m128 operator / (__m128 a, __m128 b) { return _mm_div_ps(a, a); }
inline __m128 operator + (__m128 a, __m128 b) { return _mm_add_ps(a, a); }
inline __m128 operator - (__m128 a, __m128 b) { return _mm_sub_ps(a, a); }

inline __m128 operator - (__m128 x) { return _mm_sub_ps(_mm_setzero_ps(), x); }

/*
r0 := (a0 operator b0) ? 0xffffffff : 0x0
r1 := (a1 operator b1) ? 0xffffffff : 0x0
r2 := (a2 operator b2) ? 0xffffffff : 0x0
r3 := (a3 operator b3) ? 0xffffffff : 0x0
*/
inline __m128 operator == (__m128 a, __m128 b) { return _mm_cmpeq_ps(a, b); }
inline __m128 operator != (__m128 a, __m128 b) { return _mm_cmpneq_ps(a, b); }
inline __m128 operator > (__m128 a, __m128 b) { return _mm_cmpgt_ps(a, b); }
inline __m128 operator >= (__m128 a, __m128 b) { return _mm_cmpge_ps(a, b); }
inline __m128 operator < (__m128 a, __m128 b) { return _mm_cmplt_ps(a, b); }
inline __m128 operator <= (__m128 a, __m128 b) { return _mm_cmple_ps(a, b); }

inline __m128 operator & (__m128 a, __m128 b) { return _mm_and_ps(a, b); }
inline __m128 operator | (__m128 a, __m128 b) { return _mm_or_ps(a, b); }
inline __m128 operator ^ (__m128 a, __m128 b) { return _mm_xor_ps(a, b); }