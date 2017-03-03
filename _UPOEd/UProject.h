#pragma once

#include "UCommonEd.h"

namespace UPOEd
{
	struct ProjectInfo
	{
		QString			mAbsoluteDirectory;
		QString			mName;
		TArray<Module*> mLoadedModules;

		bool NeedsSave();
		void Release();
		void LoadModules();
		void UnloadModules();
	};
};
