#include "UEngine.h"

#include "../GFXCore/UGFXLauncherWin.h"

namespace UPO
{

	Engine::~Engine()
	{

	}

	void Engine::Run()
	{
		Init();

		bExit = false;
		while (bExit)
		{
			GTTick();
			RHTick();
		}

		Release();
		
	}

	void Engine::Init()
	{
		mMainWindow = new GameWindow;

		mCurrentWorld = new World();
		mCurrentWorld->SetPlaying(true);
	}

	void Engine::Release()
	{
		if (mMainWindow) delete mMainWindow;
		mMainWindow = nullptr;
	}

	bool Engine::GTTick()
	{
		if (mMainWindow)
		{
			mMainWindow->Tick();
		}
		if(mCurrentWorld)
		{ 
			mCurrentWorld->SingleTick(WorldTickResult());
		}
		return true;
	}

	bool Engine::RHTick()
	{
		return true;
	}

	Engine* Engine::Get()
	{
		static TInstance<Engine> Ins;
		return Ins;
	}

};