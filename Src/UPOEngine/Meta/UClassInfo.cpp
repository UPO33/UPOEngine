#include "UClassInfo.h"
#include "UMetaSys.h"

#include "../Engine/UAsset.h"
#include "../Engine/UEntity.h"

namespace UPO
{
	UCLASS_BEGIN_IMPL(ClassInfo)
	UCLASS_END_IMPL(ClassInfo)

	//////////////////////////////////////////////////////////////////////////
	bool PropertyInfo::NeedBake() const
	{
		return 1;
	}

	void PropertyInfo::PrintLog()
	{
		ULOG_MESSAGE("Name %s  TypeName %s TypeInfo %p", GetName().CStr(), GetTypeName().CStr(), GetTypeInfo());
	}

	//////////////////////////////////////////////////////////////////////////
	void PropertyInfo::Bake()
	{
		if (IsTemplate())
		{
			mTemplateArgTypeInfo = MetaSys::Get()->FindType(mTemplateArgTypeName);
		}
		else
		{
			mTypeInfo = MetaSys::Get()->FindType(mTypeName);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void ClassInfo::Bake()
	{
		mParentClass = nullptr;
		mErrorUnregisteredParent = false;

		if(!mParentClassName.IsEmpty()) // has parent ?
		{
			mParentClass = (ClassInfo*)MetaSys::Get()->FindType(mParentClassName);
			if (mParentClass == nullptr)
				mErrorUnregisteredParent = true;
			else
				mErrorUnregisteredParent = false;
		}

		for (unsigned i = 0; i < NumProperty(); i++)
		{
			mProperties[i].Bake();
		}
		
		if (mParentClass)
		{
			mParentClass->mSubClasses.AddUnique(this);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	bool ClassInfo::NeedBake() const
	{
		if (mParentClass == nullptr && !mParentClassName.IsEmpty())
			return true;
		for (unsigned i = 0; i < NumProperty(); i++)
		{
			if (mProperties[i].NeedBake())
				return true;
		}
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	bool ClassInfo::IsObject() const
	{
		return IsBaseOf(Object::GetClassInfoStatic());
	}

	bool ClassInfo::IsAsset() const
	{
		return IsBaseOf(Asset::GetClassInfoStatic());
	}

	//////////////////////////////////////////////////////////////////////////
	bool ClassInfo::IsBaseOf(const ClassInfo* base) const
	{
		if (this == base) return true;

		const ClassInfo* iter = mParentClass;
		while (iter)
		{
			if (iter == base) return true;
			iter = iter->mParentClass;
		}
		return false;
	}


	//////////////////////////////////////////////////////////////////////////
	bool ClassInfo::CallDefaultConstructor(void* object) const
	{
		UASSERT(object);
		if (mDefalutConstructor)
		{
			mDefalutConstructor(nullptr);
			return true;
		}
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	bool ClassInfo::CallDestructor(void* object) const
	{
		UASSERT(object);
		if (mDestructor)
		{
			mDestructor(object);
			return true;
		}
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	bool ClassInfo::CallMetaPropertyChanged(void* object, const PropertyInfo* prp) const
	{
		UASSERT(object);
		if (mMetaPropertyChanged)
		{
			mMetaPropertyChanged(object, prp);
			return true;
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	void ClassInfo::GetAlUsedClassesInProperties(TArray<ClassInfo*>& outClasses)
	{
		outClasses.Empty();
		for (size_t i = 0; i < mProperties.Length(); i++)
		{
			PropertyInfo& prop = mProperties[i];
			if (prop.GetTypeInfo() && prop.GetTypeInfo()->IsClass())
			{
				ClassInfo* classInfo = (ClassInfo*)prop.GetType();
				outClasses.AddUnique(classInfo);
				classInfo->GetAlUsedClassesInProperties(outClasses);
			}
		}
	}

	void ClassInfo::PrintLog()
	{
		ULOG_MESSAGE("----------------- %s --------------", GetName().CStr());
		ULOG_MESSAGE("Parent %s", GetParentName().CStr());
		ULOG_MESSAGE("Size %d  Align %d", GetSize(), GetAlign());
		ULOG_MESSAGE("Header %s  Source %s  Line %d", GetHeaderFileName().CStr(), GetSourceFileName().CStr(), GetDefinitionLineNumber());
		ULOG_MESSAGE("Num Attibues %d", GetAttributes().NumAttributes());
		ULOG_MESSAGE("Properties %d", NumProperty());
		for (size_t i = 0; i < NumProperty(); i++)
		{
			mProperties[i].PrintLog();
		}
	}
	


};