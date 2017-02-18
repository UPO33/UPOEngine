#pragma once


#include "../GFXCore/UGFXCore.h"

#include "../Engine/UGameWindow.h"
#include "../Engine/UWorldRS.h"
#include "../Engine/UWorld.h"
#include "../Engine/UCanvas.h"
#include "../Engine/UMaterial.h"

#include "UDefferdRenderTargets.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class World;
	class WorldRS;


	//////////////////////////////////////////////////////////////////////////
	class GridDraw;

	//////////////////////////////////////////////////////////////////////////
	class UAPI Renderer
	{
	public:
		GameWindow*				mGameWnd;
		GFXSwapChain*			mSwapChain;
		WorldRS*				mWorldRS;
		Canvas*					mCanvas;
		PrimitiveBatch*			mPrimitiveBatch;
		HitSelectionCanvas*		mHitSelection;
		DefferdRenderTargets*	mRenderTargets;
		
		Vec2 mViewportSize;
		
		GFXVertexBuffer* mTestTriVBuffer;
		GFXInputLayout* mTestTriLayout;
		GFXConstantBuffer* mTestCBuffer;

		TArray<EntityCameraRS*>	mCamerasToRender;
		GFXConstantBuffer*		mCBPerFrame;
		GFXConstantBuffer*		mCBPerCamera;
		GFXConstantBuffer*		mCBPerStaticMesh;
		GFXConstantBuffer*		mCBHitSelection;

		int						mCurRenderingCameraIndex = -1;

		GridDraw*		mGridDraw = nullptr;

		GFXInputLayout*				mILStaticMeshVertexTypeFull;
		GFXDepthStencilState*		mMainPassDepthEnableState;
		GFXRasterizerState*			mMainPassRasterState;
		GFXRasterizerState*			mMainPassRasterStateWire;
		GFXRasterizerState*			mMainPassRasterStateTwoSide;
		GFXRasterizerState*			mMainPassRasterStateWireTwoSide;

		GFXRasterizerState*			mLUTMaterialMainPassRSState[EMaterialFlag::EMF_Max];
		ShaderConstantsCombined		mSCStaticMeshMainPass;

		Renderer();
		~Renderer();


		virtual void RenderGameWin(GameWindow*);

		void Render();
		void RenderWorld();
		void CheckRenderTargetResizing();

		void RenderStaticMeshes();
		void RenderStaticMesh(EntityStaticMeshRS*);

		GFXDepthStencilState* GetRasterizerForStaticMeshSolid();

		void UpdatePerFrameCBuffer();
		void UpdatePerCameraCBuffer(EntityCameraRS*);
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