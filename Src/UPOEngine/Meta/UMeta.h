#pragma once

#include "UTypeInfo.h"
#include "UClassInfo.h"
#include "UEnumInfo.h"
#include "UMetaSys.h"
#include "../Core/UVector.h"
namespace UPO
{
	enum ETestEnum
	{
		ETE_Element0, ETE_Element1, ETE_Element2
	};
	enum ETestFlag
	{
		ETF_Flag1 = 1,
		ETF_Flag2 = 2,
		ETF_Flag3 = 4,
		ETF_All = ETF_Flag1 | ETF_Flag2 | ETF_Flag3,
	};

	struct UAPI TestMyStruct
	{
		UCLASS(TestMyStruct, void)

		Vec3	mPosition;
		Vec3	mDirection;
		Name	mName;
		String  mStringHello;

		void MetaPropertyChanged(const PropertyInfo* prp);
	};
	struct UAPI TestMyStruct2 : TestMyStruct
	{
		UCLASS(TestMyStruct2, TestMyStruct)
			static const unsigned ES = sizeof(ETestEnum);
		Color mColor;
		ETestEnum mEnum;
		ETestFlag mFlag;

		void MetaPropertyChanged(const PropertyInfo* prp);
	};
	class UAPI TestMetaClass
	{
		UCLASS(TestMetaClass, void)

	public:
		bool	mBool;
		char	mChar;
		unsigned char mUChar;
		short mShort;
		unsigned short mUShort;
		int mInt;
		unsigned int mUInt;
		int64 mInt64;
		uint64 mUInt64;
		Vec2 mVec2;
		Vec3 mVec3;
		Vec4 mVec4;
		Color mColor;
		Color32 mColor32;
		String mString;
		Name mName;
		TestMyStruct	mMyStruct;
		TestMyStruct2	mMyStruct2;
		TArray<Vec3>	mTArrayVec3;
		TArray<int>		mTArrayInt;
		TArray<String>	mTArrayString;

		void SetZero();
		void SetValues();
		void Print();

		TestMetaClass()
		{
			SetValues();
		}
		void MetaPropertyChanged(const PropertyInfo* prp);
	};
};