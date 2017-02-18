#pragma once

#include "UEntityPrimitive.h"
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
	class UAPI EntityStaticMesh : public EntityPrimitive
	{
		UCLASS(EntityStaticMesh, EntityPrimitive)

		AStaticMesh*	mMesh;

	public:

		void SetMesh(AStaticMesh*);
		AStaticMesh* GetMesh() const { return mMesh; }


		void OnCalcBound() override;

		EntityStaticMesh();
		~EntityStaticMesh();

		void MetaBeforePropertyChange(const PropertyInfo* prp);
		void MetaAfterPropertyChange(const PropertyInfo* prp);
	};

	//////////////////////////////////////////////////////////////////////////
	class EntityStaticMeshRS : public EntityPrimitiveRS
	{
	public:
		AStaticMeshRS*	mMesh;
		Matrix4			mWorldTransform;


		EntityStaticMeshRS(EntityStaticMesh* gs, WorldRS* wrs);
		~EntityStaticMeshRS();
		EntityStaticMesh* GS() const { return (EntityStaticMesh*)mGS; }

		void OnFetch(unsigned flag) override;
		bool ShouldBeRendered(unsigned cullingmask);
	};

};