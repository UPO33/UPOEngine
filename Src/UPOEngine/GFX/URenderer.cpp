#include "URenderer.h"

#include "UScreenDrawer.h"
#include "UTestQuad.h"
#include "UPrimitiveBatch.h"
#include "UDefferdRenderTargets.h"
#include "USelectionBuffer.h"
#include "UTestQuad.h"

#include "../Engine/UInput.h"
#include "../Engine/UEntityCamera.h"
#include "../Engine/UStaticMesh.h"
#include "../Engine/UEntityStaticMesh.h"

#include "../GFXCore/UGFXDeviceDX.h"

namespace UPO
{
	UGLOBAL_SHADER_DECLIMPL(gVSGrid, GFXVertexShader, "Grid.hlsl", "VSMain");
	UGLOBAL_SHADER_DECLIMPL(gPSGrid, GFXPixelShader, "Grid.hlsl", "PSMain");

// 	UGLOBAL_SHADER_DECLIMPL(gVSBasePassForward, GFXVertexShader, "BasePassForward.hlsl", "VSMain");
// 	UGLOBAL_SHADER_DECLIMPL(gPSBasePassForward, GFXPixelShader, "BasePassForward.hlsl", "PSMain");

	UGLOBAL_SHADER_DECLIMPL(gVSBasePassDefferd, GFXVertexShader, "BasePassDeferred.hlsl", "VSMain");
	UGLOBAL_SHADER_DECLIMPL(gPSBasePassDefferd, GFXPixelShader, "BasePassDeferred.hlsl", "PSMain");

	UGLOBAL_SHADER_DECLIMPL(gVSEndPassDeferred, GFXVertexShader, "EndPassDeferred.hlsl", "VSMain");
	UGLOBAL_SHADER_DECLIMPL(gPSEndPassDeferred, GFXPixelShader, "EndPassDeferred.hlsl", "PSMain");

	//////////////////////////////////////////////////////////////////////////typedef for hlsl support
	typedef Vec4 float4;
	typedef Matrix4 matrix, float4x4;


	//////////////////////////////////////////////////////////////////////////
	struct CBPerFrame
	{
		static const unsigned Index = 0;

		float4 mSunDir;
		float4 mSunColor;
		float4 mRandomColor;
	};
	
	//////////////////////////////////////////////////////////////////////////
	struct CBPerCamera
	{
		static const unsigned Index = 1;

		matrix mProjection;
		matrix mInvProjection;
		matrix mView;
		matrix mInvView;
		matrix mWorldToCilp;
		matrix mClipToWorld;
		float4 mWorldPosition;
	};

	//////////////////////////////////////////////////////////////////////////
	struct CBPerStaticMesh
	{
		static const unsigned Index = 2;

		matrix		mLocalToWorld;
	};




	struct GridDraw
	{
		struct Vertex
		{
			Vec3		mPosition;
			Color32		mColor;
		};

		TArray<Vertex>		mLinesVertex;

		float		mGridCellSize = 100;
		int			mNumCellPerAxis = 32;

		GFXVertexBuffer*	mVBuffer = nullptr;
		GFXInputLayout*		mInputLayout = nullptr;
		GFXBlendState*		mBlendState = nullptr;

		unsigned GetNumVertex() const { return (mNumCellPerAxis + 1) * 2 * 2 + 2; }

		GridDraw()
		{
			mLinesVertex.RemoveAll();
			mLinesVertex.AddUnInit(GetNumVertex());

			
			int numCell = mNumCellPerAxis / 2;
			for (int iVertex = 0, i = -numCell; i <= numCell; i++, iVertex += 4)
			{
				mLinesVertex[iVertex].mPosition = Vec3(i * mGridCellSize, 0, -mGridCellSize * numCell);
				mLinesVertex[iVertex].mColor = Color32::BLUE;
				mLinesVertex[iVertex + 1].mPosition = Vec3(i * mGridCellSize, 0, mGridCellSize * numCell);
				mLinesVertex[iVertex + 1].mColor = Color32::BLUE;

				mLinesVertex[iVertex + 2].mPosition = Vec3(-mGridCellSize * numCell, 0, i * mGridCellSize);
				mLinesVertex[iVertex + 2].mColor = Color32::RED;
				mLinesVertex[iVertex + 3].mPosition = Vec3(mGridCellSize * numCell, 0, i * mGridCellSize);
				mLinesVertex[iVertex + 3].mColor = Color32::RED;
			}

			mLinesVertex[mLinesVertex.Length() - 2].mColor = Color32::GREEN;
			mLinesVertex[mLinesVertex.Length() - 2].mPosition = Vec3::ZERO;
			mLinesVertex[mLinesVertex.Length() - 1].mColor = Color32::GREEN;
			mLinesVertex[mLinesVertex.Length() - 1].mPosition = Vec3(0,100,0);

			{
				GFXVertexBufferDesc desc;
				desc.mImmutable = true;
				desc.mInitialData = mLinesVertex.Elements();
				desc.mSize = sizeof(GridDraw::Vertex) * mLinesVertex.Length();
				mVBuffer = gGFX->CreateVertexBuffer(desc);
				UASSERT(mVBuffer);
			}
			{
				GFXInputLayoutDesc desc = {
					gVSGrid, 
					{
						{ "POSITION", EVertexFormat::EFloat3, -1, 0, false },
						{ "COLOR", EVertexFormat::EColor32, -1, 0, false },
					}
				};
				mInputLayout = GlobalResources::GetInputLayout(desc);
				UASSERT(mInputLayout);
			}
			{
				GFXBlendStateDesc desc;
				desc.mRenderTargets[0].MakeAlphaBlending();
				mBlendState = GlobalResources::GetBlendState(desc);
				UASSERT(mBlendState);
			}
		}
		~GridDraw()
		{
			SafeDelete(mVBuffer);
		}
		void Draw()
		{
			gGFX->SetBlendState(mBlendState);
			gGFX->SetShaders(gVSGrid, gPSGrid);
			gGFX->SetIndexBuffer(nullptr);
			gGFX->SetVertexBuffer(mVBuffer, 0, sizeof(GridDraw::Vertex), 0);
			gGFX->SetPrimitiveTopology(EPrimitiveTopology::ELineList);
			gGFX->SetInputLayout(mInputLayout);
			gGFX->Draw(GetNumVertex());
		}
	};


	Color32 UDetectFPSColor(unsigned fps)
	{
		if (fps >= 50) return Color32::GREEN;
		if (fps >= 35) return Color32::YELLOW;
		return Color32::RED;
	}


	struct TestTriVertex
	{
		Vec3	mPosition;
		Color32	mColor;
	};






	void Renderer::RenderGameWin(GameWindow* gw)
	{
		if (gw && gw->mSwapchain && gw->mWorld && gw->mWorld->GetRS())
		{
			mGameWnd = gw;
			mSwapChain = gw->mSwapchain;
			mWorldRS = gw->mWorld->GetRS();
			mCanvas = gw->mCanvas;
			mPrimitiveBatch = gw->GetWorld()->GetPrimitiveBatch();
			Vec2I wndSize = mGameWnd->GetWinSize();
			mViewportSize = Vec2(wndSize.mX, wndSize.mY);
			Render();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	Vec2I UGetBiggestGameWindowSize()
	{
		Vec2I maxWinSize = Vec2I(0);
		for (GameWindow* gw : GameWindow::Instances)
		{
			maxWinSize.mX = Max(maxWinSize.mX, gw->GetWinSize().mX);
			maxWinSize.mY = Max(maxWinSize.mY, gw->GetWinSize().mY);
		}
		return maxWinSize;
	}
	
	//////////////////////////////////////////////////////////////////////////
	void Renderer::Render()
	{
		mGameWnd->BeginRender();

		CheckRenderTargetResizing();

		//update per frame cbuffer
		{
			auto mapped = gGFX->Map<CBPerFrame>(mCBPerFrame, EMapFlag::EWriteDiscard);

			gGFX->Unmap(mCBPerFrame);
		}

		
		RenderWorld();

		if (mCanvas && mGameWnd->mOptions.mRenderCanvas) //draw canvas?
		{
			if (mGameWnd->mOptions.mRealTime && mGameWnd->mOptions.mShowFPS)	//show FSP ?
			{
				char fpsText[64];
				sprintf_s(fpsText, "FPS %i  %.2fms", gFPS, mGameWnd->GetFrameElapsedSeconds() * 1000);
				mCanvas->DrawString(fpsText, Vec2(2), UDetectFPSColor(gFPS));
			}

			mCanvas->Render();
		}


		mSwapChain->Present();

		mGameWnd->EndRender();

	}
	GFXViewport UViewportFromCamera(EntityCameraRS* camera, Vec2 windowSize)
	{
		return GFXViewport(camera->mViewportOffset *windowSize, camera->mViewPortSize * windowSize, 0, 1);
	}
	void Renderer::RenderWorld()
	{
		mCamerasToRender.RemoveAll();
		mWorldRS->GetDesiredCameras(mCamerasToRender);
		
		
		for (mCurRenderingCameraIndex = 0; mCurRenderingCameraIndex < mCamerasToRender.Length(); mCurRenderingCameraIndex++)
		{
			EntityCameraRS* camera = mCamerasToRender[mCurRenderingCameraIndex];

			UpdatePerCameraCBuffer(camera);

			gGFX->SetConstentBuffer(mCBPerCamera, 1, EShaderType::EVertex);
			gGFX->SetConstentBuffer(mCBPerCamera, 1, EShaderType::EPixel);

			GFXViewport viewport = UViewportFromCamera(camera, mViewportSize);
// 			ULOG_MESSAGE("% %", viewport.mWidth, viewport.mHeight);
			gGFX->SetViewport(viewport);

			{
				mRenderTargets->BinGBuffers(true);

				RenderStaticMeshes();


				gGFX->SetShaders(gVSEndPassDeferred, gPSEndPassDeferred);
				GFXRenderTargetView* renderTargets[] = { mSwapChain->GetBackBufferView() };
				gGFX->SetRenderTarget(renderTargets, nullptr);
				gGFX->SetViewport(viewport);
				gGFX->SetResourceView(mRenderTargets->mGBufferA, 0, EShaderType::EPixel);
				gGFX->SetResourceView(mRenderTargets->mGBufferB, 1, EShaderType::EPixel);
				gGFX->SetBlendState(nullptr);
				gGFX->SetDepthStencilState(nullptr);
				gGFX->SetRasterizerState(nullptr);
				gGFX->SetIndexBuffer(nullptr);
				gGFX->SetPrimitiveTopology(EPrimitiveTopology::ETriangleList);
				gGFX->Draw(3);
				
			}
			//bind swap chain and depth buffer
// 			gGFX->ClearRenderTarget(mSwapChain->GetBackBufferView(), Color::WHITE);
// 			GFXRenderTargetView* renderTargets[] = { mSwapChain->GetBackBufferView() };
// 			gGFX->SetRenderTarget(renderTargets, mRenderTargets->mDepthStencil->GetDepthStencilView());

			//draw primitives
			if (mPrimitiveBatch && mGameWnd->mOptions.mRenderPrimitiveBatch) mPrimitiveBatch->Render();

			//draw grid
			if (mGridDraw && mGameWnd->mOptions.mRenderGrid) mGridDraw->Draw();
			
		}
		mCurRenderingCameraIndex = -1;
	}

	void Renderer::CheckRenderTargetResizing()
	{
		Vec2I bufferSize;
		Vec2I wndSize;

		mSwapChain->GetBackBufferSize(bufferSize);
		mGameWnd->GetWinSize(wndSize);

		mViewportSize.mX = wndSize.mX;
		mViewportSize.mY = wndSize.mY;

		if (!mRenderTargets) mRenderTargets = new DefferdRenderTargets(UGetBiggestGameWindowSize());


		if (wndSize != bufferSize)	//window size has changed
		{
			ULOG_MESSAGE("resizing render target, new size : %", mViewportSize);

			if (mRenderTargets) delete mRenderTargets;
			mRenderTargets = new DefferdRenderTargets(UGetBiggestGameWindowSize());

			mSwapChain->Resize(wndSize);
			if (mCanvas) mCanvas->Resize(wndSize);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void Renderer::RenderStaticMeshes()
	{
		{
			GFXRasterizerStateDesc desc;
			desc.mCullMode = ECullMode::ENone;
			desc.mWireframe = false;
			auto rs = GlobalResources::GetRasterizerState(desc);
			UASSERT(rs);
			gGFX->SetRasterizerState(rs);

		}
		{
			GFXDepthStencilStateDesc desc;
			desc.mDepthEnable = true;
			desc.mStencilEnable = false;
			gGFX->SetDepthStencilState(GlobalResources::GetDepthStencilState(desc));
		}
		gGFX->SetBlendState(nullptr);


		unsigned numSMesh = mWorldRS->mStaticMeshes.Length();
		for (unsigned iMesh = 0; iMesh < numSMesh; iMesh++)
		{
			EntityStaticMeshRS* entityMeh = mWorldRS->mStaticMeshes[iMesh];
			AStaticMeshRS* meshAsset = entityMeh->mMesh;
			if (meshAsset)
			{
				auto mapped = gGFX->Map<CBPerStaticMesh>(mCBPerStaticMesh, EMapFlag::EWriteDiscard);
				mapped->mLocalToWorld = entityMeh->mWorldTransform;
				gGFX->Unmap(mCBPerStaticMesh);

				gGFX->SetIndexBuffer(meshAsset->mIndexBuffer);
				gGFX->SetVertexBuffer(meshAsset->mVertexBuffer, 0, sizeof(AStaticMesh::VertexTypeFull), 0);
				gGFX->SetInputLayout(mILStaticMeshVertexTypeFull);
				gGFX->SetShaders(gVSBasePassDefferd, gPSBasePassDefferd);
				gGFX->SetConstentBuffer(mCBPerStaticMesh, CBPerStaticMesh::Index, EShaderType::EVertex);
				gGFX->SetPrimitiveTopology(EPrimitiveTopology::ETriangleList);
				gGFX->DrawIndexed(meshAsset->mIndexCount);
			}

		}
	}

	GFXDepthStencilState* Renderer::GetRasterizerForStaticMeshSolid()
	{
		return nullptr;
	}

	//////////////////////////////////////////////////////////////////////////
	void Renderer::UpdatePerCameraCBuffer(EntityCameraRS* camera)
	{
		auto mapped = gGFX->Map<CBPerCamera>(mCBPerCamera, EMapFlag::EWriteDiscard);

// 		Matrix4 testWorldToCLip, matProj, matView, matWorld;
// 		matWorld.MakeScale(1);
// 		matProj.MakePerspective(60, 1, 0.1f, 1000);
// 		matView.MakeViewLookAt(Vec3(-10, 10, -10), Vec3(0), Vec3(0, 1, 0));
// 		testWorldToCLip = matWorld * matView * matProj;

		mapped->mProjection = camera->mMatrixProj;
		mapped->mInvProjection = camera->mMatrixProjInv;
		mapped->mView = camera->mMatrixView;
		mapped->mInvView = camera->mMatrixViewInv;
		mapped->mWorldToCilp = camera->mMatrixWorldToClip;
		mapped->mClipToWorld = camera->mMatrixClipToWorld;
		mapped->mWorldPosition = Vec4(camera->mMatrixViewInv.GetTranslation(), 1);
		
		gGFX->Unmap(mCBPerCamera);
	}

	//////////////////////////////////////////////////////////////////////////
	Renderer::Renderer()
	{
		mCBPerFrame = gGFX->CreateConstantBuffer(sizeof(CBPerFrame));
		UASSERT(mCBPerFrame);
		mCBPerCamera = gGFX->CreateConstantBuffer(sizeof(CBPerCamera));
		UASSERT(mCBPerCamera);
		mCBPerStaticMesh = gGFX->CreateConstantBuffer(sizeof(CBPerStaticMesh));
		UASSERT(mCBPerStaticMesh);

		mGridDraw = new GridDraw();

		{
			GFXInputLayoutDesc  desc = 
			{
				gVSBasePassDefferd, 
				{
					{ "POSITION", EVertexFormat::EFloat3, -1, 0, false},
					{ "NORMAL", EVertexFormat::EFloat3, -1, 0, false },
					{ "UV", EVertexFormat::EFloat2, -1, 0, false}
				}
			};
			mILStaticMeshVertexTypeFull = GlobalResources::GetInputLayout(desc);
			UASSERT(mILStaticMeshVertexTypeFull);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	Renderer::~Renderer()
	{
		SafeDelete(mGridDraw);
		SafeDelete(mCBPerCamera);
		SafeDelete(mCBPerFrame);

	}

};
