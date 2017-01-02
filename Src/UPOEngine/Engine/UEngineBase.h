#pragma once

#include "../Core/UCommandQueue.h"


namespace UPO
{
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