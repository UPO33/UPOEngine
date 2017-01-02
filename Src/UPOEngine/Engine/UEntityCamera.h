#pragma once

#include "UEntity.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	struct Frustum;

	//////////////////////////////////////////////////////////////////////////
	class UAPI EntityCamera : public Entity
	{
		UCLASS(EntityCamera, Entity)
	public:
		bool				mRender;
		bool				mPerspective;
		float				mFieldOfView;
		float				mNearClip;
		float				mFarClip;
		Vec2				mOrthoSize;
		Vec2				mViewportOffset;
		Vec2				mViewPortScale;
		int					mSuperiority;	//cameras are rendered from less superiority
		

		EntityCamera();
		~EntityCamera();

	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI EntityCameraRS : public EntityRS
	{
	public:
		EntityCameraRS(EntityCamera* from, WorldRS* wrs);
		~EntityCameraRS();

		Matrix4		mProjMatrix;
		Matrix4		mViewMatrix;
		Matrix4		mWorldMatrix;

		bool				mRender;
		bool				mPerspective;
		float				mFieldOfView;
		float				mNearClip;
		float				mFarClip;
		Vec2				mOrthoSize;
		Vec2				mViewportOffset;
		Vec2				mViewPortScale;
		int					mSuperiority;

		unsigned			mPrivateIndex;
		EntityCamera*		mGS;
		WorldRS*			mOwner;

		bool ShouldBeReanderd() const;
		void GetFrustum(Frustum& out);
		void OnFetch();
		void Update();
	};



};