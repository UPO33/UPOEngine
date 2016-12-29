#pragma once

#include "UBasic.h"
#include "UArray.h"
#include "../Object/UObject.h"

namespace UPO
{
	enum class EDelegateBindType
	{
		Null, Function, MemberFunction, ObjectMemberFunction, Lambda, ObjectLambda
	};
	//////////////////////////////////////////////////////////////////////////
	template<typename TRet, typename... TArgs> class TDelegate
	{
		static const unsigned MAX_LAMBDA_SIZE = 32;


		using LambdaInvokeT = TRet(*)(void*, TArgs...);
		using LambdaDTorT = void(*)(void*);
		using LambdaCTorT = void(*)(void*);
		using LambdaCCTorT = void(*)(void*, const void*);

		struct SFunc
		{
			TRet(*mFunc)(TArgs...);
			inline TRet Call(TArgs... args)
			{
				return mFunc(args...);
			}
		};
		struct SMemFunc
		{
			TMFP<TRet, TArgs...> mFunc;
			void* mObj;
			//TRet(Null::*mMemfunc)(TArgs...);
			inline TRet Call(TArgs... args)
			{
				return mFunc(mObj, args...);
			}
		};
		struct SMemFuncObj
		{
			TMFP<TRet, TArgs...> mFunc;
			ObjectPtr mObj;

			SMemFuncObj(decltype(mFunc) func, Object* obj) : mFunc(func), mObj(obj) {}

			inline TRet Call(TArgs... args)
			{
				if(mObj) return mFunc(mObj, args...);
				return TRet();
			}
		};
		struct SLambda
		{
			char				mBuffer[MAX_LAMBDA_SIZE];
			LambdaInvokeT		mInvoke;
			LambdaDTorT			mDTor;
			LambdaCCTorT		mCCTor;

			SLambda(const SLambda& copy)
			{
				mInvoke = copy.mInvoke;
				mDTor = copy.mDTor;
				mCCTor = copy.mCCTor;
				
				mCCTor(mBuffer, copy.mBuffer);
			}
			~SLambda()
			{
				mDTor(mBuffer);
			}
			inline TRet Call(TArgs... args)
			{
				return mInvoke(mBuffer, args...);
			}
		};
		struct SLambdaObj
		{
			char				mBuffer[MAX_LAMBDA_SIZE];
			LambdaInvokeT		mInvoke;
			LambdaDTorT			mDTor;
			LambdaCCTorT		mCCTor;
			ObjectPtr			mObj;

			inline TRet Call(TArgs... args)
			{
				if (mObj) return mInvoke(mBuffer, args...);
				return TRet();
			}
		};

		//////////////////////////////////////////////////////////////////////////memvars
		EDelegateBindType	mType;
		char				mBuffer[sizeof(SLambdaObj)];



		SFunc* AsFunc() const { return (SFunc*)mBuffer; }
		SMemFunc* AsMemFunc() const { return (SMemFunc*)mBuffer; }
		SMemFuncObj*AsMemFuncObj() const { return (SMemFuncObj*)mBuffer; }
		SLambda* AsLambda() const { return (SLambda*)mBuffer; }
		SLambdaObj* AsLambdaObj() const { return (SLambdaObj*)mBuffer; }

		template<typename TFunc> static TRet _LambdaInvoke(void* ins, TArgs... args)
		{
			return (*(reinterpret_cast<TFunc*>(ins)))(args...);
		}
		template<typename TFunc> static void _LambdaCTor(void* p)
		{
			new (p) TFunc;
		}
		template<typename TFunc> static void _LambdaDTor(void* p)
		{
			reinterpret_cast<TFunc*>(p)->~TFunc();
		}
		template<typename TFunc> static void _LambdaCCTor(void* a, const void* b)
		{
			new (a) TFunc(*((const TFunc*)b));
		}
		template<typename TFunc> void _InitLambda(const TFunc& func)
		{
			mType = EDelegateBindType::Lambda;

			SLambda* cast = AsLambda();
			cast->mInvoke = reinterpret_cast<LambdaInvokeT>(&_LambdaInvoke<TFunc>);
			cast->mCCTor = reinterpret_cast<LambdaCCTorT>(&_LambdaCCTor<TFunc>);
			cast->mDTor = reinterpret_cast<LambdaDTorT>(&_LambdaDTor<TFunc>);

			new (cast->mBuffer) TFunc(func);
		}
		template<typename TFunc> void _InitLambdaObject(Object* obj, const TFunc& func)
		{
			if(obj)
			{
				mType = EDelegateBindType::ObjectLambda;

				SLambdaObj* cast = (SLambdaObj*)mBuffer;
				cast->mInvoke = reinterpret_cast<LambdaInvokeT>(&_LambdaInvoke<TFunc>);
				cast->mCCTor = reinterpret_cast<LambdaCCTorT>(&_LambdaCCTor<TFunc>);
				cast->mDTor = reinterpret_cast<LambdaDTorT>(&_LambdaDTor<TFunc>);

				new (cast->mBuffer) TFunc(func);
				new (&(cast->mObj)) ObjectPtr(obj);
			}
			else
			{
				mType = EDelegateBindType::Null;
			}
		}
	public:
		//null initialization
		TDelegate()
		{
			mType = EDelegateBindType::Null;
		}
		TDelegate(std::nullptr_t)
		{
			mType = EDelegateBindType::Null;
		}
		TDelegate& operator = (std::nullptr_t)
		{
			this->~TDelegate();
			return *this;
		}
		void BindStatic(TRet(*function)(TArgs...))
		{
			UASSERT(function);
			this->~TDelegate();
			mType = EDelegateBindType::Function;
			AsFunc()->mFunc = function;
		}
		template<typename TClass> void BindMember(void* object, TRet(TClass::* memberfunction)(TArgs...))
		{
			UASSERT(object && memberfunction);
			this->~TDelegate();
			mType = EDelegateBindType::MemberFunction;
			AsMemFunc()->mFunc = memberfunction;
			AsMemFunc()->mObj = object;
		}
		template<class TClass> void BindMemberObject(Object* object, TRet(TClass::* memberfunction)(TArgs...))
		{
			UASSERT(memberfunction);
			this->~TDelegate();
			if(object)
			{
				mType = EDelegateBindType::ObjectMemberFunction;
				new (mBuffer) SMemFuncObj(memberfunction, object);
			}
		}
		template<typename TLambda> void BindLambda(const TLambda& lambda)
		{
			this->~TDelegate();

			_InitLambda<TLambda>(lambda);
		}
		template<typename TLambda> void BindLambdaObject(Object* object, const TLambda& lambda)
		{
			this->~TDelegate();

			_InitLambdaObject<TLambda>(object, lambda);
		}

		EDelegateBindType BindType() const { return mType; }

		bool IsBound() const
		{
			switch (mType)
			{
			case EDelegateBindType::Null: return false;
			case EDelegateBindType::ObjectMemberFunction: return AsMemFuncObj()->mObj;
			case EDelegateBindType::ObjectLambda: return AsLambdaObj()->mObj;
			}
			return true;
		}
		TDelegate& operator = (const TDelegate& copy)
		{
			if (this == &copy) return *this;

			this->~TDelegate();

			mType = copy.mType;

			switch (mType)
			{
			case EDelegateBindType::Null:
				break;
			case EDelegateBindType::Function:
				*AsFunc() = *AsFunc();
				break;
			case EDelegateBindType::MemberFunction:
				*AsMemFunc() = copy.AsMemFunc();
				break;
			case EDelegateBindType::ObjectMemberFunction:
				new SMemFuncObj(*copy.AsMemFuncObj());
				break;
			case EDelegateBindType::Lambda:
				new SLambda(*copy.AsLambda());
				break;
			case EDelegateBindType::ObjectLambda:
				new SLambdaObj(*copy.AsLambdaObj());
				break;
			}

			return *this;
		}
		~TDelegate()
		{
			switch (mType)
			{
			case EDelegateBindType::Null:
				break;
			case EDelegateBindType::Function:
				break;
			case EDelegateBindType::MemberFunction:
				break;
			case EDelegateBindType::ObjectMemberFunction:
				AsMemFuncObj()->~SMemFuncObj();
				break;
			case EDelegateBindType::Lambda:
				AsLambda()->~SLambda();
				break;
			case EDelegateBindType::ObjectLambda:
				AsLambdaObj()->~SLambdaObj();
				break;
			}
			mType = EDelegateBindType::Null;
		}
		TRet Invoke(TArgs... args) const
		{
			switch (mType)
			{
			case EDelegateBindType::Function: return AsFunc()->Call(args...);
			case EDelegateBindType::MemberFunction: return AsMemFunc()->Call(args...);
			case EDelegateBindType::ObjectMemberFunction: return AsMemFuncObj()->Call(args...);
			case EDelegateBindType::Lambda: return AsLambda()->Call(args...);
			case EDelegateBindType::ObjectLambda: return AsLambdaObj()->Call(args...);
			}
			return TRet();
		}

		operator bool() const { return IsBound(); }

		TRet operator()(TArgs... args) const
		{
			return Invoke(args...);
		}


	};


	template<typename TRet, typename... TArgs> class TDelegateMulti
	{
		using DelegateType = TDelegate<TRet, TArgs...>;

		TArray<DelegateType>	mDeletages;

	public:
		TDelegateMulti()
		{

		}
		~TDelegateMulti()
		{

		}
		void BindStatic(TRet(*function)(TArgs...))
		{
			mDeletages.AddDefault();
			mDeletages.LastElement().BindStatic(function);
		}
		template<typename TClass> void BindMember(void* object, TRet(TClass::* memberfunction)(TArgs...))
		{
			mDeletages.AddDefault();
			mDeletages.LastElement().BindMember(object, memberfunction);
		}
		template<class TClass> void BindMemberObject(Object* object, TRet(TClass::* memberfunction)(TArgs...))
		{
			mDeletages.AddDefault();
			mDeletages.LastElement().BindMemberObject(object, memberfunction);
		}
		template<typename TLambda> void BindLambda(const TLambda& lambda)
		{
			mDeletages.AddDefault();
			mDeletages.LastElement().BindLambda(lambda);
		}
		template<typename TLambda> void BindLambdaObject(Object* object, const TLambda& lambda)
		{
			mDeletages.AddDefault();
			mDeletages.LastElement().BindLambdaObject(object, lambda);
		}
		void InvokeAll(TArgs... args) const
		{
			for (size_t i = 0; i < mDeletages.Length(); i++)
			{
				if (mDeletages[i].IsBound())
				{
					mDeletages[i].Invoke(args...);
				}
			}
		}
		void Clear()
		{
			mDeletages.RemoveAll();
		}
		TDelegateMulti& operator = (std::nullptr_t) { Clear(); return *this; }
	};
};