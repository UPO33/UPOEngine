#include "UEngine.h"

#include "../GFXCore/UGFXCore.h"
#include "../GFX/UGFX.h"
#include "../Core/UCommandQueue.h"
#include "../Engine/UWorldRS.h"
#include "../Engine/UEntityStaticMesh.h"
#include "../Engine/UEntityTest.h"


namespace UPO
{
	unsigned	gGameTickCounter = 0;
	unsigned	gRenderTickCounter = 0;
	
	struct RenderThreadData
	{
		GFXContext* mGFXContext;
		Renderer* mRenderer;
	};

	struct EngineImpl
	{

		World* mCurrentWorld;
		WorldRS*	mCurrentWorldRS;
		GameWindow* mMainWindow;
		GFXContext* mGFXContext;
		Renderer* mRenderer;
		IEngineInterface* mInterface;
		bool mLoop;
		ThreadHandle	mRenderThreadHandle;

		TCommandPool<1024 * 8, true>		mCmdQueueG2R;
		TCommandPool<1024 * 8, false>	mCmdQueueR2G;
		
		AssetCommendQueue	mAssetCommands;

		volatile bool mLoopGT = true;
		volatile bool mLoopRT = true;
		volatile float mDelta = 0;	//render writes, game only reads
		volatile bool mCompilingGlobalShader = false;

		//when render thread was released this event is signaled
		Event	mRenderThreadReleased;

		EngineImpl() :
			mRenderThreadReleased(false, false)
		{

		}
		~EngineImpl()
		{
		}

		template<typename L> void EnqueueToRender(L& l) { mCmdQueueG2R.Enqueue(l); }
		template<typename L> void EnqueueToGame(L& l) { mCmdQueueR2G.Enqueue(l); }
		//////////////////////////////////////////////////////////////////////////
		void Init()
		{
			gGameThreadID = Thread::ID();

			ULOG_MESSAGE("Initilizing Engine...");

			Thread::CreateLambda([](){

				gRenderThreadID = Thread::ID();
	
				ULOG_MESSAGE("Render Thread Created");

				gEngine.InitRT();
				gEngine.LoopRT();
			});

			mInterface->OnInit();



			mMainWindow = mInterface->OnCreateGameWindow();
			UASSERT(mMainWindow);
			ULOG_MESSAGE("Game Window Created");

			EnqueueToRender([this]() {
				mGFXContext = GFXContext::New();
				UASSERT(mGFXContext);

				mGFXContext->Init(mMainWindow);

				//mInterface->OnAfterDeviceCreation();

				UGlobalShader_CompileAll();
				mCompilingGlobalShader = false;

				mRenderer = Renderer::New();
				UASSERT(mRenderer);
				mRenderer->Init(mGFXContext);

				
			});

			mCurrentWorld = new World;
			mCurrentWorld->SetPlaying();
			
			mCurrentWorld->GetTimer()->StartTimer(1, 1, [this]() { 
				ULOG_MESSAGE("");
				EntityCreationParam ecp;
				ecp.mClass = EntityTest::GetClassInfoStatic();
				ecp.mParent = nullptr;
				mCurrentWorld->CreateEntity(ecp);
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
		void Release()
		{
			ULOG_MESSAGE("");
			mLoopRT = false;
			mRenderThreadReleased.Wait();


// 			mCurrentWorld->Release();

			
			mInterface->OnReleaseGameWindow();
			mMainWindow = nullptr;

			mInterface->OnRelease();

// 			delete mCurrentWorld;
		}
		void ReleaseRT()
		{
			UASSERT(IsRenderThread());
			ULOG_MESSAGE("");
			mRenderer->Release();
			mGFXContext->Release();
			delete mRenderer;
			delete mGFXContext;
			mRenderer = nullptr;
			mGFXContext = nullptr;
		}
		bool GTick()
		{
			mCmdQueueR2G.InvokeAll();

			bool result = true;
			
			if (mCompilingGlobalShader) return true;

			GAssetSys()->Tick(mDelta);

			result = mInterface->OnTick();

			if (mCurrentWorld)
			{
				mCurrentWorld->Tick(mDelta);
			}

			gGameTickCounter++;

			return result;
		}
		bool RTick()
		{
			bool result = true;
			mCmdQueueG2R.InvokeAll();
			mAssetCommands.InvokeAll();

			//GAssetSys()->Frame(mDelta);

			if(mRenderer)
			{
				result = mRenderer->RenderFrame();
			}
			if (mCurrentWorld)
			{
				mCurrentWorld->GetRS()->Frame();
			}

			gRenderTickCounter++;
			return result;
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


};