#pragma once

#include "UEntity.h"


namespace UPO
{
	class UAPI WorldTicking
	{
		TArray<Entity*>		mTickEnableEntities;
		TArray<Entity*>		mPendingAddEntities;
		int					mCurTicking = -1;
		
		bool				mNeedsCleanup = false;

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
			return mCurTicking != -1;
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


		void Tick()
		{
			const size_t numTick = mTickEnableEntities.Length();
			for (mCurTicking = 0; mCurTicking < numTick; mCurTicking++)
			{
				Entity* ent = mTickEnableEntities[mCurTicking];
				if (ent->FlagTest(EEF_Alive | EEF_Tickable))
				{
					ent->OnTick();
				}
			}

			Cleanup();
		}
		void Cleanup()
		{
			bool bClean = mNeedsCleanup;

			if (mPendingAddEntities.Length())
				bClean = true;
			
			if (!bClean) return;


			mTickEnableEntities.RemoveIf([](Entity* ent)
			{
				if (ent->FlagTest(EEF_Alive | EEF_Tickable)) return false;
				return true;
			});

			FetchPendingEntities();
		}
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