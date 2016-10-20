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

#include "../GFX/URenderer.h"

namespace UPO
{
	class UAPI Engine
	{
		World* mCurrentWorld;
		GameWindow* mMainWindow;
		GFXContext* mGFXContext;
		Renderer* mRenderer;

		volatile bool bLoop = true;

	public:
		Engine();
		~Engine();

		void Run();

		void Init();
		void Release();

		bool GTTick();
		bool RTTick();

		void InitWorld();

		void LoadWorld(Name name);

		static Engine* Get();
	};
}