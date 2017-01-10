#include "UCanvas.h"
#include "../GFXCore/UGFXCore.h"
#include "UGameWindow.h"
#include "UEngineBase.h"

#include "../GFXCore/UGFXDeviceDX.h"

#include "UTexture2D.h"

#include <SpriteBatch.h>
#include <SpriteFont.h>




namespace UPO
{
	static const bool UseUVSpace = false;

	UGLOBAL_SHADER_DECLIMPL(gVSCanvasUVSpace, GFXVertexShader, "Canvas.hlsl", "VSMain", { "USE_UVSPACE", "1" });
	UGLOBAL_SHADER_DECLIMPL(gPSCanvasUVSpace, GFXPixelShader, "Canvas.hlsl", "PSMain");
	
	UGLOBAL_SHADER_DECLIMPL(gVSCanvasNoUVSpace, GFXVertexShader, "Canvas.hlsl", "VSMain");
	UGLOBAL_SHADER_DECLIMPL(gPSCanvasNoUVSpace, GFXPixelShader, "Canvas.hlsl", "PSMain");


	static const unsigned MaxTextureQuadItemPerFrame = 1024;


	inline XMVECTOR ToXMVECTOR(const Color& color)
	{
		return XMLoadFloat4((XMFLOAT4*)color.mRGBA);
	}

	void Canvas::CreateResources()
	{
		UASSERT(IsRenderThread());

		//rasterizer state
		{
			GFXRasterizerStateDesc raster2DDesc;
			raster2DDesc.mCullMode = ECullMode::ENone;
			raster2DDesc.mDepthClipEnable = false;
			mRasterState = GlobalResources::GetRasterizerState(raster2DDesc);
			UASSERT(mRasterState);
		}
		//depth state
		{
			GFXDepthStencilStateDesc depthDesc;
			depthDesc.mDepthEnable = false;
			mDepthState = GlobalResources::GetDepthStencilState(depthDesc);
			UASSERT(mDepthState);
		}
		//alpha blend state
		{
			GFXBlendStateDesc desc;
			desc.mIndependentBlend = false;
			desc.mRenderTargets[0].MakeAlphaBlending();
			mAlphaBlend = GlobalResources::GetBlendState(desc);
			UASSERT(mAlphaBlend);
		}

		//vertex buffer for texture quad items
		{
			GFXVertexBufferDesc vbDesc;
			vbDesc.mSize = sizeof(TextureQuadItem::Vertex[4]) * MaxTextureQuadItemPerFrame;
			vbDesc.mDynamic = true;
			mVertexBuffer = gGFX->CreateVertexBuffer(vbDesc);
			UASSERT(mVertexBuffer);
		}

		//quad index buffer
		{
			uint16 indices[] = { 0,1,2, 1,3,2 };
			GFXIndexBufferDesc desc;
			desc.mImmutable = true;
			desc.mInitialData = indices;
			desc.mSize = sizeof(indices);
			desc.mType = EIndexBufferType::EUShort;
			mIndexBuffer = gGFX->CreateIndexBuffer(desc);
			UASSERT(mIndexBuffer);
		}

		//texture quad input layout
		{
			GFXInputLayoutDesc desc = {
				gVSCanvasUVSpace,
				{
					{"POSITION", EVertexFormat::EFloat2, -1, 0, false},
					{"UV", EVertexFormat::EFloat2, -1, 0, false},
					{"COLOR", EVertexFormat::EFloat4, -1, 0, false}
				}
			};

			mInputLayoutQuadTexture = GlobalResources::GetInputLayout(desc);
			UASSERT(mInputLayoutQuadTexture);
		}
		//sprite batch
		{
			mSpriteBatch = new DirectX::SpriteBatch(((GFXDeviceDX*)gGFX)->GetDXDeviceContext());
			UASSERT(mSpriteBatch);
			mSpriteFont = new DirectX::SpriteFont(((GFXDeviceDX*)gGFX)->GetDXDevice(), L"arial.spritefont");
			UASSERT(mSpriteFont);
		}

	}

	Canvas::Canvas(GameWindow* viewport)
	{
		mViewport = viewport;
		Vec2I size;
		mViewport->GetWinSize(size);
		mViewportSize = Vec2(size.mX, size.mY);



		mRTTextItems = mTextItems + 0;
		mGTTextItems = mTextItems + 1;

		mGTTextureQuads = mTextureQuads + 0;
		mRTTextureQuads = mTextureQuads + 1;


		EnqueueRenderCommandAndWait([this] { 
			CreateResources(); 
		});
	}




	void Canvas::DrawString(const String& text, Vec2 position, const Color& color, float scale)
	{
		if (text.IsEmpty()) return;

		USCOPE_LOCK(mSwapLock);

		mGTTextItems->AddUnInit(1);
		TextItem& item = mGTTextItems->LastElement();
		item.mPosition = position;
		item.mColor = color;
		item.mScale = scale;
		size_t numCharConverted = 0;
		mbstowcs_s(&numCharConverted, item.mText, text.CStr(), TextItem::MaxLen);
	}



	Vec2 ScreenToNDC(Vec2 screenpos, Vec2 screenSize)
	{
		Vec2 xy = screenpos / screenSize * 2 - 1;
		return Vec2(xy.mX, -xy.mY);

		//return (screenpos / screenSize * Vec2(2,-2)) - 1;
	}
// 	void Canvas::DrawTexture(ATexture2D* texture, Vec2 pos, Vec2 size, Vec2 origin, float rotationDeg, Color color)
// 	{
// // 		if (texture == nullptr || texture->GetRS() == nullptr) return;
// 		
// 		USCOPE_LOCK(mSwapLock);
// 		mGTTextureQuads->AddUnInit();
// 		TextureQuadItem& item = mGTTextureQuads->LastElement();
// 		item.mTexture = texture ? texture->GetRS() : nullptr;
// 
// 		//0,0
// 		item.mVertex[0].mPosition = pos;
// 		item.mVertex[0].mColor = color;
// 		item.mVertex[0].mUV = Vec2(0);
// 		//1,0
// 		item.mVertex[1].mPosition.mX = pos.mX + size.mX;
// 		item.mVertex[1].mPosition.mY = pos.mY;
// 		item.mVertex[1].mColor = color;
// 		item.mVertex[1].mUV = Vec2(1,0);
// 		//0,1
// 		item.mVertex[2].mPosition.mX = pos.mX;
// 		item.mVertex[2].mPosition.mY = pos.mY + size.mY;
// 		item.mVertex[2].mColor = color;
// 		item.mVertex[2].mUV = Vec2(0,1);
// 		//1,0
// 		item.mVertex[3].mPosition.mX = pos.mX + size.mX;
// 		item.mVertex[3].mPosition.mY = pos.mY;
// 		item.mVertex[3].mColor = color;
// 		item.mVertex[3].mUV = Vec2(1,0);
// 		//1,1
// 		item.mVertex[4].mPosition = pos + size;
// 		item.mVertex[4].mColor = color;
// 		item.mVertex[4].mUV = Vec2(1,1);
// 		//0,1
// 		item.mVertex[5].mPosition.mX = pos.mX;
// 		item.mVertex[5].mPosition.mY = pos.mY + size.mY;
// 		item.mVertex[5].mColor = color;
// 		item.mVertex[5].mUV = Vec2(0, 1);
// 
// // 		if(!UseUVSpace)
// // 		{
// // 			for (size_t i = 0; i < 6; i++)
// // 			{
// // 				Vec2 v = ScreenToNDC(item.mVertex[i].mPosition, mViewportSize);
// // 				item.mVertex[i].mPosition = v;
// // 				ULOG_WARN("%", item.mVertex[i].mPosition);
// // 			}
// // 		}
// 
// 		return;
// 	}


	void Canvas::Draw(const CanvasTextureItem& item)
	{
		USCOPE_LOCK(mSwapLock);
		
		auto textureQuads = GetGTTextureQuads();

		textureQuads->AddUnInit();
		TextureQuadItem& itemData = textureQuads->LastElement();

		itemData.mTexture = item.mTexture ? item.mTexture->GetRS() : nullptr;
		TextureQuadItem::Vertex* vertices = itemData.mVertices;

		Vec2 origin = item.mPosition + item.mSize * item.mPivot;
		float angleRad = item.mRotation * DEG2RAD;

		//top left
		vertices[0].mPosition.mX = item.mPosition.mX - item.mShear.mX;
		vertices[0].mPosition.mY = item.mPosition.mY - item.mShear.mY;
		vertices[0].mUV = item.mUVTopLeft;
		//top right
		vertices[1].mPosition.mX = item.mPosition.mX + item.mSize.mX - item.mShear.mX;
		vertices[1].mPosition.mY = item.mPosition.mY + item.mShear.mY;
		vertices[1].mUV.mX = item.mUVBottomRight.mX;
		vertices[1].mUV.mY = item.mUVTopLeft.mY;
		//bottom left
		vertices[2].mPosition.mX = item.mPosition.mX + item.mShear.mX;
		vertices[2].mPosition.mY = item.mPosition.mY + item.mSize.mY - item.mShear.mY;
		vertices[2].mUV.mX = item.mUVTopLeft.mX;
		vertices[2].mUV.mY = item.mUVBottomRight.mY;
		//bottom right
		vertices[3].mPosition.mX = item.mPosition.mX + item.mSize.mX + item.mShear.mX;
		vertices[3].mPosition.mY = item.mPosition.mY + item.mSize.mY + item.mShear.mY;
		vertices[3].mUV = item.mUVBottomRight;

		for (unsigned i = 0; i < 4; i++)
		{
			vertices[i].mColor = item.mColor;
			vertices[i].mPosition = ScreenToNDC(URotatePoint(vertices[i].mPosition, angleRad, origin), mViewportSize);
		}

		
	}


	void Canvas::Render()
	{
		gGFX->SetBlendState(mAlphaBlend);
		gGFX->SetRasterizerState(mRasterState);
		gGFX->SetDepthStencilState(mDepthState);

		auto textureQuads = GetRTTextureQuads();

		///////////////////////////draw Texture Quads
		if (unsigned numQuad = textureQuads->Length())
		{
			gGFX->SetShaders(gVSCanvasNoUVSpace, gPSCanvasNoUVSpace);
			gGFX->SetInputLayout(mInputLayoutQuadTexture);
			gGFX->SetPrimitiveTopology(EPrimitiveTopology::ETriangleList);
			gGFX->SetIndexBuffer(mIndexBuffer);
			
			//filling vertex buffer with quads
			auto ptrMapped = gGFX->Map<TextureQuadItem::Vertex>(mVertexBuffer, EMapFlag::EWriteDiscard);
			for (unsigned iQuad = 0; iQuad < numQuad; iQuad++)
			{
				MemCopy(ptrMapped, textureQuads->ElementAt(iQuad)->mVertices, sizeof(TextureQuadItem::Vertex[4]));
				ptrMapped += 4;
			}
			gGFX->Unmap(mVertexBuffer);


			for (unsigned iQuad = 0; iQuad < numQuad; iQuad++)
			{
				gGFX->SetVertexBuffer(mVertexBuffer, 0, sizeof(TextureQuadItem::Vertex), iQuad * sizeof(TextureQuadItem::Vertex[4]));

				GFXTexture2D* textures[] = { GlobalResources::GetDefaultTexture2D() };
				GFXSamplerStateHandle samplers[] = { GlobalResources::GetDefaultSampler() };
				gGFX->SetResourceView(textures, 0, EShaderType::EPixel);
				gGFX->SetSamplerState(samplers, 0, EShaderType::EPixel);

				gGFX->DrawIndexed(6);
			}
			//test
// 			{
// 				this->TestTriangleScreen();
// 				auto ptrMapped = gGFX->Map<TextureQuadItem>(mVertexBuffer, EMapFlag::EWriteDiscard);
// 				*ptrMapped = mRTTextureQuads->LastElement();
// 				gGFX->Unmap(mVertexBuffer);
// 				GFXTexture2D* textures[] = { GlobalResources::GetDefaultTexture2D() };
// 				GFXSamplerStateHandle samplers[] = { GlobalResources::GetDefaultSampler() };
// 				gGFX->SetResourceView(textures, 0, EShaderType::EPixel);
// 				gGFX->SetSamplerState(samplers, 0, EShaderType::EPixel);
// 				gGFX->Draw(3);
// 			}
			textureQuads->RemoveAll();
		}



		//draw strings
		if(unsigned numText = mRTTextItems->Length())
		{
			
			mSpriteBatch->Begin();
			for (TextItem& textItem : *mGTTextItems)
			{
				XMFLOAT2 pos(textItem.mPosition.mX, textItem.mPosition.mY);
				mSpriteFont->DrawString(mSpriteBatch, textItem.mText, pos, ToXMVECTOR(textItem.mColor), 0, XMFLOAT2(0, 0), textItem.mScale * 0.5f);
			}
			
			mSpriteBatch->End();
			mRTTextItems->RemoveAll();

		}



	}

	void Canvas::Swap()
	{
		USCOPE_LOCK(mSwapLock);
		UPO::Swap(mGTTextItems, mRTTextItems);
		UPO::Swap(mGTTextureQuads, mRTTextureQuads);
	}

	Canvas::~Canvas()
	{
		EnqueueRenderCommandAndWait([this] {

			mInputLayoutQuadTexture = nullptr;

			SafeDelete(mVertexBuffer);
			SafeDelete(mSpriteBatch);
			SafeDelete(mSpriteFont);

		});

	}
	
	void Canvas::Resize(Vec2I newSize)
	{
		mViewportSize = Vec2(newSize.mX, newSize.mY);
	}

};