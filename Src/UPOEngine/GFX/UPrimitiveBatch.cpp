#include "UPrimitiveBatch.h"

namespace UPO
{
	UDECL_GLOBAL_SHADER(gVSPrimitiveBatch, GFXVertexShader);
	UDECL_GLOBAL_SHADER(gPSPrimitiveBatch, GFXPixelShader);
	UIMPL_GLOBAL_SHADER(gVSPrimitiveBatch, "WorldPrimitiveBatch.hlsl", "VSMain");
	UIMPL_GLOBAL_SHADER(gPSPrimitiveBatch, "WorldPrimitiveBatch.hlsl", "PSMain");


	PrimitiveBatch::PrimitiveBatch()
	{
		CreateRenderResoures();
	}

	void PrimitiveBatch::DrawLine(const Vec3& point0, const Color32& color0, const Vec3& point1, const Color32& color1)
	{
		mLines[mGSIndex].AddUnInit();
		auto& elem = mLines->LastElement();

		elem.mPosition0 = point0;
		elem.mColor0 = color0;
		elem.mPosition1 = point1;
		elem.mColor1 = color1;
	}

	void PrimitiveBatch::DrawWireBox(const Vec3& min, const Vec3& max, const Color32& color)
	{

	}

	void PrimitiveBatch::CreateRenderResoures()
	{
		UASSERT(IsRenderThread());

		GFXVertexBuffer_Desc vbdesc;
		vbdesc.mSize = MAX_LINE_PER_FRAME * sizeof(Line);
		vbdesc.mUsage = EResourceUsage::EDynamic;

		mLinesBuffer = gGFX->CreateVertexBuffer(vbdesc);
		UASSERT(mLinesBuffer);

		GFXInputLayoutDesc ild = {
			gVSPrimitiveBatch,
			{
				GFXVertexElementDesc("POSITION", EVertexFormat::EFloat3),
				GFXVertexElementDesc("COLOR", EVertexFormat::EFloat4),
			},
		};

		mInputLayout = gGFX->CreateInputLayout(ild);
		UASSERT(mInputLayout);
	}



	void PrimitiveBatch::Swap()
	{
		mGSIndex ^= 1;
		mLines[mGSIndex].RemoveAll();
	}

	void PrimitiveBatch::Render()
	{
		UASSERT(IsRenderThread());

		auto& linedData = mLines[mGSIndex ^ 1];

		unsigned numLine = linedData.Length();
		if (numLine == 0) return;

		void* ptrMapped = mLinesBuffer->Map(EMapFlag::EWriteDiscard);
		MemCopy(ptrMapped, linedData.Elements(), sizeof(PrimitiveBatch::Line) * linedData.Length());
		mLinesBuffer->Unmap();


		gGFX->BinVertexBuffer(mLinesBuffer, sizeof(PrimitiveBatch::Line) / 2, 0);
		gGFX->BinIndexBuffer(nullptr);
		gGFX->BinInputLayout(mInputLayout);
		gGFX->BindShaders(gVSPrimitiveBatch, gPSPrimitiveBatch);
		gGFX->SetPrimitiveTopology(EPrimitiveTopology::EPT_LINELIST);
		gGFX->Draw(numLine * 2);

	}

	PrimitiveBatch::~PrimitiveBatch()
	{

	}

};