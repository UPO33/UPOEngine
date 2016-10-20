#include "_URenderer.h"
#include "UGFXLauncherWin.h"

#include "../Engine/UWorld.h"

#include "UPrimitiveDrawer.h"
#include "UScreenDrawer.h"
#include "UMeshDrawer.h"

namespace UPO
{

	void Renderer::Init()
	{
		ULOG_MESSAGE("");

		GFXTexture2D_Desc dbdesc;
		dbdesc.mDepthStencil = true;
		dbdesc.mFormat = EPixelFormat::EPT_D32_FLOAT;
		dbdesc.mResourceView = false;
		dbdesc.mWidth = mRenderTargetW;
		dbdesc.mHeight = mRenderTargetH;
		mDepthBuffer = gGFXDX->CreateTexture2D(dbdesc);

		gGFXDX->SetRenderTarget(GFXLauncherWin::Get()->GetBackBuffer(), mDepthBuffer);

		GFXConstantBuffer_Desc descCBPerFrame;
		descCBPerFrame.mSize = sizeof(CBPerFrame);
		mCBPerFrame = gGFXDX->CreateConstantBuffer(descCBPerFrame);

		GFXConstantBuffer_Desc descCBPerObject;
		descCBPerObject.mSize = sizeof(CBPerObject);
		mCBPerObject = gGFXDX->CreateConstantBuffer(descCBPerObject);

		mTimer.Start();


		mShaderSMeshVS = gGFXDX->GetShader("StaticMesh.hlsl", "VSMain", EST_VERTEX);
		mShaderSMeshPS = gGFXDX->GetShader("StaticMesh.hlsl", "PSMain", EST_PIXEL);

		GFXDepthStencilState_Desc descDepthEnable;
		descDepthEnable.mDepthEnable = true;
		mDepthEnableState = gGFXDX->CreateDepthStencilState(descDepthEnable);

		GFXRasterizerState_Desc descRSFill;
		descRSFill.mCullMode = ECM_NONE;
		mRasterStateMeshFill = gGFXDX->CreateRasterizerState(descRSFill);

		GFXRasterizerState_Desc descRSWire;
		descRSWire.mWireframe = true;
		mRasterStateMeshWire = gGFXDX->CreateRasterizerState(descRSWire);

		GFXVertexElement_Desc elems[]=
		{
			GFXVertexElement_Desc("POSITION", EPT_R32G32B32_FLOAT),
			GFXVertexElement_Desc("NORMAL", EPT_R32G32B32_FLOAT),
			GFXVertexElement_Desc("UV", EPT_R32G32_FLOAT),
		};
		mInputLayoutMesh = gGFXDX->CreateInputLayout(elems, ARRAYSIZE(elems), mShaderSMeshVS);

		mPrimitiveDrawer = new PrimitiveDrawer;
		if (!mPrimitiveDrawer->Init(this))
			ULOG_FATAL("failed to init PrimitiveDrawer");
		mScreenDrawer = new ScreenDrawer;
// 		if (!mScreenDrawer->Init(this))
			ULOG_FATAL("failed to init ScreenDrawer");
		mMeshDrawer = new MeshDrawer;
		if (!mMeshDrawer->Init(this))
			ULOG_FATAL("failed to init MeshDrawer");
		
	}
	//////////////////////////////////////////////////////////////////////////
	void Renderer::Shutdown()
	{
		ULOG_MESSAGE("");


		mPrimitiveDrawer->Release();
		delete mPrimitiveDrawer;
		mScreenDrawer->Release();
		delete mScreenDrawer;
		mMeshDrawer->Release();
		delete mMeshDrawer;
	}
	//////////////////////////////////////////////////////////////////////////
	void Renderer::Tick()
	{
		BeginScene();


		RenderScene();

		{
			mFPS.Frame();
			char buffer[128];
			sprintf_s(buffer, "UPOEngine FPS %d", mFPS.GetFPS());
			GFXLauncherWin::Get()->SetCaption(buffer);
		}
		
		EndScene();
	}
	//////////////////////////////////////////////////////////////////////////
	void Renderer::BeginScene()
	{
		//GFXLauncherWin::Get()->ClearBackBuffer(mClearColor);

		gGFXDX->ClearDepthStencil(mDepthBuffer, true, true, 1, 0xFF);
		gGFXDX->ClearRenderTarget(GFXLauncherWin::Get()->GetBackBuffer(), mClearColor);
	}
	//////////////////////////////////////////////////////////////////////////
	void Renderer::RenderScene()
	{
		RenderBasePass();
// 		ScreenRender::Get()->Tick();
	}
	//////////////////////////////////////////////////////////////////////////
	void Renderer::EndScene()
	{
		GFXLauncherWin::Get()->SawpChainPresent();
	}
	//////////////////////////////////////////////////////////////////////////
	void Renderer::RenderBasePass()
	{

		mMeshDrawer->Frame();
		mPrimitiveDrawer->Frame();
		mScreenDrawer->Frame();
	}



	void GFXRenderer::Init(unsigned w, unsigned h, GFXContext* ctx)
	{

	}

	void GFXRenderer::Release()
	{

	}

	void GFXRenderer::Resize(unsigned newW, unsigned newH)
	{
		
	}

	void GFXRenderer::Render()
	{
		Device()->SetRenderTarget(mBackBuffer, mDepthBuffer);
		Device()->ClearRenderTarget(mBackBuffer, Color(RandFloat01(), 0,0,0));
		Device()->ClearDepthStencil(mDepthBuffer, true, false, 1, 0);

		Context()->PresentSwapChain();
	}

};