#pragma once

#include <type_traits>

#include "../Core/UName.h"
#include "../Core/UString.h"
#include "../Core/UArray.h"
#include "../Object/UObject.h"

//by default the properties, classes and enums has no attributes

//override the default name of property
#define UATTR_Name(name) Attrib(EAttribID::EAT_Name, (const char*)name)

#define UATTR_Comment(Comment) Attrib(EAttribID::EAT_Comment, (const char*)Comment)
//expose the property in the specified category
#define UATTR_Category(Category) Attrib(EAttribID::EAT_Category, (const char*)Category)
//specify the minimum and maximum value for the numeric properties
#define UATTR_Range(Min, Max) Attrib(EAttribID::EAT_Range, (float)Min, (float)Max)
//abstract class cant be instanced
#define UATTR_Abstract() Attrib(EAttribID::EAT_Abstract)
//the property can not be edited in editor
#define UATTR_Uneditable() Attrib(EAttribID::EAT_Uneditable)
//the property is hidden in editor
#define UATTR_Hidden() Attrib(EAttribID::EAT_Hidden)
//volatile property or class will no be serialized/deserialized
#define UATTR_Volatile() Attrib(EAttribID::EAT_Volatile)
//a class that has this attribute can be instantiated through editor
#define UATTR_Instanceable() Attrib(EAttribID::EAT_Instanceable)
#define UATTR_Icon(Name) Attrib(EAttribID::EAT_Icon, (const char*)Name)
//show properties of an Object* or TObjectPtr<> instead of being selectable
#define UATTR_ShowProperties() Attrib(EAttribID::EAT_ShowProperties)
#define UATTR_MaterialConstant(constantName) Attrib(EAttribID::EAT_MaterialConstant, (const char*)constantName)
//editor cant add or remove element to the array
#define UATTR_ArrayConstantLength() Attrib(EAttribID::EAT_ArrayConstantLength)


//new attributes here ^^^^^^^^^^^^^


namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class Module;


	enum EAttribID
	{
		EAT_None,
		EAT_Name,
		EAT_Comment,
		EAT_Category,
		EAT_Range,
		EAT_Abstract,
		EAT_Uneditable,
		EAT_Hidden,
		EAT_Volatile,
		EAT_Instanceable,
		EAT_Icon,
		EAT_ShowProperties,
		EAT_MaterialConstant,
		EAT_ArrayConstantLength,
		////new attributes here ^^
	};
	//////////////////////////////////////////////////////////////////////////
	class UAPI Attrib
	{
		EAttribID		mID;
		String			mString;
		float			mValue0;
		float			mValue1;
	public:
		Attrib(EAttribID type = EAttribID::EAT_None) 
			: mID(type)
		{
			mValue0 = mValue1 = 0;
		}
		Attrib(EAttribID type, float f0, float f1 = 0)
			: mID(type)
		{
			mValue0 = f0;
			mValue1 = f1;
		}
		Attrib(EAttribID type, const char* str)
			: mID(type)
		{
			mString = str;
		}
		float GetValue0() const { return mValue0; }
		float GetValue1() const { return mValue1; }
		const String& GetString() const { return mString; }
		EAttribID GetID() const { return mID; }


	};




	//////////////////////////////////////////////////////////////////////////
	struct UAPI AttribPack
	{
		static const unsigned MAX_ATTRIB = 8;

		Attrib	mAttributes[MAX_ATTRIB];
		size_t	mNumAttributes;

	public:
		AttribPack();
		AttribPack(Attrib a0);
		AttribPack(Attrib a0, Attrib a1);
		AttribPack(Attrib a0, Attrib a1, Attrib a2);
		AttribPack(Attrib a0, Attrib a1, Attrib a2, Attrib a3);
		AttribPack(Attrib a0, Attrib a1, Attrib a2, Attrib a3, Attrib a4);
		AttribPack(Attrib a0, Attrib a1, Attrib a2, Attrib a3, Attrib a4, Attrib a5);
		AttribPack(Attrib a0, Attrib a1, Attrib a2, Attrib a3, Attrib a4, Attrib a5, Attrib a6);
		AttribPack(Attrib a0, Attrib a1, Attrib a2, Attrib a3, Attrib a4, Attrib a5, Attrib a6, Attrib a7);

		bool HasAttrib(EAttribID id) const;
		bool GetAttrib(EAttribID id, Attrib& out) const;
		unsigned NumAttributes() const;
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI TypeInfo : public Object
	{
		UCLASS(TypeInfo, Object)

		friend class MetaSys;

	private:
		Name					mTypeName;
		unsigned				mTypeSize;
		unsigned				mTypeAlign;
		String					mHeaderFileName;
		String					mSourceFileName;
		unsigned				mDefinitionLineNumber;
		Module*					mOwner;	// null if is common type
		AttribPack				mAttributes;
		bool					mIsClass;
		bool					mIsEnum;
		bool					mIsArithmetic;

	public:
		Name GetName() const { return mTypeName; }
		unsigned GetSize() const { return mTypeSize; }
		unsigned GetAlign() const { return mTypeAlign; }
		const String& GetHeaderFileName() { return mHeaderFileName; }
		const String& GetSourceFileName() { return mSourceFileName; }
		unsigned GetDefinitionLineNumber() { return mDefinitionLineNumber; }
		Module* GetModule() const { return mOwner; }
		const AttribPack& GetAttributes() const { return mAttributes; }
		bool HasAttrib(EAttribID id) const { return mAttributes.HasAttrib(id); }
		bool GetAttrib(EAttribID id, Attrib& out) const { return mAttributes.GetAttrib(id, out); }
		bool IsClass() const { return mIsClass; }
		bool IsEnum() const { return mIsEnum; }
		bool IsArithmetic() const { return mIsArithmetic; }
	};


};