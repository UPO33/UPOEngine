#include "UThreading.h"


namespace UPO
{
	ThreadID gGameThreadID = 0;
	ThreadID gRenderThreadID = 0;

	//////////////////////////////////////////////////////////////////////////
	bool IsGameThread()
	{
		return Thread::ID() == gGameThreadID;
	}
	//////////////////////////////////////////////////////////////////////////
	bool IsRenderThread()
	{
		return Thread::ID() == gRenderThreadID;
	}
};