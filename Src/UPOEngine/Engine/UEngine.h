#pragma once

#include "UAsset.h"
#include "UAssetSys.h"
#include "UTestAsset.h"

// #include "UInput.h"
#include "UComponent.h"
// #include "UMaterial.h"
// #include "USMesh.h"
// #include "UBehavior.h"
// #include "UAssetSys.h"
// #include "UTexture2D.h"
#include "UWorld.h"
#include "UGameWindow.h"

namespace UPO
{
	class UAPI Engine
	{
		World* mCurrentWorld;
		GameWindow* mMainWindow;

		volatile bool bExit = false;

	public:
		Engine();
		~Engine();

		void Run();

		void Init();
		void Release();

		bool GTTick();
		bool RHTick();

		void LoadWorld(Name name);

		static Engine* Get();
	};
}