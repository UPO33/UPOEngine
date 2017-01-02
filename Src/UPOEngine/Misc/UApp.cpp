
#include "UApp.h"

namespace UPO
{
	UAPI bool gIsEditor;

	UAPI App* GApp()
	{
		static TInstance<App> Ins;
		return Ins;
	}

};