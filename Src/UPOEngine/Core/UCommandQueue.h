#pragma once

#include "UBasic.h"
#include "UThreading.h"

namespace UPO
{
	template<unsigned MaxCommand = 1000> class TCommandQueueSPSC
	{
		static const unsigned MaxLambdaSize = sizeof(void*) * 7;

		struct alignas(16) ICMD
		{
			virtual ~ICMD() {}
		};
		struct alignas(16) CMDBase : ICMD
		{
			char mBuffer[MaxLambdaSize];
		};
		static_assert(sizeof(CMDBase) % 16 == 0, "should be 16 aligned");
		static const unsigned CMDSize = sizeof(CMDBase);

		alignas(64)	volatile unsigned		mHead = 0;
		volatile bool						mRuning = false;
		Event								mWaitEvent;
		alignas(64) volatile unsigned		mTail = 0;
		ThreadID							mConsumerThreadID = 0;
		alignas(64)	CMDBase					mElements[MaxCommand];


		bool IsEmpty() const { return mHead == mTail; }
		bool IsFull() const { return ((mTail + 1) % MaxCommand) == mHead; }

		void* BeginEnqueue()
		{
			UASSERT(!IsFull());	//assert if full
			return mElements + mTail;
		}
		void EndEnqueue()
		{
			mTail = (mTail + 1) % MaxCommand;
		}

		ICMD* BeginDequeue()
		{
			if (mHead == mTail) return nullptr;
			return (ICMD*)(mElements + mHead);
		}
		void EndDequeue()
		{
			mHead = (mHead + 1) % MaxCommand;
		}



	public:
		TCommandQueueSPSC(const TCommandQueueSPSC&) = delete;
		TCommandQueueSPSC& operator = (const TCommandQueueSPSC&) = delete;

		TCommandQueueSPSC() : mWaitEvent(false, false) {}

		template<typename Lambda> void Enqueue(const Lambda& proc)
		{
			if (Thread::ID() == mConsumerThreadID)
			{
				proc();
				return;
			}

			struct NewCMD : public ICMD
			{
				Lambda mProc;
				NewCMD(const Lambda& p) : mProc(p) {}
				~NewCMD()
				{
					mProc();
				}
			};
			static_assert(sizeof(NewCMD) <= CMDSize, "");
			static_assert(alignof(NewCMD) <= alignof(ICMD), "");
			void* cmd = BeginEnqueue();
			new (cmd) NewCMD(proc);
			EndEnqueue();
		}
		template<typename Lambda> void EnqueueAndWait(const Lambda& proc)
		{
			if (Thread::ID() == mConsumerThreadID)
			{
				proc();
				return;
			}

			struct NewCMD : public ICMD
			{
				Lambda mProc;
				TCommandQueueSPSC*	mOwner;
				NewCMD(const Lambda& p, TCommandQueueSPSC* owner) : mProc(p), mOwner(owner) {}
				~NewCMD()
				{
					mProc();
					mOwner->mWaitEvent.SetSignaled();
				}
			};
			static_assert(sizeof(NewCMD) <= CMDSize, "");
			static_assert(alignof(NewCMD) <= alignof(ICMD), "");
			void* cmd = BeginEnqueue();
			new (cmd) NewCMD(proc, this);
			EndEnqueue();
			
			mWaitEvent.Wait();
		}
		void SetConsumerThread(ThreadID id)
		{
			mConsumerThreadID = id;
		}
		void RunTillQuit()
		{
			mRuning = true;
			mConsumerThreadID = Thread::ID();

			while (mRuning)
			{
				if (ICMD* cmd = BeginDequeue())
				{
					cmd->~ICMD();
					EndDequeue();
				}
				else
				{
					UPause();
				}
			}
		}
		void Quit() { mRuning = false; }
		volatile bool IsRuning() const { return mRuning; }
	};

	//////////////////////////////////////////////////////////////////////////
	template<unsigned CapacityInBytes, bool GameIsWriter = true> class alignas(64) TCommandPool
	{
		static const unsigned CAPACITY = CapacityInBytes;

		struct alignas(64) Line
		{
			char*	mSeek;
			char	mBuffer[CAPACITY];
		};
		struct ICMD
		{
			char* mNext;
			virtual ~ICMD() {}
		};

		TinyLock		mLock;
		volatile Line*	mWrite;
		Line*			mRead;
		Line			mLine0;
		Line			mLine1;

		char* PEnqueue(unsigned size)
		{
			Line* pool = (Line*)mWrite;

			UASSERT(pool->mSeek + size <= pool->mBuffer + CAPACITY); //asset if full

			char* ret = pool->mSeek;
			pool->mSeek += size;
			UASSERT(((size_t)ret) % sizeof(void*) == 0);	//should be void* align
			return ret;
		}

	public:
		TCommandPool(const TCommandPool&) = delete;
		TCommandPool& operator = (const TCommandPool&) = delete;

		TCommandPool()
		{
			mLine0.mSeek = mLine0.mBuffer;
			mLine1.mSeek = mLine1.mBuffer;

			mWrite = &mLine0;
			mRead = &mLine1;
		}
		~TCommandPool()
		{
			UASSERT(mWrite->mSeek == mWrite->mBuffer);
			UASSERT(mRead->mSeek == mRead->mBuffer);
		}
		template<typename Lambda> void Enqueue(Lambda& proc)
		{
			USCOPE_LOCK(mLock);
 			UASSERT(GameIsWriter ? IsGameThread() : IsRenderThread());

			struct alignas(sizeof(void*)) NewCMD : public ICMD
			{
				Lambda mProc;
				NewCMD(Lambda& p, char* next) : mProc(p) { mNext = next; }
				~NewCMD()
				{
					mProc();
				}
			};

			UASSERT(sizeof(NewCMD) % sizeof(void*) == 0);	//command should be void* align
			char* ptr = PEnqueue(sizeof(NewCMD));
			new (ptr) NewCMD(proc, ptr + sizeof(NewCMD));
		}
		template<typename Lambda> TCommandPool& operator << (Lambda& proc)
		{
			Enqueue(proc);
			return *this;
		}
		void InvokeAll()
		{
			UASSERT(GameIsWriter ? IsRenderThread() : IsGameThread());

			//swapping read,write
			{
				USCOPE_LOCK(mLock);
				Line* tmp = mRead;
				mRead = (Line*)mWrite;
				mWrite = tmp;
			}

			Line* pool = mRead;
			{
				char* end = pool->mSeek;
				char* iter = pool->mBuffer;
				while (iter < end)
				{
					ICMD* cmd = reinterpret_cast<ICMD*>(iter);
					iter = cmd->mNext;
					cmd->~ICMD();	//execute command then destroy
				}
				pool->mSeek = pool->mBuffer;
			}
		}
	};

};