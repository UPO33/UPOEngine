#include "UGFXContext.h"

#include "UEditorContextDX.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	GFXContext* GFXContext::CreateContext()
	{
		return new GFXEditorContextDX;
	}

};