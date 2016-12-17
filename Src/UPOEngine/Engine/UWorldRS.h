#pragma once

#include "UWorld.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class EntityStaticMesh;
	class EntityStaticMeshRS;
	class EntityCamera;
	class EntityCameraRS;

	//////////////////////////////////////////////////////////////////////////
	class World;

	class WorldRS
	{
		friend World;

		Flag					mFlag;
		World*					mGS;
		TArray<Entity*>			mPendingAdd;
		TArray<Entity*>			mPendingDestroy;

		TArray<AABB>					mStaticMeshesBounds;
		BitArray						mStaticMeshesCullingState;

		TArray<EntityStaticMeshRS*>		mStaticMeshes;

	public:
		/*
		TArray<EntityCameraRS*>			mCameras;
		TArray<EntityLishtRS*>			mLights;
		*/
		void Present();
		void WaitForGameTickEnd();

		void Frame();
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