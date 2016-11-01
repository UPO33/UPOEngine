#pragma once

#include "URenderer.h"


namespace UPO
{
	UDECL_GLOBAL_SHADER(gVShaderTest, GFXVertexShader);
	UDECL_GLOBAL_SHADER(gPShaderTest, GFXPixelShader);

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