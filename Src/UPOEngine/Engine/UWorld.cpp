#include "UWorld.h"
#include "UComponent.h"
#include "UEntity.h"
#include "UEntityCamera.h"

#include "UWorldRS.h"

#include "../Meta/UMeta.h"

namespace UPO
{

	//////////////////////////////////////////////////////////////////////////
	Entity* World::CreateEntity(EntityCreationParam& param)
	{
		ClassInfo* entityClass = param.mClass;
		if (!entityClass->IsBaseOf(Entity::GetClassInfoStatic()) || entityClass->IsAbstract())
		{
			ULOG_ERROR("invalid class!");
			return nullptr;
		}

		Entity* newEntity = NewObject<Entity>(entityClass);
		newEntity->Init(param.mParent, this);

		newEntity->OnConstruct();

		if (mIsPlaying)
		{
			newEntity->FlagSet(EEF_BeginPlayWasCalled);
			newEntity->OnBeginPlay();
		}

		PushToPendingAddToRS(newEntity);

		mOnEntityCreated.InvokeAll(newEntity);

		return newEntity;
	}
	//////////////////////////////////////////////////////////////////////////
	void World::SingleTick(WorldTickResult& result)
	{
		mIsInTick = true;
		mCurTickResult = WorldTickResult();
		ChronometerAccurate frameTimer;
		frameTimer.Start();

		CheckPendingKills();

		if (mIsPlaying)
		{
			///////////begin playing
			if (mDoBeginPlay)
			{
				PerformBeginPlay();
				mDoBeginPlay = false;
			}

			PerformTick();

		}
		else
		{
			if (mDoEndPlay)
			{
				PerformEndPlay();
				mDoEndPlay = false;
			}
		}


		mCurTickResult.mConsumedTimeMS = frameTimer.SinceMiliseconds();
		mIsInTick = false;

		mNumTickSinceLastDestroy++;

		mTickEndEvent.SetSignaled();
		//we can do anything that doesn't change entities render-related properties such as audio and ..
		{
			//during here render thread is performing fetch
		}
		mFetchCompleted.Wait();

		mEntitiesPendingAddToRS.RemoveAll();
		mEntitiesPendingDestroyFromRS.RemoveAll();
		mEntitiesRenderDataDirty.RemoveAll();
		
	}
	//////////////////////////////////////////////////////////////////////////
	void World::Tick(float delta)
	{
		mDeltaTime = delta * mDeltaScale;

		CheckPendingKills();

		mIsInTick = true;

		if (mIsPlaying)
		{
			///////////begin playing
			if (mDoBeginPlay)
			{
				PerformBeginPlay();
				mDoBeginPlay = false;
			}

			mTimer.Tick(mDeltaTime);
			mTicking.Tick(mDeltaTime);

			mSecondsSincePlay += mDeltaTime;
		}
		else
		{
			if (mDoEndPlay)
			{
				PerformEndPlay();
				mDoEndPlay = false;
				mSecondsSincePlay = 0;
			}
		}

		mIsInTick = false;

		mNumTickSinceLastDestroy++;

		mTickEndEvent.SetSignaled();
		//we can do anything that doesn't change entities render-related properties such as audio and ..
		{
			//during here render thread is performing fetch
		}
		if(mRS && mIsPlaying)
		{
			mFetchCompleted.Wait();

			mEntitiesPendingAddToRS.RemoveAll();
			mEntitiesPendingDestroyFromRS.RemoveAll();
			mEntitiesRenderDataDirty.RemoveAll();
		}
	}

	void World::PerformBeginPlay()
	{
		size_t len = mEntities.Length();
		for (size_t i = 0; i < len; i++)
		{
			Entity* ent = mEntities[i];
			if (ent && ent->FlagTest(EEF_Alive))
			{
				if (!ent->FlagTest(EEF_BeginPlayWasCalled))
				{
					ent->FlagSet(EEF_BeginPlayWasCalled);
					ent->OnBeginPlay();
				}
			}
		}
	}

	void World::PerformEndPlay()
	{
		size_t len = mEntities.Length();
		for (size_t i = 0; i < len; i++)
		{
			Entity* ent = mEntities[i];
			if (ent && ent->FlagTest(EEF_Alive))
			{
				if (ent->FlagTest(EEF_BeginPlayWasCalled))
				{
					ent->FlagClear(EEF_BeginPlayWasCalled);
					ent->OnEndPlay(EEndPlayReason::EPR_EndPlay);
				}
			}
		}
	}

	void World::PerformTick()
	{
		mTimer.Tick(mDeltaTime);
		mTicking.Tick(mDeltaTime);
	}

	void World::KillDestroyedEntities()
	{
		if(mNumTickSinceLastDestroy > 60 && mNumDestroyedEntity)
		{
			mEntities.RemoveIf([&](Entity* ent) {
				if (ent->FlagTest(EEF_Alive))
				{
					return false;
				}
				else
				{
					DeleteObject(ent);
					return true;
				}
			});
			

			

			mNumDestroyedEntity = 0;
			mNumTickSinceLastDestroy = 0;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void World::PushToLimbo(Entity* deadEntity)
	{
// 		mNumTickSinceLastDestroy = 0;
// 		mNumDestroyedEntity++;
// 		mEntitiesPendingKill.Add(deadEntity->mIndexInWorld);
// 
// 		Entity* entity = nullptr;
// 		if (entity->IsReadyToDie())
// 		{
// 			DeleteObject(entity);
// 		}
	}


	Entity* World::GetRootEntity()
	{
		return mRootEntity;
	}

	void World::Release()
	{
		mTimer.StopAll();
		
	}

	WorldRS* World::CreateRS()
	{
// 		UASSERT(IsGameThread());

		WorldRS* rs = new WorldRS;
		rs->mGS = this;

		return nullptr;
	}

	World* World::Duplicate()
	{
		return new World;
	}

	void World::PushToPendingAddToRS(Entity* ent)
	{
		mEntitiesPendingAddToRS.Add(ent);
	}

	void World::PushToPendingDestroyFromRS(Entity* ent)
	{
		mEntitiesPendingDestroyFromRS.Add(ent);
	}

	void World::Intersection()
	{
		USCOPE_LOCK(mIntersection);

		if (IsRenderThread())
		{
			mTickEndEvent.Wait();
			mRS->PerformFetch();
			mFetchCompleted.SetSignaled();
		}
		else
		{
			UASSERT(IsGameThread());

			mFetchCompleted.Wait();
		}
	}

	void World::KillEntity(Entity* entity)
	{
		ULOG_MESSAGE("kiling entity [%s]", entity->mName.CStr());
		DeleteObject(entity);
	}

	void World::CheckPendingKills()
	{
		int	numPendingKill = (int)mEntitiesPendingKill.Length();
		int index = 0;
		while (index < numPendingKill)
		{
			Entity* entity = mEntitiesPendingKill[index];
			if (entity && entity->IsReadyToDie())
			{
				//remove from entities list
				mEntities.LastElement()->mIndexInWorld = entity->mIndexInWorld;
				mEntities.RemoveAtSwap(entity->mIndexInWorld);
				//remove from pending kill list
				mEntitiesPendingKill.RemoveAtSwap(index);
				KillEntity(entity);
				numPendingKill--;
			}
			else
			{
				index++;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	World::World() : 
		mTickEndEvent(false, false),
		mFetchCompleted(false, false)
	{
		mRootEntity = NewObject<Entity>();
		mRootEntity->mWorld = this;
		mRootEntity->mIndexInWorld = 0;
		mRootEntity->mName = "Root";

// 		Event eventRSCreation(false, false);
// 		EnqueueRenderCommend([this, &eventRSCreation]() {
// 
// 			WorldRS* rs = new WorldRS;
// 			rs->mGS = this;
// 			this->mRS = rs;
// // 			eventRSCreation.SetSignaled();
// 		});
// // 		eventRSCreation.Wait();

		{
			mEditorCamera =  CreateEntity<EntityCamera>(nullptr);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	World::~World()
	{
		DeleteObject(mRootEntity);

		WorldRS* rs = mRS;
		EnqueueRenderCommend([rs]()
		{
			if (rs) delete rs;
		});
	}
	void World::SetPlaying()
	{
		UASSERT(!mIsInTick);
		if (mIsPlaying) return;

		mIsPlaying = true;
		mDoBeginPlay = true;
	}

	void World::PausePlaying()
	{
		UASSERT(!mIsInTick);
		if (mIsPlaying && !mIsPaused)
		{
			mIsPaused = true;
		}
	}

	void World::ResumePlaying()
	{
		UASSERT(!mIsInTick);
		if (mIsPlaying && mIsPaused)
		{
			mIsPaused = false;
		}
	}

	void World::StopPlaying()
	{
		UASSERT(!mIsInTick);
		if (!mIsPlaying) return;

		mIsPlaying = false;
		mDoEndPlay = true;
	}

	void World::PauseGame()
	{

	}

	void World::ResumeGame()
	{

	}

	

	UCLASS_BEGIN_IMPL(World)
	UCLASS_END_IMPL(World)
};