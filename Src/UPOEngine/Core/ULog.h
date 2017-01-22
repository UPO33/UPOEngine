#pragma once

#include "UBasic.h"


namespace UPO
{
	UAPI void ULogGetLegibleThreadName(unsigned threadID, char outThreadName[32]);
	UAPI const char* ULogGetLegibleFileName(const char* fullfilename);
};