#pragma once

#include "UAsset.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class UAPI ATest : public Asset
	{
		UCLASS(ATest, Asset)

	public:
		Vec3	mVec3 = Vec3(0);
		Vec2	mVec2 = Vec2(0);
		float	mFloat = 3.14;
		String  mString;

		ATest();
		~ATest();

	};
};