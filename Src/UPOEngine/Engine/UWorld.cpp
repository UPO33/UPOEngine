#include "UWorld.h"
#include "UComponent.h"
#include "UEntity.h"

#include "../Meta/UMeta.h"

namespace UPO
{

	//////////////////////////////////////////////////////////////////////////
	Entity* World::CreateEntity(EntityCreationParam& param)
	{
		if (param.mClass && param.mClass->IsBaseOf(Entity::GetClassInfoStatic()))
		{
			Entity* newEntity = NewObject<Entity>(param.mClass);
			newEntity->Init(param.mParent, this);

			newEntity->OnConstruct();
			
			if (mIsPlaying)
			{
				newEntity->FlagSet(EEF_BeginPlayWasCalled);
				newEntity->OnBeginPlay();
			}
			return newEntity;
		}
		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	void World::SingleTick(WorldTickResult& result)
	{
		mIsInTick = true;
		mCurTickResult = WorldTickResult();
		ChronometerAccurate frameTimer;
		frameTimer.Start();

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
		}
		mFetchCompleted.Wait();
		
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

		CheckPendingKills();
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


	void World::Release()
	{
		mTimer.StopAll();
		
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
		
	}
	//////////////////////////////////////////////////////////////////////////
	World::~World()
	{

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

	


};