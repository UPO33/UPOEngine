#pragma once

#include "../Core/UCore.h"


namespace UPO
{
	struct App
	{
		String mEnginePath;
		String mEngineAssetsPath;
		String mProjectPath;
		String mProjectAssetsPath;
	};

	extern UAPI App* GApp();

	extern UAPI bool gIsEditor;

};
