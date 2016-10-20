#include "Test.h"

#include "GFXCore/UGFXLauncherWin.h"

#include <SDL.h>
#include <vector>
#include <string.h>

#define ULOGVAR_VEC3(v3) ULOG_WARN("%s %f  %f  %f", #v3, v3.mX, v3.mY, v3.mZ)
#define ULOGVAR_STR(str) ULOG_WARN("%s %s", #str, str.CStr() ? str.CStr() : "")

namespace UPO
{
	
	class TestObject : public Object 
	{
		UCLASS(TestObject, Object)

	public:
		Vec3  v0;
		Vec3  v1;
		Vec3  v2;
		String mStr;
		EPropertyType mEnum0;
		TestMetaClass mMetaClass;
		

		TestObject()
		{
			ULOG_WARN("");
		}
		~TestObject()
		{
			ULOG_WARN("");
		}
// 		void MetaSerialize(Stream&)
// 		{
// 			ULOG_MESSAGE("");
// 		}
		void SetValue()
		{
			v0 = v1 = v2 = Vec3(4.124f);
			mStr = "the string";
			mMetaClass.SetValues();
			
		}
		void Print()
		{
			ULOG_WARN("---------------------------");
			ULOGVAR_VEC3(v0);
			ULOGVAR_VEC3(v1);
			ULOGVAR_VEC3(v2);
			ULOGVAR_STR(mStr);
			
			mMetaClass.Print();
		}
	};

	UCLASS_BEGIN_IMPL(TestObject)
	UPROPERTY(v0)
		UPROPERTY(v1)
		UPROPERTY(v2)
		UPROPERTY(mStr)
		UPROPERTY(mEnum0)
		UPROPERTY(mMetaClass)
	UCLASS_END_IMPL(TestObject)

	struct AsElement
	{
		THTreeNode<AsElement> mTree;
	};

	uint64 GetNewAssetID()
	{
		FILETIME filetime;
		GetSystemTimeAsFileTime(&filetime);
		LARGE_INTEGER ret;
		ret.LowPart = filetime.dwLowDateTime;
		ret.HighPart = filetime.dwHighDateTime;
		return (uint64)ret.QuadPart;
	}
	std::vector<std::string> get_all_files_names_within_folder(std::string folder)
	{
		using namespace std;
		vector<string> names;
		string search_path = folder + "*";
		WIN32_FIND_DATAA fd;
		HANDLE hFind = ::FindFirstFileA(search_path.c_str(), &fd);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				// read all (real) files in current folder
				// , delete '!' read other 2 default folder . and ..
				ULOG_WARN("file %s", fd.cFileName);
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					names.push_back(fd.cFileName);
					
				}
			} while (::FindNextFileA(hFind, &fd));
			::FindClose(hFind);
		}
		return names;
	}

	
	void DirGetFiles(const String& dir, TArray<String>& outFiles, bool subDirectories)
	{
		DirGetFiles(dir.CStr(), outFiles, nullptr);
	}

	void StrSplit(String& s, char split, TArray<String>& out)
	{
		out.RemoveAll();

		auto len = s.Length();
		if (len == 0) return;

		const char* str = s.CStr();
		size_t last = 0;

		for (size_t i = 0; i < len; i++)
		{
			if (str[i] == split)
			{
					if (last == i)
					{
					//	out.AddDefault();
					}
					else
					{
						out.AddDefault();
						out.LastElement() = String(str + last, i - last);
					}

				last = i + 1;
			}
		}

		if (last == 0) // nothing found
		{
			//out.Add(s);
		}
		else if (last == len) //token is last char
		{

		}
		else
		{
			out.AddDefault();
			out.LastElement() = String(str + last, len - last);
		}


		for (size_t i = 0; i < out.Length(); i++)
			ULOG_ERROR("[%s]", out[i].IsEmpty() ? "empty" : out[i].CStr());
	}
	class STestRef : public RefCountable
	{
	public:
	};
	//////////////////////////////////////////////////////////////////////////
	UAPI void TestMain(int argc, const char** argv)
	{
		Engine::Get()->Run();

		system("pause");
		return;
	}

};