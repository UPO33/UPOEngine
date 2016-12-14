#include "Test.h"

#include "GFXCore/UGFXLauncherWin.h"

#include <SDL.h>
#include <vector>
#include <string.h>

#include "Misc/UMisc.h"

#define ULOGVAR_VEC3(v3) ULOG_WARN("%s %f  %f  %f", #v3, v3.mX, v3.mY, v3.mZ)
#define ULOGVAR_STR(str) ULOG_WARN("%s %s", #str, str.CStr() ? str.CStr() : "")

#include "Engine/UWorldTimer.h"
#include "Core/USparseArray.h"
#include "Core/UMatrix.h"

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

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	class EngineLauncher : public IEngineInterface
	{
		GameWindow* mGameWnd = nullptr;


		virtual bool OnInit() override
		{
			AssetSys::Get()->CollectAssetEntries();

			return true;
		}

		virtual bool OnTick() override
		{
			return mGameWnd->Tick();
		}

		virtual bool OnRelease() override
		{
			return true;
		}

		virtual GameWindow* OnCreateGameWindow() override
		{
			mGameWnd = GameWindow::New();
			mGameWnd->Init();
			return mGameWnd;
		}

		virtual void OnReleaseGameWindow() override
		{
			mGameWnd->Release();
			mGameWnd = nullptr;
			delete mGameWnd;
		}

		virtual void OnAfterDeviceCreation() override
		{
// 			UGlobalShader_CompileAll();
		}

		virtual void OnBeforeRendererRelease() override
		{
// 			UGlobalShader_ReleaseAll();
		}

	};

	//////////////////////////////////////////////////////////////////////////
	/*
	command lines
	projdir="project directory"
	*/
	void ParseCommandLine(int argc, const char** argv)
	{
		{
			String strExePath = argv[0];
			GApp()->mEnginePath = strExePath.SubStr(0, strExePath.FindRN(PATH_SEPARATOR_CHAR, 1));
			GApp()->mEngineAssetsPath = GApp()->mEnginePath + PATH_SEPARATOR_CHAR + "Assets";

			ULOG_MESSAGE("engine path %s", gEnginePath.CStr());
		}

		String argValue;
		String ProjDir = "projdir=";

		for (size_t i = 1; i < argc; i++)
		{
			argValue = argv[i];
			if (argValue.BeginsWith(ProjDir))
			{
				gProjectPath = argValue.SubStr(ProjDir.Length(), argValue.Length() - ProjDir.Length());
				ULOG_MESSAGE("Project directory %s", gProjectPath.CStr());
			}
		}

	}

#define VECPRINT(v) ULOG_WARN(#v "  %f  %f  %f", v.mX, v.mY, v.mZ)

	void DebugTest()
	{
		//Matrix
		//A * B * C 
		//applying order first C then B then A
		Matrix4 mt;
		mt.MakeTranslation(Vec3(10, 0, 0));
		Matrix4 mr;
		mr.MakeRotationY(180);
		Matrix4 mr2;
		mr2.MakeRotationY(180);
		Matrix4 mat2 = mr*mt; //mt * mr * mr2;
		mat2.MakeTranslationRotationScale(Vec3(10, 0, 0), Vec3(0), Vec3(2));
		Vec3 vt = mat2.TransformVec3W1(Vec3(0));
		VECPRINT(vt);


		int tmp = 0;
		std::cin >> tmp;
	}


	//////////////////////////////////////////////////////////////////////////
	UAPI void TestMain(int argc, const char** argv)
	{

// 		DebugTest();

// 		BitArray ba;
// 		ba.Add(true);
// 		ba.Add(false);
// 		ba.Add(false);
// 		if (ba[0])
// 		{
// 			ba[1] = true;
// 		}
// 		ba.RemoveAtSwap(0);
// 		ba.ForEach([](bool b)
// 		{
// 			ULOG_MESSAGE("%i", b ? 1 : 0);
// 		});
		//////

		ParseCommandLine(argc, argv);

		gIsEditor = false;
		
// 		auto m1 = ModuleSys::Get()->LoadModule("testmodule1.dll");
// 		auto m2 = ModuleSys::Get()->LoadModule("testmodule2.dll");
// 
// 		ModuleSys::Get()->UnloadModule(m1);
// 		ModuleSys::Get()->UnloadModule(m2);

		LaunchEngine(new EngineLauncher);
	}
	
};