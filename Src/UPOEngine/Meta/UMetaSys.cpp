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
		
		////////////Object
		outClass->mClassInfo = ClassInfo::GetClassInfoStatic();
		outClass->mRefData = nullptr;

		////////////ClassInfo
		outClass->mOwner = curModule;
		outClass->mTypeName = crp->mTypeCheck.mClassName;
		outClass->mCustomSerializer = crp->mTypeCheck.mCustomSerializer;
		outClass->mDefalutConstructor = crp->mTypeCheck.mDefaulConstructor;
		outClass->mDestructor = crp->mTypeCheck.mDestructor;
		outClass->mHasParentClass = crp->mTypeCheck.mHasParent;
		outClass->mParentClass = nullptr;
		outClass->mParentClassName = crp->mTypeCheck.mParentClassName;
		outClass->mAttributes = crp->mAttribPack;
		outClass->mHeaderFileName = crp->mHeaderFileName;
		outClass->mSourceFileName = crp->mSourceFileName;
		outClass->mDefinitionLineNumber = crp->mHeaderFileLine;
		outClass->mTypeAlign = crp->mTypeCheck.mTypeAlign;
		outClass->mTypeSize = crp->mTypeCheck.mTypeSize;

		outClass->mIsArithmetic = false;
		outClass->mIsEnum = false;
		outClass->mIsClass = true;

		outClass->mProperties.Empty();

		///////////////////initializing properties
		for (unsigned i = 0; i < crp->mNumProperty; i++)
		{
			outClass->mProperties.Add();
			PropertyInfo& prop = outClass->mProperties.LastElement();
			ZZ_PropertyRegParam& propParam = crp->mProperties[i];

			prop.mOwner = outClass;
			prop.mPropertyName = propParam.mName;
			prop.mOffset = propParam.mOffset;
			prop.mTypeName = propParam.mTypeCheck.mPropertyTypeName;
			prop.mAttributes = propParam.mAttribPack;
			prop.mTypeInfo = nullptr;
			prop.mPropertyType = propParam.mTypeCheck.mPropertyType;
			prop.mTemplateArgType = propParam.mTypeCheck.mTArrayElementPropertyType;
			prop.mTemplateArgTypeName = propParam.mTypeCheck.mTArrayElementTypeName;
			prop.mTemplateArgTypeInfo = nullptr;
		}
		
		mRegisteredTypes.Add(outClass);
		outClass->mRegistered = true;
		ULOG_SUCCESS("Class [%s] was Registered successfully", crp->mTypeCheck.mClassName);
		RebakeTypes();
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool MetaSys::UnRegClass(ClassInfo* registeredClass)
	{
		if (registeredClass && registeredClass->mRegistered)
		{
			size_t index = mRegisteredTypes.Find(registeredClass);
			if (index != ~0)
			{
				mRegisteredTypes.RemoveAtSwap(index);
				ULOG_SUCCESS("Class [%s] Unregistered", registeredClass->GetName().CStr());
				RebakeTypes();
				return true;
			}
		}
		return false;
	}


	//////////////////////////////////////////////////////////////////////////
	const TypeInfo* MetaSys::FindType(const char* name) const
	{
		Name nameAsName = name;
		for (size_t i = 0; i < mRegisteredTypes.Length(); i++)
		{
			if (mRegisteredTypes[i] && mRegisteredTypes[i]->GetName() == nameAsName)
				return mRegisteredTypes[i];
		}
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
