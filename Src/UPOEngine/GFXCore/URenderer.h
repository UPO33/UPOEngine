#pragma once


#include "UGFXContext.h"

namespace UPO
{
	class UAPI GFXRenderer
	{
	public:
		virtual bool Init(GFXContext* context) { return false; }
		virtual bool RenderFrame() { return false; }
		
		static GFXRenderer* Get();


	};

};