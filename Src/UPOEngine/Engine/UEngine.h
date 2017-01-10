#pragma once

#include "UAsset.h"
#include "UAssetSys.h"
#include "UTestAsset.h"

//#include "UInput.h"
#include "UComponent.h"
#include "UStaticMesh.h"
#include "UTexture2D.h"
#include "UWorld.h"
#include "UGameWindow.h"


namespace UPO
{


	//////////////////////////////////////////////////////////////////////////
	class UAPI IEngineInterface
	{
	public:
		virtual bool OnInit() { return true; }
		virtual bool OnBeforeWorldsTick() { return true; }
		virtual bool OnAfterWorldsTick() { return true; }
		virtual bool OnRelease() { return true; }

		virtual GameWindow* OnCreateGameWindow() { return nullptr; }
		virtual void OnReleaseGameWindow() {}

		virtual void OnAfterDeviceCreation() {};
		virtual void OnBeforeRendererRelease() {};

		void Quit();
		void LoadWorld(Name assetName);
		void SetWorld(World*);
		void LoadWorldAsync(Name assetName);

		World* CreateWorld();
		void DeleteWorld(World*);
		
		static IEngineInterface* Get();
	};

	inline IEngineInterface* GEngine() { return IEngineInterface::Get(); }
	
	UAPI void LaunchEngine(IEngineInterface* itf);
}