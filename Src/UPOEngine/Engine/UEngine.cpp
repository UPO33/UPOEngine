#include "UEngine.h"

#include "../GFXCore/UGFXCore.h"
#include "../GFX/UGFX.h"
#include "../Core/UCommandQueue.h"
#include "../Engine/UWorldRS.h"
#include "../Engine/UEntityStaticMesh.h"
#include "../Engine/UEntityTest.h"
#include "../GFXCore/UGlobalResources.h"
#include "UEngineBase.h"
#include "../Misc/UMisc.h"
#include "../GFXCore/UGFXDeviceDX.h"
#include "../GFX/UPrimitiveBatch.h"

namespace UPO
{
	unsigned	gGameTickCounter = 0;
	unsigned	gRenderTickCounter = 0;


	TRenderCommandQueue gGame2RenderQueue;

	extern CriticalSection gSwapChainLock;

	struct EngineImpl
	{

		World* mCurrentWorld;
		WorldRS*	mCurrentWorldRS;
// 		GameWindow* mMainWindow;
// 		GFXContext* mGFXContext;
		Renderer* mRenderer;
		IEngineInterface* mInterface;
		bool mLoop;

		volatile bool mLoopGT = true;

		//when render thread was released this event is signaled
		Event	mFetchCompleteEvent;
		Event	mFetchReadyEvent;

		TArray<World*>		mWorlds;
		bool mAnyWorldRemoved = false;
		
		EngineImpl() :
			mFetchCompleteEvent(false, false),
			mFetchReadyEvent(false, false)
		{

		}
		~EngineImpl()
		{
		}
		void WaitForFetch()
		{
			mFetchReadyEvent.Wait();
		}
		//////////////////////////////////////////////////////////////////////////
		void Init()
		{
			gGameThreadID = Thread::ID();

			ULOG_MESSAGE("Initilizing Engine...");

			gSeparateRenderThread = GEngineConfig()->AsBool("Engine.SeparateRenderThread", true);

			if(gSeparateRenderThread)	//create a thread for rendering
			{
				Thread::CreateLambda([]() {

					gRenderThreadID = Thread::ID();

					ULOG_MESSAGE("Render Thread Created");

					gGame2RenderQueue.RunTillQuit();
				});
			}
			else
			{
				gRenderThreadID = Thread::ID();
				gGame2RenderQueue.SetConsumerThread(Thread::ID());
			}

			EnqueueRenderCommandAndWait([this](){
				
				GFXDevice::Create();
				GlobalShaders::CompileAll();
				GlobalResources::LoadGFXResources();

				mRenderer = new Renderer;
			});


			mInterface->OnInit();
		}
		
		//////////////////////////////////////////////////////////////////////////////////////////
		void Loop()
		{
			double lastTime = 0;
			gDeltaSeconds = 1 / 60.0f;

			static unsigned FPSCounter = 0;
			static double FPSSeconds = 0;

			ChronometerAccurate timer;
			timer.Start();
			

			while (mLoopGT)
			{
				bool result;

				double curTime = timer.SinceSeconds();
				gDeltaSeconds = (float)(curTime - lastTime);
				lastTime = curTime;

				result = GTick();
				if (!result) break;

				FPSCounter++;
				FPSSeconds += gDeltaSeconds;
				if (FPSSeconds >= 1)
				{
					FPSSeconds -= 1;
					gFPS = FPSCounter;
					FPSCounter = 0;
				}
			}
		}
#if 0
		void InitRT()
		{

		}
		void LoopRT()
		{
			UASSERT(IsRenderThread());


			double lastTime = 0;
			float dealt = 60.0f / 16;

			ChronometerAccurate timer;
			timer.Start();
			 
			while (mLoopRT)
			{
				bool result;

				result = RTick();
				if (!result) break;

				Thread::Sleep(10);
				
				double curTime = timer.SinceSeconds();
				dealt = (float)(curTime - lastTime);
				lastTime = curTime;
			}

			ReleaseRT();
			mRenderThreadReleased.SetSignaled();
		}
#endif // 0
		void Release()
		{
			GameWindow::DestroyAll();

			for (World* world : mWorlds)
			{
				delete world;
			}
			mWorlds.RemoveAll();
			ULOG_MESSAGE("");
			EnqueueRenderCommandAndWait([this]() {
				ReleaseRT();
			});

			mInterface->OnRelease();
		}
		void ReleaseRT()
		{
			UASSERT(IsRenderThread());
			ULOG_MESSAGE("");
			SafeDelete(mRenderer);


			GlobalShaders::ReleaseAll();
			GlobalResources::UnloadGFXResources();
			delete gGFX;
			gGFX = nullptr;
		}
		//////////////////////////////////////////////////////////////////////////
		void Render()
		{
			if (!gEngine.mRenderer) return;

			for (GameWindow* gw : GameWindow::Instances)
			{
				 gEngine.mRenderer->RenderGameWin(gw);
			}
			for (GameWindow* gw : GameWindow::Instances)
			{
				if (gw  && gw->mSwapchain)
				{
					gw->mSwapchain->Present();
				}
			}
			
			mFetchReadyEvent.SetSignaled();

		}
		//////////////////////////////////////////////////////////////////////////game thread tick
		bool GTick()
		{
			bool result = true;

			if (!gIsEditor)
			{
				result &= GameWindow::PeekMessages();
			}

			EnqueueRenderCommand([this]() {
				Render();
				
			});

			GAssetSys()->Tick(gDeltaSeconds);



			//ticking alive worlds
			{
				result &= mInterface->OnBeforeWorldsTick();

				unsigned numWorld = mWorlds.Length();
				//ticking world
				for (unsigned i = 0; i < numWorld; i++)
				{
					if (mWorlds[i]->mIsAlive)
					{
						gCurTickingWorld = mWorlds[i];
						mWorlds[i]->Tick(gDeltaSeconds);
					}
				}
				result &= mInterface->OnAfterWorldsTick();
				
				gCurTickingWorld = nullptr;

				//removing destroyed worlds if any
				if (mAnyWorldRemoved)
				{
					mAnyWorldRemoved = false;
					mWorlds.RemoveIf([](World* world) {
						if (!world->mIsAlive)
						{
							delete world;
							return true;
						}
						return false;
					});
				}
				
				for (GameWindow* gw : GameWindow::Instances)
				{
					if (auto canvas = gw->mCanvas) canvas->Swap();
				}
				for (World* world : mWorlds)
				{
					if (auto pb = world->GetPrimitiveBatch()) pb->Swap();
				}

				//after this we can change the WorldRS and update it, maybe renderer is doing postProcess or swamping buffer
				WaitForFetch();



				/////////////fetching changes
				EnqueueRenderCommand([this]()
				{
					unsigned nWorld = mWorlds.Length();

					for (unsigned i = 0; i < nWorld; i++)
					{
						mWorlds[i]->GetRS()->Fetch();
					}
					mFetchCompleteEvent.SetSignaled();

					for (unsigned i = 0; i < nWorld; i++)
					{
						mWorlds[i]->GetRS()->AfterFetch();
					}
				});



				DuringFetch();

				mFetchCompleteEvent.Wait();

			}



			gGameTickCounter++;

			return result;
		}
		//non-gfx dependent codes only
		void DuringFetch()
		{

		}


	} gEngine;




	UAPI void LaunchEngine(IEngineInterface* itf)
	{
		gEngine.mInterface = itf;

		gEngine.Init();
		gEngine.Loop();
		gEngine.Release();
	}


	void IEngineInterface::Quit()
	{
		gEngine.mLoopGT = false;
	}



	World* IEngineInterface::CreateWorld(const WorldInitParam& param)
	{
		World* world = new World(param);
		EnqueueRenderCommandAndWait([world]() {
			gEngine.mWorlds.Add(world);
		});
		
		return world;
	}

	void IEngineInterface::DeleteWorld(World* world)
	{
		if(world->mIsAlive)
		{
			world->mIsAlive = false;
			gEngine.mAnyWorldRemoved = true;
		}
	}





	IEngineInterface* IEngineInterface::Get()
	{
		return gEngine.mInterface;
	}

};