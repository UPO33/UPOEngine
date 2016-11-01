#include "UTestQuad.h"

namespace UPO
{
	UIMPL_GLOBAL_SHADER(gVShaderTest, "TestColorQuad.hlsl", "VSMain", { "TEST_COLOR", "float4(1,1,0,0)" });
	UIMPL_GLOBAL_SHADER(gPShaderTest, "TestColorQuad.hlsl", "PSMain", { "TEST_COLOR", "float4(1,1,0,0)" });



	//////////////////////////////////////////////////////////////////////////
	TestQuadRE::TestQuadRE(Renderer* rdr) : RendererElement(rdr)
	{
		GFXRasterizerState_Desc rsd;
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

		gGFX->BindShaders(gVShaderTest, gPShaderTest);
		//gGFX->BinVertexBuffer(mVBuffer, sizeof(Vec2), 0);
		//gGFX->BinIndexBuffer(nullptr);
		//gGFX->BinInputLayout(mInputLayout);
		gGFX->SetPrimitiveTopology(EPrimitiveTopology::EPT_TRIANGLELIST);
		//gGFX->BinInputLayout(nullptr);
		//gGFX->SetRasterizer(mRSDisable);
		gGFX->Draw(3, 0);
	}

};