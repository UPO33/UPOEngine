#pragma once

#include "UAsset.h"
#include "UAssetSys.h"
#include "UTestAsset.h"

//#include "UInput.h"
#include "UComponent.h"
#include "USMesh.h"
#include "UTexture2D.h"
#include "UWorld.h"
#include "UGameWindow.h"


namespace UPO
{


	//////////////////////////////////////////////////////////////////////////
	class IEngineInterface
	{
	public:
		virtual bool OnInit() { return true; }
		virtual bool OnTick() { return true; }
		virtual bool OnRelease() { return true; }

		virtual GameWindow* OnCreateGameWindow() { return nullptr; }
		virtual void OnReleaseGameWindow() {}

		virtual void OnAfterDeviceCreation() {};
		virtual void OnBeforeRendererRelease() {};
	};

	UAPI void LaunchEngine(IEngineInterface* itf);
}