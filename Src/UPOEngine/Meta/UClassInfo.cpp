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

	size_t PropertyInfo::GetTypeSize() const
	{
		if (mTypeInfo) return mTypeInfo->GetSize();
		else PropertyType_GetTypeSize(mPropertyType);
	}

	size_t PropertyInfo::TemplateArgTypeSize() const
	{
		if (mTemplateArgTypeInfo) return mTemplateArgTypeInfo->GetSize();
		else return PropertyType_GetTypeSize(mTemplateArgType);
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
	void ClassInfo::CallDefaultConstructor(void* object) const
	{
		UASSERT(object);
		UASSERT(mDefalutConstructor);
		mDefalutConstructor(object);
	}
	//////////////////////////////////////////////////////////////////////////
	void ClassInfo::CallDestructor(void* object) const
	{
		UASSERT(object);
		UASSERT(mDestructor);
		mDestructor(object);
	}
	//////////////////////////////////////////////////////////////////////////
	void ClassInfo::CallMetaPropertyChanged(void* object, const PropertyInfo* prp) const
	{
		UASSERT(object);
		UASSERT(mMetaPropertyChanged);
		mMetaPropertyChanged(object, prp);
	}
	//////////////////////////////////////////////////////////////////////////
	void ClassInfo::CallMetaSerialize(void* object, Stream& stream) const
	{
		UASSERT(object);
		UASSERT(mMetaSerialize);
		mMetaSerialize(object, stream);
	}

	const ClassInfo* ClassInfo::GetRoot() const
	{
		ClassInfo* p = GetParent();
		while (p->GetParent())
		{
			p = p->GetParent();
		}
		return p;
	}
	//////////////////////////////////////////////////////////////////////////
	void ClassInfo::GetInheritedClasses(TArray<const ClassInfo*>& outClasses) const
	{
		outClasses.RemoveAll();
		const ClassInfo* parent = GetParent();
		while (parent)
		{
			outClasses.Add(parent);
			parent = parent->GetParent();
		}
		outClasses.Reverse();
	}

	//////////////////////////////////////////////////////////////////////////
	void ClassInfo::GetInvolvedClasses(TArray<const ClassInfo*>& outClasses, bool subProperties, bool inheritedProperties, bool removeArrayFirst) const
	{
		if(removeArrayFirst) outClasses.RemoveAll();

		TArray<const ClassInfo*> classes;

		if (inheritedProperties) GetInheritedClasses(classes);

		classes.Add(this);

		for (size_t iClass = 0; iClass < classes.Length(); iClass++)
		{
			const ClassInfo* cls = classes[iClass];
			outClasses.AddUnique(cls);

			for (size_t iPrp = 0; iPrp < cls->NumProperty(); iPrp++)
			{
				const TypeInfo* prpTypeInfo = cls->GetProperty(iPrp)->GetTypeInfo();
				if (prpTypeInfo && prpTypeInfo->IsClass())
				{
					ClassInfo* prpClassInfo = (ClassInfo*)prpTypeInfo;
					outClasses.AddUnique(prpClassInfo);
					if (subProperties)
						prpClassInfo->GetInvolvedClasses(outClasses, true, inheritedProperties, false);
				}
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
	

	//////////////////////////////////////////////////////////////////////////
	const PropertyInfo* ClassInfo::FindPropertyByName(Name name, bool includingInheritedProperties) const
	{
		for (size_t i = 0; i < mProperties.Length(); i++)
		{
			if (mProperties[i].GetName() == name) 
				return mProperties.Elements() + i;
		}
		
		if (includingInheritedProperties && GetParent())
			return GetParent()->FindPropertyByName(name, true);

		return nullptr;
	}

	size_t PropertyType_GetTypeSize(EPropertyType propertyType)
	{
		switch (propertyType)
		{
		case EPT_Unknown: return 0;
		case EPT_bool: return 1;
		case EPT_int8: return 1;
		case EPT_uint8: return 1;
		case EPT_int16: return 2;
		case EPT_uint16: return 2;
		case EPT_int32: return 4;
		case EPT_uint32: return 4;
		case EPT_int64: return 8;
		case EPT_uint64: return 8;
		case EPT_float: return 4;
		case EPT_double: return 8;
		case EPT_enum: return 4;	//enum size should be alway 4 bytes
		case EPT_TArray: return sizeof(TArray<Void>);
		case EPT_TObjectPtr: return sizeof(TObjectPtr<Object>);
		case EPT_ObjectPoniter: return sizeof(Object*);
		case EPT_MetaClass: return 0; //meta class should use TypeInfo
		}
		return 0;
	}

	bool PropertyType_IsArithmetic(EPropertyType propertyType)
	{
		bool lut[] =
		{
			0,
			1,
			1,1,  1,1,  1,1,  1,1, 
			1,1,
			0,
			0,0,0,0,0,0,0
		};
		return lut[(unsigned)propertyType];
	}

	const TypeInfo* PropertyType_GetTypeInfo(EPropertyType propertyType)
	{
		static Name lut[] =
		{
			nullptr,
			typeid(bool).name(),
			typeid(int8).name(), typeid(uint8).name(), typeid(int16).name(), typeid(uint16).name(), 
			typeid(int32).name(), typeid(uint32).name(), typeid(int64).name(), typeid(uint64).name(),
			typeid(float).name(), typeid(double).name(), 
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
		};
		return MetaSys::Get()->FindType(lut[(unsigned)propertyType]);
	}

};