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


	//////////////////////////////////////////////////////////////////////////
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
		
	public:
		Renderer*		mRenderer = nullptr;
		GameWindow*		mMainWindow = nullptr;
		EntityCameraRS*	mFreeCamera = nullptr;
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
		void AddRS(Entity* from);

		void GetDesiredCameras(TArray<EntityCameraRS*>& out);

	};


};