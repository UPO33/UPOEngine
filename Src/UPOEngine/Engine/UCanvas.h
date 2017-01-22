#pragma once

#include "../Core/UBasic.h"
#include "../Core/UVector.h"
#include "../Core/UThreading.h"
#include "../Core/UArray.h"

#include "../GFXCore/UGlobalShader.h"

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
};


namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class ATexture2D;
	class ATexture2DRS;

	//////////////////////////////////////////////////////////////////////////
	struct CanvasTextureItem
	{
		ATexture2D*		mTexture = nullptr;
		Vec2			mPosition = Vec2::ZERO;
		Color			mColor = Color::WHITE;
		Vec2			mSize = Vec2(100);
		Vec2			mUVTopLeft = Vec2::ZERO;
		Vec2			mUVBottomRight = Vec2::ONE;
		Vec2			mShear = Vec2::ZERO;
		Vec2			mPivot = Vec2(0.5f);
		float			mRotation = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI Canvas
	{
	public:
		struct TextItem
		{
			static const unsigned MaxLen = 256;

			wchar_t			mText[MaxLen];
			Vec2			mPosition;
			Color			mColor;
			float			mScale;
		};
		struct TextureQuadItem
		{
			struct Vertex
			{
				Vec2	mPosition;
				Vec2	mUV;
				Color	mColor;

			}  mVertices[4];

			ATexture2DRS*	mTexture;
		};

		struct DebugTextItem
		{
			static const unsigned MaxLen = 256;

			wchar_t		mText[MaxLen];
			float		mLifeTimeSeconds;
			Color32		mColor;

		};

		TArray<TextItem>			mTextItems[2];
		TArray<TextItem>*			mRTTextItems;
		TArray<TextItem>*			mGTTextItems;

		TArray<TextItem>* GetGTTextItems() { return mGTTextItems; }
		TArray<TextItem>* GetRTTextItems() { return mRTTextItems; }

		TArray<TextureQuadItem>				mTextureQuads[2];
		volatile TArray<TextureQuadItem>*	mGTTextureQuads;
		volatile TArray<TextureQuadItem>*	mRTTextureQuads;

		TArray<DebugTextItem>	mDebugTextItems[2];
		TArray<DebugTextItem>*	mGTDebugTextItems;
		TArray<DebugTextItem>*	mRTDebugTextItems;

		TArray<TextureQuadItem>* GetGTTextureQuads() const { return (TArray<TextureQuadItem>*)mGTTextureQuads; }
		TArray<TextureQuadItem>* GetRTTextureQuads() const { return (TArray<TextureQuadItem>*)mRTTextureQuads; }

		class GFXVertexBuffer*		mVertexBuffer;	//vertex buffer to store quad textures
		class GFXInputLayout*		mLayout;	
		class GameWindow*			mViewport;
		CriticalSection				mSwapLock;	//lock for doing swap of render and game data
		Vec2						mViewportSize;

		GFXInputLayout*				mInputLayoutQuadTexture;
		GFXIndexBuffer*				mIndexBuffer;	//index buffer for a quad

		//global resources, don't delete
		GFXRasterizerStateHandle		mRasterState;
		GFXDepthStencilStateHandle		mDepthState;
		GFXBlendStateHandle				mAlphaBlend;

		DirectX::SpriteFont*		mSpriteFont = nullptr;
		DirectX::SpriteBatch*		mSpriteBatch = nullptr;

		GFXConstantBuffer*			mCBuffer = nullptr;

		struct CBData
		{
			Color mColor;
			Matrix4 mMatrix;
		};
		void CreateResources();

	public:
		Canvas(GameWindow* viewport);
		~Canvas();

		void Resize(Vec2I newSize);

		void DrawColorQuad(Vec2 position, Vec2 size, Color32 color);
		void DrawString(const String& text, Vec2 position, const Color& color, float scale = 1);
		void AddDebugString(const String& text, const Color32& color = Color32::GREEN, float lifeTimeSeconds = 2);

// 		void DrawTexture(ATexture2D* texture, Vec2 position, Vec2 size, Color color);
// 		void DrawTexture(ATexture2D* texture, Vec2 position, Vec2 size, Color color, Vec2 u);
		
		void Draw(const CanvasTextureItem& item);

		void RefineDebugTextItems(float deltaSeconds);

		void Render();
		void Swap();
	};
};