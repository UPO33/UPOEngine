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
	inline XMVECTOR ToXMVECTOR(const Color32& color)
	{
		return XMLoadFloat4((XMFLOAT4*)(Color(color).mRGBA));
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
		//cbuffer
		{
			mCBuffer = gGFX->CreateConstantBuffer(sizeof(CBData));
			UASSERT(mCBuffer);
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

		mGTDebugTextItems = mDebugTextItems + 0;
		mRTDebugTextItems = mDebugTextItems + 1;


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

	void Canvas::AddDebugString(const String& text, const Color32& color, float lifeTimeSeconds)
	{
		USCOPE_LOCK(mSwapLock);

		auto textsItems = mGTDebugTextItems;
		textsItems->AddUnInit();
		auto& item = textsItems->LastElement();
		item.mColor = color;
		item.mLifeTimeSeconds = lifeTimeSeconds;
		size_t numCharConverted = 0;
		mbstowcs_s(&numCharConverted, item.mText, text.CStr(), DebugTextItem::MaxLen);
		
	}

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


	void Canvas::RefineDebugTextItems(float deltaSeconds)
	{


	}

	void Canvas::Render()
	{
		gGFX->SetBlendState(mAlphaBlend);
		gGFX->SetRasterizerState(mRasterState);
		gGFX->SetDepthStencilState(mDepthState);

		mSwapLock.Enter();
		auto textureQuads = GetRTTextureQuads();
		auto textItems = GetRTTextItems();
		auto debugTextItems = mRTDebugTextItems;
		mSwapLock.Leave();

		///////////////////////////draw Texture Quads
		if (unsigned numQuad = textureQuads->Length())
		{
			gGFX->SetShaders(gVSCanvasNoUVSpace, gPSCanvasNoUVSpace);
			gGFX->SetInputLayout(mInputLayoutQuadTexture);
			gGFX->SetPrimitiveTopology(EPrimitiveTopology::ETriangleList);
			gGFX->SetIndexBuffer(mIndexBuffer);


			//cbuffer
			{
				auto mapped = gGFX->Map<CBData>(mCBuffer, EMapFlag::EWriteDiscard);
				mapped->mColor = Color(RandFloat01(), 0, 0, 1);
				mapped->mMatrix = Matrix4::IDENTITY;
				gGFX->Unmap(mCBuffer);
			}

			gGFX->SetConstentBuffer(mCBuffer, 0, EShaderType::EVertex);
			gGFX->SetConstentBuffer(mCBuffer, 0, EShaderType::EPixel);

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

				GFXTexture2D* texturesToBind[] = {  GlobalResources::GetDefaultTexture2D() };
				GFXSamplerStateHandle samplersToBind[] = { GlobalResources::GetDefaultSampler() };
				if (ATexture2DRS* texture = textureQuads->ElementAt(iQuad)->mTexture)
				{
					if (texture->mTexture) texturesToBind[0] = texture->mTexture;
					if (texture->mSampler) samplersToBind[0] = texture->mSampler;
				}

				gGFX->SetResourceView(texturesToBind, 0, EShaderType::EPixel);
				gGFX->SetSamplerState(samplersToBind, 0, EShaderType::EPixel);

				gGFX->DrawIndexed(6);
			}
			textureQuads->RemoveAll();
		}



		///////////////draw strings
		if(unsigned numText = textItems->Length())
		{
			mSpriteBatch->Begin();
			for (TextItem& textItem : *textItems)
			{
				XMFLOAT2 pos(textItem.mPosition.mX, textItem.mPosition.mY);
				mSpriteFont->DrawString(mSpriteBatch, textItem.mText, pos, ToXMVECTOR(textItem.mColor), 0, XMFLOAT2(0, 0), textItem.mScale * 0.5f);
			}

			mSpriteBatch->End();
			textItems->RemoveAll();

		}

		///////////draw debug text
		if (unsigned numDebugText = debugTextItems->Length())
		{
			mSpriteBatch->Begin();
			XMFLOAT2 position = XMFLOAT2(2, 40);
			float scale = 0.4f;
			float height = XMVectorGetY(mSpriteFont->MeasureString(L"QWERTY")) * scale;
			for (auto& item : *debugTextItems)
			{
				position.y += height;
				XMVECTOR color = ToXMVECTOR(item.mColor);
				mSpriteFont->DrawString(mSpriteBatch, item.mText, position, color, 0, XMFLOAT2(0, 0), scale);
			}
			mSpriteBatch->End();

			float delta = gDeltaSeconds;
			mRTDebugTextItems->RemoveIf([delta](DebugTextItem& item) {
				item.mLifeTimeSeconds -= delta;
				return (item.mLifeTimeSeconds <= 0);
			});
		}
	}

	void Canvas::Swap()
	{
		USCOPE_LOCK(mSwapLock);
		UPO::Swap(mGTTextItems, mRTTextItems);
		UPO::Swap(mGTTextureQuads, mRTTextureQuads);

		{
			*mRTDebugTextItems += *mGTDebugTextItems;
			mGTDebugTextItems->RemoveAll();
		}
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