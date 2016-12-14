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
			ULOG_MESSAGE("");
			mContext = context;

			mTestQuad = new TestQuadRE(this);

			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		bool Release() override
		{
			ULOG_MESSAGE("");
			delete mTestQuad;
			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		bool RenderFrame() override
		{
			ChekingResize();

			//gGFX->SetRenderTarget(mContext->GetBackBuffer(), nullptr);
			gGFX->ClearRenderTarget(mContext->GetBackBuffer(), Color(1, 1, RandFloat01(), 1));
			gGFX->SetRenderTarget(mContext->GetBackBuffer(), nullptr);

			
			
			mTestQuad->Present();

			mContext->PresentSwapChain();
			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		void ChekingResize()
		{
			Vec2I newSize, curSize;
			mContext->GetGameWindow()->GetSize(newSize);
			mContext->GetBackBufferSize(curSize);

			if (curSize != newSize)
			{
				ULOG_MESSAGE("resizing render target, new size : %dx%d", newSize.mX, newSize.mY);

				mContext->Resize(newSize);
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