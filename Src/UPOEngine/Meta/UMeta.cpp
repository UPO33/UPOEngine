#include "UMeta.h"

namespace UPO
{
	UCLASS_BEGIN_IMPL(TestMyStruct)
		UPROPERTY(mPosition)
		UPROPERTY(mDirection)
		UPROPERTY(mName)
		UPROPERTY(mStringHello)
	UCLASS_END_IMPL(TestMyStruct)

	void TestMyStruct::MetaPropertyChanged(const PropertyInfo* prp)
	{
		ULOG_MESSAGE("%s", prp->GetName().CStr());
	}

	UCLASS_BEGIN_IMPL(TestMyStruct2)
	UPROPERTY(mColor)
	UPROPERTY(mEnum)
	UCLASS_END_IMPL(TestMyStruct2)

	void TestMyStruct2::MetaPropertyChanged(const PropertyInfo* prp)
	{
		ULOG_MESSAGE("%s", prp->GetName().CStr());
	}

		UCLASS_BEGIN_IMPL(TestMetaClass)
		UPROPERTY(mBool)
		UPROPERTY(mChar)
		UPROPERTY(mUChar)
		UPROPERTY(mShort)
		UPROPERTY(mUShort)
		UPROPERTY(mInt)
		UPROPERTY(mUInt)
		UPROPERTY(mInt64)
		UPROPERTY(mUInt64)
		UPROPERTY(mVec2)
		UPROPERTY(mVec3)
		UPROPERTY(mVec4)
		UPROPERTY(mColor)
		UPROPERTY(mColor32)
		UPROPERTY(mString)
		UPROPERTY(mName)
		UPROPERTY(mMyStruct)
		UPROPERTY(mMyStruct2)
		UPROPERTY(mTArrayVec3)
		UPROPERTY(mTArrayInt)
		UPROPERTY(mTArrayString)
	UCLASS_END_IMPL(TestMetaClass)



		void TestMetaClass::SetZero()
		{
			MemZero(this, sizeof(TestMetaClass));
		}

		void TestMetaClass::SetValues()
		{
			mBool = RandBool();
			mChar = RandInt() % 128;
			mUChar = RandInt() % 255;
			mShort = RandInt() % 0xFFf;
			mUShort = RandInt() % 0xFFff;
			mUInt = RandInt();
			mInt = RandInt();
			mUInt64 = RandInt();
			mUInt64 = RandInt();
			mVec2 = Vec2(RandFloat(-100, 100));
			mVec3 = Vec3(RandFloat(-200, 200));
			mVec4 = Vec4(RandFloat(-400, 400));
			mColor = Color(RandFloat01(), 1);
			mColor32 = Color32(RandInt() % 255, RandInt() % 255, RandInt() % 255, 255);
			mString = "string";
			mName = "name";
			mMyStruct = { Vec3(10), Vec3(0,1,0), "naaaaame", "helllllllllo" };
			mMyStruct2.mColor = Color(RandFloat01(), 1);
			mMyStruct2.mPosition = Vec3(RandFloat01());
			mMyStruct2.mDirection = Vec3(RandFloat01()).GetNormalized();
			mMyStruct2.mStringHello = "heeeeeeeeeeeeeeeello";
			mMyStruct2.mName = "naaaaaaaaaaaaame";
			mMyStruct2.mEnum = ETestEnum::ETE_Element0;

			mTArrayVec3.Add({ Vec3(0), Vec3(1), Vec3(2), Vec4(3) });
			mTArrayInt.Add({ 1,2,3,4,5 });
			mTArrayString.Add({ "str0", "str1", "str2" });

		}

		void TestMetaClass::Print()
		{

#define ULOGVAR_VEC2(v2) ULOG_WARN("%s %f  %f", #v2, v2.mX, v2.mY)
#define ULOGVAR_VEC3(v3) ULOG_WARN("%s %f  %f  %f", #v3, v3.mX, v3.mY, v3.mZ)
#define ULOGVAR_VEC4(v4) ULOG_WARN("%s %f  %f  %f  %f", #v4, v4.mX, v4.mY, v4.mZ, v4.mW)
#define ULOGVAR_COLOR32(c) ULOG_WARN("%s %p", #c, c.mColor)
#define ULOGVAR_STR(str) ULOG_WARN("%s %s", #str, str.CStr() ? str.CStr() : "")
#define ULOGVAR_NAME(name) ULOG_WARN("%s %s", #name, name.CStr() ? name.CStr() : "")

			ULOG_WARN("mBool %d  mChar %d  mUChar %d mShort %d  mUShort %d", mBool, mChar, mUChar, mShort, mUShort);
			ULOG_WARN("mInt %d, mUInt %d  mInt64 %i64 mUInt64  %ull", mInt, mUInt, mInt64, mUInt64);
			
			ULOGVAR_VEC2(mVec2);
			ULOGVAR_VEC3(mVec3);
			ULOGVAR_VEC4(mVec4);
			ULOGVAR_VEC4(mColor);
			ULOGVAR_COLOR32(mColor32);
			ULOGVAR_STR(mString);
			ULOGVAR_NAME(mName);
			ULOGVAR_VEC3(mMyStruct.mPosition);
			ULOGVAR_VEC3(mMyStruct.mDirection);
			ULOGVAR_STR(mMyStruct.mStringHello);
			ULOGVAR_NAME(mMyStruct.mName);
			ULOGVAR_VEC3(mMyStruct2.mPosition);
			ULOGVAR_VEC3(mMyStruct2.mDirection);
			ULOGVAR_STR(mMyStruct2.mStringHello);
			ULOGVAR_NAME(mMyStruct2.mName);
			ULOGVAR_VEC4(mMyStruct2.mColor);
			ULOG_WARN("mMyStruct2.mEnum %d", mMyStruct2.mEnum);
			for (size_t i = 0; i < mTArrayVec3.Length(); i++)
			{
				ULOGVAR_VEC3(mTArrayVec3[i]);
			}
			for (size_t i = 0; i < mTArrayInt.Length(); i++)
			{
				ULOG_WARN("mTArray[%d] %d", i, mTArrayInt[i]);
			}
			for (size_t i = 0; i < mTArrayString.Length(); i++)
			{
				ULOGVAR_STR(mTArrayString[i]);
			}
		}

	void TestMetaClass::MetaPropertyChanged(const PropertyInfo* prp)
	{
		ULOG_MESSAGE("%s", prp->GetName().CStr());
	}

};
