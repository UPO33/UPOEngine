#include "UWorldRS.h"
#include "UEntityStaticMesh.h"
#include "UEntityCamera.h"

namespace UPO
{
		//////////////////////////////////////////////////////////////////////////
// 		class WorldRS : IWorldRS
// 		{
// 			World*					mGS;
// 			TArray<Entity*>			mPendingAdd;
// 			TArray<EntityRS*>		mPendingDestroy;
// 	
// 			TArray<AABB>					mStaticMeshesBounds;
// 			BitArray						mStaticMeshesCullingState;
// 	
// 			/*
// 			TArray<EntityCameraRS*>			mCameras;
// 			TArray<EntityLishtRS*>			mLights;
// 			*/
// 			void Present()
// 			{
// 
// 			}
// 			void WaitForGameTickEnd()
// 			{
// 				
// 			}
// 
// 			void Frame()
// 			{
// 				Culling();
// 				Present();
// 				mGS->mTickEndEvent.Wait();	//wait for game tick to be finished, usually game tick is finished sooner so the is no wait
// 				PerformFetch();
// 				mGS->mFetchCompleted.SetSignaled();
// 				Reintegrate();
// 			}
// 			void Culling()
// 			{
// 	
// 			}
// 			void RenderStaticMesh(unsigned index)
// 			{
// 
// 			}
// 			void PerformFetch() override
// 			{
// 				UASSERT(IsRenderThread());
// 
// 				mPendingAdd = mGS->mEntitiesPendingAddToRS;
// 				mPendingDestroy = mGS->mEntitiesPendingDestroyFromRS;
// 				
// 				for (Entity* ent : mGS->mEntitiesRenderDataDirty)
// 				{
// 					if (ent->FlagTest(EEF_Alive | EEF_RenderDataDirty))
// 						ent->mRS->OnFetch();
// 				}
// 				mGS->mEntitiesRenderDataDirty.RemoveAll();
// 
// 				
// 			}
// 			void Reintegrate()
// 			{
// 				for (EntityRS* ent : mPendingDestroy)
// 				{
// 					DestroyRS(ent);
// 				}
// 				mPendingDestroy.RemoveAll();
// 
// 				for (Entity* ent : mPendingAdd)
// 				{
// 					CreateRS(ent);
// 				}
// 				mPendingAdd.RemoveAll();
// 			}
// 			void DestroyRS(EntityRS* ent)
// 			{
// // 				if (ent->mClass == Entity::GetClassInfoStatic())
// // 				{
// // 					mModles.LastElement()->mPrivateIndex = ent->mPrivateIndex;
// // 					mModles.RemoveAtSwap(ent->mPrivateIndex);
// // 				}
// 			}
// 			void CreateRS(Entity* from)
// 			{
// // 				if (from->GetClassInfo() == Entity::GetClassInfoStatic())
// // 				{
// // 					from->OnCreateRS();
// // 					mModles.Add(from->mRS);
// // 				}
// 			}
// 
// 		};

		void WorldRS::Present()
		{

		}

		void WorldRS::WaitForGameTickEnd()
		{

		}

		void WorldRS::Frame()
		{
			Culling();
			Present();
			mGS->mTickEndEvent.Wait();	//wait for game tick to be finished, usually game tick is finished sooner so the is no wait
			PerformFetch();
			mGS->mFetchCompleted.SetSignaled();
			Reintegrate();
		}

		void WorldRS::Culling()
		{

		}

		void WorldRS::RenderStaticMesh(unsigned index)
		{

		}

		void WorldRS::PerformFetch()
		{
			UASSERT(IsRenderThread());

			mPendingAdd = mGS->mEntitiesPendingAddToRS;
			mPendingDestroy = mGS->mEntitiesPendingDestroyFromRS;

			for (Entity* ent : mGS->mEntitiesRenderDataDirty)
			{
				if (ent->FlagTest(EEF_Alive | EEF_RenderDataDirty))
					ent->mRS->OnFetch();
			}
		}

		void WorldRS::CollectMeshesForRender()
		{
			
		}

		void WorldRS::Reintegrate()
		{
			for (Entity* ent : mPendingDestroy)
			{
				DestroyRS(ent);
			}
			mPendingDestroy.RemoveAll();

			for (Entity* ent : mPendingAdd)
			{
				CreateRS(ent);
			}
			mPendingAdd.RemoveAll();
		}

		void WorldRS::DestroyRS(Entity* ent)
		{
			if (auto sm = ent->Cast<EntityStaticMesh>())
			{
				
			}
			if (EntityCamera* camera = ent->Cast<EntityCamera>())
			{
			}
		}

		void WorldRS::CreateRS(Entity* from)
		{
			if (auto sm = from->Cast<EntityStaticMesh>())
			{
				return Create_EntityStaticMeshRS(sm);
			}
			if (auto camera = from->Cast<EntityCamera>())
			{
				return Create_EntityCameraRS(camera);
			}
		}

		void WorldRS::Create_EntityStaticMeshRS(EntityStaticMesh* from)
		{
			EntityStaticMeshRS* rs = new EntityStaticMeshRS(from, this);
			size_t index = mStaticMeshes.Add(rs);
			rs->mPrivateIndex = index;
		}

		void WorldRS::Create_EntityCameraRS(EntityCamera* camera)
		{

		}

};