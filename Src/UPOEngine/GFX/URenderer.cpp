#include "URenderer.h"

#include "UScreenDrawer.h"
#include "UTestQuad.h"
#include "UPrimitiveBatch.h"
#include "UDefferdRenderTargets.h"


#include "../Engine/UGameWindow.h"
#include "../Engine/UWorldRS.h"
#include "../Engine/UWorld.h"


namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	struct GFXRendererImpl : public Renderer
	{
		//////////////////////////////////////////////////////////////////////////
		bool Init() override
		{
// 			ULOG_MESSAGE("");
// 			mContext = context;
// 
			mTestQuad = new TestQuadRE(this);

// 			mPrimitiveBatch = new PrimitiveBatch;

			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		bool Release() override
		{
			ULOG_MESSAGE("");
			delete mTestQuad;
// 			delete mPrimitiveBatch;
			return true;
		}
		void AttachWorld(WorldRS* world) override
		{
			if (mWorld == world) return;
			mWorld = world;
			mWorld->mRenderer = this;

		}
		//////////////////////////////////////////////////////////////////////////
		bool RenderFrame() override
		{
			ChekingResize();

			//gGFX->SetRenderTarget(mContext->GetBackBuffer(), nullptr);
// 			gGFX->ClearRenderTarget(mContext->GetBackBuffer(), Color(1, 1, RandFloat01(), 1));
// 			gGFX->BindRenderTarget(mContext->GetBackBuffer(), nullptr);

			
			
			mTestQuad->Present();
// 			mPrimitiveBatch->Render();

			if (mWorld) mWorld->Frame();

// 			mContext->PresentSwapChain();
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		void ChekingResize()
		{
			Vec2I newSize, curSize;
// 			mContext->GetGameWindow()->GetSize(newSize);
// 			mContext->GetBackBufferSize(curSize);

			if (curSize != newSize)
			{
				ULOG_MESSAGE("resizing render target, new size : %", newSize);

// 				mContext->Resize(newSize);
				//resizing other render targets...
			}
		}
	};













	Renderer* Renderer::New()
	{
		return new GFXRendererImpl;
	}

	void Renderer::Delete(Renderer* renderer)
	{
		delete renderer;
	}

// 	struct RenderTask
// 	{
// 		GameWindow* mGameWnd;
// 		GFXSwapChain* mSwapChain;
// 		WorldRS*	mWorldRS;
// 		PrimitiveBatch* mPrimitiveBatch;
// 		Canvas* mCanvas;
// 		DefferdRenderTargets* mRenderTarget;
// 		
// 	};

	void Renderer::RenderGameWin(GameWindow* gw)
	{
		if (gw && gw->mSwapchain && gw->mWorld && gw->mWorld->GetRS())
		{
			WorldRS* rsToRender = gw->mWorld->GetRS();
			rsToRender->Render();

// 			ResizeIfRequired();

// 			ClearRenderTargets();
// 
// 			rsToRender->Render(gw);
// 
// 			if (gw->mPrimitiveBatch) gw->mPrimitiveBatch->Render();
// 			if (gw->mCanvas) gw->mCanvas->Render();
// 
// 			gw->mSwapchain->Swap();
		}
	}

};