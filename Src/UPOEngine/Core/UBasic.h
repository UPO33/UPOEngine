#pragma once

#include <iostream>
#include <math.h>
#include <time.h>
#include <type_traits>
#include <functional>
#include <memory.h>
#include <assert.h>

#pragma region platforms compilers and api

#if defined(_MSC_VER)
#define UCOMPILER_MSVC
#define UMODULE_IMPORT __declspec(dllimport)
#define UMODULE_EXPORT __declspec(dllexport)
#else defined(__GNUC__)
#define UCOMPILER_GCC
#define UMODULE_IMPORT
#define UMODULE_EXPORT  __attribute__((visibility("default"))) 
#endif


#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#define UPLATFORM_WIN 1
#define UPLATFORM_ID  UPLATFORM_WIN
#endif
#if defined(__linux__) || defined(__gnu_linux__)
#define UPLATFORM_LINUX 2
#define UPLATFORM_ID  UPLATFORM_LINUX
#endif
#if defined(__ANDROID__)
#define UPLATFORM_ANDROID 3
#define UPLATFORM_ID  UPLATFORM_ANDROID
#endif




#if defined(UEXPORT_ENGINE)
#define UAPI UMODULE_EXPORT
#else
#define UAPI UMODULE_IMPORT
#endif

#pragma  endregion



#pragma region Log and assert

#define ULOG_MESSAGE(format, ...) UPO::Log::Get()->Add(UPO::ELogType::ELT_Message, __FILE__, __FUNCTION__, __LINE__, format, __VA_ARGS__)
#define ULOG_SUCCESS(format, ...) UPO::Log::Get()->Add(UPO::ELogType::ELT_Success, __FILE__, __FUNCTION__, __LINE__, format, __VA_ARGS__)
#define ULOG_ERROR(format, ...) UPO::Log::Get()->Add(UPO::ELogType::ELT_Error, __FILE__, __FUNCTION__, __LINE__, format, __VA_ARGS__)
#define ULOG_WARN(format, ...) UPO::Log::Get()->Add(UPO::ELogType::ELT_Warn, __FILE__, __FUNCTION__, __LINE__, format, __VA_ARGS__)
#define ULOG_FATAL(format, ...) UPO::Log::Get()->Add(UPO::ELogType::ELT_Fatal, __FILE__, __FUNCTION__, __LINE__, format, __VA_ARGS__)

#define UASSERT(expression) { if(!(expression)) UPO::Log::Get()->Add(UPO::ELogType::ELT_Assert, __FILE__, __FUNCTION__, __LINE__, "Assertaion Failed", #expression);}

#define UASSERT_GTONLY() UASSERT(UPO::IsGameThread())
#define UASSERT_RTONLY() UASSERT(UPO::IsRenderThread())

#pragma endregion


typedef unsigned int uint32;
typedef int int32;
typedef unsigned long long uint64;
typedef long long int64;


namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	UAPI bool IsGameThread();
	UAPI bool IsRenderThread();

	UAPI void AppCrash();

#pragma region function pointer helper
	//////////////////////////////////////////////////////////////////////////function pointer
	template<typename TRet, typename... TArgs> struct FP
	{
		using Pattern = TRet(*)(TArgs...);
		Pattern mFunction = nullptr;

		FP() {}
		template <typename T> FP(T pFunction) { mFunction = pFunction; }

		TRet operator() (TArgs... args) const
		{
			return mFunction(args...);
		}
		operator bool() const { return mFunction != nullptr; }
		bool operator == (const void* p) const { return mFunction == p; }
		bool operator != (const void* p) const { return mFunction != p; }
	};
	//////////////////////////////////////////////////////////////////////////member function pointer
	template<typename TClass, typename TRet, typename... TArgs> struct MFP
	{
		using Pattern = TRet(TClass::*)(TArgs...);
		Pattern mFunction = nullptr;

		MFP() {}
		template <typename T> MFP(T pFunction) { mFunction = pFunction; }

		TRet operator() (TClass* object, TArgs... args) const
		{
			return object->*mFunction(args...);
		}
		operator bool() const { return mFunction != nullptr; }
		bool operator == (const void* p) const { return mFunction == p; }
		bool operator != (const void* p) const { return mFunction != p; }
	};
#pragma endregion

#pragma region log

	//////////////////////////////////////////////////////////////////////////
	enum  ELogType
	{
		ELT_Message, ELT_Success, ELT_Warn, ELT_Error, ELT_Fatal, ELT_Assert, ELT_Max
	};

	//////////////////////////////////////////////////////////////////////////
	struct LogEntry
	{
		static const unsigned MAX_FILE_NAME = 256;
		static const unsigned MAX_FUNCTION_NAME = 256;
		static const unsigned MAX_LOG_LENGTH = 1024;

		ELogType		mType;
		unsigned		mLineNumber;
		unsigned		mThreadID;
		char			mFileName[MAX_FILE_NAME];
		char			mFunctionName[MAX_FUNCTION_NAME];
		char			mText[MAX_LOG_LENGTH];
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI Log
	{
	public:
		static Log* Get();

		void Add(ELogType type, const char* file, const char* funcName, unsigned line, const char* format, ...);
		bool AddListener(FP<void, const LogEntry&> function);
	};
#pragma endregion

#pragma region

	constexpr bool IsPow2(size_t x)
	{
		return x && ((x & (x - 1)) == 0);
	}
	//round up to next multiple of 'multiple'. multiple must be power of 2
	inline size_t RoundUpToMulP2(size_t numToRound, size_t multiple)
	{
		UASSERT(IsPow2(multiple));
		return (numToRound + multiple - 1) & ~(multiple - 1);
	}
	inline size_t RoundUpToMul(size_t numToRound, size_t multiple)
	{
		UASSERT(multiple);
		return ((numToRound + multiple - 1) / multiple) * multiple;
	}


	static const float PI = 3.141592653589f;
	static float const RAD2DEG = 180.0f / PI;
	static float const DEG2RAD = PI / 180.0f;
	static float const FLOAT_EPSILON = 0.000001f;

	template<typename T> T Max(T a, T b) { return a > b ? a : b; }
	template<typename T> T Min(T a, T b) { return a < b ? a : b; }

	inline float Clamp(float value, float min, float max) { return value < min ? min : value > max ? max : value; }

	inline float Sqrt(float x) { return sqrtf(x); }
	inline float RSqrt(float x) { return 1.0f / sqrtf(x); }

	inline float Sin(float x) { return sinf(x); }
	inline float Cos(float x) { return cosf(x); }
	inline void SinCos(float angleRad, float& outSin, float& outCos)
	{
		outSin = ::sinf(angleRad);
		outCos = ::cosf(angleRad);
	}
	inline void SinCosDeg(float angleDeg, float& outSin, float& outCos)
	{
		outSin = ::sinf(angleDeg * DEG2RAD);
		outCos = ::cosf(angleDeg * DEG2RAD);
	}
	inline float Abs(float x) { return x < 0 ? -x : x; }

	inline float Tan(float x)
	{
		return ::tanf(x);
	}
	inline float Distance(float x, float y)
	{
		return Abs(x - y);
	}
	template<typename T> void Swap(T& a, T& b)
	{
		T tmp = a;
		a = b;
		b = tmp;
	}
	inline float Lerp(float v0, float v1, float t)
	{
		return (1 - t) * v0 + t * v1;
	}

	//////////////////////////////////////////////////////////////////////////
	struct Flag
	{
		unsigned mFlag;

		Flag()
		{
		}
		Flag(unsigned init)
		{
			mFlag = init;
		}
		operator unsigned() { return mFlag; }

		bool TestAndClear(unsigned flag)
		{
			if ((mFlag & flag) == flag)
			{
				mFlag &= ~flag;
				return true;
			}
			return false;
		}
		bool Test(unsigned flag) const
		{
			return (mFlag & flag) == flag;
		}
		void Set(unsigned flag)
		{
			mFlag |= flag;
		}
		void Clear(unsigned flag)
		{
			mFlag &= ~flag;
		}
		void Toggle(unsigned flag)
		{
			mFlag ^= flag;
		}
	};
#pragma endregion
	//////////////////////////////////////////////////////////////////////////
	template < typename T> struct TInstance
	{
		T* mPtr;
		TInstance()
		{
			mPtr = new T;
		}
		~TInstance()
		{
			if (mPtr)
			{
				delete mPtr;
				mPtr = nullptr;
			}
		}
		operator T* () const { return mPtr; }
	};



};