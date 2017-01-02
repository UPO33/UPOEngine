#pragma once

#include "UWorld.h"
#include "../Core/UFrustum.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class World;
	class Entity;
	class EntityStaticMesh;
	class EntityStaticMeshRS;
	class EntityCamera;
	class EntityCameraRS;
	class GameWindow;

	//////////////////////////////////////////////////////////////////////////
	class World;
	class Renderer;

	struct RenderViewData
	{
		static const unsigned MaxActiveCamera = 32;
		
		Frustum		mFrustum[MaxActiveCamera];
		Matrix4		mProjection[MaxActiveCamera];
		Matrix4		mView[MaxActiveCamera];
		Vec4		mWorldPosition;
		Vec4		mForward;
		unsigned	mNum;
	};

	struct RenderViewDataSingle
	{
		Frustum		mFrustum;
		unsigned	mCullingMask;	//layer0, layer1, ...
	};

	class UAPI WorldRS
	{
		friend World;
		friend class EngineImpl;
		friend Renderer;

		friend EntityStaticMeshRS;
		friend EntityCameraRS;

		Flag					mFlag;
		World*					mGS;
		EntityArray*			mPendingAdd;
		EntityArray*			mPendingDestroy;

		RenderViewData	mViewData;

		struct StaticMeshBounData
		{
			unsigned	mFlag;
			unsigned	mLayer;	//the layers of the entity
			AABB		mBound;
		};
		

		TArray<AABB>		mStaticMeshesBounds;
		TArray<unsigned>	mStaticMeshesCullingState;

		TArray<EntityStaticMeshRS*>		mStaticMeshes;
		TArray<EntityCameraRS*>			mCameras;
		
		EntityCameraRS*					mMainCamera;

	public:
		Renderer*		mRenderer = nullptr;
		GameWindow*		mMainWindow = nullptr;
		bool			mIsFetching = false;
		/*
		TArray<EntityCameraRS*>			mCameras;
		TArray<EntityLishtRS*>			mLights;
		*/
		WorldRS(World* world);
		~WorldRS();

		void Render();
		void Fetch();
		void AfterFetch();
		void Frame();

	private:
		void Present();
		void WaitForGameTickEnd();
		void Culling();
		void RenderStaticMesh(unsigned index);

		void RemoveRS(Entity* ent);
		void AddToScene(Entity* from);


		void PerformFetch();
		void CollectMeshesForRender();

	};


};