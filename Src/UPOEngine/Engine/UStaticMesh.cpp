#include "UStaticMesh.h"
#include "../Meta/UMeta.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace UPO
{
	inline Vec2 FromAssimp(const aiVector2D& in)
	{
		return Vec2(in.x, in.y);
	}
	inline Vec3 FromAssimp(const aiVector3D& in)
	{
		return Vec3(in.x, in.y, in.z);
	}

#if 0
	struct AVertexPackRS
	{
		class GFXVertexBuffer*	mVB;
		class GFXIndexBuffer* mIB;
		unsigned	mVCount;
		unsigned	mICount;
	};
	class AVertexPack : public Asset
	{
		TArray<Vec3>	mVertices;
		AVertexPackRS*	mRS;

		void OnInit() override
		{
			EnqueueRenderCommend([this]() {
				AVertexPackRS* rs = new AVertexPackRS;
				GFXVertexBuffer_Desc desc;
				{
					USCOPE_LOCK(mEditingLock);
					desc.mInitialData = mVertices.Elements();
					desc.mSize = mVertices.Length() * sizeof(Vec3);
					rs->mVB = gGFX->CreateVertexBuffer(desc);
					mRS = rs;
				}
			});
		}
		bool IsRenderDataReady() const
		{
			return mRS != nullptr;
		}
		void OnRelease() override
		{
			EnqueueRenderCommend([mRS]() {
				if (mRS)
				{
					delete mRS;
				}
			});
		}
		void MetaAfterPropertyChange(const PropertyInfo*)
		{
			EnqueueRenderCommend([this]() {
				USCOPE_LOCK();
				AVertexPackRS* rs = new AVertexPackRS;
				rs->mVB = gGFX->CreateVertexBuffer(GFXVertexBuffer_Desc());
			});
		}
		void MetaBeforePropertyChange(const PropertyInfo*)
		{
			USCOPE_LOCK(mEditingLock);
		}
	};
#endif
	//////////////////////////////////////////////////////////////////////////


	UCLASS_BEGIN_IMPL(AStaticMesh)
		UPROPERTY(mContent)
		UPROPERTY(mFlipUV)
		UPROPERTY(mGenerateSmoothNormal)
	UCLASS_END_IMPL(AStaticMesh)

	
	void AStaticMesh::OnCreate()
	{
		Assimp::Importer importer = Assimp::Importer();
		unsigned flag = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords | aiProcess_FlipUVs;
		const aiScene* scene = importer.ReadFileFromMemory(mContent.Data(), mContent.Size(), flag);
		if (scene == nullptr)
		{
			ULOG_ERROR("failed to lead StaticMesh");
		}
	}

	void AStaticMesh::OnDestroy()
	{

	}

	void AStaticMesh::MetaBeforePropertyChange(const PropertyInfo* prp)
	{

	}

	void AStaticMesh::MetaAfterPropertyChange(const PropertyInfo* prp)
	{
		EnqueueRenderCommandAndWait([]() {

		});
	}

};