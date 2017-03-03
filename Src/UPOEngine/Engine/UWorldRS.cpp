#include "UWorldRS.h"

#include "UWorld.h"
#include "UEntityStaticMesh.h"
#include "UEntityCamera.h"
#include "UEntityLight.h"

#include "../GFX/URenderer.h"

namespace UPO
{

	//////////////////////////////////////////////////////////////////////////
	WorldRS::WorldRS(World* owner)
	{
		MemZero(this, sizeof(this));

		mGS = owner;

		mPendingAdd = new EntityArray;
		mPendingDestroy = new EntityArray;

		mFreeCamera = nullptr;
		if (owner->mFreeCamera)
		{
			mFreeCamera = new (MemNew<EntityCameraRS>()) EntityCameraRS();
			owner->mFreeCamera->mRS = mFreeCamera;
			mFreeCamera->mGS = owner->mFreeCamera;
			mFreeCamera->mOwner = this;
			mFreeCamera->Fetch();
		}

		//fetching times
		{
			mDeltaTime = mGS->mDeltaTime;
			mDeltaTimeReal = mGS->mDeltaTimeReal;
			mSecondsSincePlay = mGS->mSecondsSincePlay;
			mSecondsSincePlayReal = mGS->mSecondsSincePlayReal;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	WorldRS::~WorldRS()
	{
		if (mFreeCamera) MemDelete(mFreeCamera);
		mFreeCamera = nullptr;

	}

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


		}

		void WorldRS::Culling()
		{
			//static mesh frustum cull
// 			{
// 				unsigned numStaticMesh = mStaticMeshesBounds.Length();
// 				if (numStaticMesh)
// 				{
// 					MemZero(mStaticMeshesCullingState.Elements(), sizeof(unsigned) * numStaticMesh);
// 
// 					for (unsigned iStaticMesh = 0; iStaticMesh < numStaticMesh; iStaticMesh++)
// 					{
// 						for (unsigned iView = 0; iView < mViewData.mNum; iView++)
// 						{
// 							if (mViewData[iView].mFrustum.IsInside(mStaticMeshesBounds[iStaticMesh]))
// 							{
// 								mStaticMeshesCullingState[iStaticMesh] |= iView;
// 							}
// 						}
// 					}
// 				}
// 			}
		}

		void WorldRS::RenderStaticMesh(unsigned index)
		{

		}




		void WorldRS::Render()
		{
			Culling();
			Present();
		}

		void WorldRS::Fetch()
		{
			mIsFetching = true;

			//fetching times
			{
				mDeltaTime = mGS->mDeltaTime;
				mDeltaTimeReal = mGS->mDeltaTimeReal;
				mSecondsSincePlay = mGS->mSecondsSincePlay;
				mSecondsSincePlayReal = mGS->mSecondsSincePlayReal;
			}

			//swaping
			{
				auto tmp0 = mGS->mEntitiesPendingAddToRS;
				auto tmp1 = mGS->mEntitiesPendingDestroyFromRS;

				mGS->mEntitiesPendingAddToRS = mPendingAdd;
				mGS->mEntitiesPendingDestroyFromRS = mPendingDestroy;

				mPendingAdd = tmp0;
				mPendingDestroy = tmp1;
			}

			//adding pending entities
			{
				for (Entity* ent : *mPendingAdd)
				{
					AddRS(ent);
				}
				mPendingAdd->RemoveAll();
			}

			//fetching dirty entities
			{
				for (Entity* ent : mGS->mEntitiesRenderDataDirty)
				{
					unsigned entityFlag = ent->mEntityFlag;
					//remove render dirty flags from entity
					ent->mEntityFlag.Clear(EEF_RenderDataDirty | EEF_RenderDataTransformDirty | EEF_RenderDataMiscDirty);
					ent->GetRS()->OnFetch(entityFlag);

				}
				mGS->mEntitiesRenderDataDirty.RemoveAll();
			}



			//fetching cameras are done every frame
			{
				for (EntityCameraRS* camera : mCameras)
				{
					camera->Fetch();
				}
			}

			if (mFreeCamera) mFreeCamera->Fetch();
				

			mIsFetching = false;
		}



		void WorldRS::RemoveRS(Entity* ent)
		{
			ULOG_MESSAGE("removing RS %", ent->GetName());

			ent->GetRS()->~EntityRS();
			
		}

		void WorldRS::AddRS(Entity* from)
		{
			if (auto sm = from->Cast<EntityStaticMesh>())
			{
				new (sm->GetRSMemory()) EntityStaticMeshRS(sm, this);
				return;
			}
			if (auto camera = from->Cast<EntityCamera>())
			{
				new (camera->GetRSMemory()) EntityCameraRS(camera, this);
				return;
			}
			if (auto light = from->Cast<EntityLight>())
			{
				new (light->GetRSMemory()) EntityLightRS(light, this);
				return;
			}
		}




		//get the cameras that we should render
		void WorldRS::GetDesiredCameras(TArray<EntityCameraRS*>& out)
		{
			if (mGS->GetType() == EWorldType::EEditor)
			{
				if(mFreeCamera) out.Add(mFreeCamera);
			}
			else
			{
				//extract renderable cameras
				for (EntityCameraRS* camera : mCameras)
				{
					if (camera->mRender)
					{
						camera->Bake();
					}
				}
				//sort by less superiority
				out.BubbleSort([](EntityCameraRS* a, EntityCameraRS* b) { return a->mSuperiority < b->mSuperiority; });
			}
		}

		void WorldRS::AfterFetch()
		{
			//remove  rs of destroyed entities
			{
				for (Entity* ent : *mPendingDestroy)
				{
					RemoveRS(ent);
				}
				mPendingDestroy->RemoveAll();
			}

			
			for (EntityCameraRS* camera : mCameras)
				camera->Bake();

			if (mFreeCamera) mFreeCamera->Bake();

		}
};