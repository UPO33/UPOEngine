#include "UEnumInfo.h"
#include "UMeta.h"

namespace UPO
{
	UCLASS_BEGIN_IMPL(EnumInfo)
	UCLASS_END_IMPL(EnumInfo)

	//////////////////////////////////////////////////////////////////////////
	void EnumInfo::Make(ZZ_EnumRegParam* erp)
	{

	}

	EnumElement* EnumInfo::GetElement(unsigned index) const
	{
		if (index >= mElements.Length()) return nullptr;

		return mElements.ElementAt(index);
	}

	//////////////////////////////////////////////////////////////////////////
	unsigned EnumInfo::FindElement(const String& elementName) const
	{
		for (unsigned i = 0; i < mElements.Length(); i++)
			if (mElements[i].mName == elementName)
				return i;
		return ~0;
	}
	//////////////////////////////////////////////////////////////////////////
	unsigned EnumInfo::FindElement(unsigned value) const
	{
		for (unsigned i = 0; i < mElements.Length(); i++)
			if (mElements[i].mValue == value)
				return i;
		return ~0;
	}
	//////////////////////////////////////////////////////////////////////////
	void EnumInfo::PrintDbg()
	{
// 		ULOG_MESSAGE("-------------------------------");
// 		ULOG_MESSAGE("Enum : [%s]   NumElement %d", GetName().CStr(), NumElement());
// 		for (size_t i = 0; i < NumElement(); i++)
// 		{
// 			auto elem = GetElement(i);
// 			ULOG_MESSAGE("Element name: [%s]  lname: [%s], value %d", elem->Name().CStr(), elem->LegibleName().CStr(), elem->Value());
// 		}
	}

	//////////////////////////////////////////////////////////////////////////
	String EnumElement::LegibleName() const
	{
		auto index = mName.FindRN(':', 0);
		if (index != ~0)
			return mName.SubStr(index + 1);
		return mName;
	}

};