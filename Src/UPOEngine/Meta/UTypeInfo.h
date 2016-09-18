#pragma once

#include <type_traits>

#include "../Core/UName.h"
#include "../Core/UString.h"
#include "../Core/UArray.h"
#include "../Object/UObject.h"

#define UATTR_Comment(Comment) Attrib(EAtrribID::EAT_Comment, (const char*)Comment)
#define UATTR_Range(Min, Max) Attrib(EAtrribID::EAT_Range, (float)Min, (float)Max)
#define UATTR_Abstract() Attrib(EAtrribID::EAT_Abstract)
#define UATTR_Uneditable() Attrib(EAtrribID::EAT_Uneditable)
#define UATTR_Hidden() Attrib(EAtrribID::EAT_Hidden)
#define UATTR_Volatile() Attrib(EAtrribID::EAT_Volatile)

//new attributes here ^^^^^^^^^^^^^


namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class Module;


	enum EAtrribID
	{
		EAT_None,
		EAT_Comment,
		EAT_Range,
		EAT_Abstract,
		EAT_Uneditable,
		EAT_Hidden,
		EAT_Volatile,

		////new attributes here ^^
	};
	//////////////////////////////////////////////////////////////////////////
	class UAPI Attrib
	{
		EAtrribID		mID;
		String			mString;
		float			mValue0;
		float			mValue1;
	public:
		Attrib(EAtrribID type = EAtrribID::EAT_None) 
			: mID(type)
		{
			mValue0 = mValue1 = 0;
		}
		Attrib(EAtrribID type, float f0, float f1 = 0)
			: mID(type)
		{
			mValue0 = f0;
			mValue1 = f1;
		}
		Attrib(EAtrribID type, const char* str)
			: mID(type)
		{
			mString = str;
		}
		float GetValue0() const { return mValue0; }
		float GetValue1() const { return mValue1; }
		const String& GetString() const { return mString; }
		EAtrribID GetID() const { return mID; }


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

		bool HasAttrib(EAtrribID id) const;
		bool GetAttrib(EAtrribID id, Attrib& out) const;
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
		bool IsClass() const { return mIsClass; }
		bool IsEnum() const { return mIsEnum; }
		bool IsArithmetic() const { return mIsArithmetic; }
	};


};