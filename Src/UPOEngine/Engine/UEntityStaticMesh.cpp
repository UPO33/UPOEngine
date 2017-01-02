#include "UEntityStaticMesh.h"
#include "../Meta/UMeta.h"

#include "UWorldRS.h"


namespace UPO
{
	void EntityStaticMesh::SetMesh(AStaticMesh* mesh)
	{
		if (mMesh == mesh || !IsAlive()) return;

		mMesh = mesh;

		OnCalcBound();

		TagRenderDirty();
	}

	void EntityStaticMesh::SetMaterial(AMaterial* material)
	{
		if (mMaterial == material || !IsAlive()) return;

		mMaterial = material;

		TagRenderMiscDirty();
	}

	void EntityStaticMesh::OnCalcBound()
	{
		if (mMesh)
		{
			
		}
		else
		{
			mBound = AABB(InitZero());
		}
	}

	EntityStaticMesh::EntityStaticMesh()
	{

	}

	EntityStaticMesh::~EntityStaticMesh()
	{

	}


	//////////////////////////////////////////////////////////////////////////
	EntityStaticMeshRS::EntityStaticMeshRS(EntityStaticMesh* gs, WorldRS* wrs)
	{
		mGS = gs;
		mOwner = wrs;

		mOwner->mStaticMeshesBounds.AddUnInit();
		mOwner->mStaticMeshesCullingState.AddUnInit();
		mPrivateIndex = mOwner->mStaticMeshes.Add(this);

		OnFetch();
	}

	EntityStaticMeshRS::~EntityStaticMeshRS()
	{
		mOwner->mStaticMeshes.RemoveAtSwap(mPrivateIndex);
		mOwner->mStaticMeshesBounds.RemoveAtSwap(mPrivateIndex);
		mOwner->mStaticMeshesCullingState.RemoveAtSwap(mPrivateIndex);
	}

	void EntityStaticMeshRS::OnFetch(unsigned flag)
	{
		if (flag & EEF_RenderDataTransformDirty)
		{
			mWorldTransform = mGS->GetWorldTransform();
			mOwner->mStaticMeshesBounds[mPrivateIndex] = mGS->GetBound();
		}
		else
		{
			mMesh = GS()->mMesh ? GS()->mMesh->GetRS() : nullptr;
			mMaterial = GS()->mMaterial ? GS()->mMaterial->GetRS() : nullptr;

			if (mMesh && mMaterial)	mEntityFlag.Set(EEF_RenderDataValid);
		}
	}

	bool EntityStaticMeshRS::ShouldBeRendered(unsigned cullingmask)
	{
		return 0;
	}



	UCLASS_BEGIN_IMPL(EntityStaticMesh)
	UCLASS_END_IMPL(EntityStaticMesh)
};