#pragma once

#include "UBasic.h"

#ifdef UPLATFORM_WIN
#include <windows.h>
#endif

namespace UPO
{
#ifdef UPLATFORM_WIN
	//get elapsed time since program start in milliseconds
	inline unsigned AppTickCount() { return ::GetTickCount(); }

	//////////////////////////////////////////////////////////////////////////
	struct TimeCounter
	{
		unsigned mStart = 0;

		void Start()
		{
			mStart = AppTickCount();
		}
		//get elapsed time in milliseconds
		unsigned Since() const
		{
			return AppTickCount() - mStart;
		}
	};
	//////////////////////////////////////////////////////////////////////////
	struct TimeCounterHigh
	{
		LARGE_INTEGER mFreq, mStart, mEnd;

	public:
		void Start()
		{
			QueryPerformanceFrequency(&mFreq);
			QueryPerformanceCounter(&mStart);
		}
		float SinceSeconds()
		{
			QueryPerformanceCounter(&mEnd);
			return (float)((mEnd.QuadPart - mStart.QuadPart) / (double)mFreq.QuadPart);
		}
		float SinceMiliseconds()
		{
			QueryPerformanceCounter(&mEnd);
			return (float)((mEnd.QuadPart - mStart.QuadPart) * 1000.0 / (double)mFreq.QuadPart);
		}
		unsigned long long SinceMicroseconds()
		{
			QueryPerformanceCounter(&mEnd);
			return (mEnd.QuadPart - mStart.QuadPart) * 1000000 / (mFreq.QuadPart);
		}
	};

#else

#endif

	//////////////////////////////////////////////////////////////////////////
	class UAPI FPSCounter
	{
		unsigned mFPS = 0;
		unsigned mCounter = 0;
		bool mInited = false;

		TimeCounter mTimer;

	public:
		unsigned GetFPS() const { return mFPS; }

		void Frame();
	};

	UAPI unsigned GetSecondsSince2000();

};