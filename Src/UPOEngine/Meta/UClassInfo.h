#pragma once

#include "UTypeInfo.h"


namespace UPO
{
	static const unsigned MAX_INHERITANCE = 32;

	//////////////////////////////////////////////////////////////////////////
	class ClassInfo;
	class Serializer;
	class PropertyInfo;

	//////////////////////////////////////////////////////////////////////////
	struct UAPI SPropertyChain
	{
		PropertyInfo* mProperties[MAX_INHERITANCE];
		unsigned mNumProperty;
//DEPRECATED
// 		//performs MetaPropertyChanged from down to up
// 		//@object : the object that first property in inside it
// 		void PerformMetaPropertyChanged(void* object);
	};

	//////////////////////////////////////////////////////////////////////////
	struct UAPI SClassChain
	{
		ClassInfo* mClasses[MAX_INHERITANCE];
		unsigned mNumClass;
	};

	//////////////////////////////////////////////////////////////////////////
	enum EPropertyType
	{
		EPT_Unknown,
		EPT_bool,
		EPT_int8, EPT_uint8, EPT_int16, EPT_uint16, EPT_int32, EPT_uint32, EPT_int64, EPT_uint64,	//integral types
		EPT_float, EPT_double,	//floating point types
		EPT_enum,
		EPT_TArray, EPT_TObjectPtr,
		EPT_ObjectPoniter, //Object*
		EPT_MetaClass
	};

	UAPI size_t PropertyType_GetTypeSize(EPropertyType propertyType);
	UAPI bool PropertyType_IsArithmetic(EPropertyType propertyType);
	UAPI const TypeInfo* PropertyType_GetTypeInfo(EPropertyType propertyType);

	//////////////////////////////////////////////////////////////////////////
	class UAPI PropertyInfo
	{
		friend class MetaSys;
	public:
		/*
		note: unregistered type , template , not baked property == null
		*/
		TypeInfo*				mTypeInfo = nullptr;
		/*
		name of the type, e.g: int, UPO::Vec3,
		for templates its UPO::TArray, UPO::TObjectPtr, etc.. 
		*/
		Name					mTypeName;
		Name					mPropertyName;
		EPropertyType			mPropertyType = EPropertyType::EPT_Unknown;
		unsigned				mOffset = 0;
		AttribPack				mAttributes;
		ClassInfo*				mOwner = nullptr;
		
		TypeInfo*				mTemplateArgTypeInfo = nullptr;
		Name					mTemplateArgTypeName;
		EPropertyType			mTemplateArgType = EPropertyType::EPT_Unknown;


		void Bake();
		bool NeedBake() const;
		TypeInfo* GetTypeInfo() const { return mTypeInfo; }
		EPropertyType GetType() const { return mPropertyType; }
		const Name& GetName() const { return mPropertyName; }
		String GetLegibleName() const;
		const Name& GetTypeName() const { return mTypeName; }
		unsigned GetOffset() const { return mOffset; }
		const AttribPack& GetAttributes() const { return mAttributes; }
		bool HasAttrib(EAtrribID id) const { return mAttributes.HasAttrib(id); }
		bool GetAttrib(EAtrribID id, Attrib& out) const { return mAttributes.GetAttrib(id, out); }
		ClassInfo* GetOwner() const { return mOwner; }

		size_t GetTypeSize() const;

		TypeInfo* TemplateArgTypeInfo() const { return mTemplateArgTypeInfo; }
		const Name& TemplateArgTypeName() const { return mTemplateArgTypeName; }
		EPropertyType TemplateArgType() const { return mTemplateArgType; }
		size_t TemplateArgTypeSize() const;

		bool IsEnum() const { return mPropertyType == EPropertyType::EPT_enum; }
		bool IsTemplate() const 
		{ 
			return mPropertyType == EPropertyType::EPT_TArray || mPropertyType == EPropertyType::EPT_TObjectPtr;
		}
		bool IsArithmetic() const
		{ 
			return !IsTemplate() && !IsEnum() && mPropertyType != EPropertyType::EPT_MetaClass && mPropertyType != EPropertyType::EPT_ObjectPoniter;
		}
		bool HasAnyError() const 
		{
			if (mTypeInfo == nullptr) return true;
			if (IsTemplate() && mTemplateArgTypeInfo == nullptr) return true;
			return false;
		}

		void* Map(void* object) const
		{
			return ((char*)object) + mOffset;
		}
		void PrintLog();
	};

	

	//////////////////////////////////////////////////////////////////////////
	class UAPI ClassInfo : public TypeInfo
	{
		UCLASS(ClassInfo, TypeInfo)

		friend class MetaSys;

	private:
		ClassInfo*							mParentClass = nullptr;	//null if not backed
		Name								mParentClassName;
		bool								mHasParentClass = false;
		bool								mErrorUnregisteredParent = false;

		TFP<void,void*>						mDefalutConstructor = nullptr;
		TFP<void,void*>						mDestructor = nullptr;
		
		//DEPRECATED
// 		TMFP<void, const PropertyInfo*>		mMetaPropertyChanged = nullptr;

		TMFP<void, Stream&>					mMetaSerialize = nullptr;

		TMFP<void, const PropertyInfo*>		mMetaBeforePropertyChange = nullptr;
		TMFP<void, const PropertyInfo*>		mMetaAfterPropertyChange = nullptr;

		TArray<PropertyInfo>				mProperties;
		TArray<ClassInfo*>					mSubClasses;
		bool								mIsAbstract;
		bool								mRegistered = false;
		
		
		void Bake();
		bool NeedBake() const;

	public:
		bool IsBaseOf(const ClassInfo* base) const;
		template<typename T> bool IsBaseOf() const
		{
			return IsBaseOf(T::GetClassInfoStatic());
		}
		bool IsAbstract() const { return mIsAbstract; }

		//calls default constructor on object
		bool Has_DefaultConstructor() const { return mDefalutConstructor; }
		void Call_DefaultConstructor(void* object) const;
		//calls destructor on object
		void Call_Destructor(void* object) const;
		bool Has_Destructor() const { return mDestructor; }

		//DEPRECATED
// 		bool HasMetaPropertyChanged() const { return mMetaPropertyChanged != nullptr; }
// 		void CallMetaPropertyChanged(void* object, const PropertyInfo* prp) const;

		bool Has_MetaBeforePropertyChange() const { return mMetaBeforePropertyChange; }
		void Call_MetaBeforePropertyChange(void* obj, const PropertyInfo* prp);
		bool Has_MetaAfterPropertyChange() const { return mMetaAfterPropertyChange; }
		void Call_MetaAfterPropertyChange(void* obj, const PropertyInfo* prp);



		bool Has_MetaSerialize() const { return mMetaSerialize; }
		void Call_MetaSerialize(void* object, Stream& stream) const;

		size_t NumProperty() const { return mProperties.Length(); }
		const TArray<PropertyInfo>& GetProperties() const { return mProperties; }
		const PropertyInfo* GetProperty(size_t index) const { return mProperties.Elements() + index; }
		//return true if this class has specified property
		bool HasProperty(const PropertyInfo* prp) const;

		bool HasParent() const { return !mParentClassName.IsEmpty(); }
		ClassInfo* GetParent() const { return mParentClass; }
		Name GetParentName() const { return mParentClassName; }
		ClassInfo* GetRoot() const;
		//Get all inherited classes from root
		//e.g Object -> Entity -> Component -> 
		void GetInheritedClasses(TArray<ClassInfo*>& outClasses) const;

		//Get all inherited classes from/toward root
		//e.g Object -> Entity -> Component -> ... 
		//@includingThis : add this class to list too
		void GetClassChain(SClassChain& out, bool reverse = false, bool includingThis = true) const;

		void GetInvolvedClasses(TArray<ClassInfo*>& outClasses, bool subProperties, bool inheritedProperties, bool removeArrayFirst = true) const;

		bool GetPropertyChain(const PropertyInfo* propertyToFind, SPropertyChain& out) const;

		//get all classes that inherit from this class
		const TArray<ClassInfo*>& GetSubClasses() const { return mSubClasses; }
		void PrintLog();

		PropertyInfo* FindPropertyByName(Name name, bool includingInheritedProperties) const;

	};
	

};

