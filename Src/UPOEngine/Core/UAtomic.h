#pragma once

#include "UBasic.h"

#ifdef UPLATFORM_WIN
#include <windows.h>

namespace UPO
{
	namespace Atomic
	{
		//Performs an atomic and/or/xor operation on the specified value and returns the original value of the dst parameter.
		inline long And(volatile long* dst, long value) { return InterlockedAnd(dst, value); }
		inline long Xor(volatile long* dst, long value) { return InterlockedXor(dst, value); }
		inline long Or(volatile long* dst, long value) { return InterlockedOr(dst, value); }

		//returns the resulting decremented/incremented value. 
		inline long Inc(volatile long* addend) { return InterlockedIncrement(addend); }
		inline long Dec(volatile long* addend) { return InterlockedDecrement(addend); }

		//returns the result of the operation.
		inline long Add(volatile long* addend, long value) { return InterlockedAdd(addend, value); }

		//returns the initial value of the addend parameter.
		inline long ExchangeAdd(volatile long* addend, long value) { return InterlockedExchangeAdd(addend, value); }

		//set 'value' to 'target' and return the initial value of 'target'
		inline long Exchange(volatile long* target, long value) { return InterlockedExchange(target, value); }

		//if the 'dst' value is equal to the 'cmp', the 'exchange' value is stored in 'dst', otherwise no operation is performed.
		//returns the initial value of the 'dst'
		inline long CompareExchange(volatile long* dst, long exchange, long cmp) { return InterlockedCompareExchange(dst, exchange, cmp); }
	};

};
#else
#error
#endif

