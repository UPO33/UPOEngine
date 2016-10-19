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
	//////////////////////////////////////////////////////////////////////////
	String PropertyInfo::GetLegibleName() const
	{
		return String(mPropertyName.CStr(), mPropertyName.Length());
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
			mParentClass = MetaSys::Get()->FindClass(mParentClassName);
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

		ClassInfo* iter = mParentClass;
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

	bool ClassInfo::HasProperty(const PropertyInfo* prp) const
	{
		if (prp == nullptr) return false;
		if (mProperties.Length() == 0) return false;
		return mProperties.IsWithin(prp);
	}

	ClassInfo* ClassInfo::GetRoot() const
	{
		ClassInfo* p = GetParent();
		while (p->GetParent())
		{
			p = p->GetParent();
		}
		return p;
	}
	//////////////////////////////////////////////////////////////////////////
	void ClassInfo::GetInheritedClasses(TArray<ClassInfo*>& outClasses) const
	{
		outClasses.RemoveAll();
		ClassInfo* parent = GetParent();
		while (parent)
		{
			outClasses.Add(parent);
			parent = parent->GetParent();
		}
		outClasses.Reverse();
	}
	//////////////////////////////////////////////////////////////////////////
	void ClassInfo::GetClassChain(SClassChain& out, bool reverse, bool includingThis) const
	{
		unsigned numClass = 0;
		ClassInfo* classesTowardRoot[MAX_INHERITANCE];

		if (includingThis)
			classesTowardRoot[numClass++] = (ClassInfo*)this;

		ClassInfo* parent = GetParent();
		while (parent)
		{
			classesTowardRoot[numClass++] = parent;
			parent = parent->GetParent();
		}

		if(reverse)
		{
			for (unsigned i = 0; i < numClass; i++)
				out.mClasses[i] = classesTowardRoot[i];
		}
		else
		{
			for (unsigned i = 0; i < numClass; i++)
				out.mClasses[i] = classesTowardRoot[numClass - i - 1];
		}

		out.mNumClass = numClass;
	}

	//////////////////////////////////////////////////////////////////////////
	void ClassInfo::GetInvolvedClasses(TArray<ClassInfo*>& outClasses, bool subProperties, bool inheritedProperties, bool removeArrayFirst) const
	{
		if(removeArrayFirst) outClasses.RemoveAll();

		TArray<ClassInfo*> classes;

		if (inheritedProperties) GetInheritedClasses(classes);

		classes.Add((ClassInfo*)this);

		for (size_t iClass = 0; iClass < classes.Length(); iClass++)
		{
			ClassInfo* cls = classes[iClass];
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

	//////////////////////////////////////////////////////////////////////////
	bool _GetPropertyChain(const ClassInfo* classToFind, const PropertyInfo* propertyToFind, SPropertyChain& out)
	{
		//this, Entity, Object, ..
		SClassChain classesTowardRoot;
		classToFind->GetClassChain(classesTowardRoot, true, true);


		for (unsigned iClass = 0; iClass < classesTowardRoot.mNumClass; iClass++)
		{
			auto cls = classesTowardRoot.mClasses[iClass];
			for (unsigned iProperty = 0; iProperty < cls->NumProperty(); iProperty++)
			{
				PropertyInfo* prp = (PropertyInfo*)cls->GetProperty(iProperty);
				if (prp == propertyToFind) // found?
				{
					out.mProperties[out.mNumProperty++] = prp;
					return true;
				}
				else
				{
					if (prp->GetTypeInfo() && prp->GetTypeInfo()->IsClass())
					{
						out.mProperties[out.mNumProperty++] = prp;
						if (_GetPropertyChain(((ClassInfo*)prp->GetType()), propertyToFind, out))
							return true;
					}
				}
			}
		}
		return false;
	}
	bool ClassInfo::GetPropertyChain(const PropertyInfo* propertyToFind, SPropertyChain& out) const
	{
		out.mNumProperty = 0;
		if (propertyToFind == nullptr) return false;
		return _GetPropertyChain(this, propertyToFind, out);
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
	PropertyInfo* ClassInfo::FindPropertyByName(Name name, bool includingInheritedProperties) const
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

	void SPropertyChain::PerformMetaPropertyChanged(void* object)
	{
		UASSERT(object);

		if (mNumProperty == 0) return;

		void* objs[MAX_INHERITANCE];
		void* obj = object;
		for (unsigned i = 0; i < mNumProperty; i++)
		{
			objs[i] = obj = mProperties[i]->Map(obj);
		}
		for (unsigned i = mNumProperty - 1; i >= 0; i--)
		{
			if (mProperties[i]->GetOwner()->HasMetaPropertyChanged())
			{
				mProperties[i]->GetOwner()->CallMetaPropertyChanged(objs[i], mProperties[i]);
			}
		}
	}

};