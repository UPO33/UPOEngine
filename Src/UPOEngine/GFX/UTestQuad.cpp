#include "UTestQuad.h"

namespace UPO
{
	UGLOBAL_SHADER_IMPL(gVShaderTest, "TestColorQuad.hlsl", "VSMain", { "TEST_COLOR", "float4(1,1,0,0)" });
	UGLOBAL_SHADER_IMPL(gPShaderTest, "TestColorQuad.hlsl", "PSMain", { "TEST_COLOR", "float4(1,1,0,0)" });



	//////////////////////////////////////////////////////////////////////////
	TestQuadRE::TestQuadRE(Renderer* rdr) : RendererElement(rdr)
	{
		GFXRasterizerStateDesc rsd;
		rsd.mCullMode = ECullMode::ENone;

		mRSDisable = gGFX->CreateRasterizerState(rsd);
		
		
	}
	//////////////////////////////////////////////////////////////////////////
	TestQuadRE::~TestQuadRE()
	{

	}
	//////////////////////////////////////////////////////////////////////////
	void TestQuadRE::Present()
	{

		gGFX->SetShaders(gVShaderTest, gPShaderTest);
		//gGFX->BinVertexBuffer(mVBuffer, sizeof(Vec2), 0);
		//gGFX->BinIndexBuffer(nullptr);
		//gGFX->BinInputLayout(mInputLayout);
		gGFX->SetPrimitiveTopology(EPrimitiveTopology::ETriangleList);
		//gGFX->BinInputLayout(nullptr);
		//gGFX->SetRasterizer(mRSDisable);
		gGFX->Draw(3, 0);
	}

};