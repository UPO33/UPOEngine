#pragma once

#include "UEntity.h"

namespace UPO
{
	class UAPI EntityGFX : public Entity
	{
		unsigned	mRSDirty : 1;
		unsigned	mTarnsformDirty : 1;
	};

	class EntityRS
	{
		Entity*		mOwner;
	};
};