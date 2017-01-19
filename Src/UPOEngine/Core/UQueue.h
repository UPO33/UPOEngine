#pragma once

#include "UBasic.h"
#include "UThreading.h"

namespace UPO
{
	template<typename T> class FIFO
	{
		struct Node
		{
			Node*	mNext;
			T		mValue;
		};

		Node* mHead;
		Node* mTail;
// 		long state; // 0, 1 enqueue , 2 dequeue

		FIFO()
		{
			mHead = mTail = nullptr;
		}
		~FIFO()
		{

		}
		void EnqueueUnInit()
		{
			//dst, exchange, comp
// 			while (::InterlockedCompareExchange(&state, 1, 2) == 2) {}
// 
// 			while (state == 2) {}
// 			state == 1;

			Node* newNode = new Node;
			newNode->mValue = data;

			newNode->mNext = mHead;
			mHead = newNode;

			state = 0;
		}
		bool Dequeue(T& out)
		{
// 			while (state == 1) {}
// 			state = 2;

			if (mTail)
			{
				out = mTail->mValue;
				mTail = mTail->mNext;
				return true;
			}
			return false;

			state = 0;
		}
	};
#if 0

	//////////////////////////////////////////////////////////////////////////
	template<typename T, size_t  Capacity> class TStaticQueue
	{
	public:
		volatile size_t		mHead = 0;
		volatile size_t		mTail = 0;

		T					mElements[Capacity];




		bool IsEmpty() const { return mHead == mTail; }
		bool IsFull() const { ((mTail + 1) % Capacity) == mHead; }

		bool Enqueue(const T& data)
		{
			if (((mTail + 1) % Capacity) == mHead) return false;
			mElements[mTail] = data;
			mTail = (mTail + 1) % Capacity;
			return true;
		}
		bool Dequeue(T& out)
		{
			if (mHead == mTail) return false;
			out = mElements[mHead];
			mHead = (mHead + 1) % Capacity;
			return true;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	template<typename T, size_t Capacity> class TStaticQueue
	{
		char	 mElements[sizeof(T[Capacity])];	//avoiding ctor call
		unsigned mWrite;
		unsigned mRead;
		unsigned mCurSize;

	public:
		TStaticQueue()
		{
			mWrite = mRead = mCurSize = 0;
		}
		~TStaticQueue()
		{
			ForEach([](T& elem) {
				elem.~T();
			});
		}
		//enqueue an element without calling contractor, return value is valid until next Enqueue or Dequeue call
		//returns null if queue is full
		T* EnqueueNoCTor()
		{
			if (mCurSize == Capacity /*IsFull()*/) return nullptr;

			T* ret = (((T*)mElements) + mWrite);

			mWrite = (mWrite + 1) % Capacity;
			mCurSize++;

			return ret;
		}
		//dequeue an element without calling destructor, return value is valid until next Enqueue or Dequeue call
		//returns null if queue is empty
		T* DequeueNoDTor()
		{
			if (mCurSize == 0 /*IsEmpty()*/) return nullptr;

			T* ret = (((T*)mElements) + mRead);

			mRead = (mRead + 1) % Capacity;
			mCurSize--;

			return ret;
		}
		bool Enqueue(const T& element)
		{
			T* elem = EnqueueNoCTor();
			if (elem)
			{
				new (elem) T(element);
				return true;
			}
			return false;
		}
		bool Dequeue(T& outElement)
		{
			T* elem = DequeueNoDTor();
			if (elem)
			{
				outElement = *elem;
				elem->~T();
				return true;
			}
			return false;
		}
		bool IsEmpty() const
		{
			return mCurSize == 0;
		}
		bool IsFull() const
		{
			return mCurSize == Capacity;
		}
		size_t NumElement() const
		{
			return mCurSize;
		}

		//iterate from older to newer
		template<typename Lambda> void ForEach(Lambda proc)
		{
			for (unsigned i = 0, unsigned j = mRead; i < mCurSize; i++, j++)
			{
				proc(((T*)mElements)[j % Capacity]);
			}
		}
	};
#endif

	//////////////////////////////////////////////////////////////////////////
	template<typename T, unsigned _Capacity> class TCircularQueue
	{



		static_assert(_Capacity > 2, "");

		unsigned					mRead = 0;
		unsigned					mWrite = 0;
		alignas(alignof(T)) char	mElements[sizeof(T[_Capacity])];

	public:
		typedef T ElementType;
		static const unsigned CAPACITY = _Capacity;

		bool IsEmpty() const { return mRead == mWrite; }
		bool IsFull() const { return ((mWrite + 1) % _Capacity) == mRead; }

		template<typename... TArgs> T* Push(TArgs... args)
		{
			if (IsFull()) return nullptr;

			T* ptr = (T*)(mElements + mWrite * sizeof(T));

			new ((void*)ptr) T(args...);

			mWrite = (mWrite + 1) % _Capacity;
			return ptr;
		}
		bool Pop(T& out)
		{
			if (IsEmpty()) return false;

			T* ptr = (T*)(mElements + mRead * sizeof(T));
			out = *ptr;
			ptr->~T();
			mRead = (mRead + 1) % _Capacity;
			return true;
		}

		T* BeginPush()
		{
			if (IsFull()) return nullptr;

			T* ptr = (T*)(mElements + mWrite * sizeof(T));

			return ptr;
		}
		void EndPush()
		{
			mWrite = (mWrite + 1) % _Capacity;
		}

		T* BeginPop()
		{
			if (IsEmpty()) return nullptr;

			T* ptr = (T*)(mElements + mRead * sizeof(T));
			return ptr;
		}
		void EndPop()
		{
			mRead = (mRead + 1) % _Capacity;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	template<typename T, unsigned _Capacity> class TCircularQueueSQSP
	{
		static_assert(_Capacity > 2, "");

		/*
		only producer writes to mWrite and only consumer writes to mRead
		*/

		std::atomic<unsigned>		mRead = 0;
		std::atomic<unsigned>		mWrite = 0;
		alignas(alignof(T)) char	mElements[sizeof(T[_Capacity])];

	public:
		typedef T ElementType;
		static const unsigned CAPACITY = _Capacity;

		bool IsEmpty() const { return mRead.load() == mWrite.load(); }
		bool IsFull() const
		{
			return ((mWrite.load() + 1) % _Capacity) == mRead.load();
		}


		template<typename... TArgs> T* Push(TArgs... args)
		{
			unsigned curWrite = mWrite.load(std::memory_order_relaxed);
			unsigned nextWrite = (curWrite + 1) % _Capacity;
			if (nextWrite == mRead.load(std::memory_order_acquire)) return nullptr; //null if full

			T* ptr = (T*)(mElements + curWrite * sizeof(T));

			new ((void*)ptr) T(args...);

			mWrite.store(nextWrite, std::memory_order_release);
			return ptr;
		}
		bool Pop(T& out)
		{
			unsigned curRead = mRead.load(std::memory_order_relaxed);
			if (curRead == mWrite.load(std::memory_order_acquire)) return false;	//return false if empty

			T* ptr = (T*)(mElements + curRead * sizeof(T));
			out = *ptr;
			ptr->~T();
			mRead.store((curRead + 1) % _Capacity, std::memory_order_release);
			return true;
		}

		T* BeginPush()
		{
			unsigned curWrite = mWrite.load(std::memory_order_relaxed);
			unsigned nextWrite = (curWrite + 1) % _Capacity;
			if (nextWrite == mRead.load(std::memory_order_acquire)) return nullptr; //null if full

			T* ptr = (T*)(mElements + curWrite * sizeof(T));

			return ptr;
		}
		void EndPush()
		{
			mWrite.store((mWrite.load(std::memory_order_relaxed) + 1) % _Capacity, std::memory_order_release);
		}

		T* BeginPop()
		{
			unsigned curRead = mRead.load(std::memory_order_relaxed);
			if (curRead == mWrite.load(std::memory_order_acquire)) return false;	//return false if empty

			T* ptr = (T*)(mElements + curRead * sizeof(T));
			out = *ptr;
			return ptr;
		}
		void EndPop()
		{
			mRead.store((mRead.load(std::memory_order_relaxed) + 1) % _Capacity, std::memory_order_release);
		}
	};


	template<typename T, unsigned Capacity> class TRingBuffer
	{
		unsigned		mHead = 0;
		unsigned		mTail = 0;
		T				mElements[Capacity];

	public:
		bool IsEmpty() const { return mHead == mTail; }
		bool IsFull() const { ((mTail + 1) % Capacity) == mHead; }

		//always return valid pointer
		T* Write()
		{
			T* ret = mElements + mTail;
			mTail = (mTail + 1) % Capacity;
			ret;
		}
		//returns null if empty
		T* Read()
		{
			if (mHead == mTail) return nullptr;
			T* ret = mElements + mHead;
			mHead = (mHead + 1) % Capacity;
			return ret;
		}
	};
};