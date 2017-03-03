#include "UEntityCamera.h"
#include "../Meta/UMeta.h"
#include "UWorldRS.h"
#include "UGameWindow.h"
#include "UEntityCameraVisualizer.h"

namespace UPO
{
	  

	UCLASS_BEGIN_IMPL(EntityCamera, UATTR_Icon("EntityCamera.png"), UATTR_Instanceable())
		UPROPERTY(mRender)
		UPROPERTY(mPerspective)
		UPROPERTY(mFieldOfView, UATTR_Range(1, 180))
		UPROPERTY(mNearClip, UATTR_Range(0.001f, 9999999.0f))
		UPROPERTY(mFarClip, UATTR_Range(0.02f, 9999999.0f))
		UPROPERTY(mOrthoSize)
		UPROPERTY(mViewportOffset, UATTR_Range(0,1))
		UPROPERTY(mViewPortSize, UATTR_Range(0,1))
		UPROPERTY(mSuperiority, UATTR_Range(-1000, 1000))
	UCLASS_END_IMPL(EntityCamera);



	EntityCamera::EntityCamera()
	{
		mRender = true;
		mPerspective = true;
		mFieldOfView = 70;
		mNearClip = 0.01f;
		mFarClip = 10000.0f;
		mOrthoSize = Vec2(100, 100);
		mViewportOffset = Vec2(0, 0);
		mViewPortSize = Vec2(1, 1);
		mSuperiority = 0;
#ifndef UENDUSER
		mVisualizer = new EntityCameraVisualizer(this);
#endif
	}


	EntityCamera::~EntityCamera()
	{

	}


	//initialize without adding to world rs
	EntityCameraRS::EntityCameraRS()
	{
		mGS = nullptr;
		mPrivateIndex = InvalidIndex;
		mRSFlag = 0;

	}
	//initialize and add to WorldRS
	EntityCameraRS::EntityCameraRS(EntityCamera* from, WorldRS* wrs)
	{
		mGS = from;
		mOwner = wrs;

		mPrivateIndex = mOwner->mCameras.Add(this);

		Fetch();

		mGS->mRS = this;
	}
	//remove from WorldRS
	EntityCameraRS::~EntityCameraRS()
	{
		if(mPrivateIndex != InvalidIndex)
		{
			mOwner->mCameras.LastElement()->mPrivateIndex = mPrivateIndex;
			mOwner->mCameras.RemoveAtSwap(mPrivateIndex);
		}
	}

	bool EntityCameraRS::ShouldBeReanderd() const
	{
		return mRender;
	}

	void EntityCameraRS::GetFrustum(Frustum& out)
	{

	}

	void EntityCameraRS::Fetch()
	{
		EntityCamera* gs = (EntityCamera*)mGS;
		FetchFrom(gs);
	}

	void EntityCameraRS::FetchFrom(EntityCamera* gs)
	{
		mRender = gs->mRender;
		mPerspective = gs->mPerspective;
		mFieldOfView = gs->mFieldOfView;
		mNearClip = gs->mNearClip;
		mFarClip = gs->mFarClip;
		mOrthoSize = gs->mOrthoSize;
		mViewportOffset = gs->mViewportOffset;
		mViewPortSize = gs->mViewPortSize;
		mSuperiority = gs->mSuperiority;


		mMatrixViewInv = gs->GetWorldTransform();
		mMatrixViewInv.RemoveScaling();
	}

	void EntityCameraRS::Bake()
	{
		if (mPerspective)
		{
			float aspect = 1;
			if (mOwner->mMainWindow)
			{
				Vec2I wndSize = mOwner->mMainWindow->GetWinSize();
				Vec2 vv = mViewPortSize * Vec2(wndSize.mX, wndSize.mY);
				aspect = vv.mX / vv.mY;
			}
			
			mMatrixProj.MakePerspective(mFieldOfView, aspect, mNearClip, mFarClip);
		}
		else
		{
			mMatrixProj.MakeOrtho(mOrthoSize.mX, mOrthoSize.mY, mNearClip, mFarClip);
		}

		mMatrixView = mMatrixViewInv.GetInverse();
		mMatrixProjInv = mMatrixProj.GetInverse();
		mMatrixWorldToClip = mMatrixView * mMatrixProj;
		mMatrixClipToWorld = mMatrixWorldToClip.GetInverse();
	}



	void EntityCameraRS::OnFetch(unsigned flag)
	{
	}

};