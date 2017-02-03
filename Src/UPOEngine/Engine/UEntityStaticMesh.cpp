#include "UEntityStaticMesh.h"
#include "../Meta/UMeta.h"

#include "UWorldRS.h"


namespace UPO
{
	void EntityStaticMesh::SetMesh(AStaticMesh* mesh)
	{
		if (mMesh == mesh || !IsAlive()) return;

		mMesh = mesh;
		SetMaterial(mesh ? mesh->GetDefaultMaterial() : nullptr);

		OnCalcBound();

		TagRenderMiscDirty();
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
		mMesh = nullptr;
		mMaterial = nullptr;
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
		mOwner->mStaticMeshes.LastElement()->mPrivateIndex = mPrivateIndex;

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
			mMesh = GS()->GetMesh() ? GS()->GetMesh()->GetRS() : nullptr;
			mMaterial = GS()->GetMaterial() ? GS()->GetMaterial()->GetRS() : nullptr;

			mRSFlag.Clear(ERS_RenderDataValid);
			if (mMesh && mMaterial) mRSFlag.Set(ERS_RenderDataValid);

			this->EntityPrimitiveRS::Fetch();
		}
	}

	bool EntityStaticMeshRS::ShouldBeRendered(unsigned cullingmask)
	{
		return 0;
	}



	UCLASS_BEGIN_IMPL(EntityStaticMesh, UATTR_Icon("EntityStaticMesh.png"), UATTR_Instanceable())
		UPROPERTY(mMesh)
		UPROPERTY(mMaterial)
	UCLASS_END_IMPL(EntityStaticMesh)



};