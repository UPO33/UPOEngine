#include "UEditorConfig.h"

namespace UPOEd
{

	EditorConfig::EditorConfig()
	{
		SetDefault("Style", "dark.stylesheet");

		InitFromINI("Editor.ini");
	}

	EditorConfig* EditorConfig::Get()
	{
		static TInstance<EditorConfig> Ins;
		return Ins;

	}

};