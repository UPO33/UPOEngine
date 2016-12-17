struct MemFuncTypesParseResult
{
	const type_info*	mClassType;
	const type_info*	mReturnType;
	const type_info*	mArgs[16];
	unsigned			mArgsCount;
	
	void DbgLog()
	{
		std::cout << "(" << mReturnType->name() << ")(" << mClassType->name() << "::*)(";
		for (unsigned i = 0; i < mArgsCount; i++)
		{
			std::cout << mArgs[i]->name() << (i == mArgsCount-1 ? "" : ", ");
		}
		std::cout << ")\n";
	}
};

/*
template for parsing member function types

usage
	MemFuncTypesParseResult result = TMemFuncParser<decletype(&MyClass::MyMemberFunction)>::Get();
*/
template <typename T> struct TMemFuncParser { };

template<typename TRet, typename TClass, typename... TArgs> struct TMemFuncParser<TRet(TClass::*)(TArgs...)>
{
	using ClassT = TClass;
	using ReturnT = TRet;
	using ArgsTupleT = std::tuple<TArgs...>;

	static const unsigned ARGS_COUNT = sizeof...(TArgs);
private:
	struct _X
	{
		template<unsigned N> static void GetTypeName(const type_info** out)
		{
			out[N] = &typeid(std::tuple_element<N, ArgsTupleT>::type);

			GetTypeName<N - 1>(out);
		}
		template<> static void GetTypeName<0>(const type_info** out)
		{
			out[0] = &typeid(std::tuple_element<0, ArgsTupleT>::type);
		}
	};
	struct _Y
	{
		template<unsigned N> static void GetTypeName(const type_info** out)
		{}
	};

public:
	static MemFuncTypesParseResult Get()
	{
		MemFuncTypesParseResult result;
		result.mArgsCount = ARGS_COUNT;
		result.mClassType = &typeid(ClassT);
		result.mReturnType = &typeid(ReturnT);
		std::conditional<ARGS_COUNT, _X, _Y>::type::GetTypeName<ARGS_COUNT - 1>(result.mArgs);
		return result;
	}


};
