#pragma once

#include "UTimer.h"

#include <time.h>

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	void FPSCounter::Frame()
	{
		if (!mInited) //first run?
		{
			mTimer.Start();
			mInited = true;
		}

		mCounter++;
		if (mTimer.Since() >= 1000)
		{
			mFPS = mCounter;
			mCounter = 0;
			mTimer.Start();
		}
	}

	UAPI unsigned GetSecondsSince2000()
	{
		time_t now;
		time(&now);
		tm tm2000 = { 0,0,0,0,0, 100, 0,0,0 };
		return (unsigned)difftime(now, mktime(&tm2000));
	}

};