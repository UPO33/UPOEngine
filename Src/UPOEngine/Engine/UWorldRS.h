#pragma once

#include "../Core/UCore.h"

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

	class UAPI WorldRS
	{
		friend World;
		friend class EngineImpl;
		friend Renderer;

		Flag					mFlag;
		World*					mGS;
		TArray<Entity*>			mPendingAdd;
		TArray<Entity*>			mPendingDestroy;

		TArray<AABB>					mStaticMeshesBounds;
		BitArray						mStaticMeshesCullingState;

		TArray<EntityStaticMeshRS*>		mStaticMeshes;
		TArray<EntityCameraRS*>			mCameras;

		EntityCameraRS*					mMainCamera;

	public:
		Renderer*		mRenderer = nullptr;
		GameWindow*		mMainWindow = nullptr;
		GFXSwapChain*	mMainWndSwapChain = nullptr;

		/*
		TArray<EntityCameraRS*>			mCameras;
		TArray<EntityLishtRS*>			mLights;
		*/
		WorldRS();
		~WorldRS();


		void Frame();
	private:
		void Present();
		void WaitForGameTickEnd();
		void Culling();
		void RenderStaticMesh(unsigned index);
		void Reintegrate();
		void DestroyRS(Entity* ent);
		void CreateRS(Entity* from);
		void Create_EntityStaticMeshRS(EntityStaticMesh* from);
		void Create_EntityCameraRS(EntityCamera* camera);
		void PerformFetch();
		void CollectMeshesForRender();
	};

};