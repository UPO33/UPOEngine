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
		UPROPERTY(mMaterials, UATTR_ArrayConstantLength())
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
		mBound = AABB::MakeFromPoints(mVertices.Elements(), mVertices.Length(), sizeof(VertexTypeFull));
	}

#if 0
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

		ULOG_WARN("numMesh % numVertex % numIndex %", scene->mNumMeshes, numVertex, numIndex);
		ULOG_WARN("%", scene->mNumMaterials);

		VertexTypeFull* meshVertices = new VertexTypeFull[numVertex];
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

#endif
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


		StaticMeshRenderData* renderData = new StaticMeshRenderData;
		renderData->mNumSelection = scene->mNumMeshes;

		//materials
		{
			while (mMaterials.Length() < scene->mNumMaterials)
			{
				mMaterials.Add(nullptr);
			}
		}

		mVertices.RemoveAll();
		mIndices.RemoveAll();

		unsigned vertexOffset = 0;
		unsigned indexOffset = 0;

		for (unsigned iMesh = 0; iMesh < scene->mNumMeshes; iMesh++)
		{
			aiMesh* mesh = scene->mMeshes[iMesh];

			unsigned numVertex = mesh->mNumVertices;
			unsigned numIndex = mesh->mNumFaces * 3;

			VertexTypeFull* meshVertices = new VertexTypeFull[numVertex];
			IndexType* meshIndices = new IndexType[numIndex];

			renderData->mSelections[iMesh].mVertexCount = numVertex;
			renderData->mSelections[iMesh].mIndexCount = numIndex;
			renderData->mSelections[iMesh].mMaterialIndex = mesh->mMaterialIndex;
			renderData->mSelections[iMesh].mVertexOffset = vertexOffset;
			renderData->mSelections[iMesh].mIndexOffset = indexOffset;

			vertexOffset += numVertex;
			indexOffset += numIndex;

			//copy vertices
			for (unsigned iVertex = 0; iVertex < mesh->mNumVertices; iVertex++)
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


			mVertices.Append(meshVertices, numVertex);
			mIndices.Append(meshIndices, numIndex);

			delete meshVertices;
			delete meshIndices;
		}

		



		UpdateBound();

		EnqueueRenderCommandAndWait([this, renderData]() {
			mRS = new AStaticMeshRS(this, renderData);
		});
	}

	void AStaticMesh::OnDestroy()
	{
		EnqueueRenderCommandAndWait([this]() {
			SafeDelete(mRS);
		});
	}

	AMaterial* AStaticMesh::GetMaterial(unsigned index) const
	{
		if (mMaterials.IsIndexValid(index))
			return mMaterials[index];
		return nullptr;
	}

	void AStaticMesh::MetaBeforePropertyChange(const PropertyInfo* prp)
	{

	}

	void AStaticMesh::MetaAfterPropertyChange(const PropertyInfo* prp)
	{

	}
	//////////////////////////////////////////////////////////////////////////
	AStaticMeshRS::AStaticMeshRS(AStaticMesh* sm, StaticMeshRenderData* rd)
	{
		mOwner = sm;
		mBound = sm->mBound;

		mRenderData = *rd;

		delete rd;

		//vertex buffer
		{
			GFXVertexBufferDesc desc;
			desc.mInitialData = sm->mVertices.Elements();
			desc.mImmutable = true;
			desc.mSize = sizeof(AStaticMesh::VertexTypeFull) * sm->mVertices.Length();
			mRenderData.mVertexBuffer = gGFX->CreateVertexBuffer(desc);
			UASSERT(mRenderData.mVertexBuffer);
		}
		//index buffer
		{
			GFXIndexBufferDesc desc;
			desc.mImmutable = true;
			desc.mInitialData = sm->mIndices.Elements();
			desc.mSize = sizeof(AStaticMesh::IndexType) * sm->mIndices.Length();
			desc.mType = (sizeof(AStaticMesh::IndexType) == 2) ? EIndexBufferType::EUShort : EIndexBufferType::EUInt;
			mRenderData.mIndexBuffer = gGFX->CreateIndexBuffer(desc);
			UASSERT(mRenderData.mIndexBuffer);
		}

		sm->mRS = this;
	}
	//////////////////////////////////////////////////////////////////////////
	AStaticMeshRS::~AStaticMeshRS()
	{

	}

	StaticMeshRenderData::~StaticMeshRenderData()
	{
		SafeDelete(mIndexBuffer);
		SafeDelete(mVertexBuffer);
		MemZero(this, sizeof(StaticMeshRenderData));
	}

};