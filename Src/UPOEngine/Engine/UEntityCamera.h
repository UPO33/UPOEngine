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

		EntityCamera();
		~EntityCamera();
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI EntityCameraRS : public EntityRS
	{
	public:
		EntityCameraRS(EntityCamera* from, WorldRS* wrs);
		~EntityCameraRS();

		Matrix4		mProj;
		Matrix4		mView;
		
		ECameraProjection	mProjection;
		float				mFieldOfView;
		float				mNearClip;
		float				mFarClip;
		float				mOrthoWidth;
		float				mOrthoHeight;
		WorldRS*			mWorldRS;
		EntityCamera*		mGS;


		void OnFetch() override;
	};
};