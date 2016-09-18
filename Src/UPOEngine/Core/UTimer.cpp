#pragma once

#include "UTimer.h"

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

};