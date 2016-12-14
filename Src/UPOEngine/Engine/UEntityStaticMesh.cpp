#include "UEntityStaticMesh.h"

namespace UPO
{
// 	template< class TEntityClass, class TRSData> class TEntityRS
// 	{
// 		TRSData			mData;
// 		TEntityClass*	mGS;
// 		Flag			mEntityFlag;
// 		Flag			mRSFlag;
// 		unsigned		mCullingMask;
// 		unsigned		mPrivateIndex;
// 		ClassInfo*		mClass;
// 
// 		template<typename T = Entity> T* Owner() const { return mGS; }
// 
// 		virtual void OnFetch() {}
// 	};

	class EntityStaticMeshRS : public EntityRS
	{
		AStaticMeshRS*	mMesh;
		AMaterialRS*	mMaterial;
		AABB			mBound;
		Matrix4			mWorldTransform;

		EntityStaticMesh* Owner() const { return (EntityStaticMesh*)mGS; }

		void OnFetch() override
		{
			mEntityFlag = mGS->mEntityFlag;

			if (mEntityFlag.Test(EEF_RenderDataTransformDirty))
			{

			}
			{
				mMesh = Owner()->mMesh ? Owner()->mMesh->GetRS() : nullptr;
				mMaterial = Owner()->mMaterial ? Owner()->mMaterial->GetRS() : nullptr;

				if (mMesh && mMaterial)	mEntityFlag.Set(EEF_RenderDataValid);
			}
		}
		bool ShouldBeRendered(unsigned cullingmask)
		{
			return mEntityFlag.Test(EEF_RenderDataValid | EEF_Visible) && (mCullingMask & cullingmask);
		}
	};

	

	void EntityStaticMesh::RSMeshFinished(AStaticMeshRS*)
	{

	}

	void EntityStaticMesh::SetMesh(AStaticMesh* mesh)
	{
		if (mMesh == mesh) return;

		mMesh = mesh;
		if (mesh == nullptr) 
			FlagClear(EEF_RenderDataValid);
		else if (!mesh->IsRSReady())
		{
			mesh->AddRSCompleteListener(this, &EntityStaticMesh::RSMeshFinished);
			FlagClear(EEF_RenderDataValid);
		}

		TagRenderDataDirty(EEF_RenderDataMiscDirty);
	}

	void EntityStaticMesh::SetMaterial(AMaterial* material)
	{
		if (mMaterial == material) return;

		mMaterial = material;
		if (material == nullptr) FlagClear(EEF_RenderDataValid);

		TagRenderDataDirty(EEF_RenderDataMiscDirty);
	}

	EntityStaticMesh::EntityStaticMesh()
	{

	}

	EntityStaticMesh::~EntityStaticMesh()
	{

	}

};