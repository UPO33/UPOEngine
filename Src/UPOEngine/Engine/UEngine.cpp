#include "UEngine.h"

#include "../GFXCore/UGFXCore.h"
#include "../GFX/UGFX.h"

namespace UPO
{
	unsigned	gGameTickCounter = 0;
	unsigned	gRenderTickCounter = 0;
	
	struct 
	{
		World* mCurrentWorld;
		GameWindow* mMainWindow;
		GFXContext* mGFXContext;
		Renderer* mRenderer;
		IEngineInterface* mInterface;
		bool mLoop;

		void Init()
		{
			mInterface->OnInit();

			mMainWindow = mInterface->OnCreateGameWindow();
			UASSERT(mMainWindow);

			mGFXContext = GFXContext::New();
			UASSERT(mGFXContext);
			mGFXContext->Init(mMainWindow);

			mInterface->OnAfterDeviceCreation();

			mRenderer = Renderer::New();
			UASSERT(mRenderer);
			mRenderer->Init(mGFXContext);
		}
		void Loop()
		{
			bool bLoop = true;
			while (bLoop)
			{
				bool result;

				result = GTick();
				if (!result) break;

				result = RTick();
				if (!result) break;

				Thread::Sleep(10);
			}
		}
		void Release()
		{
			mInterface->OnBeforeRendererRelease();

			if (mRenderer) mRenderer->Release();
			if (mGFXContext) mGFXContext->Release();
			
			mInterface->OnReleaseGameWindow();
			mMainWindow = nullptr;

			mInterface->OnRelease();

			delete mRenderer;
			delete mGFXContext;
			delete mMainWindow;
		}
		bool GTick()
		{
			bool result = true;

			result = mInterface->OnTick();

			if (mCurrentWorld)
			{
				mCurrentWorld->SingleTick(WorldTickResult());
			}

			gGameTickCounter++;

			return result;
		}
		bool RTick()
		{
			return mRenderer->RenderFrame();
			gRenderTickCounter++;
		}

	} gEngine;


	UAPI void LaunchEngine(IEngineInterface* itf)
	{
		gEngine.mInterface = itf;


		gEngine.Init();
		gEngine.Loop();
		gEngine.Release();
	}


};