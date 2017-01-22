#include "UPrimitiveBatch.h"
#include "../Engine/UEngineBase.h"
#include "../Engine/UWorld.h"

namespace UPO
{
	UGLOBAL_SHADER_DECL(gVSPrimitiveBatch, GFXVertexShader);
	UGLOBAL_SHADER_DECL(gPSPrimitiveBatch, GFXPixelShader);
	UGLOBAL_SHADER_IMPL(gVSPrimitiveBatch, "WorldPrimitiveBatch.hlsl", "VSMain");
	UGLOBAL_SHADER_IMPL(gPSPrimitiveBatch, "WorldPrimitiveBatch.hlsl", "PSMain");


	static const unsigned MaxLineInVertexBuffer = 8000;

	PrimitiveBatch::PrimitiveBatch(World* owner)
	{
		mOwner = owner;
		mDelta = 0;

		mGTLines = mLines + 0;
		mRTLines = mLines + 1;

		mRTMeshes = mMeshes + 0;
		mGTMeshes = mMeshes + 1;

		EnqueueRenderCommandAndWait([this]() {
			CreateRenderResoures();
		});
		
	}

	void PrimitiveBatch::DrawLine(const Vec3& point0, const Color32& color0, const Vec3& point1, const Color32& color1)
	{
		USCOPE_LOCK(mSwapLock);
		auto lines = GetGTLines();
		auto index = lines->AddUnInit(2);
		LineVertex* vertices = lines->ElementAt(index);

		vertices[0].mPosition = point0;
		vertices[0].mColor = color0;
		vertices[1].mPosition = point1;
		vertices[1].mColor = color1;

	}

	void PrimitiveBatch::DrawWireBox(const Vec3& min, const Vec3& max, const Color32& color)
	{
		DrawLine(min, color, max, color);
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
				gVSPrimitiveBatch,
				{
					GFXVertexElementDesc("POSITION", EVertexFormat::EFloat3),
					GFXVertexElementDesc("COLOR", EVertexFormat::EColor32),
				},
			};

			mInputLayout = GlobalResources::GetInputLayout(ild);
			UASSERT(mInputLayout);
		}
		//cbuffer
		{
			mConstantBuffer = gGFX->CreateConstantBuffer(sizeof(Matrix4));
			UASSERT(mConstantBuffer);
		}

	}



	void PrimitiveBatch::Tick(float delta)
	{
		USCOPE_LOCK(mSwapLock);

		mDelta = delta;

		UPO::Swap(mGTLines, mRTLines);

		{
			*mRTMeshes += *mGTMeshes;
			mGTMeshes->RemoveAll();
		}
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

		

		//update cbuffer
		if(0)
		{
			Matrix4 testWorldToCLip, matProj, matView, matWorld;
			matWorld.MakeScale(1);
			matProj.MakePerspective(60, 1, 0.1f, 1000);
			matView.MakeViewLookAt(Vec3(-10, 10, -10), Vec3(0), Vec3(0, 1, 0));
			//matView = Matrix4::IDENTITY;
#if 1
			testWorldToCLip =  matWorld * matView * matProj;
			//testWorldToCLip.Transpose();
#else
			testWorldToCLip = matProj * matView;
#endif 

			void* ptrMapped = gGFX->Map(mConstantBuffer, EMapFlag::EWriteDiscard);
			MemCopy(ptrMapped, &testWorldToCLip, sizeof(Matrix4));
			gGFX->Unmap(mConstantBuffer);
		}

		gGFX->SetBlendState(nullptr);
		gGFX->SetVertexBuffer(mLinesBuffer, 0, sizeof(LineVertex), 0);
		gGFX->SetIndexBuffer(nullptr);
		gGFX->SetConstentBuffer(mConstantBuffer, 0, EShaderType::EVertex);
		gGFX->SetInputLayout(mInputLayout);
		gGFX->SetShaders(gVSPrimitiveBatch, gPSPrimitiveBatch);
		gGFX->SetPrimitiveTopology(EPrimitiveTopology::ELineList);
		gGFX->Draw(numVertex);


		{
			USCOPE_LOCK(mSwapLock);

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

};