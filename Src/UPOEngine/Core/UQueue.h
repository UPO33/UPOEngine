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

#if 0
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

// 	template<typename T, size_t Capacity> class TRingBuffer
// 	{
// 		volatile size_t		mHead = 0;
// 		volatile size_t		mTail = 0;
// 		T					mElements[Capacity];
// 
// 	public:
// 		bool IsEmpty() const { return mHead == mTail; }
// 		bool IsFull() const { ((mTail + 1) % Capacity) == mHead; }
// 
// 		T* Write()
// 		{
// 			mElements[mTail]
// 			mTail = (mTail + 1) % Capacity;
// 			return true;
// 		}
// 		bool Dequeue(T& out)
// 		{
// 			if (mHead == mTail) return false;
// 			out = mElements[mHead];
// 			mHead = (mHead + 1) % Capacity;
// 			return true;
// 		}
// 	};
};