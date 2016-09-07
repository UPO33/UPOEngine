#pragma once

#include "UGFXDevice.h"
#include "UGFXDeviceDX.h"

namespace UPO
{
	class UAPI ScreenRender
	{
	public:
		static ScreenRender* Get();
		
		static const unsigned MAX_VERTEX_COUNT = 6 * 256;

		struct VertexType
		{
			Vec2	mPosition;	//uv space
			Vec2	mUV;
		};

		void Init(GFXDevice* gfx);
		void DrawTexture(GFXTexture2D* texture, Vec2 pos, Vec2 size, Vec2 uvTopLeft = Vec2(0), Vec2 uvRightBottom = Vec2(1));

		void Frame();

		GFXDeviceDX* mGFX = nullptr;
		GFXShader* mVertexShader = nullptr;
		GFXShader* mPixelShader = nullptr;
		GFXVertexBuffer* mQuadBuffer = nullptr;
		GFXDepthStencilState* mDepthStencilState = nullptr;
		GFXSamplerState* mSampler = nullptr;
		GFXRasterizerState* mRaster = nullptr;
		GFXInputLayout* mLayout = nullptr;
		GFXTexture2D* mTexture = nullptr;
	};
};