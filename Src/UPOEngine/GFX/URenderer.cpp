#include "URenderer.h"

#include "UScreenDrawer.h"
#include "UTestQuad.h"
#include "../Engine/UGameWindow.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	struct GFXRendererImpl : public Renderer
	{
		GFXContext* mContext = nullptr;
		
		ScreenDrawer* mScreenDrawer = nullptr;
		TestQuadRE* mTestQuad = nullptr;

		//////////////////////////////////////////////////////////////////////////
		bool Init(GFXContext* context) override
		{
			mContext = context;

			mTestQuad = new TestQuadRE(this);

			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		bool Release() override
		{
			delete mTestQuad;
			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		bool RenderFrame() override
		{
			ChekingResize();

			gGFX->SetRenderTarget(mContext->GetBackBuffer(), nullptr);
			gGFX->ClearRenderTarget(mContext->GetBackBuffer(), Color(0.1, 0, 0, 0));
			
			mTestQuad->Present();

			mContext->PresentSwapChain();
			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		void ChekingResize()
		{
			auto window = mContext->GetGameWindow();
			unsigned newWidth = window->GetWidth();
			unsigned newHeight = window->GetHeight();


			if (mContext->BackBufferWidth() != newWidth || mContext->BackBufferHeight() != newHeight)
			{
				ULOG_MESSAGE("resizing render target, new size : %dx%d", newWidth, newHeight);

				mContext->Resize(newWidth, newHeight);
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

};