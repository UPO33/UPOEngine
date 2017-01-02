#include "UConfig.h"
#include "UFileSys.h"


namespace UPO
{
	struct INIReader
	{
		const char* mContent = nullptr;
		size_t mContentSize = 0;
		int mPos = 0;
		Config* mOut = nullptr;
		String mCurSection;

		INIReader(const Buffer& iniContent, Config* out)
		{
			mContent = (const char*)iniContent.Data();
			mContentSize = iniContent.Size();
			mOut = out;
		}
		bool ReadLine(String& outLine)
		{
			if (mPos == mContentSize) return false;	//we r at the end

			unsigned begin = mPos;

			while (mPos != mContentSize)
			{
				if (mContent[mPos] == '\n')
				{

					if (mPos != begin)
					{
						mPos++;
						break;
					}

				}

				mPos++;
			}

			

			outLine = String(mContent + begin, mPos - begin);
			return true;
		}
		bool Parse()
		{
			String line;
			while (ReadLine(line))
			{
				line.Trim();

				if (line.IsEmpty()) continue;

				char firstChar = line[0U];

				switch (firstChar)
				{
				case '[':
					ParseSection(line);
					break;
				case ';':
				case '#':
				case '\n':
				case '\t':
				case ' ':
					break;
				default:
					ParseKey(line);
				}
			}
			return true;
		}
		//[SectionName]
		void ParseSection(const String& line)
		{
			auto endSecIndex = line.FindN(']');
			if (endSecIndex != ~0)
			{
				mCurSection = line.SubStr(1, endSecIndex - 1);
				mCurSection.Trim();
			}
			else
			{
				mCurSection.SetEmpty();
			}
		}
		//Keyname=value
		void ParseKey(const String& line)
		{
			auto assignIndex = line.FindN('=');
			if (assignIndex != ~0)
			{
				String keyName = line.SubStr(0, assignIndex);
				keyName.Trim();
				String keyValueStr = line.SubStr(assignIndex + 1);
				keyValueStr.Trim();


				ConfigValue configValue;
				if (ParseValue(keyValueStr, configValue))
				{
					char keyFullName[1024];
					if(mCurSection.IsEmpty())
						sprintf_s(keyFullName, "%s", keyName.CStr());
					else
						sprintf_s(keyFullName, "%s.%s", mCurSection.CStr(), keyName.CStr());

					mOut->AddKey(Name(keyFullName), configValue);
				}
			}
		}
		bool ParseValue(String& value, ConfigValue& out)
		{
			if (value.IsEmpty()) return false;
			
			static String StrTrue = "true";
			static String StrFalse = "false";

			if (value.Equal(StrTrue)) //bool true ?
			{
				out.mType = EConfigValue::ECV_Bool;
				out.mValueBool = true;
				return true;
			}
			if (value.Equal(StrFalse)) //bool false ?
			{
				out.mType = EConfigValue::ECV_Bool;
				out.mValueBool = false;
				return true;
			}

			if (value[0U] == '"' && value[value.Length() - 1U] == '"') //string ?
			{
				if (value.Length() == 1) return false;

				out.mType = EConfigValue::ECV_String;
				out.mValueString = value.SubStr(1, value.Length() - 2);
				return true;
			}
			
			if (value[0U] == '(')
			{
				ULOG_FATAL("not implimented");
				return false;
			}
			else // number ?
			{
				out.mType = EConfigValue::ECV_Number;
				value.ToFloat(out.mValueNumber);
				return true;
			}
			return false;
		}
	};



	ConfigValue* Config::Find(Name name) const
	{
		UASSERT(mKeysValue.Length() == mKeysName.Length());

		for (size_t i = 0; i < mKeysName.Length(); i++)
		{
			if (mKeysName[i] == name) return mKeysValue.ElementAt(i);
		}
		return nullptr;
	}

	bool Config::AddKey(Name name, ConfigValue value)
	{
		if (ConfigValue* found = Find(name))
		{
			*found = value;
			return true;
		}

		mKeysName.Add(name);
		mKeysValue.Add(value);

		return true;
	}

	void Config::SetDefault(Name name, const char* value)
	{
		ConfigValue cv;
		cv.mType = EConfigValue::ECV_String;
		cv.mValueString = value;
		AddKey(name, cv);
	}

	void Config::SetDefault(Name name, float value)
	{
		ConfigValue cv;
		cv.mType = EConfigValue::ECV_Number;
		cv.mValueNumber = value;
		AddKey(name, cv);
	}

	void Config::SetDefault(Name name, bool value)
	{
		ConfigValue cv;
		cv.mType = EConfigValue::ECV_Bool;
		cv.mValueBool = value;
		AddKey(name, cv);
	}

	float Config::AsNumber(Name name, float defaultValue /*= 0*/) const
	{
		ConfigValue* value = Find(name);

		if (value && value->mType == EConfigValue::ECV_Number)
			return value->mValueNumber;

		return defaultValue;
	}

	Name Config::AsName(Name name, Name defaultValue /*= Name::Empty*/) const
	{
		ConfigValue* value = Find(name);

		if (value && value->mType == EConfigValue::ECV_String)
			return value->mValueString;

		return defaultValue;
	}

	bool Config::AsBool(Name name, bool defaultValue /*= false*/) const
	{
		ConfigValue* value = Find(name);

		if (value && value->mType == EConfigValue::ECV_Bool)
			return value->mValueBool;

		return defaultValue;
	}

	bool Config::InitFromINI(const char* iniFilename)
	{
		Buffer content;
		if (File::OpenReadFull(iniFilename, content))
		{
			return InitFromINI(content);
		}
		else
		{
			ULOG_FATAL("Failed to load config file [%]", iniFilename ? iniFilename : "");
			return false;
		}
	}
	inline bool IsINIComment(char chr) { return chr == ';' || chr == '#'; }

	bool Config::InitFromINI(const Buffer& iniContent)
	{
		INIReader reader = INIReader(iniContent, this);
		return reader.Parse();
	}

	void Config::PrintDbg()
	{
		for (size_t i = 0; i < mKeysName.Length(); i++)
		{
			ULOG_WARN("Key [%]", mKeysName[i]);
			if (mKeysValue[i].mType == EConfigValue::ECV_Number)
				ULOG_WARN("Vlaue NUmber [%]", mKeysValue[i].mValueNumber);
			if (mKeysValue[i].mType == EConfigValue::ECV_String)
				ULOG_WARN("Vlaue String [%]", mKeysValue[i].mValueString);
		}
	}

};