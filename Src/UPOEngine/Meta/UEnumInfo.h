#pragma once

#include "UTypeInfo.h"


struct ZZ_EnumRegParam
{
	const char* mEnumName;
	const char* mComment;
	bool mIsBitFlag;
	unsigned mNumElement;
	unsigned* mElementsValue;
	const char* mElementsName; 	//#__VA_ARGS__
	const char* mSourceFileName;
	unsigned mLineNumber;
};


namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class EnumInfo;

	//////////////////////////////////////////////////////////////////////////
	class UAPI EnumElement
	{
		friend EnumInfo;
		friend MetaSys;

		String mName;
		unsigned mValue;
		

	public:
		EnumElement() {}
		EnumElement(const String& name, unsigned value) : mName(name), mValue(value) {}

		const String& Name() const { return mName; }
		String LegibleName() const;
		unsigned Value() const { return mValue; }
	};
	
	//////////////////////////////////////////////////////////////////////////
	class UAPI EnumInfo : public TypeInfo
	{
		UCLASS(EnumInfo, TypeInfo)

		friend MetaSys;
		
		TArray<EnumElement>	mElements;
		const char* mComment;
		bool mIsBitFlag;
		
		void Make(ZZ_EnumRegParam* erp);

	public:
		size_t NumElement() const { return mElements.Length(); }
		EnumElement* GetElement(unsigned index) const;
		unsigned FindElement(const String& elementName) const;
		unsigned FindElement(unsigned value) const;
		

		const TArray<EnumElement>& Elements() const { return mElements; }
		const char* GetComment() const { return mComment; }
		bool IsBitFlag() const { return mIsBitFlag; }

		void PrintDbg();
	};
};

#define UENUM(Enum, BitFlag, Comment, ...)\
	struct UWELDINNER(ZZ_RegEnum, __COUNTER__)\
	{\
		static_assert(std::is_enum<Enum>::value, "'" #Enum "' is not a enum");\
		static_assert(sizeof(Enum) == 4, "only 32bit enum is supported");\
		struct S\
		{\
			UPO::EnumInfo mEnum;\
			S() {\
				static Enum elementsValue[] = {  __VA_ARGS__ };\
				ZZ_EnumRegParam regParam;\
				regParam.mEnumName = typeid(Enum).name() + 5; /*strlen("enum ")*/ \
				regParam.mComment = Comment;\
				regParam.mIsBitFlag = (bool)BitFlag;\
				regParam.mNumElement = sizeof(elementsValue) / sizeof(elementsValue[0]);\
				regParam.mElementsValue = (unsigned*)elementsValue;\
				regParam.mElementsName = #__VA_ARGS__;\
				regParam.mSourceFileName = __FILE__;\
				regParam.mLineNumber = __LINE__;\
				UPO::MetaSys::Get()->RegEnum(&regParam, &mEnum);\
			}\
			~S() {\
				UPO::MetaSys::Get()->UnregEnum(&mEnum);\
			}\
		} m;\
	} UWELDINNER(ZZ_EnumIns, __COUNTER__);\


	
	
	
	
	
	