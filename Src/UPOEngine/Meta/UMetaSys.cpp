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
		
		if (FindType(crp->mTypeCheck.mClassName))
		{
			ULOG_ERROR("failed to register class [%s], currently there is a type with the same name", crp->mTypeCheck.mClassName);
			return false;
		}
		
		///////////////Object
		outClass->mClassInfo = ClassInfo::GetClassInfoStatic();
		outClass->mRefData = nullptr;

		//////////////TypeInfo
		outClass->mOwner = ModuleSys::Get()->GetCurrentLoadingModule();
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
		outClass->mIsAbstract = crp->mTypeCheck.mIsAbstract;
		outClass->mHasParentClass = crp->mTypeCheck.mHasParent;
		outClass->mParentClass = nullptr;
		outClass->mParentClassName = crp->mTypeCheck.mParentClassName;

		outClass->mDefalutConstructor = crp->mTypeCheck.mDefaulConstructor;
		outClass->mDestructor = crp->mTypeCheck.mDestructor;

		outClass->mMetaSerialize = crp->mTypeCheck.mMetaSerialize;
		//DEPRECATED
// 		outClass->mMetaPropertyChanged = crp->mTypeCheck.mMetaPropertyChanged;

		outClass->mMetaBeforePropertyChange = crp->mTypeCheck.mMetaBeforePropertyChange;
		outClass->mMetaAfterPropertyChange = crp->mTypeCheck.mMetaAfterPropertyChange;

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
		ULOG_SUCCESS("Class [%] was Registered", crp->mTypeCheck.mClassName);
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
	bool MetaSys::UnregClass(ClassInfo* registeredClass)
	{
		if (registeredClass)
		{
			size_t index = mRegisteredTypes.Find(registeredClass);
			if (index != ~0)
			{
				//if has parent remove from parent subclass array
				if (registeredClass->mParentClass) 
					registeredClass->mParentClass->mSubClasses.RemoveSwap(registeredClass);

				mRegisteredTypes.RemoveAtSwap(index);
				ULOG_SUCCESS("Class [%] Unregistered", registeredClass->GetName());
				RebakeTypes();
				return true;
			}
		}
		return false;
	}


	//////////////////////////////////////////////////////////////////////////
	bool MetaSys::RegEnum(ZZ_EnumRegParam* erp, EnumInfo* outEnum)
	{
		UASSERT(erp && outEnum);

		if (FindType(erp->mEnumName))
		{
			ULOG_ERROR("failed to register enum [%], currently there is a type with the same name", erp->mEnumName);
			return false;
		}

		//////////////////////Object
		outEnum->mClassInfo = EnumInfo::GetClassInfoStatic();
		outEnum->mRefData = nullptr;

		///////////////TypeInfo
		outEnum->mOwner = ModuleSys::Get()->GetCurrentLoadingModule();
		outEnum->mTypeName = erp->mEnumName;
		outEnum->mHeaderFileName = nullptr;
		outEnum->mSourceFileName = erp->mSourceFileName;
		outEnum->mDefinitionLineNumber = erp->mLineNumber;
		outEnum->mTypeAlign = 4;
		outEnum->mTypeSize = 4;
		outEnum->mIsArithmetic = false;
		outEnum->mIsEnum = true;
		outEnum->mIsClass = false;

		/////////////EnumInfo
		outEnum->mComment = erp->mComment;
		outEnum->mIsBitFlag = erp->mIsBitFlag;
		outEnum->mElements.AddDefault(erp->mNumElement);
		TArray<String> elementsName;
		String(erp->mElementsName).Split(',', elementsName);
		//removing spaces
		elementsName.ForEach([](String& str) { str.Trim(); });

		/////////elements
		for (size_t i = 0; i < erp->mNumElement; i++)
		{
			outEnum->mElements[i] = EnumElement(elementsName[i], erp->mElementsValue[i]);
		}

		mRegisteredTypes.Add(outEnum);
		ULOG_SUCCESS("enum [%] was Registered ", erp->mEnumName);
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool MetaSys::UnregEnum(EnumInfo* registeredEnum)
	{
		if (registeredEnum)
		{
			size_t index = mRegisteredTypes.Find(registeredEnum);
			if (index != ~0)
			{
				mRegisteredTypes.RemoveAtSwap(index);
				ULOG_SUCCESS("enum [%] Unregistered", registeredEnum->GetName());
				return true;
			}
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	TypeInfo* MetaSys::FindType(Name name) const
	{
		for (size_t i = 0; i < mRegisteredTypes.Length(); i++)
		{
			if (mRegisteredTypes[i] && mRegisteredTypes[i]->GetName() == name)
				return mRegisteredTypes[i];
		}
		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	ClassInfo* MetaSys::FindClass(Name name) const
	{
		auto ti = FindType(name);
		if (ti && ti->IsClass()) return (ClassInfo*)ti;
		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	EnumInfo* MetaSys::FinEnum(Name name) const
	{
		auto ti = FindType(name);
		if (ti && ti->IsEnum()) return (EnumInfo*)ti;
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
