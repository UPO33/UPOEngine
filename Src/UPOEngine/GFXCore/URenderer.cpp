#include "URenderer.h"

#include "UScreenDrawer.h"

namespace UPO
{

	struct GFXRendererImpl : public GFXRenderer
	{
		GFXContext* mContext = nullptr;
		
		ScreenDrawer* mScreenDrawer = nullptr;

		struct MyStruct
		{
			GFXShader* mVShader;
			GFXShader* mPShader;
			GFXVertexBuffer* mVBuffer;
			GFXInputLayout* mInputLayout;

			void Init()
			{
				mVShader = gGFX->GetShader("TestColorQuad.hlsl", "VSMain", EShaderType::EST_VERTEX);
				mPShader = gGFX->GetShader("TestColorQuad.hlsl", "PSMain", EShaderType::EST_PIXEL);

				Vec2 verts[] = { Vec2(0,0), Vec2(1,0), Vec2(0,1) };

				GFXVertexBuffer_Desc vbd;
				vbd.mInitialData = verts;
				vbd.mSize = sizeof(verts);
			
				mVBuffer = gGFX->CreateVertexBuffer(vbd);

				GFXVertexElement_Desc elemDisc[] = { {"POSITION", EPixelFormat::EPT_R32G32_FLOAT} };
				mInputLayout = gGFX->CreateInputLayout(elemDisc, 1, mVShader);
			}

			void Frame()
			{

				gGFX->BindShaders(mVShader, mPShader);
				gGFX->BinVertexBuffer(mVBuffer, sizeof(Vec2), 0);
				gGFX->BinIndexBuffer(nullptr);
				gGFX->BinInputLayout(mInputLayout);
				gGFX->SetPrimitiveTopology(EPT_TRIANGLELIST);

				gGFX->Draw(3, 0);
			}

		} mTest; 

		//////////////////////////////////////////////////////////////////////////
		bool Init(GFXContext* context) override
		{
			mContext = context;

			mScreenDrawer = new ScreenDrawer();
			mScreenDrawer->Init(this);

			mTest.Init();

			return false;
		}
		//////////////////////////////////////////////////////////////////////////
		bool RenderFrame() override
		{

			gGFX->SetRenderTarget(mContext->GetBackBuffer(), nullptr);
			gGFX->ClearRenderTarget(mContext->GetBackBuffer(), Color(0.1, 0, 0, 0));
			
			//mTest.Frame();
			mScreenDrawer->Frame();

			mContext->PresentSwapChain();
			return false;
		}
		
	};
	GFXRenderer* GFXRenderer::Get()
	{
		static TInstance<GFXRendererImpl> Ins;
		return Ins;
	}

};