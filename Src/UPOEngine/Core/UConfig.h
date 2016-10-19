#pragma once

#include "UBasic.h"
#include "UName.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class Config;
	class ConfigSection;
	class ConfigValue;


	//////////////////////////////////////////////////////////////////////////
	enum EConfigValue
	{
		ECV_Invalid,

		ECV_Number,
		ECV_String,
		ECV_Bool,

		ECV_Max
	};
	//////////////////////////////////////////////////////////////////////////
	class ConfigValue
	{
	public:

		EConfigValue mType;
		unsigned mValueBuffer[4];
		
		float mValueNumber;
		Name mValueString;
		bool mValueBool;
	};
	//////////////////////////////////////////////////////////////////////////
	class ConfigSection
	{
		TArray<Name>		mKeysName;
		TArray<ConfigValue>	mKeysValue;

	public:
		ConfigValue* Find(Name keyName) const
		{
			for (size_t i = 0; i < mKeysName.Length(); i++)
				if (mKeysName[i] == keyName)
					return mKeysValue.ElementAt(i);
			return nullptr;
		}
	};


	/*
	a config is a .ini file that will be loaded at start up
	in order to get the value of a key from config file use asNumber || asName, ...
	key's name should be  SectionName.KeyName  or  KeyName  if has no section
	*/
	class UAPI Config
	{
		TArray<Name>			mKeysName;
		TArray<ConfigValue>		mKeysValue;

	public:
		ConfigValue* Find(Name name) const;
		bool AddKey(Name name, ConfigValue value);
		void SetDefault(Name name, float value);
		void SetDefault(Name name, const char* value);
		void SetDefault(Name name, bool value);

		float AsNumber(Name name, float defaultValue = 0) const;
		Name AsName(Name name, Name defaultValue = Name::Empty) const;
		bool AsBool(Name name, bool defaultValue = false) const;

		bool InitFromINI(const char* iniFilename);
		bool InitFromINI(const Buffer& iniContent);

		void PrintDbg();
	};
};