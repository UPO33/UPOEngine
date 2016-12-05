#pragma once

#include "UEntity.h"


namespace UPO
{
	class UAPI WorldTicking
	{
		TArray<Entity*>		mTickEnableEntities;
		TArray<Entity*>		mPendingAddEntities;
		int					mCurTickingIndex = -1;
		unsigned			mTickCounterReintegrate = 0;

	public:
		WorldTicking()
		{
			mTickEnableEntities.SetCapacity(1024);
			mPendingAddEntities.SetCapacity(128);
		}
		~WorldTicking()
		{

		}
		bool IsTicking()
		{
			return mCurTickingIndex != -1;
		}
		void RegTick(Entity* entity)
		{
			UASSERT(entity);
			UASSERT(entity->FlagTest(EEF_Tickable | EEF_Alive));

			if(IsTicking())
			{ 
				UASSERT(!mPendingAddEntities.HasElement(entity));
				mPendingAddEntities.Add(entity);
			}
			else
			{
				UASSERT(!mTickEnableEntities.HasElement(entity));
				mTickEnableEntities.Add(entity);
			}
		}
		void UnregTick(Entity* entity)
		{
			UASSERT(entity);
			if (IsTicking())
			{

			}
			else
			{
				Reintegrate();
			}
		}

		void Tick()
		{
			const int numTick = mTickEnableEntities.Length();
			for (mCurTickingIndex = 0; mCurTickingIndex < numTick; mCurTickingIndex++)
			{
				Entity* ent = mTickEnableEntities[mCurTickingIndex];
				if (ent->FlagTest(EEF_Alive | EEF_Tickable))
				{
					ent->OnTick();
				}
			}
			
			mCurTickingIndex = -1;
			mTickCounterReintegrate++;

			if (NeedsReintegrate())
				Reintegrate();
		}
		//////////////////////////////////////////////////////////////////////////
		bool NeedsReintegrate()
		{
			const unsigned CLEAN_PER_FRAME = 50;

			return mTickCounterReintegrate > CLEAN_PER_FRAME || mPendingAddEntities.Length();
		}
		//////////////////////////////////////////////////////////////////////////
		void Reintegrate()
		{
			mTickEnableEntities.RemoveIf([](Entity* ent)
			{
				if (ent->FlagTest(EEF_Alive | EEF_Tickable)) return false;
				return true;
			});

			FetchPendingEntities();

			mTickCounterReintegrate = 0;
		}
		//////////////////////////////////////////////////////////////////////////
		void FetchPendingEntities()
		{
			for (Entity* ent : mPendingAddEntities)
			{
				if (ent->FlagTest(EEF_Alive | EEF_Tickable))
				{
					UASSERT(!mTickEnableEntities.HasElement(ent));
					mTickEnableEntities.Add(ent);
				}
			}

			mPendingAddEntities.RemoveAll();
		}
	};
};