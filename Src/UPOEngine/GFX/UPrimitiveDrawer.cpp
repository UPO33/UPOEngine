#include "UPrimitiveDrawer.h"

#if 0
namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	void PrimitiveDrawer::DrawLine(const Vec3& p0, const Vec3& p1, Color32 color)
	{
		if (mNumLineVertex >= MAX_LINE_IN_BUFFER) return;

		mLineVertices[mNumLineVertex].position = p0;
		mLineVertices[mNumLineVertex].color = color;

		mLineVertices[mNumLineVertex + 1].position = p1;
		mLineVertices[mNumLineVertex + 1].color = color;

		mNumLineVertex += 2;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PrimitiveDrawer::Init(Renderer* renderer)
	{
		mRenderer = renderer;

		mVSLineDraw = gGFX->GetShader("LineDrawer.hlsl", "VSMain", EST_VERTEX);
		mPSLineDraw = gGFX->GetShader("LineDrawer.hlsl", "PSMain", EST_PIXEL);

		GFXVertexBuffer_Desc vbdesc;
		vbdesc.mSize = sizeof(LineVertex) * MAX_LINE_IN_BUFFER;
		vbdesc.mUsage = EResourceUsage::EBU_DYNAMIC;
		mLineBuffer = gGFX->CreateVertexBuffer(vbdesc);

		GFXVertexElement_Desc vertexElements[]
		{
			GFXVertexElement_Desc("POSITION", EPixelFormat::EPT_R32G32B32_FLOAT),
			GFXVertexElement_Desc("COLOR", EPixelFormat::EPT_R8G8B8A8_UNORM),
		};
		mLineDrawLayout = gGFX->CreateInputLayout(vertexElements, ARRAYSIZE(vertexElements), mVSLineDraw);

		if (mVSLineDraw && mPSLineDraw && mLineBuffer && mLineDrawLayout) return true;
		Release();
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	void PrimitiveDrawer::Release()
	{
		if (mLineDrawLayout) mLineDrawLayout->Release();
		if (mLineBuffer) mLineBuffer->Release();
		if (mVSLineDraw) mVSLineDraw->Release();
		if (mPSLineDraw) mPSLineDraw->Release();
		
		mLineDrawLayout = nullptr;
		mLineBuffer = nullptr;
		mVSLineDraw = nullptr;
		mPSLineDraw = nullptr;

		mRenderer = nullptr;

		mNumLineVertex = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	void PrimitiveDrawer::Frame()
	{
		Test();

		void* mapped = mLineBuffer->Map(EMapFlag::EMF_WRITE_DISCARD);
		MemCopy(mapped, mLineVertices, mNumLineVertex * sizeof(LineVertex));
		mLineBuffer->Unmap();

		gGFX->BindShaders(mVSLineDraw, mPSLineDraw);
		gGFX->BinVertexBuffer(mLineBuffer, sizeof(LineVertex), 0);
		gGFX->SetPrimitiveTopology(EPrimitiveTopology::EPT_LINELIST);
		gGFX->BinInputLayout(mLineDrawLayout);
		gGFX->Draw(mNumLineVertex);

		mNumLineVertex = 0;
	}
	//////////////////////////////////////////////////////////////////////////
	void PrimitiveDrawer::Test()
	{
		DrawLine(Vec3(0), Vec3(1000, 0, 0), Color32::RED);
		DrawLine(Vec3(0), Vec3(0, 1000, 0), Color32::GREEN);
		DrawLine(Vec3(0), Vec3(0, 0, 1000), Color32::BLUE);
	}

};
#endif