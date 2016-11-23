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
		bool mIsEditor;
	};

	extern UAPI App* GApp();

	extern UAPI bool gIsEditor;
	extern UAPI String gProjectPath;
	extern UAPI String gEnginePath;
};
