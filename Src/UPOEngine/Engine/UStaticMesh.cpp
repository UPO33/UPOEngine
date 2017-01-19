#include "UStaticMesh.h"
#include "../Meta/UMeta.h"
#include "../GFXCore/UGFXDevice.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace UPO
{

	UCLASS_BEGIN_IMPL(AStaticMesh, UATTR_Icon("StaticMesh.png"))
		UPROPERTY(mContent, UATTR_Hidden())
		UPROPERTY(mFlipUV)
		UPROPERTY(mGenerateSmoothNormal)
	UCLASS_END_IMPL(AStaticMesh)



	inline Vec2 UFromAssimp(const aiVector2D& in)
	{
		return Vec2(in.x, in.y);
	}
	inline Vec3 UFromAssimp(const aiVector3D& in)
	{
		return Vec3(in.x, in.y, in.z);
	}


	void AStaticMesh::UpdateBound()
	{
		mBound = AABB::MakeFromPoints(mVertices.Elements(), mVertices.Length(), sizeof(VertexType));
	}

	//////////////////////////////////////////////////////////////////////////
	void AStaticMesh::OnCreate()
	{
		Assimp::Importer importer = Assimp::Importer();
		unsigned flag = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords | aiProcess_FlipUVs | aiProcess_ConvertToLeftHanded;
		const aiScene* scene = importer.ReadFileFromMemory(mContent.Data(), mContent.Size(), flag);
		if (scene == nullptr)
		{
			ULOG_ERROR("failed to lead StaticMesh");
			return;
		}

		
		aiMesh* mesh = scene->mMeshes[0];
		unsigned numVertex = mesh->mNumVertices;
		unsigned numIndex = mesh->mNumFaces * 3;

		ULOG_MESSAGE("numMesh % numVertex % numIndex %", scene->mNumMeshes, numVertex, numIndex);

		VertexType* meshVertices = new VertexType[numVertex];
		IndexType* meshIndices = new IndexType[numIndex];

		//copy vertices
		for (unsigned iVertex = 0; iVertex < numVertex; iVertex++)
		{
			meshVertices[iVertex].mPosition = UFromAssimp(mesh->mVertices[iVertex]);
			if (mesh->HasNormals())
				meshVertices[iVertex].mNormal = UFromAssimp(mesh->mNormals[iVertex]);
			else
				meshVertices[iVertex].mNormal = Vec3(0, 0, 1);

			if (mesh->GetNumUVChannels() > 0)
				meshVertices[iVertex].mUV = UFromAssimp(mesh->mTextureCoords[0][iVertex]);
			else
				meshVertices[iVertex].mUV = Vec2::ZERO;

		}

		//copy indices
		for (unsigned iFace = 0; iFace < mesh->mNumFaces; iFace++)
		{
			for (unsigned iIndex = 0; iIndex < 3; iIndex++)
			{
				meshIndices[iFace * 3 + iIndex] = mesh->mFaces[iFace].mIndices[iIndex];
			} 
		}

		
		mVertices.RemoveAll();
		mIndices.RemoveAll();
		mVertices.Append(meshVertices, numVertex);
		mIndices.Append(meshIndices, numIndex);

		UpdateBound();

		EnqueueRenderCommandAndWait([this]() {
			mRS = new AStaticMeshRS(this);
		});
	}

	void AStaticMesh::OnDestroy()
	{
		EnqueueRenderCommandAndWait([this]() {
			SafeDelete(mRS);
		});
	}

	void AStaticMesh::MetaBeforePropertyChange(const PropertyInfo* prp)
	{

	}

	void AStaticMesh::MetaAfterPropertyChange(const PropertyInfo* prp)
	{

	}
	//////////////////////////////////////////////////////////////////////////
	AStaticMeshRS::AStaticMeshRS(AStaticMesh* sm)
	{
		mOwner = sm;
		mBound = sm->mBound;
		mIndexCount = sm->mIndices.Length();
		mVertexCount = sm->mVertices.Length();

		//vertex buffer
		{
			GFXVertexBufferDesc desc;
			desc.mInitialData = sm->mVertices.Elements();
			desc.mImmutable = true;
			desc.mSize = sizeof(AStaticMesh::VertexType) * sm->mVertices.Length();
			mVertexBuffer = gGFX->CreateVertexBuffer(desc);
			UASSERT(mVertexBuffer);
		}
		//index buffer
		{
			GFXIndexBufferDesc desc;
			desc.mImmutable = true;
			desc.mInitialData = sm->mIndices.Elements();
			desc.mSize = sizeof(AStaticMesh::IndexType) * sm->mIndices.Length();
			desc.mType = (sizeof(AStaticMesh::IndexType) == 2) ? EIndexBufferType::EUShort : EIndexBufferType::EUInt;
			mIndexBuffer = gGFX->CreateIndexBuffer(desc);
			UASSERT(mIndexBuffer);
		}

		sm->mRS = this;
	}
	//////////////////////////////////////////////////////////////////////////
	AStaticMeshRS::~AStaticMeshRS()
	{
		SafeDelete(mVertexBuffer);
		SafeDelete(mIndexBuffer);
	}

};