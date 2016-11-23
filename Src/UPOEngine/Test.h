#pragma once

#include "UPOEngine.h"


namespace UPO
{
	
	struct GlobaLCTorDTorLog
	{
		GlobaLCTorDTorLog()
		{
			ULOG_MESSAGE("");
		}
		~GlobaLCTorDTorLog()
		{
			ULOG_MESSAGE("");
		}
	};

	extern UAPI GlobaLCTorDTorLog gGlobaLCTorDTorLog;

	UAPI void TestMain(int argc, const char** argv);
};