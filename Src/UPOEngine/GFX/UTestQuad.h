#pragma once

#include "URenderer.h"


namespace UPO
{
	UGLOBAL_SHADER_DECL(gVShaderTest, GFXVertexShader);
	UGLOBAL_SHADER_DECL(gPShaderTest, GFXPixelShader);

	class TestQuadRE : public RendererElement
	{
		GFXVertexBufferRef mVBuffer;
		GFXInputLayoutRef mInputLayout;
		GFXRasterizerStateRef mRSDisable;
public:
		TestQuadRE(Renderer* rdr);
		~TestQuadRE();

		void Present() override;
	};
};