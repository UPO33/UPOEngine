#pragma once

#include "UEntity.h"
#include "UStaticMesh.h"
#include "UMaterial.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class AStaticMesh;
	class AMaterial;

	enum EEntityGFXFlag
	{
		EGF_Visible,
		EGF_CastShadow,
		EGF_ReceiveShadow,
	};
	//////////////////////////////////////////////////////////////////////////
	class UAPI EntityStaticMesh : public Entity
	{
		UCLASS(EntityStaticMesh, Entity)

		AStaticMesh*	mMesh;
		AMaterial*		mMaterial;
		
		void RSMeshFinished(AStaticMeshRS*);
	public:

		void SetMesh(AStaticMesh*);
		AStaticMesh* GetMesh() const { return mMesh; }

		void SetMaterial(AMaterial*);
		AMaterial* GetMaterial() const { return mMaterial; }
		
		
		void CalcWorldBound(AABB& out);

		EntityStaticMesh();
		~EntityStaticMesh();
	};

	//////////////////////////////////////////////////////////////////////////
	class EntityStaticMeshRS : public EntityRS
	{
	public:
		AStaticMeshRS*	mMesh;
		AMaterialRS*	mMaterial;
		AABB			mBound;
		Matrix4			mWorldTransform;

		static const unsigned EEF_RenderDirtyFlags = 0;

		EntityStaticMeshRS(EntityStaticMesh* gs, WorldRS* wrs);
		EntityStaticMesh* Owner() const;

		void OnFetch() override;
		bool ShouldBeRendered(unsigned cullingmask);
	};

};