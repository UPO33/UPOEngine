#include "UEngineBase.h"

namespace UPO
{
	UAPI unsigned	gFPS = 0;
	UAPI World*		gCurTickingWorld = nullptr;
	UAPI bool		gSeparateRenderThread = false;
	UAPI float		gDeltaSeconds = 0;
};