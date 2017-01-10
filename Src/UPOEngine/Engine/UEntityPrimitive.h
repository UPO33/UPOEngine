#pragma once

#include "UEntity.h"


namespace UPO
{
	class UAPI EntityPrimitive : public Entity
	{
		UCLASS(EntityPrimitive, Entity)

	public:
		
		bool		mVisible = true;
		bool		mCastShadow = true;

	};
};