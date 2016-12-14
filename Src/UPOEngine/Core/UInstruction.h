#pragma once

#include "UBasic.h"
#include "UAtomic.h"

namespace UPO
{
#ifdef UPLATFORM_WIN
	inline void UPause() { _mm_pause(); }
#else
	inline void UPause() {  }
#endif
};