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
	class InputState;
	class HitSelectionCanvas;

	//////////////////////////////////////////////////////////////////////////
	static const float EditorCameraMinFOV = 30;
	static const float EditorCameraMaxFOV = 150;
	static const float EditorCameraMaxSpeed = 100;

	//////////////////////////////////////////////////////////////////////////
	struct GameWindowOptions
	{
		UCLASS(GameWindowOptions, void)

		bool	mRealTime = true;
		bool	mRenderStaticMeshes = true;
		bool	mRenderPrimitiveBatch = true;
		bool	mRenderCanvas = true;
		bool	mShowFPS = true;
		bool	mRenderGrid = true;
		bool	mShowBounds = true;
		bool	mVisualizeGBuffer = false;
		float	mEditorCameraSpeed = 100;
		float	mEditorCameraFOV = 60;
	};

	//////////////////////////////////////////////////////////////////////////
	struct UAPI GameWindowCreationParam
	{
		Vec2I		mSize = Vec2I(400, 400);
		bool		mFulllScreen = false;
		wchar_t*	mWindowClassName = L"UPOEngine";
		bool		mCreateCanvas = true;
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

		Canvas*					mCanvas = nullptr;	//main canvas that cover whole window
		GFXSwapChain*			mSwapchain = nullptr;
		InputState*				mInputState = nullptr;
		HitSelectionCanvas*		mHitSelection = nullptr;
		GameWindowOptions		mOptions;
		GameWindowCreationParam mCreationParam;
		Color					mClearColor = Color(0.9f, 0.9f, 0.9f, 0);
		bool					mHasFocus;

		void BeginRender();
		void EndRender();

		double GetFrameElapsedSeconds() const { return mFrameElapsedSeconds; }
		bool HasFocus() const { return mHasFocus; }

	protected:
		World*					mWorld = nullptr;	//world to render from

		double					mFrameElapsedSeconds = 0;
		ChronometerAccurate		mFrameTimer;

		void*					mWindowHandle = nullptr;

		bool					mIsReady = false;
		bool					mRegistered = false;
		Vec2I					mMousePosition;
		


	public:

		GameWindow();
		~GameWindow();

		Vec2I GetMousePosition() const { return mMousePosition; }

		bool InitAndReg(const GameWindowCreationParam& param);
		bool ReleaseAndUnreg();

		Canvas* GetCanvas() const { return mCanvas; }
		InputState* GetInputState() const { return mInputState; }
		GFXSwapChain* GetSwapchain() const { return mSwapchain; }
		HitSelectionCanvas* GetHitSelection() const { return mHitSelection; }

		virtual bool CreateCanvas();;
		virtual bool DestroyCanvas();;

		virtual bool CreateSwapChain();
		virtual bool DestroySwapChain();

		virtual void OnCreateWindow() {};
		virtual void OnDestroyWindow() {};

		virtual void GetWinSize(Vec2I& out) = 0;
		virtual void* GetWinHandle() { return mWindowHandle; }

		Vec2I GetWinSize()
		{
			Vec2I ret;
			GetWinSize(ret);
			return ret;
		}

		void SetWorld(World*);
		inline World* GetWorld() const { return mWorld; }


		static bool PeekMessages();

		//create and register a launcher window
		static GameWindow* CreateLauncherWin(const GameWindowCreationParam& param);
		static void Destroy(GameWindow* wnd);
		//destroy all registered windows, its used for clean up
		static void DestroyAll();
	};
};