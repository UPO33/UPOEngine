#pragma once

#include "UEntity.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class EntityLight : public Entity
	{
		UCLASS(EntityLight, Entity)
		
	public:
		bool	mActive = true;
		bool	mCastShadow = true;
		bool	mIsSpot = false;
		Color	mColor = Color::WHITE;
		float	mRadius = 100;
		float	mSpotInnerAngle = 45;
		float	mSpotOuterAngle = 60;
		
		EntityLight();
		~EntityLight();

	};

	//////////////////////////////////////////////////////////////////////////
	class EntityLightRS : public EntityRS
	{
	public:
		EntityLightRS(EntityLight* gs, WorldRS* wrs);
		~EntityLightRS();

		float	mRadius;
		Color	mColor;
		Vec3	mWorldPosition;

		void OnFetch(unsigned flag) override;

	};
};