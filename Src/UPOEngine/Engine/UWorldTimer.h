#pragma once

#include "../Core/UDelegate.h"

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
		TDelegate<void>			mProc;
		float					mInterval;
		float					mPauseTime;
		unsigned				mID;

		

		void Check();
		void Pause();
		void Resume();
		void Stop();

		bool ShouldBeRemoved() const 
		{ 
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
		TimerHandle& operator = (std::nullptr_t)
		{
			Invalidate();
			return *this;
		}
		bool IsValid() const { return GetEntry() != nullptr; }
		operator bool() const { return IsValid(); }

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
		
		TimerEntry* GetNewTimerEntry(float intervalSeconds, unsigned numRepeat);

	public:
		WorldTimer();
		~WorldTimer();

		bool IsTicking() const { return mIsTicking; }

		
		//'proc' will be called 'numRepeat' times every 'intervalSeconds' seconds
		TimerHandle StartTimer(float intervalSeconds, unsigned numRepeat, void(*proc)());

		template<typename TClass> TimerHandle StartTimer(float intervalSeconds, unsigned numRepeat, Object* object, void(TClass::* memfunc)())
		{
			if (numRepeat == 0 || memfunc == nullptr || object == nullptr) return TimerHandle();

			TimerEntry* newEntry = GetNewTimerEntry(intervalSeconds, numRepeat);
			newEntry->mProc.BindMemberObject<TClass>(object, memfunc);

			return TimerHandle(newEntry);
		}
		template<typename Lambda> TimerHandle StartTimer(float intervalSeconds, unsigned numRepeat, Object* object, const Lambda& lambda)
		{
			if (numRepeat == 0 || object == nullptr) return TimerHandle();

			TimerEntry* newEntry = GetNewTimerEntry(intervalSeconds, numRepeat);
			newEntry->mProc.BindLambdaObject(object, lambda);

			return TimerHandle(newEntry);
		}
		template<typename Lambda> TimerHandle StartTimer(float intervalSeconds, unsigned numRepeat, const Lambda& lambda)
		{
			if (numRepeat == 0) return TimerHandle();

			TimerEntry* newEntry = GetNewTimerEntry(intervalSeconds, numRepeat);
			newEntry->mProc.BindLambda(lambda);

			return TimerHandle(newEntry);
		}
		void StopAll();
	};
};