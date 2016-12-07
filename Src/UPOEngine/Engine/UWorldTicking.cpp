#include "UWorldTicking.h"

namespace UPO
{

	WorldTicking::WorldTicking()
	{
		mTickEnableEntities.SetCapacity(1024);
		mPendingAddEntities.SetCapacity(128);
	}

	WorldTicking::~WorldTicking()
	{

	}

	void WorldTicking::SetTickEnable(Entity* entity, bool enable)
	{
		if (!entity->IsAlive() || entity->IsTickEnable() == enable) return;

		enable ? entity->FlagSet(EEF_Tickable) : entity->FlagClear(EEF_Tickable);
		if (enable)
		{
			if (!entity->mTickPendingAdd)
			{
				entity->mTickPendingAdd = true;
				mPendingAddEntities.Add(entity);
			}
		}
	}
	void WorldTicking::AdjustEntityTick(Entity* entity)
	{
		if (entity->FlagTest(EEF_Tickable))
		{
			if (!entity->mTickPendingAdd)
			{
				entity->mTickPendingAdd = true;
				mPendingAddEntities.Add(entity);
			}
		}
	}

	void WorldTicking::Tick(float delta)
	{
		mIsTicking = true;

		bool hasRotten = false;
		size_t numTick = mTickEnableEntities.Length();
		for (size_t i = 0; i < numTick; i++)
		{
			Entity* ent = mTickEnableEntities[i];
			if (ent->FlagTest(EEF_Alive | EEF_Tickable | EEF_BeginPlayWasCalled))
			{
				ent->OnTick();
			}
			else
			{
				hasRotten = true;
			}
		}

		mIsTicking = false;
		mTickCounterReintegrate++;

		if (hasRotten && NeedsReintegrate()) Reintegrate();

		FetchPendingEntities();
	}

	bool WorldTicking::NeedsReintegrate()
	{
		const unsigned CLEAN_PER_FRAME = 50;

		return mTickCounterReintegrate > CLEAN_PER_FRAME;
	}

	void WorldTicking::Reintegrate()
	{
		mTickEnableEntities.RemoveIf([](Entity* ent)
		{
			if (ent->FlagTest(EEF_Alive | EEF_Tickable)) return false;
			else
			{
				ent->mTickRegistered = false;
				return true;
			}

		});
	}

	void WorldTicking::FetchPendingEntities()
	{
		for (Entity* ent : mPendingAddEntities)
		{
			ent->mTickPendingAdd = false;
			if (ent->FlagTest(EEF_Alive | EEF_Tickable))
			{
				if (!ent->mTickRegistered)
				{
					mTickEnableEntities.Add(ent);
					ent->mTickRegistered = true;
				}
			}
		}

		mPendingAddEntities.RemoveAll();
	}

};