#include "UThreading.h"
#include "UThreading_Win.h"

namespace UPO
{

	//////////////////////////////////////////////////////////////////////////
	void Thread::Sleep(unsigned milliseconds)
	{
		::Sleep(milliseconds);
	}
	//////////////////////////////////////////////////////////////////////////
	ThreadID Thread::ID()
	{
		return (ThreadID)::GetCurrentThreadId();
	}

};