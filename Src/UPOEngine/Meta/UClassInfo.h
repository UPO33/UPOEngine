#pragma once

#include "UTypeInfo.h"


namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class ClassInfo;
	class Serializer;

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

	//////////////////////////////////////////////////////////////////////////
	class UAPI PropertyInfo
	{
		friend class MetaSys;
	public:
		/*
		unregistered type , template , not baked property == null
		*/
		const TypeInfo*			mTypeInfo = nullptr;
		/*
		name of the type, e.g: int, UPO::Vec3,
		for templates its UPO::TArray, UPO::TObjectPtr, etc.. 
		*/
		Name					mTypeName;
		Name					mPropertyName;
		EPropertyType			mPropertyType = EPropertyType::EPT_Unknown;
		unsigned				mOffset = 0;
		AttribPack				mAttributes;
		const ClassInfo*		mOwner = nullptr;
		
		const TypeInfo*			mTemplateArgTypeInfo = nullptr;
		Name					mTemplateArgTypeName;
		EPropertyType			mTemplateArgType = EPropertyType::EPT_Unknown;


		void Bake();
		bool NeedBake() const;
		const TypeInfo* GetTypeInfo() const { return mTypeInfo; }
		EPropertyType GetType() const { return mPropertyType; }
		const Name& GetName() const { return mPropertyName; }
		const Name& GetTypeName() const { return mTypeName; }
		unsigned GetOffset() const { return mOffset; }
		const AttribPack& GetAttributes() const { return mAttributes; }
		const ClassInfo* GetOwner() const { return mOwner; }

		size_t GetTypeSize() const
		{
			if (mTypeInfo) return mTypeInfo->GetSize();
			return 0;/////////////////////////////////////////TODO
		}
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
		ClassInfo*					mParentClass = nullptr;	//null if not backed
		Name						mParentClassName;
		bool						mHasParentClass = false;
		bool						mErrorUnregisteredParent = false;
		FP<void,void*>				mDefalutConstructor = nullptr;
		FP<void,void*>				mDestructor = nullptr;
		FP<void, void*, Stream&>	mCustomSerializer = nullptr; // valid if implemented

		MFP<void, const PropertyInfo*>	mMetaPropertyChanged = nullptr;
		MFP<void, Stream&>				mMetaSerialize = nullptr;
		
		TArray<PropertyInfo>		mProperties;
		TArray<const ClassInfo*>	mSubClasses;
		bool						mRegistered = false;
		
		
		void Bake();
		bool NeedBake() const;

	public:
		bool IsObject() const;
		bool IsAsset() const;
		bool IsBaseOf(const ClassInfo* base) const;

		//calls default constructor on object
		bool CallDefaultConstructor(void* object) const;
		//calls destructor on object
		bool CallDestructor(void* object) const;

		auto GetCustomSerializeFunction() const { return mCustomSerializer; }
		bool HasCustomSerializer() const { return mCustomSerializer != nullptr; }

// 		bool HasMetaPropertyChanged() const { return mMetaPropertyChanged != nullptr; }
		bool CallMetaPropertyChanged(void* object, const PropertyInfo* prp) const;

		size_t NumProperty() const { return mProperties.Length(); }
		const TArray<PropertyInfo>& GetProperties() const { return mProperties; }
		const PropertyInfo& GetProperty(size_t index) const { return mProperties[index]; }
		bool HasParent() const { return !mParentClassName.IsEmpty(); }
		ClassInfo* GetParent() const { return mParentClass; }
		Name GetParentName() const { return mParentClassName; }

		void GetAlUsedClassesInProperties(TArray<ClassInfo*>& outClasses);

		//get all classes that inherit from this class
		const TArray<const ClassInfo*>& GetSubClasses() const { return mSubClasses; }
		void PrintLog();


	};
	
	//////////////////////////////////////////////////////////////////////////
// 	class UAPI Serializer
// 	{
// 		struct UAPI SerProcess
// 		{
// 			const ClassInfo*	mClassInfo;
// 			void*				mObject;
// 			Stream				mStream;
// 
// 			SerProcess(const ClassInfo* ci, void* object, Stream& stream) : mClassInfo(ci), mObject(object), mStream(stream) { }
// 
// 			void Begin();
// 
// 			void WriteToeknBegin()
// 			{
// 			}
// 			void WriteTokenEnd()
// 			{
// 
// 			}
// 			void WriteName(const Name& name)
// 			{
// 				uint16 len = name.Length();
// 				mStream.RW(len);
// 				if (len) mStream.Bytes((void*)name.CStr(), len);
// 			}
// 			void WriteEPropertyType(EPropertType type)
// 			{
// 				uint8 prpType = (uint8)type;
// 				mStream.RW(prpType);
// 			}
// 			void WriteProperty(const PropertyInfo* prp, void* object)
// 			{
// 				WriteName(prp->GetName());
// 				WriteEPropertyType(prp->GetType());
// 				void* propValue = prp->Map(object);
// 
// 				switch (prp->GetType())
// 				{
// 				case EPropertType::EPT_ObjectPoniter:
// 				{
// 					Object** ppObject = ((Object**)propValue);
// 					if (*ppObject == nullptr)
// 					{
// 						uint32 zero = 0;
// 						mStream.RW(zero);
// 					}
// 					else
// 					{
// 
// 					}
// 				}
// 				break;
// 				case EPropertType::EPT_TArray :
// 				{
// 
// 				};
// 				break;
// 				case EPropertType::EPT_TObjectPtr:
// 				{
// 
// 				}
// 				break;
// 				case EPropertType::EPT_MetaClass :
// 				{
// 					ClassInfo* metaClass = (ClassInfo*)prp->GetTypeInfo();
// 					WriteName(metaClass->GetName());
// 					WriteToeknBegin();
// 					for (size_t i = 0; i < metaClass->NumProperty(); i++)
// 					{
// 						WriteProperty(&metaClass->GetProperty(i), propValue);
// 					}
// 					WriteToeknBegin();
// 				}
// 				break;
// 				default:
// 				{
// 					mStream.Bytes(propValue, prp->GetTypeSize());
// 				}
// 				break;
// 				}
// 			}
// 			void Ser(const PropertyInfo* property, void* object)
// 			{
// 				//write property type
// 				if (property->IsArithmetic() || property->IsEnum())
// 				{
// 					mStream.Bytes(object, property->GetTypeSize());
// 				}
// 			}
// 		};
// 		void Serialize(const ClassInfo* classInfo, void* object, Stream& stream);
// 
// 	};

};

