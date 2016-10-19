#pragma once

#include "UCommonEd.h"

namespace UPOEd
{
	struct EditorConfig : public Config
	{
		EditorConfig();

		static EditorConfig* Get();
	};
};