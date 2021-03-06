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
#include "Engine/UEntityTest.h"
#include <DirectXMath.h>

namespace UPO
{
	
	class alignas(16) TestObject : public Object 
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
		GameWindow* mGameWnd2 = nullptr;
		World*		mStartupWorld = nullptr;

		virtual bool OnInit() override
		{
			AssetSys::Get()->CollectAssetEntries();

			GameWindowCreationParam gwcp = InitConfig();
			gwcp.mVSyncEnable = true;


			mGameWnd = GameWindow::CreateLauncherWin(gwcp);

			WorldInitParam wip;
			wip.mStartPlaying = true;
			wip.mWorldType = EWorldType::EEditor;

			mStartupWorld = GEngine()->CreateWorld(wip);
			mStartupWorld->CreateEntity<EntityTest>(nullptr);
			mGameWnd->SetWorld(mStartupWorld);

// 
// 			mStartupWorld = GEngine()->CreateWorld();
// 			mStartupWorld->CreateEntity<EntityTest>(nullptr);
// 			mStartupWorld->SetPlaying();
// 			mGameWnd2->SetWorld(mStartupWorld);

			

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
// 			GEngine()->DeleteWorld(mStartupWorld);
// 			GameWindow::Instances.ForEach([](GameWindow* gw) { GameWindow::Destroy(gw); });
// 			mStartupWorld = nullptr;
// 			mGameWnd = nullptr;
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

	



	void Test(SVec2 f)
	{

	}
	bool AngleEquality(Vec3 a, Vec3& b)
	{
		Matrix4 m0, m1;
		m0.MakeRotationXYZ(a);
		m1.MakeRotationXYZ(b);
		return m0.GetRow(2) | m1.GetRow(2);

	}
	float PointPlaneDist(const Vec3& planeNormal, const Vec3& planePoint, const Vec3& point)
	{
		return Dot(planeNormal, point - planePoint);
	}

	Vec3 Floor(const Vec3& v)
	{
		return Vec3(::floor(v.mX), ::floor(v.mY), ::floor(v.mZ));
	}
	Vec4 TOUPO(DirectX::XMFLOAT4 v)
	{
		return Vec4(v.x, v.y, v.z, v.w);
	}
	float TruncateDecimal(const float value, const float decimal)
	{
		return ((int)(value * decimal)) / decimal;
	}
	Vec3 TruncateDecimal(const Vec3& v, const float decimal)
	{
		return Vec3(
			((int)(v.mX * decimal)) / decimal,
			((int)(v.mY * decimal)) / decimal,
			((int)(v.mZ * decimal)) / decimal
			);
	}

	void DebugTest()
	{

	}


	//////////////////////////////////////////////////////////////////////////
	UAPI void TestMain(int argc, const char** argv)
	{
		try
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
		catch (const std::exception&)
		{
			ULOG_ERROR("");
		}



	}
	
};