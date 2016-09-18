#pragma once

#include "UGFXDeviceDX.h"


namespace UPO
{
	//////////////////////////////////////////////////////////////////////////fwd decl
	class SMesh;
	class EntitySMesh;
	class EntityCamera;
	class PrimitiveDrawer;
	class ScreenDrawer;
	class MeshDrawer;


	//////////////////////////////////////////////////////////////////////////
	struct alignas(16) CBPerFrame
	{
		Matrix mProj;
		Matrix mView;
		Matrix mViewProj;
		Vector3 mCameraForward;
		float  mTime;
		Vector3 mCameraPosition;
		Vector3 mAmbientColor;
		Vector3 mSunColor;
		Vector3 mSunDir;
		float	mPadding0;
	};

	//////////////////////////////////////////////////////////////////////////
	struct alignas(16) CBPerObject
	{
		Matrix mWorld;
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI Renderer
	{
	public:

		unsigned			mRenderTargetW = 0;
		unsigned			mRenderTargetH = 0;

		GFXTexture2D*		mDepthBuffer = nullptr;
		Color				mClearColor = Color(1, 1, 0, 0);
		FPSCounter			mFPS;

		EntityCamera*		mCamera = nullptr;
		EntitySMesh*		mMeshBuilding = nullptr;
		SMesh*				mSMBuilding = nullptr;
		SMesh*				mSMeshCube = nullptr;

		GFXConstantBuffer*	mCBPerFrame = nullptr;
		GFXConstantBuffer*	mCBPerObject = nullptr;
		Vector3				mAmbientColor = Vector3(0.1f);
		Vector3				mSunColor = Vector3(1);
		Vector3				mSunDir = Vector3(-1, -1, 0);
		GFXShader*			mShaderSMeshVS = nullptr;
		GFXShader*			mShaderSMeshPS = nullptr;

		TimeCounterHigh		mTimer;


		GFXDepthStencilState*	mDepthEnableState = nullptr;
		GFXRasterizerState*		mRasterStateMeshFill = nullptr;
		GFXRasterizerState*		mRasterStateMeshWire = nullptr;
		GFXInputLayout*			mInputLayoutMesh = nullptr;

		PrimitiveDrawer*		mPrimitiveDrawer = nullptr;
		ScreenDrawer*			mScreenDrawer = nullptr;
		MeshDrawer*				mMeshDrawer = nullptr;

		PrimitiveDrawer*		GetPrimitiveDrawer() const { return mPrimitiveDrawer; }
		ScreenDrawer* 			GetScreenDrawer() const { return mScreenDrawer; }
		MeshDrawer*				GetMeshDrawer() const { return mMeshDrawer; }

		Renderer(unsigned w, unsigned h) : mRenderTargetW(w), mRenderTargetH(h)
		{}

		float GetAspectRatio() const
		{
			return ((float)mRenderTargetW / (float)mRenderTargetH);
		}
		void Init();
		void Shutdown();
		void Tick();

		void BeginScene();
		void RenderScene();
		void EndScene();

		void RenderBasePass();
	};

};