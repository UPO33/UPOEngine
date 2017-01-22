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

		TagRenderMiscDirty();
	}

	void EntityStaticMesh::SetMaterial(AMaterial* material)
	{
// 		if (mMaterial == material || !IsAlive()) return;
// 
// 		mMaterial = material;
// 
// 		TagRenderMiscDirty();
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
		mMesh = nullptr;
	}

	EntityStaticMesh::~EntityStaticMesh()
	{

	}


	void EntityStaticMesh::MetaBeforePropertyChange(const PropertyInfo* prp)
	{
		
	}

	void EntityStaticMesh::MetaAfterPropertyChange(const PropertyInfo* prp)
	{
		ULOG_MESSAGE("");
		this->TagRenderDirty();
	}

	//////////////////////////////////////////////////////////////////////////
	EntityStaticMeshRS::EntityStaticMeshRS(EntityStaticMesh* gs, WorldRS* wrs)
	{
		mGS = gs;
		mOwner = wrs;

		mOwner->mStaticMeshesBounds.AddUnInit();
		mOwner->mStaticMeshesCullingState.AddUnInit();
		mPrivateIndex = mOwner->mStaticMeshes.Add(this);

		OnFetch(EEF_RenderDataTransformDirty | EEF_RenderDataMiscDirty);
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
		if (flag & EEF_RenderDataMiscDirty)
		{
			mMesh = GS()->mMesh ? GS()->mMesh->GetRS() : nullptr;
// 			mMaterial = GS()->mMaterial ? GS()->mMaterial->GetRS() : nullptr;

			if (mMesh)	mRSFlag.Set(EEF_RenderDataValid);
		}
	}

	bool EntityStaticMeshRS::ShouldBeRendered(unsigned cullingmask)
	{
		return 0;
	}



	UCLASS_BEGIN_IMPL(EntityStaticMesh)
		UPROPERTY(mMesh)
	UCLASS_END_IMPL(EntityStaticMesh)



};