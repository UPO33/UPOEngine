#pragma once

#include "UAsset.h"
#include "UMaterial.h"
#include "../Core/UBound.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class EntityStaticMesh;
	class AStaticMesh;
	class AMaterial;

	//////////////////////////////////////////////////////////////////////////
	class UAPI AStaticMeshRS
	{
	public:
		class GFXVertexBuffer*	mVertexBuffer;
		class GFXIndexBuffer*	mIndexBuffer;
		class GFXInputLayout*	mLayout;
		unsigned				mVertexCount;
		unsigned				mIndexCount;
		AABB					mBound;
		AStaticMesh*			mOwner;

		AStaticMeshRS(AStaticMesh*);
		~AStaticMeshRS();
	};


	//////////////////////////////////////////////////////////////////////////
	class UAPI AStaticMesh : public Asset
	{
		UCLASS(AStaticMesh, Asset)

		friend AStaticMeshRS;
		friend UPOEd::AssetConverter;

	public:
		struct VertexTypeFull
		{
			Vec3 mPosition;
			Vec3 mNormal;
			Vec2 mUV;
		};
		struct VertexTypePositionOnly
		{
			Vec3	mPosition;
		};

		typedef unsigned short IndexType;

	private:
		Buffer						mContent;
		AStaticMeshRS*				mRS = nullptr;
		AABB						mBound;
		bool						mFlipUV = false;
		bool						mGenerateSmoothNormal = false;
		TArray<VertexTypeFull>		mVertices;
		TArray<IndexType>			mIndices;
		AMaterial*					mDefaultMaterial = nullptr;

		/*
		Material*	mMaterial;
		*/

		void UpdateBound();
		virtual void OnCreate() override;
		virtual void OnDestroy() override;

	public:
		AStaticMeshRS* GetRS() const { return mRS; }
		AMaterial* GetDefaultMaterial() const { return mDefaultMaterial; }
		const AABB& GetBound() { return mBound; }

		void MetaBeforePropertyChange(const PropertyInfo* prp);
		void MetaAfterPropertyChange(const PropertyInfo* prp);
	};


};