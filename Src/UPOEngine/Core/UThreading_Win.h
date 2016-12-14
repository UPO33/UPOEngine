#pragma once

#include "UBasic.h"
#include <windows.h>



namespace UPO
{

	//////////////////////////////////////////////////////////////////////////
	class CriticalSection
	{
		CRITICAL_SECTION	mCriticleSection;

	public:
		CriticalSection()
		{
			::InitializeCriticalSection(&mCriticleSection);
		}
		~CriticalSection()
		{
			::DeleteCriticalSection(&mCriticleSection);
		}
		//If the critical section is successfully entered or the current thread already owns the critical section, 
		//the return value is nonzero. If another thread already owns the critical section, the return value is zero.
		bool TryEnter()
		{
			return ::TryEnterCriticalSection(&mCriticleSection);
		}
		void Enter()
		{
			::EnterCriticalSection(&mCriticleSection);
		}
		void Leave()
		{
			::LeaveCriticalSection(&mCriticleSection);
		}
	};


	//////////////////////////////////////////////////////////////////////////
	class Event
	{
		HANDLE	mHandle;
	public:
		/*
		@initSignaled
		indicates whether initial state is signaled or nonsignaled
		@manualRest
		If manualRest parameter is true, the function creates a manual - reset event object, which requires the use
		of the ResetEvent function to set the event state to nonsignaled. If this parameter is false, the function creates
		an auto-reset event object, and system automatically resets the event state to nonsignaled after a single waiting thread has been released.
		*/
		Event(bool initSignaled, bool manualRest)
		{
			mHandle = CreateEventA(nullptr, manualRest, initSignaled, nullptr);
			UASSERT(mHandle != nullptr);
		}
		~Event()
		{
			if (mHandle)
			{
				::CloseHandle(mHandle);
			}
		}
		// wait for event object to be signaled.
		unsigned Wait()
		{
			UASSERT(mHandle != nullptr);
			return ::WaitForSingleObject(mHandle, INFINITE);
		}
		//wait for event object to be signaled or out interval elapses.
		unsigned Wait(unsigned timeOutMiliseconds)
		{
			UASSERT(mHandle != nullptr);
			return ::WaitForSingleObject(mHandle, timeOutMiliseconds);
		}
		//Sets the event object to the nonsignaled state.
		//if is already nonsignaled has no effect.
		bool SetNonSignaled()
		{
			UASSERT(mHandle != nullptr);
			return ::ResetEvent(mHandle);
		}

		//Sets the event object to the signaled state.
		//Setting an event that is already set has no effect.
		//
		//manual-reset event remains signaled until it is set explicitly to the nonsignaled state by the SetNonSignaled function.
		//Any number of waiting threads, or threads that subsequently begin wait operations for the specified event 
		//object by calling one of the wait functions, can be released while the object's state is signaled.
		//
		//auto - reset event object remains signaled until a single waiting thread is released, at which time the system automatically
		//sets the state to nonsignaled.If no threads are waiting, the event object's state remains signaled.
		bool SetSignaled()
		{
			UASSERT(mHandle != nullptr);
			return ::SetEvent(mHandle) != 0;
		}
	};

	typedef HANDLE ThreadHandle;
#if 0
	//////////////////////////////////////////////////////////////////////////
	class UAPI Thread
	{
		HANDLE		mHandle = nullptr;
		// 		ThreadID	mThreadID = 0;
		// 		unsigned	mExitCode = 0;

	public:
		/*
		/////////////////////////////parent class should implement these
		virtual void OnInit() {};
		virtual void OnRun() {};


		//@runImmedialely
		//	runs thread immediately after creation or The thread is created in a suspended state, and does not run until the Resume() is called.
		void Run(bool runImmedialely = true, const char* debugName = nullptr)
		{
		UASSERT(mHandle == nullptr);

		mHandle = CreateThread(
		NULL,       // default security attributes
		0,          // default stack size
		(LPTHREAD_START_ROUTINE)ThreadProc,
		(void*)this,       // no thread function arguments
		runImmedialely ? 0 : CREATE_SUSPENDED,          //creation flags
		(LPDWORD)&mThreadID); // receive thread identifier

		UASSERT(mHandle);
		ULOG_SUCCESS("Thread %s Created", mDebugName ? mDebugName : "");
		}
		*/
		//If the function succeeds, execution of the specified thread is suspended and the thread's suspend count is incremented.
		void Suspend()
		{
			UASSERT(mHandle != nullptr);
			::SuspendThread(mHandle);
		}
		//Decrements suspend count. When the suspend count is decremented to zero, the execution of the thread is resumed.
		void Resume()
		{
			//If the function succeeds, the return value is the thread's previous suspend count.
			//If the function fails, the return value is(DWORD) - 1.
			::ResumeThread(mHandle);
		}
		bool Terminate(unsigned exitCode = 0)
		{
			if (mHandle == nullptr) return false;
			if (::TerminateThread(mHandle, (DWORD)exitCode))
			{
				// 				mExitCode = exitCode;
				return true;
			}
			return false;
		}
	private:
		// 		static DWORD WINAPI ThreadProc(void* param)
		// 		{
		// 			Thread* thread = (Thread*)param;
		// 			ULOG_MESSAGE("Thread Started %s ", thread->mDebugName ? thread->mDebugName : "");
		// 			thread->OnInit();
		// 			thread->OnRun();
		// 			return 0;
		// 		}

#endif

	namespace Private
	{
		struct ThreadExec
		{
			virtual void Exec() {};
		};

		static DWORD WINAPI ProcLambda(void* param)
		{
			ThreadExec* ins = reinterpret_cast<ThreadExec*>(param);
			ins->Exec();
			delete ins;
			return 0;
		}
	};

	namespace Thread
	{
		inline void Sleep(unsigned milliseconds) { ::Sleep(milliseconds); }
		inline ThreadID ID() { return (ThreadID)::GetCurrentThreadId(); }


		template<typename Lambda> ThreadHandle CreateLambda(Lambda proc, bool runImmedialely = true)
		{
			struct Impl : Private::ThreadExec
			{
				Lambda mProc;
				Impl(Lambda& p) : mProc(p) {}
				void Exec() override { mProc(); }
			};

			ThreadHandle handle = (ThreadHandle)CreateThread(
				NULL,       // default security attributes
				0,          // default stack size
				(LPTHREAD_START_ROUTINE)Private::ProcLambda,
				(void*)(new Impl(proc)),
				runImmedialely ? 0 : CREATE_SUSPENDED,          //creation flags
				nullptr);
			return handle;
		};


	};	
};