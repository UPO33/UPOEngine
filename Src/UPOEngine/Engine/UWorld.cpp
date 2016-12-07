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
		TimeCounterHigh frameTimer;
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
		KillDestroyedEntities();

		mCurTickResult.mConsumedTimeMS = frameTimer.SinceMiliseconds();
		mIsInTick = false;

		mNumTickSinceLastDestroy++;
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
			
			mRootEntities.RemoveIf([&](Entity* ent) {
				return !(ent->FlagTest(EEF_Alive));
			});
			

			mNumDestroyedEntity = 0;
			mNumTickSinceLastDestroy = 0;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void World::PushToLimbo(Entity* deadEntity)
	{
		mNumTickSinceLastDestroy = 0;
		mNumDestroyedEntity++;
	}


	void World::SetPlaying(bool playing)
	{
		if (mIsPlaying == playing) return;

		mIsPlaying = playing;
		if (playing)
		{
			mDoBeginPlay = true;
			ULOG_MESSAGE("world play");
		}
		else
		{
			mDoEndPlay = true;
			ULOG_MESSAGE("world stop");
		}
	}

	void World::SetPlaying()
	{
		if (mIsPlaying) return;

		mIsPlaying = true;
		mDoBeginPlay = true;
	}

	void World::Pause()
	{
		if (mIsPlaying && !mIsPaused)
		{
			mIsPaused = true;
		}
	}

	void World::Resume()
	{
		if (mIsPlaying && mIsPaused)
		{
			mIsPaused = false;
		}
	}

	void World::StopPlaying()
	{
		if (!mIsPlaying) return;

		mIsPlaying = false;
		mDoEndPlay = true;
	}

	void World::AddEntityToList(Entity* ent)
	{
		mEntities.Add(ent);
	}

};