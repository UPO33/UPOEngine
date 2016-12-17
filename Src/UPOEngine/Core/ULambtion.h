#pragma once

#include "UBasic.h"

namespace UPO
{
	template<typename TRet, typename... TArgs> struct TLambtion
	{
		static const unsigned MAX_LAMBDA_SIZE = 32;

		typedef TRet(*InvokeT)(void*, TArgs...);
		typedef void(*DTorT)(void*);
		typedef void(*CTorT)(void*);
		typedef void(*CCTorT)(void*, const void*);

		char		mBuffer[MAX_LAMBDA_SIZE];
		InvokeT		mInvoke;
		DTorT		mDTor;
		CCTorT		mCCTor;

		template<typename TFunc> static TRet _Invoke(void* ins, TArgs... args)
		{
			return (*(reinterpret_cast<TFunc*>(ins)))(args...);
		}
		template<typename TFunc> static void _CTor(void* p)
		{
			new (p) TFunc;
		}
		template<typename TFunc> static void _DTor(void* p)
		{
			reinterpret_cast<TFunc*>(p)->~TFunc();
		}
		template<typename TFunc> static void _CCTor(void* a, const void* b)
		{
			new (a) TFunc(*((const TFunc*)b));
		}
		template<typename TFunc> void _Init(const TFunc& func)
		{
			mInvoke = reinterpret_cast<InvokeT>(&_Invoke<TFunc>);
			mCCTor = reinterpret_cast<CCTorT>(&_CCTor<TFunc>);
			mDTor = reinterpret_cast<DTorT>(&_DTor<TFunc>);

			new (mBuffer) TFunc(func);
		}
	public:
		typedef TRet TReturnType;
		typedef std::tuple<TArgs...> TArgsTuple;
		static unsigned const ARGS_COUNT = sizeof...(TArgs);

		TLambtion()
		{
			mInvoke = nullptr;
			mCCTor = nullptr;
			mDTor = nullptr;
		}
		TLambtion(std::nullptr_t)
		{
			new (this) TLambtion;
		}
		template<typename TFunc> TLambtion(const TFunc& func)
		{
			static_assert(sizeof(TFunc) <= MAX_LAMBDA_SIZE, "");

			_Init<TFunc>(func);
		}
		template<typename TFunc> TLambtion& operator = (const TFunc& func)
		{
			static_assert(sizeof(TFunc) <= MAX_LAMBDA_SIZE, "");

			if (mDTor) mDTor(mBuffer);
			_Init<TFunc>(func);
			return *this;
		}
		TLambtion& operator = (std::nullptr_t)
		{
			this->~TLambtion();
			return *this;
		}
		TLambtion& operator = (const TLambtion& copy)
		{
			if (this == &copy) return *this;

			if (mDTor) mDTor(mBuffer);

			mInvoke = copy.mInvoke;
			mCCTor = copy.mCCTor;
			mDTor = copy.mDTor;

			if (mCCTor) mCCTor(mBuffer, copy.mBuffer);

			return *this;
		}
		~TLambtion()
		{
			if (mDTor) mDTor(mBuffer);
			mInvoke = nullptr;
			mCCTor = nullptr;
			mDTor = nullptr;
		}
		operator bool() const { return mInvoke != nullptr; }

		TRet operator()(TArgs... args)
		{
			return mInvoke(mBuffer, args...);
		}
	};

};