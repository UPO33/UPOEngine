#include "UScreenDrawer.h"

namespace UPO
{
	
	//////////////////////////////////////////////////////////////////////////
	ScreenDrawer::ScreenDrawer(Renderer* rdr) : RendererElement(rdr)
	{
		mVertexShader = gGFX->GetShader("Canvas.hlsl", "VSMain", EShaderType::EST_VERTEX);
		UASSERT(mVertexShader);
		mPixelShader = gGFX->GetShader("Canvas.hlsl", "PSMain", EShaderType::EST_PIXEL);
		UASSERT(mPixelShader);

		///////////////constant buffer
		GFXConstantBuffer_Desc cbDesc;
		cbDesc.mSize = sizeof(CBPerObject);
		cbDesc.mInitialData = new CBPerObject;
		mCBPerObject = gGFX->CreateConstantBuffer(cbDesc);

		GFXVertexElement_Desc vElements[] =
		{
			GFXVertexElement_Desc("POSITION", EPixelFormat::EPT_R32G32_FLOAT),
			GFXVertexElement_Desc("UV", EPixelFormat::EPT_R32G32_FLOAT)
		};
		mLayout = gGFX->CreateInputLayout(vElements, ARRAYSIZE(vElements), mVertexShader);

		GFXVertexBuffer_Desc vbDesc;
		vbDesc.mUsage = EResourceUsage::EBU_DYNAMIC;
		vbDesc.mSize = sizeof(VertexType) * 32;
		mQuadBuffer = gGFX->CreateVertexBuffer(vbDesc);

		GFXDepthStencilState_Desc dsDesc;
		dsDesc.mDepthEnable = false;
		dsDesc.mStencilEnable = false;
		mDepthStencilState = gGFX->CreateDepthStencilState(dsDesc);

		GFXSamplerState_Desc ssDesc;
		ssDesc.mAddressU = ETextureAddress::ETA_CLAMP;
		ssDesc.mAddressV = ETextureAddress::ETA_CLAMP;
		ssDesc.mFilter = ETextureFilter::ETF_MIN_MAG_MIP_POINT;
		mSampler = gGFX->CreateSamplerState(ssDesc);


		/////////////blend state
		GFXBlendState_Desc bdesc;
		bdesc.mRenderTargets[0].mBlendEnable = true;
		bdesc.mRenderTargets[0].mSrcBlend = EBlendElement::EBE_SRC_ALPHA;
		bdesc.mRenderTargets[0].mDestBlend = EBlendElement::EBE_INV_SRC_ALPHA;
		mAlphaBlend = gGFX->CreateBlendState(bdesc);

		GFXRasterizerState_Desc rs;
		rs.mCullMode = ECullMode::ECM_NONE;
		mRaster = gGFX->CreateRasterizerState(rs);
	}

	ScreenDrawer::~ScreenDrawer()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	void ScreenDrawer::DrawTexture(GFXTexture2D* texture, Vec2 pos, Vec2 size, Color color, Vec2 uvTopLeft, Vec2 uvRightBottom)
	{
		{
			VertexType* vertices = mQuadBuffer->Map<VertexType>(EMapFlag::EMF_WRITE_DISCARD);
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

			CBPerObject* obj = mCBPerObject->Map<CBPerObject>(EMapFlag::EMF_WRITE_DISCARD);
			obj->mColor = color;
			mCBPerObject->Unmap();
		}



		gGFX->BinInputLayout(mLayout);
		gGFX->SetRasterizer(mRaster);
		gGFX->SetDepthStencilState(mDepthStencilState);
		gGFX->SetPrimitiveTopology(EPrimitiveTopology::EPT_TRIANGLELIST);
		gGFX->BinVertexBuffer(mQuadBuffer, sizeof(VertexType), 0);
		gGFX->BindShaders(mVertexShader, mPixelShader);
		gGFX->BindTexture(texture, 0, EST_PIXEL);
		gGFX->BindSamplerState(mSampler, 0, EST_PIXEL);
		gGFX->BindConstantBuffer(mCBPerObject, 0, EST_PIXEL);
		float brgba[] = { 0,0,0,0 };
		gGFX->BindBlendState(mAlphaBlend, brgba);
		gGFX->Draw(6, 0);
	}

	void ScreenDrawer::Present()
	{
	}
};