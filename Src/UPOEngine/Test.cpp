#include "Test.h"

#include "GFXCore/UGFXLauncherWin.h"

#include <SDL.h>
#include <vector>
#include <string.h>

#include "Misc/UMisc.h"


#include "Engine/UWorldTimer.h"
#include "Core/USparseArray.h"
#include "Core/UMatrix.h"
#include "core/UDelegate.h"
#include "core/UPlane.h"

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
		World*		mStartupWorld = nullptr;

		virtual bool OnInit() override
		{
			AssetSys::Get()->CollectAssetEntries();

			GameWindowCreationParam gwcp = InitConfig();
			mGameWnd = GEngine()->CreateGameWindow(gwcp);

			UASSERT(mGameWnd);


			mStartupWorld = GEngine()->CreateWorld();

			mGameWnd->SetWorld(mStartupWorld);

			return true;
		}

		virtual bool OnBeforeWorldsTick() override
		{
			return true;
		}
		virtual bool OnAfterWorldsTick() override
		{
			return true;
		}

		virtual bool OnRelease() override
		{
			GEngine()->DeleteWorld(mStartupWorld);
			GameWindow::Instances.ForEach([](GameWindow* gw) { GameWindow::Destroy(gw); });
			mStartupWorld = nullptr;
			mGameWnd = nullptr;
			return true;
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
		}

		String argValue;
		String ProjDir = "projdir=";

		for (size_t i = 1; i < argc; i++)
		{
			argValue = argv[i];
			if (argValue.BeginsWith(ProjDir))
			{
				GApp()->mProjectPath = argValue.SubStr(ProjDir.Length(), argValue.Length() - ProjDir.Length());
				GApp()->mProjectAssetsPath = GApp()->mProjectPath + PATH_SEPARATOR_CHAR + "Assets";
			}
		}

	}

	struct SVec2
	{
		float x, y;

		SVec2() {}
		SVec2(float, float) {}
		SVec2(Vec2) {}
		explicit SVec2(float) { x = y = 0; }
		

	};

	


	float AngleEq(Vec3 a, Vec3 b)
	{
		Matrix4 m0, m1;
		m0.MakeRotationXYZ(a);
		m1.MakeRotationXYZ(b);
		return (Dot(m0.GetColumn(2) , m1.GetColumn(2)));
	}
	void Test(SVec2 f)
	{

	}
	float PointPlaneDist(const Vec3& planeNormal, const Vec3& planePoint, const Vec3& point)
	{
		return Dot(planeNormal, point - planePoint);
	}

	void DebugTest()
	{
// 		Matrix4 rotation;
// 		for (size_t i = 0; i < 10; i++)
// 		{
// 			Vec3 ang = Vec3(i * 10, 90 , 0);
// 			rotation.MakeRotationXYZ(ang);
// 			Vec3 v0 = rotation.GetRotationEuler1();
// 		}

		UPO::Plane p0(Vec3(0, 0, 0), Vec3(0,1,0));
		Vec3 pos = Vec3(123, -23, 55);
		Vec3 p1 = p0.ProjectPoint(pos);
		
// 		String s = "sdfsd";
// 		ULOG_MESSAGE("%", s);
// 		ULOG_WARN("% % %", (p1 - pos).Length(), p0.DotCoord(pos), PointPlaneDist(Vec3(0, 1, 0), Vec3(0, 0, 0), pos) );
// 		int tmp = 0;
// 		std::cin >> tmp;
	}


	//////////////////////////////////////////////////////////////////////////
	UAPI void TestMain(int argc, const char** argv)
	{

		DebugTest();


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