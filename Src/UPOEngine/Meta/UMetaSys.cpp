#include "UMetaSys.h"
#include "../Core/UVector.h"


namespace UPO
{
	MetaSys* MetaSys::Get()
	{
		static MetaSys Ins;
		return &Ins;
	}
	//////////////////////////////////////////////////////////////////////////
	void MetaSys::RegisterCommonTypes()
	{

#define UMAKETYPE(Type) \
		{\
			static TypeInfo type; \
			type.mClassInfo = TypeInfo::GetClassInfoStatic(); \
			type.mRefData = nullptr; \
			type.mTypeName = typeid(Type).name(); \
			type.mTypeSize = sizeof(Type); \
			type.mTypeAlign = alignof(Type); \
			type.mHeaderFileName = ""; \
			type.mSourceFileName = ""; \
			type.mDefinitionLineNumber = 0; \
			type.mOwner = nullptr; \
			type.mAttributes = AttribPack(); \
			type.mIsClass = false; \
			type.mIsEnum = false; \
			type.mIsArithmetic = true; \
			mRegisteredTypes.Add(&type);\
		}\


		UMAKETYPE(bool);

		UMAKETYPE(char);
		UMAKETYPE(signed char);
		UMAKETYPE(unsigned char);

		UMAKETYPE(short);
		UMAKETYPE(signed short);
		UMAKETYPE(unsigned short);

		UMAKETYPE(signed);
		UMAKETYPE(unsigned);

		UMAKETYPE(int);
		UMAKETYPE(signed int);
		UMAKETYPE(unsigned int);

		UMAKETYPE(long);
		UMAKETYPE(signed long);
		UMAKETYPE(unsigned long);

		UMAKETYPE(long long);
		UMAKETYPE(signed long long);
		UMAKETYPE(unsigned long long);

		UMAKETYPE(float);
		UMAKETYPE(double);
		UMAKETYPE(long double);

		UMAKETYPE(wchar_t);
		UMAKETYPE(char16_t);
		UMAKETYPE(char32_t);



#undef UMAKETYPE

	}
	//////////////////////////////////////////////////////////////////////////
	MetaSys::MetaSys()
	{	
		RegisterCommonTypes();
	}
	//////////////////////////////////////////////////////////////////////////
	bool MetaSys::RegClass(ZZ_ClassRegParam* crp, ClassInfo* outClass)
	{
		UASSERT(crp && outClass);
		
		Module* curModule = ModuleSys::Get()->GetCurrentLoadingModule();

		if (outClass->mRegistered)
		{
			ULOG_ERROR("class [%s] previously registered at module [%s]", crp->mTypeCheck.mClassName, curModule ? curModule->GetName().CStr() : "null");
			return false;
		}
		
		///////////////Object
		outClass->mClassInfo = ClassInfo::GetClassInfoStatic();
		outClass->mRefData = nullptr;

		//////////////TypeInfo
		outClass->mOwner = curModule;
		outClass->mTypeName = crp->mTypeCheck.mClassName;
		outClass->mAttributes = crp->mAttribPack;
		outClass->mHeaderFileName = crp->mHeaderFileName;
		outClass->mSourceFileName = crp->mSourceFileName;
		outClass->mDefinitionLineNumber = crp->mHeaderFileLine;
		outClass->mTypeAlign = crp->mTypeCheck.mTypeAlign;
		outClass->mTypeSize = crp->mTypeCheck.mTypeSize;
		outClass->mIsArithmetic = false;
		outClass->mIsEnum = false;
		outClass->mIsClass = true;

		////////////ClassInfo
		outClass->mHasParentClass = crp->mTypeCheck.mHasParent;
		outClass->mParentClass = nullptr;
		outClass->mParentClassName = crp->mTypeCheck.mParentClassName;

		outClass->mDefalutConstructor = crp->mTypeCheck.mDefaulConstructor;
		outClass->mDestructor = crp->mTypeCheck.mDestructor;

		outClass->mMetaSerialize = crp->mTypeCheck.mMetaSerialize;
		outClass->mMetaPropertyChanged = crp->mTypeCheck.mMetaPropertyChanged;

		/////////////^^^^^ Meta class functions here ^^^^^^

		

		///////////////////initializing properties
		outClass->mProperties.RemoveAll();
		outClass->mProperties.AddDefault(crp->mNumProperty);

		for (unsigned i = 0; i < crp->mNumProperty; i++)
		{
			RegProperty(crp->mProperties + i, outClass, outClass->mProperties.Elements() + i);
		}
		
		mRegisteredTypes.Add(outClass);
		outClass->mRegistered = true;
		ULOG_SUCCESS("Class [%s] was Registered successfully", crp->mTypeCheck.mClassName);
		RebakeTypes();
		return true;
	}
	void MetaSys::RegProperty(ZZ_PropertyRegParam* regParam, ClassInfo* owner, PropertyInfo* prp)
	{
		prp->mAttributes = regParam->mAttribPack;
		prp->mOffset = regParam->mOffset;
		prp->mOwner = owner;
		prp->mPropertyName = regParam->mName;
		prp->mPropertyType = regParam->mTypeCheck.mPropertyType;
		prp->mTypeName = regParam->mTypeCheck.mPropertyTypeName;
	
		if (prp->mPropertyType == EPropertyType::EPT_TArray)
		{
			prp->mTemplateArgType = regParam->mTypeCheck.mTArrayElementPropertyType;
			prp->mTemplateArgTypeName = regParam->mTypeCheck.mTArrayElementTypeName;
		}
		else if(prp->mPropertyType == EPropertyType::EPT_TObjectPtr)
		{ 
			prp->mTemplateArgType = EPropertyType::EPT_MetaClass;
			prp->mTemplateArgTypeName = regParam->mTypeCheck.mTObjectPtrObjectTypeName;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	bool MetaSys::UnRegClass(ClassInfo* registeredClass)
	{
		if (registeredClass && registeredClass->mRegistered)
		{
			size_t index = mRegisteredTypes.Find(registeredClass);
			if (index != ~0)
			{
				//if has parent remove from parent subclass array
				if (registeredClass->mParentClass) 
					registeredClass->mParentClass->mSubClasses.RemoveSwap(registeredClass);

				mRegisteredTypes.RemoveAtSwap(index);
				ULOG_SUCCESS("Class [%s] Unregistered", registeredClass->GetName().CStr());
				RebakeTypes();
				return true;
			}
		}
		return false;
	}


	
	//////////////////////////////////////////////////////////////////////////
	const TypeInfo* MetaSys::FindType(Name name) const
	{
		for (size_t i = 0; i < mRegisteredTypes.Length(); i++)
		{
			if (mRegisteredTypes[i] && mRegisteredTypes[i]->GetName() == name)
				return mRegisteredTypes[i];
		}
		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	const ClassInfo* MetaSys::FindClass(Name name) const
	{
		auto ti = FindType(name);
		if (ti && ti->IsClass()) return (const ClassInfo*)ti;
		return nullptr;
	}

	//////////////////////////////////////////////////////////////////////////
	bool MetaSys::RebakeTypes()
	{
		for (size_t i = 0; i < mRegisteredTypes.Length(); i++)
		{
			TypeInfo* type = mRegisteredTypes[i];
			if (type && type->IsClass())
				((ClassInfo*)type)->Bake();
		}
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	MetaSys::~MetaSys()
	{

	}
};
