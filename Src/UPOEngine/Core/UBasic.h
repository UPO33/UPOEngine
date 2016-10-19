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

#define UASSERT(expression) { if(!(expression)) UPO::Log::Get()->Add(UPO::ELogType::ELT_Assert, __FILE__, __FUNCTION__, __LINE__, "Assertaion Failed  %s", #expression); }

#define UASSERT_GTONLY() UASSERT(UPO::IsGameThread())
#define UASSERT_RTONLY() UASSERT(UPO::IsRenderThread())

#pragma endregion

//template<class T, size_t N> size_t size(T(&)[N]) { return N; }

#define UARRAYLEN(Array)  (sizeof(Array) / sizeof(Array[0]))


#define UWELD(Prefix, Suffix) Prefix##Suffix
#define UWELDINNER(Prefix, Suffix) UWELD(Prefix, Suffix)

namespace UPO
{
	class ClassInfo;
	class TypeInfo;
	class Stream;
	class Buffer;
};

//rest in ../Meta/UMeta.h
#define UCLASS(Class, ParentClass)\
public:\
	static int ZZZLineNumber() { return __LINE__; }\
	static const char* ZZZFileName() { return __FILE__; }\
	typedef ParentClass Parent;\
	typedef Class Self;\
	friend struct ZZZ_##Class;\
	static UPO::ClassInfo* GetClassInfoStatic();\
	void ZZZIsMeta() { static_assert(std::is_same<std::remove_pointer<decltype(this)>::type, Class>::value, "wrong Claass"); }\







/*
in order to check a class has the specified member function or to get pointer to member function whether available or not,
first u must declare that with UDECLARE_MEMBERFUNCTION_CHECKING(DeclarationName, FunctionName, ReturnType, Args...)
then u are able to use UCLASS_HAS_MEMBERFUNCTION and UCLASS_GET_MEMBERFUNCTION
note:
it doesn't take the parent's class functions into account
*/
#define UDECLARE_MEMBERFUNCTION_CHECKING(DeclarationName, FunctionName, ReturnType, ...)\
	template <class Type> struct Z_##DeclarationName##HasFuncChecker\
	{\
		using TFunc = ReturnType (Type::*) (__VA_ARGS__);\
		struct FakeType { ReturnType FunctionName(__VA_ARGS__) { return exit(0); } };\
		template <typename T, T> struct TypeCheck; \
		template <typename T> struct FuncCheck { typedef ReturnType(T::*fptr)(__VA_ARGS__); }; \
		template <typename T> static char HasFunc(TypeCheck< typename FuncCheck<T>::fptr, &T::FunctionName >*); \
		template <typename T> static long  HasFunc(...); \
		static bool const value = (sizeof(HasFunc<Type>(0)) == sizeof(char)); \
	}; \
	template<typename T> struct Z_##DeclarationName##HasFunc\
	{\
		using TFunc = ReturnType (UPO::Void::*) (__VA_ARGS__);\
		struct FakeType { ReturnType FunctionName(__VA_ARGS__) { return exit(0); } };\
		struct FakeT { static bool const value = 0; };\
		static bool const value = std::conditional<std::is_class<T>::value, Z_##DeclarationName##HasFuncChecker<T>, FakeT>::type::value;\
		static TFunc GetFunc() \
		{\
			using TargetClass = std::conditional<value, T, FakeType>::type;\
			if (std::is_same<TargetClass, FakeType>::value) return nullptr;\
			else return (TFunc)&TargetClass::FunctionName;\
		}\
	}; \

//return true if 'Class' has specified function function must be declared with UDECLARE_MEMBERFUNCTION_CHECKING once
#define UCLASS_HAS_MEMBERFUNCTION(DeclarationName, Class) Z_##DeclarationName##HasFunc<Class>::value
//return pointer to member function as ReturnType(Void::*)(Args), u can cast result to any type
//or use MFP<ReturnType, Args> for convenience, returns null if not exist
//function must be declared with UDECLARE_MEMBERFUNCTION_CHECKING once
#define UCLASS_GET_MEMBERFUNCTION(DeclarationName, Class) Z_##DeclarationName##HasFunc<Class>::GetFunc()


namespace UPO
{
	typedef unsigned char byte;

	typedef unsigned char uint8;
	typedef signed char int8;
	typedef unsigned short uint16;
	typedef signed short int16;
	typedef unsigned int uint32;
	typedef int int32;
	typedef unsigned long long uint64;
	typedef long long int64;

	
	class Void {};

	using VoidFuncPtr = void(*)();
	using VoidMemFuncPtr = void(Void::*)();


	//////////////////////////////////////////////////////////////////////////
	UAPI bool IsGameThread();
	UAPI bool IsRenderThread();

	UAPI void AppCrash();

#pragma region function pointer helper
	template <typename TFunction = VoidMemFuncPtr> void* MemFunc2Ptr(TFunction function)
	{
		// 		union
		// 		{
		// 			TFunction f;
		// 			void* p;
		// 		};
		// 		f = function;
		// 		return p; 
		return (void*&)function;
	}
	template<typename TFunction = VoidMemFuncPtr> TFunction Ptr2MemFunc(const void* function)
	{
		union
		{
			TFunction f;
			const void* p;
		};
		p = function;
		return f;
	}
	//////////////////////////////////////////////////////////////////////////function pointer
	template<typename TRet, typename... TArgs> class TFP
	{
	public:
		using Pattern = TRet(*)(TArgs...);
	private:
		Pattern mFunction;
	public:
		TFP() {}
		TFP(std::nullptr_t) { mFunction = nullptr; }
		template<typename T> TFP(T pFunction) { mFunction = pFunction; }


		TRet operator() (TArgs... args) const
		{
			UASSERT(mFunction);
			return mFunction(args...);
		}
		operator bool() const { return mFunction != nullptr; }
		operator void* () const { return (void*)mFunction; }
		bool operator == (const void* p) const { return mFunction == p; }
		bool operator != (const void* p) const { return mFunction != p; }
		bool operator == (const TFP& fp) const { return mFunction == fp.mFunction; }
		bool operator != (const TFP& fp) const { return mFunction != fp.mFunction; }
	};

	//////////////////////////////////////////////////////////////////////////member function pointer
	template<typename TRet, typename... TArgs> class TMFP
	{
	public:
		using Pattern = TRet(Void::*)(TArgs...);
		Pattern mFunction;


		TMFP() {}
		TMFP(std::nullptr_t) { mFunction = nullptr; }
		template <typename T> TMFP(TRet(T::* pFunction)(TArgs...)) { mFunction = (Pattern)pFunction; }

		TRet operator() (void* object, TArgs... args) const
		{
			UASSERT(mFunction);
			return (((Void*)object)->*mFunction)(args...);
		}
		operator bool() const { return mFunction != nullptr; }
		operator void* () const { return MemFunc2Ptr(mFunction); }

		template<typename TTRet, typename... TTArgs> bool operator == (const TMFP<TTRet, TTArgs...>& other) const
		{
			return (VoidMemFuncPtr)mFunction == (VoidMemFuncPtr)other.mFunction;
		}
		template<typename TTRet, typename... TTArgs> bool operator != (const TMFP<TTRet, TTArgs...>& other) const
		{
			return (VoidMemFuncPtr)mFunction != (VoidMemFuncPtr)other.mFunction;
		}
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
		bool AddListener(TFP<void, const LogEntry&> function);
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
	inline int Sign(int value)
	{
		return value < 0 ? -1 : value > 0 ? 1 : 0;
	}

	inline bool RandBool() { return rand() % 2 == 0; }
	inline int RandInt() { return rand(); }
	inline float RandFloat01() { return ((float)rand()) / RAND_MAX; }
	inline float RandFloat(float min, float max) { return Lerp(min, max, RandFloat01()); }

	
	//////////////////////////////////////////////////////////////////////////
	struct UAPI Flag
	{
		UCLASS(Flag, void)

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

	//////////////////////////////////////////////////////////////////////////
	class Object;
	class Asset;

	////////////////////////////////////////////////////////////////////TT_IsMetaClass
	template <class Type> struct ZZ_Has_ZZZIsMeta
	{
		template <typename T, T> struct TypeCheck;
		template <typename T> struct FuncCheck { typedef void (T::*fptr)(); };
		template <typename T> static char HasFunc(TypeCheck< typename FuncCheck<T>::fptr, &T::ZZZIsMeta >*);
		template <typename T> static long  HasFunc(...);
		static bool const value = (sizeof(HasFunc<Type>(0)) == sizeof(char));
	};

	//checks whether T is meta class or not
	template<typename T> struct TT_IsMetaClass
	{
		struct FakeT { static bool const value = 0; };
		static bool const value = std::conditional<std::is_class<T>::value, ZZ_Has_ZZZIsMeta<T>, FakeT>::type::value;
	};

	//////////////////////////////////////////////////////////////////////////TT_IsTArray
	template <class Type> struct ZZ_Has_ZZZIsTArray
	{
		template <typename T, T> struct TypeCheck;
		template <typename T> struct FuncCheck { typedef void (T::*fptr)(); };
		template <typename T> static char HasFunc(TypeCheck< typename FuncCheck<T>::fptr, &T::ZZZIsTArray >*);
		template <typename T> static long  HasFunc(...);
		static bool const value = (sizeof(HasFunc<Type>(0)) == sizeof(char));
	};
	struct ZZ_FakeZZZIsTArray { static bool const value = 0; };

	//checks whether T is TArray or not
	template<typename T> struct TT_IsTArray
	{
		static bool const value = std::conditional<std::is_class<T>::value, ZZ_Has_ZZZIsTArray<T>, ZZ_FakeZZZIsTArray>::type::value;
	};

	//////////////////////////////////////////////////////////////////////////TT_IsObjectPtr
	template <class Type> struct ZZ_Has_ZZZIsTObjectPtr
	{
		template <typename T, T> struct TypeCheck;
		template <typename T> struct FuncCheck { typedef void (T::*fptr)(); };
		template <typename T> static char HasFunc(TypeCheck< typename FuncCheck<T>::fptr, &T::ZZZIsTObjectPtr >*);
		template <typename T> static long HasFunc(...);
		static bool const value = (sizeof(HasFunc<Type>(0)) == sizeof(char));
	};
	struct ZZ_FakeZZZIsTObjectPtr { static bool const value = false; };

	//checks whether T is TObjectPtr or not
	template<typename T> struct TT_IsTObjectPtr
	{
		static bool const value = std::conditional<std::is_class<T>::value, ZZ_Has_ZZZIsTObjectPtr<T>, ZZ_FakeZZZIsTObjectPtr>::type::value;
	};

	//////////////////////////////////////////////////////////////////////////TT_IsAsset
	//checks whether T is object or not
	template<typename T> struct TT_IsObject
	{
		static bool const value = TT_IsMetaClass<T>::value && std::is_base_of<Object, T>::value;
	};
	//checks whether T is asset or not
	template<typename T> struct TT_IsAsset
	{
		static bool const value = TT_IsMetaClass<T>::value && std::is_base_of<Asset, T>::value;
	};
};