#include "UDefferdRenderTargets.h"

namespace UPO
{

	void DefferdRenderTargets::BinGBuffers(bool clear)
	{
		if (clear)
		{
			gGFX->ClearRenderTarget(mGBufferA->GetRenderTargetView(), Color(0, 0, 0, 0));
			gGFX->ClearRenderTarget(mGBufferB->GetRenderTargetView(), Color(0, 0, 0, 0));
			gGFX->ClearDepthStencil(mDepthStencil->GetDepthStencilView(), true, true, 1, 0);
		}

		GFXRenderTargetView* renderTargets[] = {
			mGBufferA->GetRenderTargetView(),
			mGBufferB->GetRenderTargetView(),
		};

		gGFX->SetRenderTarget(renderTargets, mDepthStencil->GetDepthStencilView());
	}

	/*
	all render targets must have same size, sample count, resource type
	*/
	DefferdRenderTargets::DefferdRenderTargets(Vec2I size)
	{
		GFXTexture2DDesc desc;
		desc.mWidth = size.mX;
		desc.mHeight = size.mY;
		

		//ID buffer
		{
			desc.mFlag = ETextureFlag::ERenderTargatable;
			desc.mFormat = EPixelFormat::R32_UINT;
			mIDBuffer = gGFX->CreateTexture2D(desc);
			UASSERT(mIDBuffer);
		}
		//depth stencil
		{
			desc.mFlag = ETextureFlag::EDepthStencil | ETextureFlag::EShaderResourceView;
			desc.mFormat = EPixelFormat::R32_TYPELESS;
			mDepthStencil = gGFX->CreateTexture2D(desc);
			UASSERT(mDepthStencil);
		}
		//gbuffer position
		{
			desc.mFlag = ETextureFlag::EShaderResourceView | ETextureFlag::ERenderTargatable;
			desc.mFormat = EPixelFormat::R32G32B32A32_FLOAT;
			mGBufferA = gGFX->CreateTexture2D(desc);
			UASSERT(mGBufferA);
		}
		//gbuffer diffuse
		{
			desc.mFlag = ETextureFlag::EShaderResourceView | ETextureFlag::ERenderTargatable;
			desc.mFormat = EPixelFormat::R16G16B16A16_FLOAT;
			mGBufferB = gGFX->CreateTexture2D(desc);
			UASSERT(mGBufferB);
		}
	}

	DefferdRenderTargets::~DefferdRenderTargets()
	{
		SafeDelete(mIDBuffer);
		SafeDelete(mDepthStencil);
		SafeDelete(mGBufferA);
		SafeDelete(mGBufferB);
	}

	ForwardRenderTargets::ForwardRenderTargets(Vec2I size)
	{

	}

	ForwardRenderTargets::~ForwardRenderTargets()
	{

	}

};