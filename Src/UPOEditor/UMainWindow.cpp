#include "UMainWindow.h"

namespace UPOEd
{









	UCLASS_BEGIN_IMPL(TestObject)
		UPROPERTY(mMetaClass)
		UPROPERTY(mPropertyType)
		UPROPERTY(mIntNoEdit, UATTR_Uneditable(), UATTR_Comment("this cant be edited"))
		UPROPERTY(mFloat01, UATTR_Range(0,1))
		UPROPERTY(mTArrayMS)
	UCLASS_END_IMPL(TestObject)

	UCLASS_BEGIN_IMPL(StructStr)
		UPROPERTY(mStr)
	UCLASS_END_IMPL(StructStr)

	TestObject::TestObject()
	{
		mTArrayMS.AddDefault();
		mTArrayMS.AddDefault();
// 		mTArrayMS[0].mStr = 10;
// 		mTArrayMS[1].mStr = 20;
	}

	TestObject::~TestObject()
	{

	}
	
	void TestObject::MetaPropertyChanged(const PropertyInfo* prp)
	{
		ULOG_MESSAGE("%s", prp->GetName().CStr());
	}

	void TestObject::Tick()
	{
		mFloat01 += 1;
		mMetaClass.mColor = Color(RandFloat01());

		
	}

	StructStr::StructStr()
	{
		mStr.mPosition = Vec3(1, 1, 1);
		mStr.mStringHello = " hello";
		ULOG_WARN("%d", this);
	}

	StructStr::~StructStr()
	{
		ULOG_WARN("");
	}

	void StructStr::MetaPropertyChanged(const PropertyInfo* prp)
	{
		ULOG_MESSAGE("%s", prp->GetName().CStr());
	}

};