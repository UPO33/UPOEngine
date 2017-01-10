#pragma once

#include "../GFXCore/UGFXCore.h"

namespace UPO
{
	class UAPI DefferdRenderTargets
	{
	public:
		Vec2I					mSize;
		GFXTexture2D*			mIDRenderTarget = nullptr;

		DefferdRenderTargets(Vec2I size);
		~DefferdRenderTargets();
	};
};