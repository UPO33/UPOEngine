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
		ULOG_MESSAGE("%", prp->GetName());
	}

	UCLASS_BEGIN_IMPL(TestMyStruct2)
	UPROPERTY(mColor)
	UPROPERTY(mEnum)
	UPROPERTY(mFlag)
	UCLASS_END_IMPL(TestMyStruct2)

	void TestMyStruct2::MetaPropertyChanged(const PropertyInfo* prp)
	{
		ULOG_MESSAGE("%", prp->GetName());
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


	UENUM(ETestEnum, false, nullptr
		, ETestEnum::ETE_Element0
		, ETestEnum::ETE_Element1
		, ETestEnum::ETE_Element2);

	UENUM(ETestFlag, true, nullptr
		, ETestFlag::ETF_Flag1
		, ETestFlag::ETF_Flag2
		, ETestFlag::ETF_Flag3
		, ETestFlag::ETF_All);


		void TestMetaClass::SetZero()
		{
			MemZero(this, sizeof(TestMetaClass));
		}

		void TestMetaClass::SetValues()
		{
			srand(time(nullptr));

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
			mColor = Color(1, 0, 0, 1);
			mColor32 = Color32(255, 0, 0, 0); //Color32(RandInt() % 255, RandInt() % 255, RandInt() % 255, 255);
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
		}

	void TestMetaClass::MetaPropertyChanged(const PropertyInfo* prp)
	{
		ULOG_MESSAGE("%", prp->GetName());
	}

};
