#pragma once

#include "UBasic.h"

#ifdef UPLATFORM_WIN
#define DELEGATE_CALLING_CONVENTION __thiscall
#else
#define DELEGATE_CALLING_CONVENTION __cdecl
#endif

namespace UPO
{
	/*
	
	*/
	template<typename... TArgs> class TDelegateSingle
	{

		void*	mObject;
		void*	mFunction;

	public:
		inline TDelegateSingle() : mObject(nullptr), mFunction(nullptr) {}
		inline TDelegateSingle(std::nullptr_t) : mObject(nullptr), mFunction(nullptr) {}

		template<typename T> TDelegateSingle(T* object, void (DELEGATE_CALLING_CONVENTION T::*function)(TArgs...))
		{
			Set(object, function);
		}
		template<typename T> void Set(T* object, void (DELEGATE_CALLING_CONVENTION T::*function)(TArgs...))
		{
			SetObject(object);
			SetFunction(function);
		}
		inline void SetObject(void* object)
		{
			mObject = object;
		}
		inline void SetNull()
		{
			mObject = nullptr;
			mFunction = nullptr;
		}
		template<typename T> void SetFunction(void (DELEGATE_CALLING_CONVENTION T::*function)(TArgs...))
		{
			mFunction = (void*&)function;
		}
		inline bool Invoke(TArgs... args) const
		{
			if (mObject && mFunction)
			{
				using Func = void (DELEGATE_CALLING_CONVENTION *)(void*, TArgs...);
				((Func)mFunction)(mObject, args...);
				return true;
			}
			return false;
		}
	};

};