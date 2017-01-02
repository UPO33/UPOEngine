#include "UEngine.h"

#include "../GFXCore/UGFXCore.h"
#include "../GFX/UGFX.h"
#include "../Core/UCommandQueue.h"
#include "../Engine/UWorldRS.h"
#include "../Engine/UEntityStaticMesh.h"
#include "../Engine/UEntityTest.h"

#include "UEngineBase.h"
#include "../Misc/UMisc.h"

namespace UPO
{
	unsigned	gGameTickCounter = 0;
	unsigned	gRenderTickCounter = 0;
	
	TRenderCommandQueue gGame2RenderQueue;


	struct EngineImpl
	{

		World* mCurrentWorld;
		WorldRS*	mCurrentWorldRS;
// 		GameWindow* mMainWindow;
// 		GFXContext* mGFXContext;
		Renderer* mRenderer;
		IEngineInterface* mInterface;
		bool mLoop;
		AssetCommendQueue	mAssetCommands;

		volatile bool mLoopGT = true;
		volatile bool mLoopRT = true;
		volatile float mDelta = 0;	//render writes, game only reads
		volatile bool mCompilingGlobalShader = false;

		//when render thread was released this event is signaled
		Event	mFetchCompleteEvent;

		TArray<World*>		mWorlds;
		bool mAnyWorldRemoved = false;
		
		EngineImpl() :
			mFetchCompleteEvent(false, false)
		{

		}
		~EngineImpl()
		{
		}

		//////////////////////////////////////////////////////////////////////////
		void Init()
		{
			gGameThreadID = Thread::ID();

			ULOG_MESSAGE("Initilizing Engine...");

			Thread::CreateLambda([](){

				gRenderThreadID = Thread::ID();
	
				ULOG_MESSAGE("Render Thread Created");

				gGame2RenderQueue.RunTillQuit();
			});

			mInterface->OnInit();

			EnqueueRenderCommandAndWait([this](){
				
				GFXDevice::Create();
				UGlobalShader_CompileAll();
				mRenderer = Renderer::New();
				UASSERT(mRenderer);
			});
		}

		void Loop()
		{
			double lastTime = 0;
			mDelta = 1 / 60.0f;

			ChronometerAccurate timer;
			timer.Start();

			while (mLoopGT)
			{
				bool result;

				result = GTick();
				if (!result) break;

				Thread::Sleep(10);

				double curTime = timer.SinceSeconds();
				mDelta = (float)(curTime - lastTime);
				lastTime = curTime;
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
			mRenderer->Release();
			delete mRenderer;
			mRenderer = nullptr;
			delete gGFX;
		}
		void Render()
		{
			if (!gEngine.mRenderer) return;

			for (GameWindow* gw : GameWindow::Instances)
			{
				 gEngine.mRenderer->RenderGameWin(gw);
			}
		}
		//////////////////////////////////////////////////////////////////////////game thread tick
		bool GTick()
		{
			EnqueueRenderCommand([this]() {
				Render();
			});

			bool result = true;
			
			GAssetSys()->Tick(mDelta);

			if(!gIsEditor)
			{
				result = GameWindow::PeekMessages();
				if (!result) return false;
			}

			//ticking alive worlds
			{
				result = mInterface->OnBeforeWorldsTick();
				if (!result) return false;

				unsigned numWorld = mWorlds.Length();
				//ticking world
				for (unsigned i = 0; i < numWorld; i++)
				{
					if (mWorlds[i]->mIsAlive)
					{
						mWorlds[i]->Tick(mDelta);
					}
				}

				result = mInterface->OnAfterWorldsTick();
				if (!result) return false;

				///////////////wait for render command to finish
				EnqueueRenderCommandAndWait([]() {});

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

				/////////////fetching changes
				EnqueueRenderCommand([this]()
				{
					unsigned numWorld = mWorlds.Length();

					for (unsigned i = 0; i < numWorld; i++)
					{
						mWorlds[i]->GetRS()->Fetch();
					}

					mFetchCompleteEvent.SetSignaled();

					for (unsigned i = 0; i < numWorld; i++)
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



	UAPI AssetCommendQueue* UGetAssetCommandQueue()
	{
		return &gEngine.mAssetCommands;
	}

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

	void IEngineInterface::LoadWorld(Name assetName)
	{

	}

	void IEngineInterface::SetWorld(World* world)
	{
	}

	World* IEngineInterface::CreateWorld()
	{
		World* world = new World;
		gEngine.mWorlds.Add(world);
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

	GameWindow* IEngineInterface::CreateGameWindow(const GameWindowCreationParam& cp)
	{
		return GameWindow::Create(cp);
	}

	void IEngineInterface::DeleteGameWindow(GameWindow* gw)
	{
		GameWindow::Destroy(gw);
	}

	IEngineInterface* IEngineInterface::Get()
	{
		return gEngine.mInterface;
	}

};