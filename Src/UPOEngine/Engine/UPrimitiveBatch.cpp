#include "UPrimitiveBatch.h"
#include "UGameWindow.h"
#include "UHitSelection.h"

#include "../GFX/UShaderConstants.h"

#include "../Engine/UEngineBase.h"
#include "../Engine/UWorld.h"

namespace UPO
{
	UGLOBAL_SHADER_DECLIMPL2(gVSPrimitiveBatch, GFXVertexShader, "WorldPrimitiveBatch.hlsl", "VSMain");
	UGLOBAL_SHADER_DECLIMPL2(gPSPrimitiveBatch, GFXPixelShader, "WorldPrimitiveBatch.hlsl", "PSMain");

// 	UGLOBAL_SHADER_DECLIMPL2(gVSWireShpere, GFXVertexShader, "WorldPrimitiveBatch.hlsl", "VSWireSphere");
// 	UGLOBAL_SHADER_DECLIMPL2(gGSWireSphere, GFXGeometryShader, "WorldPrimitiveBatch.hlsl", "GSWireSphere");
// 	UGLOBAL_SHADER_DECLIMPL2(gVSWireCircle, GFXVertexShader, "WorldPrimitiveBatch.hlsl", "VSWireCircle");
// 
	static const unsigned MaxLineInVertexBuffer = 8000;

	PrimitiveBatch::PrimitiveBatch(World* owner)
	{
		mOwner = owner;
		mDelta = 0;
		mHitProxy = nullptr;

		mGTLines = mLines + 0;
		mRTLines = mLines + 1;

		mRTMeshes = mMeshes + 0;
		mGTMeshes = mMeshes + 1;

		mGTSphere = mShperes + 0;
		mRTSphere = mShperes + 1;

		EnqueueRenderCommandAndWait([this]() {
			CreateRenderResoures();
		});
		

	}

	void PrimitiveBatch::DrawLine(const Vec3& point0, const Color32& color0, const Vec3& point1, const Color32& color1)
	{
		auto lines = GetGTLines();
		auto index = lines->AddUnInit(2);
		LineVertex* vertices = lines->ElementAt(index);

		vertices[0].mPosition = point0;
		vertices[0].mColor = color0;
		vertices[0].mHitID = mHitProxyID;

		vertices[1].mPosition = point1;
		vertices[1].mColor = color1;
		vertices[1].mHitID = mHitProxyID;
	}



	void PrimitiveBatch::DrawLine(const Vec3& point0, const Vec3& point1, const Color32& color)
	{
		DrawLine(point0, color, point1, color);
	}

	void PrimitiveBatch::DrawWireBox(const Vec3& min, const Vec3& max, const Color32& color)
	{
		DrawLine(Vec3(min.mX, min.mY, min.mZ), color, Vec3(min.mX, max.mY, min.mZ), color);
		DrawLine(Vec3(max.mX, min.mY, min.mZ), color, Vec3(max.mX, max.mY, min.mZ), color);
		DrawLine(Vec3(max.mX, min.mY, max.mZ), color, Vec3(max.mX, max.mY, max.mZ), color);
		DrawLine(Vec3(min.mX, min.mY, max.mZ), color, Vec3(min.mX, max.mY, max.mZ), color);
		
		DrawLine(Vec3(min.mX, min.mY, min.mZ), color, Vec3(min.mX, min.mY, max.mZ), color);
		DrawLine(Vec3(min.mX, max.mY, min.mZ), color, Vec3(min.mX, max.mY, max.mZ), color);

		DrawLine(Vec3(max.mX, min.mY, min.mZ), color, Vec3(max.mX, min.mY, max.mZ), color);
		DrawLine(Vec3(max.mX, max.mY, min.mZ), color, Vec3(max.mX, max.mY, max.mZ), color);


		DrawLine(Vec3(min.mX, min.mY, min.mZ), color, Vec3(max.mX, min.mY, min.mZ), color);
		DrawLine(Vec3(min.mX, min.mY, max.mZ), color, Vec3(max.mX, min.mY, max.mZ), color);

		DrawLine(Vec3(min.mX, max.mY, min.mZ), color, Vec3(max.mX, max.mY, min.mZ), color);
		DrawLine(Vec3(min.mX, max.mY, max.mZ), color, Vec3(max.mX, max.mY, max.mZ), color);

	}

	void PrimitiveBatch::DrawWireBox(const Vec3& extent, const Transform& transform, const Color32& color)
	{
	}

	void PrimitiveBatch::DrawWireMesh(const AStaticMesh* mesh, const Transform& transform, const Color& color, float lifeTimeSeconds)
	{
		if (!mesh) return;

		USCOPE_LOCK(mSwapLock);
		
		auto meshes = GetGTMeshes();
		meshes->AddUnInit();
		auto& item = meshes->LastElement();
		
		item.mMesh = mesh;
		item.mTransform = transform;
		item.mColor = color;
		item.mLifeTime = lifeTimeSeconds;
		
	}

	void PrimitiveBatch::DrawWireFrustum(const Matrix4& frustum, const Color32& color)
	{
		Vec3 fars[4];
		Vec3 nears[4];

		for (size_t i = 0; i < 4; i++)
		{
			const Vec2 coords[] = {
				Vec2(-1, 1),
				Vec2(1, 1),
				Vec2(1, -1),
				Vec2(-1, -1)
			};

			Vec4 vFar = frustum.TransformVec4(Vec4(coords[i], Vec2(1, 1)));
			Vec4 vNear = frustum.TransformVec4(Vec4(coords[i], Vec2(0, 1)));

			fars[i] = Vec3(vFar) / vFar.mW;
			nears[i] = Vec3(vNear) / vNear.mW;
		}

		for (size_t i = 0; i < 4; i++)
		{
			DrawLine(fars[i], fars[(i + 1) % 4], color);
			DrawLine(nears[i], nears[(i + 1) % 4], color);
			DrawLine(nears[i], fars[i], color);
		}
	}


	void PrimitiveBatch::DrawWireSphere(const Vec3& position, float radius, Color32 color)
	{
		DrawCircle(position, radius, color, Vec3(1, 0, 0), Vec3(0, 0, 1));
		DrawCircle(position, radius, color, Vec3(1, 0, 0), Vec3(0, 1, 0));
		DrawCircle(position, radius, color, Vec3(0, 0, 1), Vec3(0, 1, 0));
	}

	void PrimitiveBatch::CreateRenderResoures()
	{
		UASSERT(IsRenderThread());
		//vertex buffer
		{
			GFXVertexBufferDesc vbdesc;
			vbdesc.mSize = MaxLineInVertexBuffer * sizeof(LineVertex[2]);
			vbdesc.mDynamic = true;

			mLinesBuffer = gGFX->CreateVertexBuffer(vbdesc);
			UASSERT(mLinesBuffer);
		}
		//input layout
		{
			GFXInputLayoutDesc ild = {
				gVSPrimitiveBatch->Get(ShaderConstantsCombined()),
				{
					GFXVertexElementDesc("POSITION", EVertexFormat::EFloat3),
					GFXVertexElementDesc("COLOR", EVertexFormat::EColor32),
					GFXVertexElementDesc("HITID", EVertexFormat::EUInt32),
				},
			};

			mInputLayout = GlobalResources::GetInputLayout(ild);
			UASSERT(mInputLayout);
		}
		{
// 			GFXInputLayoutDesc desc = 
// 			{
// 				gVSWireShpere->Get(ShaderConstantsCombined()),
// 				{
// 					GFXVertexElementDesc("POSITION", EVertexFormat::EFloat3),
// 					GFXVertexElementDesc("RADIUS", EVertexFormat::EFloat1),
// 					GFXVertexElementDesc("COLOR", EVertexFormat::EColor32),
// 					GFXVertexElementDesc("HITID", EVertexFormat::EUInt32)
// 				},
// 			};
// 
// 			 mILWireSphere = gGFX->CreateInputLayout(desc);
// 			 UASSERT(mILWireSphere);
		}
		//cbuffer
		{
			mConstantBuffer = gGFX->CreateConstantBuffer(sizeof(Matrix4));
			UASSERT(mConstantBuffer);
		}



	}



	void PrimitiveBatch::DrawCircle(const Vec3& position, float radius, Color32 color, const Vec3& xAxis, const Vec3& zAxis)
	{
		float ss, cc;
		SinCosDeg(0, ss, cc);
		Vec3 v1 = position + (xAxis * ss * radius) + (zAxis * cc * radius);

		for (size_t i = 1; i <= 40; i++)
		{
			const float f = 360.0 / 40;
			SinCosDeg(i * f, ss, cc);
			Vec3 v2 = position + (xAxis * ss * radius) + (zAxis * cc * radius);
			DrawLine(v1, v2, color);
			v1 = v2;
		}
	}

	void PrimitiveBatch::Tick(float delta)
	{

	}


	void PrimitiveBatch::Render()
	{
		UASSERT(IsRenderThread());
		

		LineVertexArray* vertices = GetRTLines();
		

		unsigned numVertex = vertices->Length();
		if (numVertex == 0) return;

		//update vertex buffer
		{
			void* ptrMapped = gGFX->Map(mLinesBuffer, EMapFlag::EWriteDiscard);
			MemCopy(ptrMapped, vertices->Elements(), sizeof(LineVertex) * numVertex);
			gGFX->Unmap(mLinesBuffer);

			vertices->RemoveAll();
		}


		gGFX->SetBlendState(nullptr);
		gGFX->SetVertexBuffer(mLinesBuffer, 0, sizeof(LineVertex), 0);
		gGFX->SetIndexBuffer(nullptr);
		gGFX->SetInputLayout(mInputLayout);
		
		//selecting shader
		ShaderConstantsCombined sc;
		if (mOwner->GetGameWindow()->GetHitSelection())
			sc |= ShaderConstants::USE_HITSELECTION;


		gGFX->SetShaders(gVSPrimitiveBatch->Get(sc), nullptr, nullptr, nullptr, gPSPrimitiveBatch->Get(sc));
		

		gGFX->SetPrimitiveTopology(EPrimitiveTopology::ELineList);
		gGFX->Draw(numVertex);

// 		for(size_t i = 0; i < mRTSphere->Length(); i++)
// 		{
// 			gGFX->SetInputLayout(mILWireSphere);
// 			gGFX->SetIndexBuffer(nullptr);
// 			{
// 				auto mapped = gGFX->Map<ShpereItem>(mLinesBuffer, EMapFlag::EWriteDiscard);
// 				*mapped = *mRTSphere->ElementAt(i);
// 				gGFX->Unmap(mLinesBuffer);
// 			}
// 			gGFX->SetVertexBuffer(mLinesBuffer, 0, sizeof(ShpereItem));
// 			gGFX->SetShaders(gVSWireShpere->Get(sc), nullptr, nullptr, gGSWireSphere->Get(sc), gPSPrimitiveBatch->Get(sc));
// 			gGFX->SetPrimitiveTopology(EPrimitiveTopology::EPointList);
// 			gGFX->Draw(1);
// 		}
// 
// 		mRTSphere->RemoveAll();


		{
			StaticMeshItemArray* meshes = GetRTMeshes();
			///////////////////////// meshes
			if (meshes->Length())
			{
				for (auto& mesh : *meshes)
				{
					//draw...
				}

				float delta = mDelta;
				mMeshes->RemoveIf([delta](StaticMeshItem& item) {
					item.mLifeTime -= delta;
					return (item.mLifeTime <= 0);
				});
			}
		}
	}

	PrimitiveBatch::~PrimitiveBatch()
	{
		EnqueueRenderCommandAndWait([this]() {
			SafeDelete(mLinesBuffer);
			SafeDelete(mConstantBuffer);
		});
	}

	void PrimitiveBatch::Swap()
	{
		UPO::Swap(mGTLines, mRTLines);
		UPO::Swap(mGTSphere, mRTSphere);

		{
			*mRTMeshes += *mGTMeshes;
			mGTMeshes->RemoveAll();
		}
	}

	void PrimitiveBatch::SetHitProxy(HPBaseRef hp)
	{
		mHitProxy = hp;
		mHitProxyID = mOwner->GetGameWindow()->GetHitSelection()->RegisterProxy(mHitProxy);
	}

	HPBase* PrimitiveBatch::GetHitProxy() const
	{
		return mHitProxy;
	}

};