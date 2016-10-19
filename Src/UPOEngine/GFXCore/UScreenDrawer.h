#pragma once

#include "UGFXDevice.h"
#include "UGFXDeviceDX.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////fwd decl
	class GFXRenderer;

	//////////////////////////////////////////////////////////////////////////
	class UAPI ScreenDrawer
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

		bool Init(GFXRenderer*);
		void Release();

		void DrawTexture(GFXTexture2D* texture, Vec2 pos, Vec2 size, Color color, Vec2 uvTopLeft = Vec2(0), Vec2 uvRightBottom = Vec2(1));

		void Frame();

		GFXRenderer*					mRenderer = nullptr;
		GFXShader*						mVertexShader = nullptr;
		GFXShader*						mPixelShader = nullptr;
		GFXVertexBuffer*				mQuadBuffer = nullptr;
		GFXDepthStencilState*			mDepthStencilState = nullptr;
		GFXSamplerState*				mSampler = nullptr;
		GFXRasterizerState*				mRaster = nullptr;
		GFXInputLayout*					mLayout = nullptr;
		GFXBlendState*					mAlphaBlend = nullptr;
		GFXConstantBuffer*				mCBPerObject = nullptr;
		GFXTexture2D*					mTextureTile1 = nullptr;
		GFXTexture2D*					mTextureTile2 = nullptr;
	};
};