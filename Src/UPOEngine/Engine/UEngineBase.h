#pragma once

#include "../Core/UCommandQueue.h"


namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class World;

	extern UAPI unsigned	gFPS;
	extern UAPI World*		gCurTickingWorld;
	extern UAPI bool		gSeparateRenderThread;
	extern UAPI float		gDeltaSeconds;

	typedef TCommandQueueSPSC<2000> TRenderCommandQueue;
	extern TRenderCommandQueue gGame2RenderQueue;	//UEngine.cpp

	template<typename TLambda> void EnqueueRenderCommand(TLambda& proc)
	{
		gGame2RenderQueue.Enqueue(proc);
	}
	template<typename TLambda> void EnqueueRenderCommandAndWait(TLambda& proc)
	{
		gGame2RenderQueue.EnqueueAndWait(proc);
	}
};