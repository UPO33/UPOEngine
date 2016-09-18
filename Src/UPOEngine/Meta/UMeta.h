#pragma once

#include "UTypeInfo.h"
#include "UClassInfo.h"
#include "UEnumInfo.h"
#include "UMetaSys.h"
#include "../Core/UVector.h"
namespace UPO
{

	struct UAPI TestMyStruct
	{
		UCLASS(TestMyStruct, void)

		Vec3	mPosition = Vec3(0);
		Vec3	mDirection = Vec3(0, 1, 0);
		Name	mName = "MyName";
		String  mStringHello = "hello";

		void MetaPropertyChanged(const PropertyInfo* prp);
	};
	struct UAPI TestMyStruct2 : TestMyStruct
	{
		UCLASS(TestMyStruct2, TestMyStruct)

		Color mColor = Color(1,1,1,1);

		void MetaPropertyChanged(const PropertyInfo* prp);
	};
	class UAPI TestMetaClass
	{
		UCLASS(TestMetaClass, void)

	public:
		bool	mBool = true;
		char	mChar = 1;
		unsigned char mUChar = 2;
		short mShort = 3;
		unsigned short mUShort = 4;
		int mInt = 5;
		int mUInt = 6;
		int64 mInt64 = 7;
		uint64 mUInt64 = 8;
		Vec2 mVec2 = Vec2(9.999f);
		Vec3 mVec3 = Vec3(10.111f);
		Vec4 mVec4 = Vec4(111.1111f);
		Color mColor = Color(1, 0, 0, 1);
		Color32 mColor32 = Color32::GREEN;
		String mString = "string";
		Name mName = Name("name");
		TestMyStruct	mMyStruct;
		TestMyStruct2	mMyStruct2;

		void MetaPropertyChanged(const PropertyInfo* prp);
	};
};