#pragma once

#include "UConfig.h"

namespace UPO
{
	/*
	Engine.ini 
	*/
	class UAPI EngineConfig : public Config
	{
	public:
		EngineConfig();
		
		static EngineConfig* Get();

	};

	inline EngineConfig* GEngineConfig() { return EngineConfig::Get(); }
};