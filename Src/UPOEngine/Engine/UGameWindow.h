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
	};

	struct GameWindowCreationParam
	{
		Vec2I		mSize = Vec2I(400, 400);
		bool		mFulllScreen = false;
		wchar_t*	mWindowClassName = L"UPOEngine";
		bool		mCreateCanvas = true;
		bool		mCreatePrimitiveBatch = true;

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

	protected:
		World*					mWorld = nullptr;	//world to render from
		GameWindowRenderOptions	mOptions;
		GameWindowCreationParam mCreationParam;

		void*					mWindowHandle = nullptr;

		bool					mIsReady = false;
		bool					mRegistered = false;

		bool Init(const GameWindowCreationParam& param);

	public:

		virtual void CreatePrimitiveBatch() {};
		virtual void DestroyPrimitiveBatch() {};

		virtual void CreateCanvas() {};
		virtual void DestroyCanvas() {};

		virtual void CreateSwapChain() {};
		virtual void DestroySwapChain() {};

		virtual void OnCreateWindow() {};
		virtual void OnDestroyWindow() {};

		virtual void GetWinSize(Vec2I& out) {}
		virtual void* GetWinHandle() { return mWindowHandle; }


		void SetWorld(World*);
		inline World* GetWorld() const { return mWorld; }
		static bool PeekMessages();

		static GameWindow* Create(const GameWindowCreationParam& param);
		static void Destroy(GameWindow*);
	};
};