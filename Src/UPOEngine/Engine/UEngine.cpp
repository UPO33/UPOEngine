#include "UEngine.h"

#include "../GFXCore/UGFXLauncherWin.h"

namespace UPO
{

	Engine::Engine()
	{

	}

	Engine::~Engine()
	{

	}

	void Engine::Run()
	{
		Init();

		bLoop = true;
		while (bLoop)
		{
			bool result;
			
			result = GTTick();
			if(!result) break;

			result = RTTick();
			if(!result) break;

			Thread::Sleep(100);
		}

		Release();
		
	}

	void Engine::Init()
	{
		mMainWindow = GameWindow::New();
		mMainWindow->Init();

		mGFXContext = GFXContext::New();
		mGFXContext->Init(mMainWindow);

		mRenderer = Renderer::New();
		mRenderer->Init(mGFXContext);

		InitWorld();
	}

	void Engine::Release()
	{
		if (mRenderer) mRenderer->Release();
		if (mGFXContext) mGFXContext->Release();
		if (mMainWindow) mMainWindow->Release();
		
	}

	bool Engine::GTTick()
	{
		bool result = true;
		
		result = mMainWindow->Tick();
	
		if(mCurrentWorld)
		{ 
			mCurrentWorld->SingleTick(WorldTickResult());
		}

		return result;
	}

	bool Engine::RTTick()
	{
		return mRenderer->RenderFrame();
	}



	void Engine::InitWorld()
	{
		mCurrentWorld = new World();
		
		EntityCreationParam ecp;
		ecp.mClass = Entity::GetClassInfoStatic();
		ecp.mParent = nullptr;

		mCurrentWorld->CreateEntity(ecp);
		mCurrentWorld->CreateEntity(ecp);
		mCurrentWorld->CreateEntity(ecp);

		mCurrentWorld->SetPlaying(true);

	}

	Engine* Engine::Get()
	{
		static TInstance<Engine> Ins;
		return Ins;
	}

};