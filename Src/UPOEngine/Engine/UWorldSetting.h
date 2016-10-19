#pragma once

#include "UAsset.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class UAPI AWorldSetting : public Asset
	{
		UCLASS(AWorldSetting, Asset)

	public:
		Vec3 mSunDirection = Vec3(1,1,0);
		Color mSunLight = Color(1, 0.9, 0.9, 1);
		
	};
};