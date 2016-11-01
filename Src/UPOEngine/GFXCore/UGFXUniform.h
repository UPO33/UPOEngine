#pragma once

#include "../Core/UCore.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class GFXUniformBase;

	//////////////////////////////////////////////////////////////////////////
	enum class EUniformVarType
	{
		EFloat,
		EFloat4,
		EFloat4x4,
	};

	
	template<typename VarType> class GFXUniformVar
	{
		EUniformVarType mType;
		GFXUniformBase& mOwner;
		size_t mOffsetInBuffer;
		size_t mIndexInBuffer;
		VarType	mData;
		unsigned mArrayLength;
		unsigned mChanged : 1;
	public:
		GFXUniformVar(GFXVertexBuffer& owner, VarType inittialValue) : mOwner(owner), mData(inittialValue)
		{

		}
		
		GFXUniformVar& operator = (const VarType& data)
		{
			mData = data;
			mChanged = 1;
			return *this;
		}
		const VarType& operator () const { return mData; }
	};


	//////////////////////////////////////////////////////////////////////////
	class GFXUniformBase
	{
		ClassInfo* mClass;
	public:
		virtual bool IsBuffer() const = 0;

		virtual void* Map() = 0;
		virtual void UnMap() = 0;

		template<typename T> T* Map() { return (T*)Map(); }
	};


};