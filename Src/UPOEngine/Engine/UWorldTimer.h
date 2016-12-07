#pragma once

#include "UEntity.h"


namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class WorldTimer;
	class TimerEntry;
	class TimerHandle;
	class World;

	//////////////////////////////////////////////////////////////////////////
	class TimerEntry
	{
		friend TimerHandle;
		friend WorldTimer;

		WorldTimer*				mOwner;
		float					mFireTime;
		unsigned				mNumRepeat;
		unsigned				mIsPaused;
		TObjectPtr<Object>		mObject;
		float					mInterval;
		TFP<void>				mFunc;
		TMFP<void>				mMemFunc;
		std::function<void()>	mStdFunc;
		float					mPauseTime;
		unsigned				mID;

		void Check();
	public:
		void Pause();
		void Resume();
		void Stop();

		bool ShouldBeRemoved() const 
		{ 
// 			if (mNumRepeat == 0) return true;
// 			if (mFunc) return false;
// 			return !mObject;
			return mNumRepeat == 0;
		}
		
	};

	//////////////////////////////////////////////////////////////////////////
	class TimerHandle
	{
		TimerEntry*	mEntry;	//this pointer is always valid because we use a kind of allocator which only grows up
		unsigned	mID;	//when a new TimerEntry is created or removed new ID is assigned to it
	public:
		TimerHandle() : mEntry(nullptr), mID(0) {}
		TimerHandle(TimerEntry* entry) : mEntry(entry), mID(entry->mID) {}
		//return pointer to TimerEntry if its removed return null
		TimerEntry* GetEntry() const
		{
			if (mEntry && mEntry->mID == mID) return mEntry;
			return nullptr;
		}
		void Invalidate() { mEntry = nullptr; mID = 0; }
		bool IsValid() const { return GetEntry() != nullptr; }
		
		void Pause() const { if (auto entry = GetEntry()) entry->Pause(); }
		void Resume() const { if (auto entry = GetEntry()) entry->Resume(); }
		void Stop() const { if (auto entry = GetEntry()) entry->Stop(); }
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI WorldTimer
	{
		friend TimerEntry;
		friend TimerHandle;
		friend World;

		float					mSeconds = 0;
		TArray<TimerEntry*>		mTimers;
		unsigned				mNumInvalid = 0;
		bool					mIsTicking = false;



		void Tick(float delta);
		
		TimerEntry* GetNewTimerEntry();

	public:
		WorldTimer();
		~WorldTimer();

		bool IsTicking() const { return mIsTicking; }

		
		//'proc' will be called 'numRepeat' times every 'intervalSeconds' seconds
		TimerHandle StartTimer(float intervalSeconds, unsigned numRepeat, void(*proc)());
		TimerHandle StartTimer(float intervalSeconds, unsigned numRepeat, Object* object, TMFP<void> proc);
		TimerHandle StartTimer(float intervalSeconds, unsigned numRepeat, Object* object, std::function<void()> proc);

		void StopAll();
	};
};