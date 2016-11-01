#pragma once

#include "../Core/UCore.h"


namespace UPO
{
	struct App
	{
		String mEnginePath;
		String mEngineContentPath;
		String mProjectPath;
		String mProjectContentPath;
		bool mIsEditor;

		static String EngineContentPath();
		static String ProjectContentPath();
	};

	extern UAPI App* GApp();

	extern UAPI bool gIsEditor;
	extern UAPI String gProjectPath;
	extern UAPI String gEnginePath;
};
