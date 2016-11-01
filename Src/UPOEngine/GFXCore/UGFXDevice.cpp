#pragma once

#include "UGFXDeviceDX.h"

namespace UPO
{

	UAPI const char* EnumToStr(EShaderType in)
	{
		static const char* LUT[] =
		{
			"Vertex",
			"Hull",
			"Domain",
			"Geometry",
			"Pixel",
			"Compute",
		};
		return LUT[(unsigned)in];
	}

	UAPI GFXDevice* gGFX = nullptr;





};