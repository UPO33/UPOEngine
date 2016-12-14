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
		bool				mIsTicking = false;

	public:
		WorldTicking();
		~WorldTicking();
		bool IsTicking() const { return mIsTicking; }
		void SetTickEnable(Entity* entity, bool enable);
		void Tick(float delta);
		bool NeedsReintegrate();
		void Reintegrate();
		void FetchPendingEntities();
		void AdjustEntityTick(Entity* entity);

		void RemoveAll();
	};
};