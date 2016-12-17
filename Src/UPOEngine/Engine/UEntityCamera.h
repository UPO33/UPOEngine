#pragma once

#include "UEntity.h"

namespace UPO
{
	enum class ECameraProjection
	{
		Perspective, Orthographic
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI EntityCamera : public Entity
	{
		UCLASS(EntityCamera, Entity)
	public:
		ECameraProjection	mProjection;
		float				mFieldOfView;
		float				mNearClip;
		float				mFarClip;
		float				mOrthoWidth;
		float				mOrthoHeight;
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI EntityCameraRS
	{
		Matrix4		mProj;
		Matrix4		mView;

		ECameraProjection	mProjection;
		float				mFieldOfView;
		float				mNearClip;
		float				mFarClip;
		float				mOrthoWidth;
		float				mOrthoHeight;
	};
};