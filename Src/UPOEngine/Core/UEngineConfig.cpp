#include "UEngineConfig.h"

namespace UPO
{

	EngineConfig::EngineConfig()
	{
		SetDefault("Game.StartupWorld", "main.world");
		SetDefault("Game.Name", "UPOEngine");

		SetDefault("Camera.MaxFOV", 170.0f);

		SetDefault("GFX.MultiSample", 0.0f);
		SetDefault("GFX.VSync", false);


		//^^^^^^^^^^^^^ new config here ^^^^^^^^^^^^^

		InitFromINI("Engine.ini");
	}

	EngineConfig* EngineConfig::Get()
	{
		static TInstance<EngineConfig> Ins;
		return Ins;
	}
};