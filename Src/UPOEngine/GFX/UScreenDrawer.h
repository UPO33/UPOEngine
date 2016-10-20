#pragma once

#include "URenderer.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////fwd decl
	class Renderer;

	//////////////////////////////////////////////////////////////////////////
	class UAPI ScreenDrawer : public RendererElement
	{
	public:
		static const unsigned MAX_VERTEX_COUNT = 6 * 256;

		struct VertexType
		{
			Vec2	mPosition;	//uv space
			Vec2	mUV;
		};

		struct CBPerObject
		{
			Color mColor;
		};

		ScreenDrawer(Renderer* rdr);
		~ScreenDrawer();

		void DrawTexture(GFXTexture2D* texture, Vec2 pos, Vec2 size, Color color, Vec2 uvTopLeft = Vec2(0), Vec2 uvRightBottom = Vec2(1));

		void Present() override;

		Renderer*						mRenderer = nullptr;

		GFXShaderRef					mVertexShader = nullptr;
		GFXShaderRef					mPixelShader = nullptr;
		GFXVertexBufferRef				mQuadBuffer = nullptr;
		GFXDepthStencilStateRef			mDepthStencilState = nullptr;
		GFXSamplerStateRef				mSampler = nullptr;
		GFXRasterizerStateRef			mRaster = nullptr;
		GFXInputLayoutRef				mLayout = nullptr;
		GFXBlendStateRef				mAlphaBlend = nullptr;
		GFXConstantBufferRef			mCBPerObject = nullptr;
	};
};