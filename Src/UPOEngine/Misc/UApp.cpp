
#include "UApp.h"

namespace UPO
{
	UAPI bool gIsEditor;
	UAPI String gProjectPath;
	UAPI String gEnginePath;

	UAPI App* GApp()
	{
		static TInstance<App> Ins;
		return Ins;
	}

};