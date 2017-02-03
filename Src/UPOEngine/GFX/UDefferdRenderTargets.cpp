#include "UDefferdRenderTargets.h"

namespace UPO
{

	void DefferdRenderTargets::BinGBuffers(bool clear)
	{
		if (clear)
		{
			gGFX->ClearRenderTarget(mGBufferA->GetRenderTargetView(), Color(0, 0, 0, 0));
			gGFX->ClearRenderTarget(mGBufferB->GetRenderTargetView(), Color(0, 0, 0, 0));
			gGFX->ClearRenderTarget(mGBufferC->GetRenderTargetView(), Color(0, 0, 0, 0));

			gGFX->ClearDepthStencil(mDepthStencilView, true, true, 1, 0);

			if (mIDBuffer)
			{
				gGFX->ClearRenderTarget(mIDBuffer->GetRenderTargetView(), Color(0));
			}
		}

		if(mIDBuffer)
		{
			GFXRenderTargetView* renderTargets[] = {
				mGBufferA->GetRenderTargetView(),
				mGBufferB->GetRenderTargetView(),
				mGBufferC->GetRenderTargetView(),
				mIDBuffer->GetRenderTargetView(),
			};

			gGFX->SetRenderTarget(renderTargets, mDepthStencilView);
		}
		else
		{
			GFXRenderTargetView* renderTargets[] = {
				mGBufferA->GetRenderTargetView(),
				mGBufferB->GetRenderTargetView(),
				mGBufferC->GetRenderTargetView(),
			};

			gGFX->SetRenderTarget(renderTargets, mDepthStencilView);
		}
		
	}

	/*
	all render targets must have same size, sample count, resource type
	*/
	DefferdRenderTargets::DefferdRenderTargets(Vec2I size, bool createIDBuffer)
	{
		GFXTexture2DDesc desc;
		desc.mWidth = size.mX;
		desc.mHeight = size.mY;
		

		//ID buffer
		if(createIDBuffer)
		{
			desc.mFlag = ETextureFlag::ERenderTargatable | ETextureFlag::EShaderResourceView;
			desc.mFormat = EPixelFormat::R32_UINT;
			mIDBuffer = gGFX->CreateTexture2D(desc);
			UASSERT(mIDBuffer);

			desc.mFlag = ETextureFlag::ECPURead;
			desc.mFormat = EPixelFormat::R32_UINT;
			mIDBufferCPURead = gGFX->CreateTexture2D(desc);
			UASSERT(mIDBufferCPURead);
		}
		else
		{
			mIDBuffer = nullptr;
			mIDBufferCPURead = nullptr;
		}
		//depth stencil
		{
#if 0
			auto formatTypeless = EPixelFormat::R24G8_TYPELESS;
			auto formatDepth = EPixelFormat::D24_UNORM_S8_UINT;
			auto formatReadDepth = EPixelFormat::R24_UNORM_X8_TYPELESS;
#else
			auto formatTypeless = EPixelFormat::R32_TYPELESS;
			auto formatDepth = EPixelFormat::D32_FLOAT;
			auto formatReadDepth = EPixelFormat::R32_FLOAT;
#endif 
			desc.mFlag = ETextureFlag::EDepthStencil | ETextureFlag::EShaderResourceView;
			desc.mFormat = formatTypeless;
			mDepthStencil = gGFX->CreateTexture2D(desc);
			UASSERT(mDepthStencil);

			mDepthStencilView = gGFX->CreateDepthStencilView(mDepthStencil, 0, formatDepth);
			UASSERT(mDepthStencilView);

			mDepthReadView = gGFX->CreateShaderResourceView(mDepthStencil, 0, 1, formatReadDepth);
			UASSERT(mDepthReadView);
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
		//gbuffer normal
		{
			desc.mFlag = ETextureFlag::EShaderResourceView | ETextureFlag::ERenderTargatable;
			desc.mFormat = EPixelFormat::R16G16B16A16_FLOAT;
			mGBufferC = gGFX->CreateTexture2D(desc);
			UASSERT(mGBufferC);
		}
	}

	DefferdRenderTargets::~DefferdRenderTargets()
	{
		SafeDelete(mIDBuffer);
		SafeDelete(mIDBufferCPURead);
		SafeDelete(mDepthStencilView);
		SafeDelete(mDepthReadView);
		SafeDelete(mDepthStencil);
		SafeDelete(mGBufferA);
		SafeDelete(mGBufferB);
		SafeDelete(mGBufferC);
	}

	ForwardRenderTargets::ForwardRenderTargets(Vec2I size)
	{

	}

	ForwardRenderTargets::~ForwardRenderTargets()
	{

	}

};