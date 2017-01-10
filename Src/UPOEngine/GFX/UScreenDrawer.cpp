#include "UScreenDrawer.h"

namespace UPO
{
#if 0
	//////////////////////////////////////////////////////////////////////////
	ScreenDrawer::ScreenDrawer(Renderer* rdr) : RendererElement(rdr)
	{
// 		mVertexShader = gGFX->CreateShader<GFXVertexShader>("Canvas.hlsl", "VSMain");
// 		UASSERT(mVertexShader);
// 		mPixelShader = gGFX->CreateShader<GFXPixelShader>("Canvas.hlsl", "PSMain");
// 		UASSERT(mPixelShader);
		///////////////constant buffer
		GFXConstantBuffer_Desc cbDesc;
		cbDesc.mSize = sizeof(CBPerObject);
		cbDesc.mInitialData = new CBPerObject;
		mCBPerObject = gGFX->CreateConstantBuffer(cbDesc);


		{
			GFXInputLayoutDesc desc = 
			{
				mVertexShader,				
				{
					GFXVertexElementDesc("POSITION", EVertexFormat::EFloat2),
					GFXVertexElementDesc("UV", EVertexFormat::EFloat2),
				},
			};

			mLayout = gGFX->CreateInputLayout(desc);
		}

		GFXVertexBuffer_Desc vbDesc;
		vbDesc.mUsage = EResourceUsage::EDynamic;
		vbDesc.mSize = sizeof(VertexType) * 32;
		mQuadBuffer = gGFX->CreateVertexBuffer(vbDesc);

		GFXDepthStencilState_Desc dsDesc;
		dsDesc.mDepthEnable = false;
		dsDesc.mStencilEnable = false;
		mDepthStencilState = gGFX->CreateDepthStencilState(dsDesc);

		GFXSamplerState_Desc ssDesc;
		ssDesc.mAddressU = ETextureAddress::EClamp;
		ssDesc.mAddressV = ETextureAddress::EClamp;
		ssDesc.mFilter = ETextureFilter::ETF_MIN_MAG_MIP_POINT;
		mSampler = gGFX->CreateSamplerState(ssDesc);


		/////////////blend state
		GFXBlendState_Desc bdesc;
		bdesc.mRenderTargets[0].mBlendEnable = true;
		bdesc.mRenderTargets[0].mSrcBlend = EBlendFactor::ESrcAlpha;
		bdesc.mRenderTargets[0].mDestBlend = EBlendFactor::EInvSrcAlpha;
		mAlphaBlend = gGFX->CreateBlendState(bdesc);

		GFXRasterizerState_Desc rs;
		rs.mCullMode = ECullMode::ENone;
		mRaster = gGFX->CreateRasterizerState(rs);

	}

	ScreenDrawer::~ScreenDrawer()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	void ScreenDrawer::DrawTexture(GFXTexture2D* texture, Vec2 pos, Vec2 size, Color color, Vec2 uvTopLeft, Vec2 uvRightBottom)
	{
		{
			VertexType* vertices = mQuadBuffer->Map<VertexType>(EMapFlag::EWriteDiscard);
			//0,0
			vertices[0].mPosition = pos;
			vertices[0].mUV = uvTopLeft;
			//1,0
			vertices[1].mPosition.mX = pos.mX + size.mX;
			vertices[1].mPosition.mY = pos.mY;
			vertices[1].mUV.mX = uvRightBottom.mX;
			vertices[1].mUV.mY = uvTopLeft.mY;
			//0,1
			vertices[2].mPosition.mX = pos.mX;
			vertices[2].mPosition.mY = pos.mY + size.mY;
			vertices[2].mUV.mX = uvTopLeft.mX;
			vertices[2].mUV.mY = uvRightBottom.mY;

			//1,0
			vertices[3].mPosition.mX = pos.mX + size.mX;
			vertices[3].mPosition.mY = pos.mY;
			vertices[3].mUV.mX = uvRightBottom.mX;
			vertices[3].mUV.mY = uvTopLeft.mY;
			//1,1
			vertices[4].mPosition = pos + size;
			vertices[4].mUV = uvRightBottom;
			//0,1
			vertices[5].mPosition.mX = pos.mX;
			vertices[5].mPosition.mY = pos.mY + size.mY;
			vertices[5].mUV.mX = uvTopLeft.mX;
			vertices[5].mUV.mY = uvRightBottom.mY;

			mQuadBuffer->Unmap();

			CBPerObject* obj = mCBPerObject->Map<CBPerObject>(EMapFlag::EWriteDiscard);
			obj->mColor = color;
			mCBPerObject->Unmap();
		}



		gGFX->BinInputLayout(mLayout);
		gGFX->SetRasterizerState(mRaster);
		gGFX->SetDepthStencilState(mDepthStencilState);
		gGFX->SetPrimitiveTopology(EPrimitiveTopology::ETriangleList);
		gGFX->SetVertexBuffer(mQuadBuffer, sizeof(VertexType), 0);
		gGFX->BindShaders(mVertexShader, mPixelShader);
		gGFX->BindTexture(texture, 0, EShaderType::EPixel);
		gGFX->SetSamplerState(mSampler, 0, EShaderType::EPixel);
		gGFX->BindConstantBuffer(mCBPerObject, 0, EShaderType::EPixel);
		float brgba[] = { 0,0,0,0 };
		gGFX->SetBlendState(mAlphaBlend, brgba);
		gGFX->Draw(6, 0);
	}

	void ScreenDrawer::Present()
	{
		DrawTexture(nullptr, Vec2(0.1), Vec2(0.4), Color(1));
	}
#endif

};