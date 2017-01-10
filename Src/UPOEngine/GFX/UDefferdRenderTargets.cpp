#include "UDefferdRenderTargets.h"

namespace UPO
{
	/*
	all render targets must have same size, sample count, resource type
	*/
	DefferdRenderTargets::DefferdRenderTargets(Vec2I size)
	{
		GFXTexture2DDesc desc;
		desc.mWidth = size.mX;
		desc.mHeight = size.mY;
		desc.mFlag = ETextureFlag::ERenderTargatable;

		{
			desc.mFormat = EPixelFormat::R32_UINT;
			
			mIDRenderTarget = gGFX->CreateTexture2D(desc);
			UASSERT(mIDRenderTarget);
		}
	}

	DefferdRenderTargets::~DefferdRenderTargets()
	{
		SafeDelete(mIDRenderTarget);
	}

};