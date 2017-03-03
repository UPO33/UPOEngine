#include "UEntityStaticMesh.h"
#include "../Meta/UMeta.h"

#include "UWorldRS.h"
#include "UEntityStaticMeshVisualizer.h"

namespace UPO
{
	void EntityStaticMesh::SetMesh(AStaticMesh* mesh)
	{
		if (!IsAlive()) return;

		mMesh = mesh;

		if(mMesh)
			mMaterials = mesh->GetMaterials();

		OnCalcBound();

		TagRenderDirty();
	}



	void EntityStaticMesh::OnCalcBound()
	{
		if (mMesh)
		{
			mBound =  AABB::TransformBound(mMesh->GetBound(), mWorldTransform);
		}
		else
		{
			mBound = AABB(InitZero());
		}
	}

	EntityStaticMesh::EntityStaticMesh()
	{
		mMesh = nullptr;
#ifndef UENDUSER
		mVisualizer = new EntityStaticMeshVisualizer(this);
#endif
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
		if (prp && UPROPERTY_NAME_EQUAL(prp, mMesh))
		{
			SetMesh(mMesh);
			mNeedsPropertyBrowserRefresh = true;
		}
		else
		{
			this->TagRenderDirty();
			this->OnCalcBound();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	EntityStaticMeshRS::EntityStaticMeshRS(EntityStaticMesh* gs, WorldRS* wrs) : EntityPrimitiveRS(gs, wrs)
	{
		mOwner->mStaticMeshesBounds.AddUnInit();
		mOwner->mStaticMeshesCullingState.AddUnInit();
		mPrivateIndex = mOwner->mStaticMeshes.Add(this);

		OnFetch(EEF_RenderDataTransformDirty | EEF_RenderDataMiscDirty);

		mGS->mRS = this;
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
		EntityPrimitiveRS::OnFetch(flag);

		if (flag & EEF_RenderDataTransformDirty)
		{
			mWorldTransform = mGS->GetWorldTransform();
			mOwner->mStaticMeshesBounds[mPrivateIndex] = mGS->GetBound();
		}
		if (flag & EEF_RenderDataMiscDirty)
		{
			mMesh = GS()->GetMesh() ? GS()->GetMesh()->GetRS() : nullptr;

			mRSFlag.Clear(ERS_RenderDataValid);
			if (mMesh) mRSFlag.Set(ERS_RenderDataValid);
		}
	}

	bool EntityStaticMeshRS::ShouldBeRendered(unsigned cullingmask)
	{
		return 0;
	}



	UCLASS_BEGIN_IMPL(EntityStaticMesh, UATTR_Icon("EntityStaticMesh.png"), UATTR_Instanceable())
		UPROPERTY(mMesh)
	UCLASS_END_IMPL(EntityStaticMesh)



};