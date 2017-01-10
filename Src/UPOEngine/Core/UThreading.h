#pragma once

#include "UBasic.h"
#include "UInstruction.h"

namespace UPO
{
	typedef unsigned ThreadID;
};



#define USCOPE_LOCK(lock)	TScpoedLock<decltype(lock)>	ZZ_Lock(lock)


namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	template<typename TLock> class TScpoedLock
	{
		TLock& mLock;
	public:
		TScpoedLock(TLock& lock) : mLock(lock) { mLock.Enter(); }
		~TScpoedLock() { mLock.Leave(); }
	};

	//////////////////////////////////////////////////////////////////////////
	class NullLock
	{
	public:
		void Enter()
		{}
		void Leave()
		{}
	};

	//////////////////////////////////////////////////////////////////////////
	class TinyLock
	{
		volatile long mlock;

	public:

		TinyLock() : mlock(0) {}
		~TinyLock() {}

		void Enter()
		{
			while (Atomic::Exchange(&mlock, 1) == 1)
			{
				UPause();
			}
		}

		void Leave()
		{
			Atomic::Exchange(&mlock, 0);
		}
	};


	/*
		Moved to UBasic.h

		UAPI bool IsGameThread();
		UAPI bool IsRenderThread();
	*/

	extern UAPI ThreadID gGameThreadID;
	extern UAPI ThreadID gRenderThreadID;

};

#ifdef UPLATFORM_WIN
#include "UThreading_Win.h"
#else
#error
#endif