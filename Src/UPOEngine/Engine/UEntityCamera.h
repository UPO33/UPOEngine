#pragma once

#include "UEntity.h"
#include "../Core/UFrustum.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	struct Frustum;
	class WorldEditorData;

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
		Vec2				mViewPortSize;
		int					mSuperiority;	//cameras are rendered from less superiority
		

		EntityCamera();
		~EntityCamera();

	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI EntityCameraRS : public EntityRS
	{
	public:
		EntityCameraRS();
		EntityCameraRS(EntityCamera* from, WorldRS* wrs);
		~EntityCameraRS();

		Matrix4		mMatrixProj;
		Matrix4		mMatrixProjInv;
		Matrix4		mMatrixView;
		Matrix4		mMatrixViewInv;
		Matrix4		mMatrixWorldToClip;
		Matrix4		mMatrixClipToWorld;
		Frustum		mFrustum;

		bool				mRender;
		bool				mPerspective;
		float				mFieldOfView;
		float				mNearClip;
		float				mFarClip;
		Vec2				mOrthoSize;
		Vec2				mViewportOffset;
		Vec2				mViewPortSize;
		int					mSuperiority;


		//cameras fetch is performed every frame without considering dirty flags
		void Fetch();
		void FetchFrom(EntityCamera*);

		EntityCamera* GS() const { return (EntityCamera*)mOwner; }

		bool ShouldBeReanderd() const;
		void GetFrustum(Frustum& out);
		//make matrices and ... from existing data 
		void Bake();

		void OnFetch(unsigned flag) override;
	};


};