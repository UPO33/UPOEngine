#pragma once


#include "../GFXCore/UGFXCore.h"

#include "../Engine/UGameWindow.h"
#include "../Engine/UWorldRS.h"
#include "../Engine/UWorld.h"
#include "../Engine/UCanvas.h"

#include "UDefferdRenderTargets.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class World;
	class WorldRS;

	//////////////////////////////////////////////////////////////////////////
	class UAPI Renderer
	{
	public:
		GameWindow* mGameWnd;
		GFXSwapChain* mSwapChain;
		WorldRS*	mWorldRS;
		PrimitiveBatch* mPrimitiveBatch;
		Canvas* mCanvas;
		DefferdRenderTargets* mRenderTargets = nullptr;
		class TestQuadRE* mQuad;
		CriticalSection mSwapChainLock;

		Renderer();
		~Renderer();

		static Renderer* New();
		static void Delete(Renderer*);

		virtual void RenderGameWin(GameWindow*);

		void Render();
		void CheckRenderTargetResizing();
	};


	//////////////////////////////////////////////////////////////////////////
	class RendererElement
	{
		Renderer* mRenderer;

	public:
		RendererElement(Renderer* renderer) : mRenderer(renderer) {}
		virtual ~RendererElement(){}

		Renderer* GetRenderer() const { return mRenderer; }

		virtual void Present(){}
	};
};