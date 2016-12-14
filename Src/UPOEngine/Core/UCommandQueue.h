#pragma once

#include "UBasic.h"
#include "UThreading.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	template<unsigned CapacityInBytes, bool GameIsWriter = true> class alignas(64) CommandPool
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
		CommandPool(const CommandPool&) = delete;
		CommandPool& operator = (const CommandPool&) = delete;

		CommandPool()
		{
			mLine0.mSeek = mLine0.mBuffer;
			mLine1.mSeek = mLine1.mBuffer;

			mWrite = &mLine0;
			mRead = &mLine1;
		}
		~CommandPool()
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
		template<typename Lambda> CommandPool& operator << (Lambda& proc)
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