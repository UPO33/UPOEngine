#pragma once

#include "../GFXCore/UGFXCore.h"

namespace UPO
{
	class UAPI DefferdRenderTargets
	{
	public:
		Vec2I					mSize;
		GFXTexture2D*			mIDBuffer = nullptr;
		GFXTexture2D*			mIDBufferCPURead = nullptr;
		GFXTexture2D*			mDepthStencil = nullptr;
		GFXDepthStencilView*	mDepthStencilView;
		GFXShaderResourceView*	mDepthReadView;
		GFXTexture2D*			mGBufferA;
		GFXTexture2D*			mGBufferB;
		GFXTexture2D*			mGBufferC;

		void BinGBuffers(bool clear);
		

		DefferdRenderTargets(Vec2I size, bool createIDBuffer);
		~DefferdRenderTargets();
	};



	class UAPI ForwardRenderTargets
	{
	public:
		Vec2I			mSize;
		GFXTexture2D*	mDepthStencil;
		GFXTexture2D*	mColor;

		ForwardRenderTargets(Vec2I size);
		~ForwardRenderTargets();
	};
};