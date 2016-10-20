#pragma once

#include "URenderer.h"


namespace UPO
{
	class TestQuadRE : public RendererElement
	{
		GFXShaderRef mVShader;
		GFXShaderRef mPShader;
		GFXVertexBufferRef mVBuffer;
		GFXInputLayoutRef mInputLayout;

public:
		TestQuadRE(Renderer* rdr);
		~TestQuadRE();

		void Present() override;
	};
};