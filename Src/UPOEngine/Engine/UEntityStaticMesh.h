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
};