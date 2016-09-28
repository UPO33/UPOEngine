#pragma once

#include "UTypeInfo.h"
#include "UEnumInfo.h"
#include "UClassInfo.h"
#include "../Object/UObject.h"
#include "../Core/UModule.h"
#include "../Core/UStream.h"


namespace UPO
{
#if 0
	struct ZZ_FakeTArray { using ElementType = int; /*int to not get sizeof error*/ };
	struct ZZ_FakeTObjectPtr { using ObjectType = Object; };

	template <typename T> struct ZZ_TPropertyTypeChecker
	{
		//valid property types : arithmetic, enum, meta class, TArray, Object*, TObjectPtr
		//valid array element types : arithmetic, enum, meta class, Object*
		using PropertyType = T;
		static const bool bPropertyIsRefrence = std::is_reference<T>::value;
		static const bool bPropertyIsArray = std::is_array<T>::value;
		static const bool bPropertyIsPointer = std::is_pointer<T>::value;
		static const bool bPropertyIsPointerPointer = std::is_pointer<std::remove_pointer<T>::type>::value;
		static const bool bPropertyIsObjectPointer = bPropertyIsPointer && std::is_base_of<Object, std::remove_pointer<T>::type>::value;
		static const bool bPropertyIsConst = std::is_const<T>::value;
		static const bool bPropertyIsMetaClass = TT_IsMetaClass<std::remove_pointer<T>::type>::value;
		static const bool bPropertyIsArithmetic = std::is_arithmetic<T>::value;
		static const bool bPropertyIsEnum = std::is_enum<T>::value;
		static const bool bPropertyIsSigned = std::is_signed<T>::value;
		static const bool bPropertyIsBool = std::is_same<T,bool>::value;
		static const bool bPropertyIsFloat = std::is_same<T,float>::value;
		static const bool bPropertyIsDouble = std::is_same<T, double>::value;
		static const bool bPropertyIsIntegral = std::is_integral<T>::value;
		

		static const bool bIsTArray = TT_IsTArray<T>::value;
		using ArrayElementType = typename std::conditional<bIsTArray, T, ZZ_FakeTArray>::type::ElementType;
		static const bool bArrayElementIsRefrence = std::is_reference<ArrayElementType>::value;
		static const bool bArrayElementIsArray = std::is_array<ArrayElementType>::value;
		static const bool bArrayElementIsPointer = std::is_pointer<ArrayElementType>::value;
		static const bool bArrayElementIsPointerPointer = std::is_pointer<std::remove_pointer<ArrayElementType>::type>::value;
		static const bool bArrayElementIsObjectPointer = bArrayElementIsPointer && std::is_base_of<Object, std::remove_pointer<ArrayElementType>::type>::value;
		static const bool bArrayElementIsConst = std::is_const<ArrayElementType>::value;
		static const bool bArrayElementIsMetaClass = TT_IsMetaClass<std::remove_pointer<ArrayElementType>::type>::value;
		static const bool bArrayElementIsArithmetic = std::is_arithmetic<ArrayElementType>::value;
		static const bool bArrayElementIsSigned = std::is_signed<ArrayElementType>::value;
		static const bool bArrayElementIsBool = std::is_same<ArrayElementType, bool>::value;
		static const bool bArrayElementIsFloat = std::is_same<ArrayElementType, float>::value;
		static const bool bArrayElementIsDouble = std::is_same<ArrayElementType, double>::value;
		static const bool bArrayElementIsIntegral = std::is_integral<ArrayElementType>::value;
		static const bool bArrayElementIsEnum = std::is_enum<ArrayElementType>::value;

		//TObjectPtr itself doesn't allow non object argument type so we don't need to check it here 
		static const bool bIsTObjectPtr = TT_IsTObjectPtr<T>::value;
		using ObjectPtrObjectType = typename std::conditional<bIsTObjectPtr, T, ZZ_FakeTObjectPtr>::type::ObjectType;

		//types without any cont or pointer
		using PropertyTypeJC = typename std::remove_const<typename std::remove_pointer<typename PropertyType>::type>::type;
		using ArrayElementTypeJC = typename std::remove_const<typename std::remove_pointer<typename ArrayElementType>::type>::type;;
		using ObjectPtrObjectTypeJC = typename std::remove_const<typename std::remove_pointer<typename ObjectPtrObjectType>::type>::type;;;

		static_assert(!bPropertyIsRefrence, "reference is not allowed");
		static_assert(!bPropertyIsPointerPointer, "pointer to pointer is not allowed");
		static_assert(!bPropertyIsConst, "property must not be const");
		static_assert(!bPropertyIsArray, "array is not supported use TArray instead");
		static_assert(bPropertyIsPointer ? bPropertyIsObjectPointer && bPropertyIsMetaClass : true, "only pointer to meta class Object is allowed");
		static_assert(!bPropertyIsPointer ? bPropertyIsEnum || bPropertyIsArithmetic || bPropertyIsMetaClass || bIsTArray || bIsTObjectPtr : true, "only arithmetic, enum, meta class, TArray, TObjectPtr, Object* are allowed");

		static_assert(bIsTArray ? !bArrayElementIsConst : true, "array element must not be const");
		static_assert(bIsTArray ? !bArrayElementIsArray : true, "array is not supported as TArray element");
		static_assert(bIsTArray ? !bArrayElementIsPointerPointer : true, "pointer to pointer is not allowed");
		static const bool bValidArrayElement = bArrayElementIsPointer ? bArrayElementIsMetaClass && bArrayElementIsObjectPointer
			: bArrayElementIsEnum || bArrayElementIsMetaClass || bArrayElementIsArithmetic;
		static_assert(bIsTArray ? bValidArrayElement : true, "valid TArray element types are arithmetic, enum, meta class, Object*");
	};

	struct ZZ_PropertTypeCheckResult
	{
		size_t mPropertyIsRefrence : 1;
		size_t mPropertyIsArray : 1;
		size_t mPropertyIsPointer : 1;
		size_t mPropertyIsPointerPointer : 1;
		size_t mPropertyIsObjectPointer : 1;
		size_t mPropertyIsConst : 1;
		size_t mPropertyIsMetaClass : 1;
		size_t mPropertyIsArithmetic : 1;
		size_t mPropertyIsEnum : 1;
		size_t mPropertyIsSigned : 1;
		size_t mPropertyIsBool : 1;
		size_t mPropertyIsFloat : 1;
		size_t mPropertyIsDouble : 1;
		size_t mPropertyIsIntegral : 1;

		size_t mIsTArray : 1;

		size_t mArrayElementIsRefrence : 1;
		size_t mArrayElementIsArray : 1;
		size_t mArrayElementIsPointer : 1;
		size_t mArrayElementIsPointerPointer : 1;
		size_t mArrayElementIsObjectPointer : 1;
		size_t mArrayElementIsConst : 1;
		size_t mArrayElementIsMetaClass : 1;
		size_t mArrayElementIsArithmetic : 1;
		size_t mArrayElementIsEnum : 1;
		size_t mArrayElementIsSigned : 1;
		size_t mArrayElementIsBool : 1;
		size_t mArrayElementIsFloat : 1;
		size_t mArrayElementDouble : 1;
		size_t mArrayElementIsIntegral : 1;

		size_t mIsTObjectPtr : 1;

		String mPropertyTypeName;	//perfect name of property type eg: bool, X::Vec3, ..
		String mTArrayElementTypeName;	//prefect name of the TArray element eg: int, X::Object, ...
		String mTObjectPtrObjectTypeName;	//prefect name of TObjectPtr' object

		unsigned mTypeAlign;
		unsigned mTypeSize;
		unsigned mTArrayElementTypeSize;
		unsigned mTArrayElementTypeAlign;


		template<typename Type = int> void Make()
		{
			typedef ZZ_TPropertyTypeChecker<Type> T;
			mPropertyIsRefrence = T::bPropertyIsRefrence;
			mPropertyIsArray = T::bPropertyIsArray;
			mPropertyIsPointer = T::bPropertyIsPointer;
			mPropertyIsPointerPointer = T::bPropertyIsPointerPointer;
			mPropertyIsObjectPointer = T::bPropertyIsObjectPointer;
			mPropertyIsConst = T::bPropertyIsConst;
			mPropertyIsMetaClass = T::bPropertyIsMetaClass;
			mPropertyIsArithmetic = T::bPropertyIsArithmetic;
			mPropertyIsEnum = T::bPropertyIsEnum;
			mPropertyIsSigned = T::bPropertyIsSigned;
			mPropertyIsBool = T::bPropertyIsBool;
			mPropertyIsFloat = T::bPropertyIsFloat;
			mPropertyIsDouble = T::bPropertyIsDouble;
			mPropertyIsIntegral = T::bPropertyIsIntegral;

			mIsTArray = T::bIsTArray;
			mArrayElementIsRefrence = T::bArrayElementIsRefrence;
			mArrayElementIsArray = T::bArrayElementIsArray;
			mArrayElementIsPointer = T::bArrayElementIsPointer;
			mArrayElementIsPointerPointer = T::bArrayElementIsPointerPointer;
			mArrayElementIsObjectPointer = T::bArrayElementIsObjectPointer;
			mArrayElementIsConst = T::bArrayElementIsConst;
			mArrayElementIsMetaClass = T::bArrayElementIsMetaClass;
			mArrayElementIsArithmetic = T::bArrayElementIsArithmetic;
			mArrayElementIsEnum = T::bArrayElementIsEnum;
			mIsTObjectPtr = T::bIsTObjectPtr;

			mTypeSize = sizeof(T::PropertyTypeJC);
			mTypeAlign = alignof(T::PropertyTypeJC);
			mTArrayElementTypeSize = sizeof(T::ArrayElementTypeJC);
			mTArrayElementTypeAlign = alignof(T::ArrayElementTypeJC);

			mPropertyTypeName = mTArrayElementTypeName = mTObjectPtrObjectTypeName = nullptr;

			////////////////////////catching type names with typeid()
			if (mIsTArray)
			{
				if (mArrayElementIsEnum)
					mTArrayElementTypeName = typeid(T::ArrayElementTypeJC).name() + strlen("enum ");
				else if (mArrayElementIsMetaClass)
					mTArrayElementTypeName = strchr(typeid(T::ArrayElementTypeJC).name(), ' ') + 1; //ignore struct or class prefix
				else if (mArrayElementIsArithmetic)
					mTArrayElementTypeName = typeid(T::ArrayElementTypeJC).name();
				else
					UASSERT(false);
				mPropertyTypeName = "UPO::TArray";
			}
			else if (mIsTObjectPtr)
			{
				mTObjectPtrObjectTypeName = strchr(typeid(T::ObjectPtrObjectTypeJC).name(), ' ') + 1; // remove class or struct prefix
				mPropertyTypeName = "UPO::TObjectPtr";
			}
			else
			{
				if (mPropertyIsArithmetic)
					mPropertyTypeName = typeid(T::PropertyTypeJC).name();
				else if (mPropertyIsEnum)
					mPropertyTypeName = typeid(T::PropertyTypeJC).name() + strlen("enum ");
				else if (mPropertyIsMetaClass)
					mPropertyTypeName = strchr(typeid(T::PropertyTypeJC).name(), ' ') + 1; //remove struct or class prefix
				else
					UASSERT(false);
			}
		}
	};


	struct ZZ_PropertyRegParam
	{
		const char*	mName = nullptr;	//eg mVar0;
		ZZ_PropertTypeCheckResult mTypeCheck;
		size_t mOffset = 0;
		AttribPack mAttribPack;
	};
#endif



	/*
	template to convert any type to EPropertyType
	*/
	template < typename T> struct ZZ_AsEPropertyType
	{
		static const bool bIsPointer = std::is_pointer<T>::value;
		static const bool bIsMetaClass = TT_IsMetaClass<std::remove_pointer<T>::type>::value;
		static const bool bIsObjectPointer = bIsPointer && bIsMetaClass && std::is_base_of<Object, std::remove_pointer<T>::type>::value;
		static const bool bIsEnum = std::is_enum<T>::value;
		static const bool bIsTArray = TT_IsTArray<T>::value;
		static const bool bIsTObjectPtr = TT_IsTObjectPtr<T>::value;

		static const EPropertyType value = bIsEnum ? EPT_enum : bIsTArray ? EPT_TArray : bIsTObjectPtr ? EPT_TObjectPtr :
			bIsPointer ? (bIsObjectPointer ? EPT_ObjectPoniter : EPT_Unknown) : bIsMetaClass ? EPT_MetaClass : EPT_Unknown;
	};
	template<> struct ZZ_AsEPropertyType<bool>
	{
		static const EPropertyType value = EPropertyType::EPT_bool;
	};
	template<> struct ZZ_AsEPropertyType<char>
	{
		static const EPropertyType value = EPropertyType::EPT_int8;
	};
	template<> struct ZZ_AsEPropertyType<unsigned char>
	{
		static const EPropertyType value = EPropertyType::EPT_uint8;
	};
	template<> struct ZZ_AsEPropertyType<short>
	{
		static const EPropertyType value = EPropertyType::EPT_int16;
	};
	template<> struct ZZ_AsEPropertyType<unsigned short>
	{
		static const EPropertyType value = EPropertyType::EPT_uint16;
	};
	template<> struct ZZ_AsEPropertyType<int>
	{
		static const EPropertyType value = EPropertyType::EPT_int32;
	};
	template<> struct ZZ_AsEPropertyType<unsigned int>
	{
		static const EPropertyType value = EPropertyType::EPT_uint32;
	};
	template<> struct ZZ_AsEPropertyType<long>
	{
		static const EPropertyType value = EPropertyType::EPT_int32;
	};
	template<> struct ZZ_AsEPropertyType<unsigned long>
	{
		static const EPropertyType value = EPropertyType::EPT_uint32;
	};
	template<> struct ZZ_AsEPropertyType<long long>
	{
		static const EPropertyType value = EPropertyType::EPT_int64;
	};
	template<> struct ZZ_AsEPropertyType<unsigned long long>
	{
		static const EPropertyType value = EPropertyType::EPT_uint64;
	};
	template<> struct ZZ_AsEPropertyType<float>
	{
		static const EPropertyType value = EPropertyType::EPT_float;
	};
	template<> struct ZZ_AsEPropertyType<double>
	{
		static const EPropertyType value = EPropertyType::EPT_double;
	};
	/*
	template to check a property type, error and ...
	*/
	template<typename T> struct ZZ_TPropertyTypeCheck
	{
		struct ZZ_FakeTArray { using ElementType = void*; /*void* to not get sizeof error*/ };
		struct ZZ_FakeTObjectPtr { using ObjectType = Object; };

		static_assert(std::is_enum<T>::value ? sizeof(T) == 4 : true, "only 32bit enum is supported");

		static const bool bIsTArray = TT_IsTArray<T>::value;
		static const bool bIsTObjectPtr = TT_IsTObjectPtr<T>::value;

		using ArrayElementType = typename std::conditional<bIsTArray, T, ZZ_FakeTArray>::type::ElementType;
		using ObjectPtrObjectType = typename std::conditional<bIsTObjectPtr, T, ZZ_FakeTObjectPtr>::type::ObjectType;

		//types without any cont or pointer
		using PropertyTypeJC = typename std::remove_const<typename std::remove_pointer<typename T>::type>::type;
		using ArrayElementTypeJC = typename std::remove_const<typename std::remove_pointer<typename ArrayElementType>::type>::type;;
		using ObjectPtrObjectTypeJC = typename std::remove_const<typename std::remove_pointer<typename ObjectPtrObjectType>::type>::type;;;

		static const EPropertyType PropertyType = ZZ_AsEPropertyType<T>::value;
		static_assert(PropertyType != EPropertyType::EPT_Unknown, "invalid property type, the valid types are arithmetic, enum, meta class, TArray, Object*, TObjectPtr");
		static const EPropertyType ArrayElementPropertyType = ZZ_AsEPropertyType<ArrayElementType>::value;
		static const bool ArrayElementIsInvalid = ArrayElementPropertyType == EPropertyType::EPT_Unknown || ArrayElementPropertyType == EPropertyType::EPT_TArray || ArrayElementPropertyType == EPropertyType::EPT_TObjectPtr;
		static_assert(!(bIsTArray && ArrayElementIsInvalid), "invalid TArray element, the valid types are arithmetic, enum, meta class, Object*");

	};
	/*

	*/
	struct ZZ_PropertyCheckResult
	{
		EPropertyType	mPropertyType;
		EPropertyType	mTArrayElementPropertyType;	//valid if  mPropertyType == EPT_TArray
		String			mPropertyTypeName;	//perfect name of property type eg: bool, UPO::Vec3, ..
		String			mTArrayElementTypeName;	//prefect name of the TArray element eg: int, UPO::Object, ...
		String			mTObjectPtrObjectTypeName;	//prefect name of TObjectPtr's object
// 		size_t			mTypeSize;
// 		size_t			mTypeAlign;
// 		size_t			mTArrayElementTypeSize;
// 		size_t			mTArrayElementTypeAlign;

		template<typename T> void Make()
		{
			mPropertyType = T::PropertyType;
			mTArrayElementPropertyType = T::ArrayElementPropertyType;
// 			mTypeSize = sizeof(T::PropertyTypeJC);
// 			mTypeAlign = alignof(T::PropertyTypeJC);
// 			mTArrayElementTypeSize = sizeof(T::ArrayElementTypeJC);
// 			mTArrayElementTypeAlign = alignof(T::ArrayElementTypeJC);

			mPropertyTypeName = mTArrayElementTypeName = mTObjectPtrObjectTypeName = nullptr;

			////////////////////////catching type names with typeid()
			if (mPropertyType == EPropertyType::EPT_TArray)
			{
				mPropertyTypeName = "UPO::TArray";
				if (mTArrayElementPropertyType == EPropertyType::EPT_MetaClass
					|| mTArrayElementPropertyType == EPropertyType::EPT_ObjectPoniter
					|| mTArrayElementPropertyType == EPropertyType::EPT_enum)
				{
					mTArrayElementTypeName = strchr(typeid(T::ArrayElementTypeJC).name(), ' ') + 1; //remove struct or class or enum prefix
				}
				else
				{
					mTArrayElementTypeName = typeid(T::ArrayElementTypeJC).name();
				}
			}
			else if (mPropertyType == EPropertyType::EPT_TObjectPtr)
			{
				mTObjectPtrObjectTypeName = strchr(typeid(T::ObjectPtrObjectTypeJC).name(), ' ') + 1; // remove class or struct prefix
				mPropertyTypeName = "UPO::TObjectPtr";
			}
			else if(mPropertyType == EPropertyType::EPT_MetaClass 
				|| mPropertyType == EPropertyType::EPT_ObjectPoniter
				|| mPropertyType == EPropertyType::EPT_enum)
			{
				mPropertyTypeName = strchr(typeid(T::PropertyTypeJC).name(), ' ') + 1; //remove struct or class or enum prefix
			}
			else
			{
				mPropertyTypeName = typeid(T::PropertyTypeJC).name();
			}
		}
	};
	struct ZZ_PropertyRegParam
	{
		ZZ_PropertyCheckResult mTypeCheck;
		const char* mName;
		unsigned	mOffset;
		AttribPack mAttribPack;
	};


	UDECLARE_MEMBERFUNCTION_CHECKING(MetaSerializeCheck, MetaSerialize, void, UPO::Stream&);
	UDECLARE_MEMBERFUNCTION_CHECKING(MetaPropertyChangedCheck, MetaPropertyChanged, void, const UPO::PropertyInfo*);


	///////////new meta class function declaration here ^^^^^^^^^^^^^



	struct ZZ_ClassTypeCheckResult
	{
		const char* mClassName = nullptr;	//eg UPO::EntityStaticMesh 
		const char* mParentClassName = nullptr;	//eg UPO::Entity
		bool mHasParent = false;
		size_t mTypeSize = 0;
		size_t mTypeAlign = 0;
		// 	size_t mParentTypeSize = 0;
		// 	size_t mParentTypeAlign = 0;

		UPO::TFP<void, void*>								mDefaulConstructor = nullptr;
		UPO::TFP<void, void*>								mDestructor = nullptr;

		UPO::TMFP<void, Stream&>				mMetaSerialize = nullptr;
		UPO::TMFP<void, const PropertyInfo*>	mMetaPropertyChanged = nullptr;

		template<typename TClass, typename TParent> void Make()
		{
			// 		static_assert(std::is_class<TClass>::value, "XClass arg0 must be class");
			// 		static_assert(std::is_default_constructible<TClass>::value && std::is_destructible<TClass>::value, "Class must be default constructible and destructible");
			mHasParent = !std::is_void<TParent>::value  && !std::is_null_pointer<TParent>::value;
			mParentClassName = mHasParent ? (strchr(typeid(TParent).name(), ' ') + 1) : nullptr;
			mClassName = strchr(typeid(TClass).name(), ' ') + 1;
			mTypeSize = sizeof(TClass);
			mTypeAlign = sizeof(TClass);
			// 		mParentTypeSize = sizeof(void);
			// 		mParentTypeAlign = alignof(void);

			mDefaulConstructor = [](void* object) { new (object) TClass; };
			mDestructor = [](void* object) { ((TClass*)object)->~TClass(); };

			{
				mMetaSerialize = UCLASS_GET_MEMBERFUNCTION(MetaSerializeCheck, TClass);
				mMetaPropertyChanged = UCLASS_GET_MEMBERFUNCTION(MetaPropertyChangedCheck, TClass);
			}

		}
	};


	struct ZZ_ClassRegParam
	{
		static const unsigned MAX_PROPERTY_COUNT = 512;

		const char* mHeaderFileName = nullptr;
		const char* mSourceFileName = nullptr;
		int	mHeaderFileLine = 0;
		int mSourceFileLine = 0;
		ZZ_ClassTypeCheckResult mTypeCheck;
		ZZ_PropertyRegParam mProperties[MAX_PROPERTY_COUNT];
		size_t mNumProperty = 0;

		AttribPack mAttribPack;
		bool mIsRegistered = false;
	};


};	//namespace UPO

#if 1
#define UCLASS_BEGIN_IMPL(Class, ...)\
	struct ZZZ_##Class\
	{\
		typedef Class TClass;\
		typedef Class::Parent TParentClass;\
		UPO::ClassInfo mClassInfo;\
		ZZZ_##Class()\
		{\
			UPO::ZZ_ClassRegParam crp;\
			crp.mTypeCheck.Make<TClass, TParentClass>();\
			crp.mHeaderFileName = TClass::ZZZFileName();\
			crp.mHeaderFileLine = TClass::ZZZLineNumber();\
			crp.mSourceFileName = __FILE__;\
			crp.mSourceFileLine = __LINE__;\
			crp.mAttribPack = UPO::AttribPack(__VA_ARGS__);\
			crp.mNumProperty = 0;\




#define UPROPERTY(Property, ...)\
			{\
				typedef decltype(TClass::##Property) TPropertyType;\
				crp.mProperties[crp.mNumProperty].mTypeCheck.Make<ZZ_TPropertyTypeCheck<TPropertyType>>();\
				crp.mProperties[crp.mNumProperty].mName = #Property;\
				crp.mProperties[crp.mNumProperty].mOffset = offsetof(TClass, Property);\
				crp.mProperties[crp.mNumProperty].mAttribPack = UPO::AttribPack(__VA_ARGS__);\
				crp.mNumProperty++;\
			}\


#define UCLASS_END_IMPL(Class)\
			UPO::MetaSys::Get()->RegClass(&crp, &mClassInfo); \
		}\
		~ZZZ_##Class()\
		{\
			UPO::MetaSys::Get()->UnRegClass(&mClassInfo);\
		}\
	} ZZZ_##Class##Instance;\
	const UPO::ClassInfo* Class##::GetClassInfoStatic() { return &ZZZ_##Class##Instance.mClassInfo; }\

#else
#define UCLASS_BEGIN_IMPL(Class, ...)
#define UPROPERTY(Property, ...)
#define UCLASS_END_IMPL(Class)
#endif


//////////////////////////////////////////////////////////////////////////
namespace UPO
{
	class UAPI MetaSys
	{
	public:
		TArray<TypeInfo*>	mRegisteredTypes;
// 		TArray<ClassInfo*>	mRegisteredClass;

		void RegisterCommonTypes();

	public:
		static MetaSys* Get();
		
		MetaSys();
		~MetaSys();

		bool RegClass(ZZ_ClassRegParam* crp, ClassInfo* outClass);
		bool UnRegClass(ClassInfo* registeredClass);

		//finds a specified type by name eg : int, UPO::Vec3, UPO::Object, ...
		const TypeInfo* FindType(Name name) const;
		const ClassInfo* FindClass(Name name) const;
		const TArray<TypeInfo*>&	GetAllTypes() const { return mRegisteredTypes; }

	private:
		void RegProperty(ZZ_PropertyRegParam* regParam, ClassInfo* owner, PropertyInfo* prp);
		bool RebakeTypes();
	};
};

// template <typename T> X::TypeInfo* GetTypeInfo()
// {
// 	const char* name = typeid(T).name();
// 	if (std::is_class<T>::value)
// 	{
// 		return X::MetaSys::Get()->FindClass(name + (name[0] == 'c' ? 6 /*strlen("class ")*/ : 7 /*strlen("struct ")*/));
// 	}
// 	else if (std::is_enum<T>::value)
// 	{
// 		return X::MetaSys::Get->FindEnum(name + 5 /*strlen("enum ")*/);
// 	}
// 	return X::MetaSys::Get()->FindCommonType(name);
// }
// template<typename T> X::ClassInfo* GetClassInfo()
// {
// 	static_assert(std::is_class<T>::value, "T must be class or struct");
// 	const char* name = typeid(T).name();
// 	return X::MetaSys::Get()->FindClass(name + (name[0] == 'c' ? 6 /*strlen("class ")*/ : 7 /*strlen("struct ")*/));
// }






