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
	};
	class AVertexPack : public Asset
	{
		TArray<Vec3>	mVertices;
		AVertexPackRS*	mRS;

		void OnInit()
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
		void OnRelease()
		{
			EnqueueRenderCommend([this]() {
				if (mRS)
				{
					delete mRS;
					mRS = nullptr;
				}
				EnqueueRenderCommend([this]() {

				});
			});
		}
		void MetaPropertyChanged(PropertyInfo*)
		{
			EnqueueRenderCommend([this]() {
				USCOPE_LOCK();
				AVertexPackRS* rs = new AVertexPackRS;
				rs->mVB = gGFX->CreateVertexBuffer(GFXVertexBuffer_Desc());
			});
		}
		void MetaPpopertyBeforeChange(PropertyInfo*)
		{
			USCOPE_LOCK(mEditingLock);
		}
	};
#endif
	//////////////////////////////////////////////////////////////////////////

	bool AStaticMesh::IsRSReady() const
	{
		return mRS;
	}

	void AStaticMesh::AddRSCompleteListener(EntityStaticMesh* receiver, TMFP<void, AStaticMeshRS*> proc)
	{

	}

	void AStaticMesh::OnInit()
	{
		Assimp::Importer importer = Assimp::Importer();
		unsigned flag = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords | aiProcess_FlipUVs;
		const aiScene* scene = importer.ReadFileFromMemory(mContent.Data(), mContent.Size(), flag);
		if (scene == nullptr)
		{
			ULOG_ERROR("failed to lead StaticMesh");
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void AStaticMesh::OnInitRS()
	{

	}
	//////////////////////////////////////////////////////////////////////////
	void AStaticMesh::OnRelease()
	{

	}
	//////////////////////////////////////////////////////////////////////////
	void AStaticMesh::OnReleaseRS()
	{

	}


	UCLASS_BEGIN_IMPL(AStaticMesh)
		UPROPERTY(mContent)
		UPROPERTY(mFlipUV)
		UPROPERTY(mGenerateSmoothNormal)
	UCLASS_END_IMPL(AStaticMesh)

};