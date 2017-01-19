#pragma once

#include "../GFXCore/UGFXCore.h"

namespace UPO
{
	class UAPI DefferdRenderTargets
	{
	public:
		Vec2I					mSize;
		GFXTexture2D*			mIDBuffer = nullptr;
		GFXTexture2D*			mDepthStencil = nullptr;
		
		GFXTexture2D*			mGBufferA;
		GFXTexture2D*			mGBufferB;

		void BinGBuffers(bool clear);
		

		DefferdRenderTargets(Vec2I size);
		~DefferdRenderTargets();
	};
};