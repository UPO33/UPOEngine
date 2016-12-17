#include "UWorldTimer.h"

namespace UPO
{
	FreeListAllocator	gTimerEntryAllocator { sizeof(TimerEntry), 64 };
	unsigned			gTimerIDCounter = 0;



	inline unsigned GetNewTimerID()
	{
		static unsigned Counter = 0;
		return ++Counter;
	}

	TimerEntry* WorldTimer::GetNewTimerEntry(float intervalSeconds, unsigned numRepeat)
	{
		TimerEntry* newEntry = new (gTimerEntryAllocator.Alloc()) TimerEntry;
		newEntry->mOwner = this;
		newEntry->mIsPaused = false;
		newEntry->mPauseTime = 0;
		newEntry->mInterval = intervalSeconds;
		newEntry->mNumRepeat = numRepeat;
		newEntry->mFireTime = mSeconds + intervalSeconds;
		newEntry->mID = GetNewTimerID();

		mTimers.Add(newEntry);

		return newEntry;
	}

	WorldTimer::WorldTimer()
	{
		mTimers.SetCapacity(128);
	}

	WorldTimer::~WorldTimer()
	{
		UASSERT(!IsTicking());
		StopAll();
		mSeconds = 0;
	}

	TimerHandle WorldTimer::StartTimer(float intervalSeconds, unsigned numRepeat, void(*proc)())
	{
		if (numRepeat == 0 || proc == nullptr) return TimerHandle();

		TimerEntry* newEntry = GetNewTimerEntry(intervalSeconds, numRepeat);
		newEntry->mProc.BindStatic(proc);
		UASSERT(newEntry->mProc.IsBound());
		return TimerHandle(newEntry);
	}

	void WorldTimer::StopAll()
	{
		if (mIsTicking)
		{
			for (TimerEntry* iter : mTimers)
				iter->Stop();
		}
		else
		{
			for (TimerEntry* iter : mTimers)
			{
				iter->Stop();
				gTimerEntryAllocator.Free(iter);
			}
			mTimers.RemoveAll();
			mNumInvalid = 0;
		}
	}

	void WorldTimer::Tick(float delta)
	{
		static const unsigned MAX_INVALID = 8;

		mSeconds += delta;
		mIsTicking = true;

		/////////////checking timers
		unsigned numTimer = mTimers.Length();
		for (unsigned i = 0; i < numTimer; i++)
		{
			mTimers[i]->Check();
		}

		mIsTicking = false;

		///////////checking arrangements and cleanup
		if (mNumInvalid > MAX_INVALID)
		{
			mNumInvalid = 0;
			mTimers.RemoveIf([](TimerEntry* entry)
			{
				if (entry->ShouldBeRemoved())
				{
					gTimerEntryAllocator.Free(entry);
					return true;
				}
				return false;
			});
		}

		
	}

	void TimerEntry::Pause()
	{
		if (mIsPaused || mNumRepeat == 0) return;

		mIsPaused = true;
		mPauseTime = mOwner->mSeconds;
	}

	void TimerEntry::Resume()
	{
		if (!mIsPaused || mNumRepeat == 0) return;

		mIsPaused = false;
		float pauseDuration = mOwner->mSeconds - mPauseTime;
		mFireTime += pauseDuration;
	}

	void TimerEntry::Stop()
	{
		if (mNumRepeat == 0) return;
		mNumRepeat = 0;
		mID = GetNewTimerID();
	}



	void TimerEntry::Check()
	{
		// member function and lambda with capture need Object
		// mNumRepeat == 0 means the timer is stoped and should be removed later
		// along with setting mNumReapeat to zero we have to change ID to new unique ID
		// 

		if (mOwner->mSeconds >= mFireTime && mNumRepeat)
		{
			if (mIsPaused)
			{
				if(!mProc.IsBound())
				{
					mNumRepeat = 0;
					mID = GetNewTimerID();
					mOwner->mNumInvalid++;
				}
			}
			else
			{
				mNumRepeat--;
				mFireTime += mInterval;

				if (mProc.IsBound())
					mProc.Invoke();
				else
					mNumRepeat = 0;

				if (mNumRepeat == 0)
				{
					mID = GetNewTimerID();
					mOwner->mNumInvalid++;
				}
			}

		}
	}

};