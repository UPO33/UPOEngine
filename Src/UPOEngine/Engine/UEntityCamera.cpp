#include "UEntityCamera.h"
#include "../Meta/UMeta.h"
#include "UWorldRS.h"

namespace UPO
{
	  

	UCLASS_BEGIN_IMPL(EntityCamera)
		UPROPERTY(mRender)
		UPROPERTY(mPerspective)
		UPROPERTY(mFieldOfView, UATTR_Range(1, 180))
		UPROPERTY(mNearClip, UATTR_Range(0.001f, 9999999.0f))
		UPROPERTY(mFarClip, UATTR_Range(0.02f, 9999999.0f))
		UPROPERTY(mOrthoSize)
		UPROPERTY(mViewportOffset, UATTR_Range(0,1))
		UPROPERTY(mViewPortScale, UATTR_Range(0,1))
		UPROPERTY(mSuperiority, UATTR_Range(-1000, 1000))
	UCLASS_END_IMPL(EntityCamera);

	void EntityCamera::OnCreateRS()
	{
		new  (GetRSMemory()) EntityCameraRS(this, nullptr);
	}

	EntityCamera::EntityCamera()
	{
		mRender = true;
		mPerspective = true;
		mFieldOfView = 70;
		mNearClip = 0.01f;
		mFarClip = 10000.0f;
		mOrthoSize = Vec2(100, 100);
		mViewportOffset = Vec2(0, 0);
		mViewPortScale = Vec2(1, 1);
		mSuperiority = 0;
	}


	EntityCamera::~EntityCamera()
	{

	}



	EntityCameraRS::EntityCameraRS(EntityCamera* from, WorldRS* wrs)
	{
		ZeroType(*this);

		mGS = from;
		mOwner = wrs;

		mPrivateIndex = mOwner->mCameras.Add(this);

		OnFetch();
	}

	EntityCameraRS::~EntityCameraRS()
	{
		mOwner->mCameras.RemoveAtSwap(mPrivateIndex);
	}

	bool EntityCameraRS::ShouldBeReanderd() const
	{

	}

	void EntityCameraRS::GetFrustum(Frustum& out)
	{

	}

	void EntityCameraRS::OnFetch()
	{
		mRender = mGS->mRender;
		mPerspective = mGS->mPerspective;
		mFieldOfView = mGS->mFieldOfView;
		mNearClip = mGS->mNearClip;
		mFarClip = mGS->mFarClip;
		mOrthoSize = mGS->mOrthoSize;
		mViewportOffset = mGS->mViewportOffset;
		mViewPortScale = mGS->mViewPortScale;
		mSuperiority = mGS->mSuperiority;

		mWorldMatrix = mGS->GetWorldTransform();
	}

	void EntityCameraRS::Update()
	{
		if (mPerspective)
		{
			mProjMatrix.MakePerspective(mFieldOfView, 1, mNearClip, mFarClip);
		}
		else
		{
			mProjMatrix.MakeOrtho(mOrthoSize.mX, mOrthoSize.mY, mNearClip, mFarClip);
		}

		mViewMatrix = mWorldMatrix;
		mViewMatrix.RemoveScaling();
		mViewMatrix.InvertGeneral();
	}

};