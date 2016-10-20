#include "UTestQuad.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	TestQuadRE::TestQuadRE(Renderer* rdr) : RendererElement(rdr)
	{
		mVShader = gGFX->GetShader("TestColorQuad.hlsl", "VSMain", EShaderType::EST_VERTEX);
		mPShader = gGFX->GetShader("TestColorQuad.hlsl", "PSMain", EShaderType::EST_PIXEL);

		Vec2 verts[] = { Vec2(0,0), Vec2(1,0), Vec2(0,1) };

		GFXVertexBuffer_Desc vbd;
		vbd.mInitialData = verts;
		vbd.mSize = sizeof(verts);

		mVBuffer = gGFX->CreateVertexBuffer(vbd);

		GFXVertexElement_Desc elemDisc[] = { { "POSITION", EPixelFormat::EPT_R32G32_FLOAT } };
		mInputLayout = gGFX->CreateInputLayout(elemDisc, 1, mVShader);
	}
	//////////////////////////////////////////////////////////////////////////
	TestQuadRE::~TestQuadRE()
	{

	}
	//////////////////////////////////////////////////////////////////////////
	void TestQuadRE::Present()
	{

		gGFX->BindShaders(mVShader, mPShader);
		gGFX->BinVertexBuffer(mVBuffer, sizeof(Vec2), 0);
		gGFX->BinIndexBuffer(nullptr);
		gGFX->BinInputLayout(mInputLayout);
		gGFX->SetPrimitiveTopology(EPT_TRIANGLELIST);

		gGFX->Draw(3, 0);
	}

};