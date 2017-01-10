#include "URenderer.h"

#include "UScreenDrawer.h"
#include "UTestQuad.h"
#include "UPrimitiveBatch.h"
#include "UDefferdRenderTargets.h"

#include "UTestQuad.h"

#include "../Engine/UInput.h"
namespace UPO
{
	Color32 UDetectFPSColor(unsigned fps)
	{
		if (fps >= 50) return Color32::GREEN;
		if (fps >= 35) return Color32::YELLOW;
		return Color32::RED;
	}

	Renderer* Renderer::New()
	{
		auto renderer = new Renderer;
		renderer->mQuad = new TestQuadRE(renderer);
		return renderer;
	}

	void Renderer::Delete(Renderer* renderer)
	{
		delete renderer;
	}



	void Renderer::RenderGameWin(GameWindow* gw)
	{
		if (gw && gw->mSwapchain && gw->mWorld && gw->mWorld->GetRS())
		{
			mGameWnd = gw;
			mSwapChain = gw->mSwapchain;
			mWorldRS = gw->mWorld->GetRS();
			mPrimitiveBatch = gw->mPrimitiveBatch;
			mCanvas = gw->mCanvas;
			Render();
		}
	}


	void Renderer::Render()
	{
		Vec2I wndSize;
		mSwapChain->GetBackBufferSize(wndSize);


		mGameWnd->BeginRender();
	
		CheckRenderTargetResizing();
		
		
// 		mWorldRS->Render();

		//after that signal we dont need wordRS
		//SignaleFeachReady();

		

		//clear back buffer
		gGFX->ClearRenderTarget(mSwapChain->GetBackBufferView(), mGameWnd->mClearColor);



		GFXRenderTargetView* renderTargets[] = { 
			mSwapChain->GetBackBufferView(),
			mRenderTargets->mIDRenderTarget->GetRenderTargetView()
		};

		gGFX->SetRenderTarget(renderTargets, nullptr);

		
		GFXViewport viewport = { 0, 0, (float)wndSize.mX, (float)wndSize.mY, 0, 1 };
		gGFX->SetViewport(viewport);

		if (mCanvas)
		{
			if (mGameWnd->mOptions.mRealTime && mGameWnd->mOptions.mShowFPS)	//show FSP ?
			{
				char fpsText[64];
				sprintf_s(fpsText, "FPS %i  %.2fms", gFPS, mGameWnd->GetFrameElapsedSeconds() * 1000);
				mCanvas->DrawString(fpsText, Vec2(2), UDetectFPSColor(gFPS));

				{
					StringStreamOut str;
					SPrintAuto(str, "Wnd % Mouse %", wndSize, Input::GetMousePosition());
					auto strStr = str.str();
					mCanvas->DrawString(strStr.c_str(), Vec2(2, 60), Color::GREEN);
				}
			}

			mCanvas->Render();

			
		}

		
// 		mSwapChainLock.Enter();
// 		mSwapChain->Present();
// 		mSwapChainLock.Leave();
// 		mCanvas->Swap();
		
		mGameWnd->EndRender();

	}

	void Renderer::CheckRenderTargetResizing()
	{
		Vec2I newSize, curSize;
		mGameWnd->GetWinSize(newSize);
		mSwapChain->GetBackBufferSize(curSize);

		if (!mRenderTargets)
			mRenderTargets = new DefferdRenderTargets(newSize);

		if (newSize != curSize)
		{
			ULOG_MESSAGE("resizing render target, new size : %", newSize);

			if (mRenderTargets)
				delete mRenderTargets;
			mRenderTargets = new DefferdRenderTargets(newSize);

			mSwapChain->Resize(newSize);
			if (mCanvas) mCanvas->Resize(newSize);
		}
	}

	Renderer::Renderer()
	{

	}

	Renderer::~Renderer()
	{

	}

};