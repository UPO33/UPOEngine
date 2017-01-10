#pragma once

#include "../Core/UCore.h"

namespace UPO
{ 
	//////////////////////////////////////////////////////////////////////////
	class World;
	class WorldRS;
	class PrimitiveBatch;
	class Canvas;
	class GFXSwapChain;
	class Renderer;

	struct GameWindowRenderOptions
	{
		bool	mRealTime = true;
		bool	mRenderStaticMeshes = true;
		bool	mRenderPrimitiveBatch = true;
		bool	mRenderCanvas = false;
		bool	mShowFPS = true;
	};

	struct UAPI GameWindowCreationParam
	{
		Vec2I		mSize = Vec2I(400, 400);
		bool		mFulllScreen = false;
		wchar_t*	mWindowClassName = L"UPOEngine";
		bool		mCreateCanvas = true;
		bool		mCreatePrimitiveBatch = true;
		unsigned	mSampleCount = 1;
		bool		mVSyncEnable = false;

		GameWindowCreationParam(){}
		GameWindowCreationParam(InitConfig);
	};
	//////////////////////////////////////////////////////////////////////////
	class UAPI GameWindow abstract
	{
		friend Renderer;
	public:
		static TArray<GameWindow*>	Instances;

	
		PrimitiveBatch*			mPrimitiveBatch = nullptr;
		Canvas*					mCanvas = nullptr;	//main canvas that cover whole window
		GFXSwapChain*			mSwapchain = nullptr;
		GameWindowRenderOptions	mOptions;
		GameWindowCreationParam mCreationParam;
		Color					mClearColor = Color(0.9f, 0.9f, 0.9f, 0);
		bool					mHasFocus;

		void BeginRender();
		void EndRender();

		double GetFrameElapsedSeconds() const { return mFrameElapsedSeconds; }

	protected:
		World*					mWorld = nullptr;	//world to render from

		double					mFrameElapsedSeconds = 0;
		ChronometerAccurate		mFrameTimer;

		void*					mWindowHandle = nullptr;

		bool					mIsReady = false;
		bool					mRegistered = false;

		

	public:

		GameWindow();
		~GameWindow();

		bool InitAndRegister(const GameWindowCreationParam& param);
		bool Release();

		virtual void CreatePrimitiveBatch() {};
		virtual void DestroyPrimitiveBatch() {};

		virtual bool CreateCanvas();;
		virtual bool DestroyCanvas();;

		virtual bool CreateSwapChain();
		virtual bool DestroySwapChain();

		virtual void OnCreateWindow() {};
		virtual void OnDestroyWindow() {};

		virtual void GetWinSize(Vec2I& out) = 0;
		virtual void* GetWinHandle() { return mWindowHandle; }


		void SetWorld(World*);
		inline World* GetWorld() const { return mWorld; }


		static bool PeekMessages();

		//create and register a launcher window
		static GameWindow* CreateLauncherWin(const GameWindowCreationParam& param);
	};
};