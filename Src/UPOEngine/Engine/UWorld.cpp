#include "UWorld.h"
#include "../Meta/UMeta.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	Entity* World::CreateEntity(EntityCreationParam& param)
	{
		if (param.mClass && param.mClass->IsBaseOf(Entity::GetClassInfoStatic()))
		{
			Entity* newEntity = (Entity*)NewObject(param.mClass);
			newEntity->mWorld = this;
			newEntity->mName = param.mName;
			newEntity->mEntityFlag = EEF_Alive | EEF_Initilized;

			AddEntityToList(newEntity);

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
			if (mIsFirtsTick)
			{
				size_t numEntity = mEntities.Length();
				for (size_t i = 0; i < numEntity; i++)
				{
					Entity* ent = mEntities[i];
					if (ent->IsAlive() && !ent->FlagTest(EEF_BeginPlayWasCalled))
					{
						ent->FlagSet(EEF_BeginPlayWasCalled);
						ent->OnBeginPlay();
					}
				}
				mIsFirtsTick = false;
			}

			/////////////ticking
			{
				size_t numEntity = mEntities.Length();
				for (size_t i = 0; i < numEntity; i++)
				{
					Entity* ent = mEntities[i];
					if (ent->FlagTest(EEF_Alive | EEF_Initilized | EEF_BeginPlayWasCalled | EEF_Tickable))
					{
						ent->OnTick();
						mCurTickResult.mNumTickedEntity++;
					}
				}
			}
		}

		mCurTickResult.mConsumedTimeMS = frameTimer.SinceMiliseconds();
		mIsInTick = false;
	}

};