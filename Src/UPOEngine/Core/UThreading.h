#pragma once

#include "UBasic.h"

namespace UPO
{
	typedef unsigned ThreadID;
};

#if UPLATFORM_WIN
#include "UThreading_Win.h"
#else

#endif

namespace UPO
{
	

	/*
		Moved to UBasic.h

		UAPI bool IsGameThread();
		UAPI bool IsRenderThread();
	*/

	extern UAPI ThreadID gGameThreadID;
	extern UAPI ThreadID gRenderThreadID;

};