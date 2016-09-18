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
	UCLASS_END_IMPL(TestMetaClass)

	void TestMetaClass::MetaPropertyChanged(const PropertyInfo* prp)
	{
		ULOG_MESSAGE("%s", prp->GetName().CStr());
	}

};
