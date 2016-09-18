#include "USMesh.h"
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


	UCLASS_BEGIN_IMPL(StaticMesh)
		UPROPERTY(mContent)
		UPROPERTY(mVertexCount, UATTR_Uneditable())
		UPROPERTY(mIndexCount, UATTR_Uneditable())
		UPROPERTY(mFlipUV)
		UPROPERTY(mGenerateSmoothNormal)
	UCLASS_END_IMPL(StaticMesh)

	//////////////////////////////////////////////////////////////////////////
	void StaticMesh::OnInit()
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
	void StaticMesh::OnInitRS()
	{

	}
	//////////////////////////////////////////////////////////////////////////
	void StaticMesh::OnRelease()
	{

	}
	//////////////////////////////////////////////////////////////////////////
	void StaticMesh::OnReleaseRS()
	{

	}

};