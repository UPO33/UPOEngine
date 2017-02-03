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
		bool		mReceiveShadow = true;
		bool		mMainPassEnable = true;
		bool		mCutomDepthStencilEnable = false;
		bool		mSelected = false;
		uint8		mCustomStencilValue = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI EntityPrimitiveRS : public EntityRS
	{
	public:
		unsigned	mCustomStencilValue;

		void Fetch();

	};
};