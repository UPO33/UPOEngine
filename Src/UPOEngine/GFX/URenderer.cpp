#include "URenderer.h"


#include "UDefferdRenderTargets.h"
#include "USelectionBuffer.h"
#include "UShaderConstants.h"

#include "../Engine/UInput.h"
#include "../Engine/UEntityCamera.h"
#include "../Engine/UStaticMesh.h"
#include "../Engine/UEntityStaticMesh.h"
#include "../Engine/UHitSelection.h"
#include "../Engine/UPrimitiveBatch.h"

#include "../GFXCore/UGFXDeviceDX.h"

namespace UPO
{
	UGLOBAL_SHADER_DECLIMPL(gVSGrid, GFXVertexShader, "Grid.hlsl", "VSMain");
	UGLOBAL_SHADER_DECLIMPL(gPSGrid, GFXPixelShader, "Grid.hlsl", "PSMain");


	UGLOBAL_SHADER_DECLIMPL2(gVSEndPass, GFXVertexShader, "EndPassDeferred.hlsl", "VSMain");
	UGLOBAL_SHADER_DECLIMPL2(gPSEndPass, GFXPixelShader, "EndPassDeferred.hlsl", "PSMain");

	UGLOBAL_SHADER_DECLIMPL2(gVSBasePassNull, GFXVertexShader, "BasePassDeferredNull.hlsl", "VSMain");
	UGLOBAL_SHADER_DECLIMPL2(gPSBasePassNull, GFXPixelShader, "BasePassDeferredNull.hlsl", "PSMain");


	//////////////////////////////////////////////////////////////////////////typedef for hlsl support
	typedef Vec2 float2;
	typedef Vec4 float4;
	typedef Matrix4 matrix, float4x4;
	typedef Vec2I uint2;

	//////////////////////////////////////////////////////////////////////////
	struct CBPerFrame
	{
		static const unsigned Index = 0;

		float4	mSunDir;
		float4	mSunColor;
		float4	mRandomColor;
		uint2	mGBufferTextureSize;
		float	mDeltaTime;
		float	mDeltaTimeReal;
		float	mSecondsSincePlay;
		float	mSecondsSincePlayReal;
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
		float2 mViewportSize;
		float2 mInvViewportSize;
		float2 mNearFarClip;
	};
	
	//////////////////////////////////////////////////////////////////////////
	struct CBPerStaticMesh
	{
		static const unsigned Index = 2;

		matrix		mLocalToWorld;
	};

	//////////////////////////////////////////////////////////////////////////
	struct CBHitSelection
	{
		static const unsigned Index = 3;

		unsigned	mHitID;
	};


	struct GridDraw
	{
		struct Vertex
		{
			Vec3		mPosition;
			Color32		mColor;
		};

		TArray<Vertex>		mLinesVertex;

		float		mGridCellSize = 10;
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
			gGFX->SetShaders(gVSGrid, nullptr, nullptr, nullptr, gPSGrid);
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
			mHitSelection = gw->mHitSelection;
			mPrimitiveBatch = gw->mWorld->GetPrimitiveBatch();
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

		UpdatePerFrameCBuffer();
		{
			gGFX->SetConstentBuffer(mCBPerFrame, CBPerFrame::Index, EShaderType::EVertex);
			gGFX->SetConstentBuffer(mCBPerFrame, CBPerFrame::Index, EShaderType::EHull);
			gGFX->SetConstentBuffer(mCBPerFrame, CBPerFrame::Index, EShaderType::EDomain);
			gGFX->SetConstentBuffer(mCBPerFrame, CBPerFrame::Index, EShaderType::EGeometry);
			gGFX->SetConstentBuffer(mCBPerFrame, CBPerFrame::Index, EShaderType::EPixel);
		}


		RenderWorld();

		if (mHitSelection)
		{
			mHitSelection->CatchAt(mGameWnd->mInputState->GetMousePosition(), mRenderTargets->mIDBufferCPURead);
		}

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

			{
				gGFX->SetConstentBuffer(mCBPerCamera, CBPerCamera::Index, EShaderType::EVertex);
				gGFX->SetConstentBuffer(mCBPerCamera, CBPerCamera::Index, EShaderType::EDomain);
				gGFX->SetConstentBuffer(mCBPerCamera, CBPerCamera::Index, EShaderType::EHull);
				gGFX->SetConstentBuffer(mCBPerCamera, CBPerCamera::Index, EShaderType::EGeometry);
				gGFX->SetConstentBuffer(mCBPerCamera, CBPerCamera::Index, EShaderType::EPixel);
			}

			GFXViewport viewport = UViewportFromCamera(camera, mViewportSize);
// 			ULOG_MESSAGE("% %", viewport.mWidth, viewport.mHeight);
			gGFX->SetViewport(viewport);

			{
				mRenderTargets->BinGBuffers(true);

				if(mGameWnd->mOptions.mRenderStaticMeshes) RenderStaticMeshes();

				if (mPrimitiveBatch) mPrimitiveBatch->Render();

				//hit selection copy resources
				if (mRenderTargets->mIDBuffer)
				{
					gGFX->CopyResource(mRenderTargets->mIDBufferCPURead, mRenderTargets->mIDBuffer);
				}

				ShaderConstantsCombined scEndPass;
				if (mGameWnd->mOptions.mVisualizeGBuffer)
					scEndPass |= ShaderConstants::VISALIZE_GBUFFER;
				gGFX->SetShaders(gVSEndPass->Get(scEndPass), nullptr, nullptr, nullptr, gPSEndPass->Get(scEndPass));

				GFXRenderTargetView* renderTargets[] = { mSwapChain->GetBackBufferView() };
				gGFX->SetRenderTarget(renderTargets, nullptr);
				gGFX->SetViewport(viewport);


				gGFX->SetBlendState(nullptr);
				gGFX->SetDepthStencilState(nullptr);
				gGFX->SetRasterizerState(nullptr);
				gGFX->SetIndexBuffer(nullptr);

				//gGFX->ClearDepthStencil(mRenderTargets->mDepthStencilView, true, true, 0.5, 0);

				//bind end passs
				GFXShaderResourceView* views[] =
				{
					mRenderTargets->mGBufferA->GetShaderResourceView(),
					mRenderTargets->mGBufferB->GetShaderResourceView(),
					mRenderTargets->mGBufferC->GetShaderResourceView(),
					mRenderTargets->mDepthReadView,
					mRenderTargets->mIDBuffer->GetShaderResourceView(),
					
				};
				gGFX->SetResourceView(views, 0, EShaderType::EPixel);


				gGFX->SetPrimitiveTopology(EPrimitiveTopology::ETriangleList);
				gGFX->Draw(3);
				
			}
			//bind swap chain and depth buffer
// 			gGFX->ClearRenderTarget(mSwapChain->GetBackBufferView(), Color::WHITE);
			GFXRenderTargetView* renderTargets[] = { mSwapChain->GetBackBufferView() };
			gGFX->SetRenderTarget(renderTargets, mRenderTargets->mDepthStencil->GetDepthStencilView());
			gGFX->SetDepthStencilState(TDepthStencilState<>::Get());
// 
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
		
		if (!mRenderTargets) mRenderTargets = new DefferdRenderTargets(UGetBiggestGameWindowSize(), true);


		if (wndSize != bufferSize)	//window size has changed
		{
			ULOG_MESSAGE("resizing render target, new size : %", mViewportSize);

			if (mRenderTargets) delete mRenderTargets;
			mRenderTargets = new DefferdRenderTargets(UGetBiggestGameWindowSize(), true);

			mSwapChain->Resize(wndSize);
			if (mCanvas) mCanvas->Resize(wndSize);
			if (mHitSelection) mHitSelection->Resize(wndSize);
			
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void Renderer::RenderStaticMeshes()
	{
		mSCStaticMeshMainPass = ShaderConstants::STATIC_MESH;
		if (mHitSelection)
		{
			mSCStaticMeshMainPass |= ShaderConstants::USE_HITSELECTION;
		}

		gGFX->SetDepthStencilState(TDepthStencilState<true, true, EComparisonFunc::ELess>::Get());
		gGFX->SetRasterizerState(TRasterizerState<false, ECullMode::EBack>::Get());
		gGFX->SetBlendState(nullptr);

		gGFX->SetConstentBuffer(mCBPerStaticMesh, CBPerStaticMesh::Index, EShaderType::EVertex);	//bind static mesh buffer
		gGFX->SetConstentBuffer(mCBHitSelection, CBHitSelection::Index, EShaderType::EPixel);		//bind hist selection

		gGFX->SetPrimitiveTopology(EPrimitiveTopology::ETriangleList);
		gGFX->SetInputLayout(mILStaticMeshVertexTypeFull);

		size_t numSMesh = mWorldRS->mStaticMeshes.Length();
		for (size_t iMesh = 0; iMesh < numSMesh; iMesh++)
		{
			EntityStaticMeshRS* entityMesh = mWorldRS->mStaticMeshes[iMesh];
			RenderStaticMesh(entityMesh);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Renderer::RenderStaticMesh(EntityStaticMeshRS* entityStaticMesh)
	{
		if (entityStaticMesh->mRSFlag.Test(ERS_RenderDataValid | ERS_Visible | ERS_MainPassEnable))
		{
			AStaticMeshRS*	meshAsset = entityStaticMesh->mMesh;

			//PerObject data
			{
				auto mapped = gGFX->Map<CBPerStaticMesh>(mCBPerStaticMesh, EMapFlag::EWriteDiscard);
				mapped->mLocalToWorld = entityStaticMesh->mWorldTransform;
				gGFX->Unmap(mCBPerStaticMesh);
			}

			//hit selection
			if (mHitSelection)
			{
				auto mapped = gGFX->Map<CBHitSelection>(mCBHitSelection, EMapFlag::EWriteDiscard);
				mapped->mHitID = mHitSelection->RegisterRTProxy(new HPEntity(entityStaticMesh->mGS));
				gGFX->Unmap(mCBHitSelection);
			}


			gGFX->SetIndexBuffer(meshAsset->mRenderData.mIndexBuffer);
			gGFX->SetVertexBuffer(meshAsset->mRenderData.mVertexBuffer, 0, sizeof(AStaticMesh::VertexTypeFull), 0);

			for (unsigned iSection = 0; iSection < meshAsset->mRenderData.mNumSelection; iSection++)
			{
				auto& section = meshAsset->mRenderData.mSelections[iSection];
				AMaterialRS* materialToUse = entityStaticMesh->mMaterials[section.mMaterialIndex];
				
				if (!materialToUse) continue;

				materialToUse->Bind();

				GFXRasterizerState* rsState = mLUTMaterialMainPassRSState[materialToUse->GetFlag()];
				gGFX->SetRasterizerState(rsState);

				gGFX->SetShader(materialToUse->GetShaderBound(mSCStaticMeshMainPass));

				gGFX->DrawIndexed(section.mIndexCount, section.mIndexOffset, section.mVertexOffset);

			}
		}
	}

	GFXDepthStencilState* Renderer::GetRasterizerForStaticMeshSolid()
	{
		return nullptr;
	}

	void Renderer::UpdatePerFrameCBuffer()
	{
		auto mapped = gGFX->Map<CBPerFrame>(mCBPerFrame, EMapFlag::EWriteDiscard);

		UASSERT(mRenderTargets && mWorldRS);

		mapped->mGBufferTextureSize = mRenderTargets->mSize;
		mapped->mRandomColor = Vec4(RandFloat01(), RandFloat01(), RandFloat01(), RandFloat01());
		
		mapped->mDeltaTime = mWorldRS->mDeltaTime;
		mapped->mDeltaTimeReal = mWorldRS->mDeltaTimeReal;
		mapped->mSecondsSincePlay = mWorldRS->mSecondsSincePlay;
		mapped->mSecondsSincePlayReal = mWorldRS->mSecondsSincePlayReal;

		gGFX->Unmap(mCBPerFrame);
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
		mapped->mViewportSize = mViewportSize;
		mapped->mInvViewportSize =  Vec2(1) / mViewportSize;
		mapped->mNearFarClip.mX = camera->mNearClip;
		mapped->mNearFarClip.mY = camera->mFarClip;
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
		mCBHitSelection = gGFX->CreateConstantBuffer(sizeof(CBHitSelection));
		UASSERT(mCBHitSelection);

		mGridDraw = new GridDraw();

		{
			GFXInputLayoutDesc  desc = 
			{
				gVSBasePassNull->Get(ShaderConstantsCombined()),
				{
					{ "POSITION", EVertexFormat::EFloat3, -1, 0, false},
					{ "NORMAL", EVertexFormat::EFloat3, -1, 0, false },
					{ "UV", EVertexFormat::EFloat2, -1, 0, false}
				}
			};
			mILStaticMeshVertexTypeFull = GlobalResources::GetInputLayout(desc);
			UASSERT(mILStaticMeshVertexTypeFull);
		}


		{
			GFXDepthStencilStateDesc desc;
			mMainPassDepthEnableState = GlobalResources::GetDepthStencilState(desc);
			UASSERT(mMainPassDepthEnableState);
		}
		{
			GFXRasterizerStateDesc desc;

			desc.mCullMode = ECullMode::EBack;
			desc.mWireframe = false;
			mMainPassRasterState = GlobalResources::GetRasterizerState(desc);
			UASSERT(mMainPassRasterState);
			
			desc.mCullMode = ECullMode::EBack;
			desc.mWireframe = true;
			mMainPassRasterStateWire = GlobalResources::GetRasterizerState(desc);
			UASSERT(mMainPassRasterStateWire);

			desc.mWireframe = false;
			desc.mCullMode = ECullMode::ENone;
			mMainPassRasterStateTwoSide = GlobalResources::GetRasterizerState(desc);
			UASSERT(mMainPassRasterStateTwoSide);

			desc.mWireframe = true;
			desc.mCullMode = ECullMode::ENone;
			mMainPassRasterStateWireTwoSide = GlobalResources::GetRasterizerState(desc);
			UASSERT(mMainPassRasterStateWireTwoSide);

		}

		{
			MemZero(mLUTMaterialMainPassRSState, sizeof(mLUTMaterialMainPassRSState));

			mLUTMaterialMainPassRSState[EMF_Default] = TRasterizerState<>::Get();
			mLUTMaterialMainPassRSState[EMF_Wireframe] = TRasterizerState<true>::Get();
			mLUTMaterialMainPassRSState[EMF_TwoSided] = TRasterizerState<false, ECullMode::ENone>::Get();
			mLUTMaterialMainPassRSState[EMF_Wireframe | EMF_TwoSided] = TRasterizerState<true, ECullMode::ENone>::Get();
		}

		{
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
