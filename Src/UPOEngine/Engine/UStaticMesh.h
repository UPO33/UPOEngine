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

	class UAPI StaticMeshRenderData
	{
	public:
		static const unsigned MaxSelection = 32;

		class GFXVertexBuffer*	mVertexBuffer;
		class GFXIndexBuffer*	mIndexBuffer;
		class GFXInputLayout*	mLayout;

		struct SelectionInfo
		{
			unsigned	mVertexOffset = 0;
			unsigned	mIndexOffset = 0;
			unsigned	mVertexCount = 0;
			unsigned	mIndexCount = 0;
			unsigned	mMaterialIndex = 0;
		};

		unsigned		mNumSelection = 0;
		SelectionInfo	mSelections[MaxSelection];

		~StaticMeshRenderData();
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI AStaticMeshRS
	{
	public:
		AABB					mBound;
		AStaticMesh*			mOwner;

		StaticMeshRenderData	mRenderData;

		AStaticMeshRS(AStaticMesh*, StaticMeshRenderData*);
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
		TArray<AMaterial*>			mMaterials;
		Transform					mImportTransform;

		void UpdateBound();
		virtual void OnCreate() override;
		virtual void OnDestroy() override;

	public:
		AStaticMeshRS* GetRS() const { return mRS; }
		AMaterial* GetMaterial(unsigned index) const;
		const TArray<AMaterial*>& GetMaterials() const { return mMaterials; }
		unsigned GetNumMaterial() const { return mMaterials.Length(); }
		const AABB& GetBound() { return mBound; }

		unsigned GetNumVertices() const { return mVertices.Length(); }
		unsigned GetNumIndices() const { return mIndices.Length(); }

		void MetaBeforePropertyChange(const PropertyInfo* prp);
		void MetaAfterPropertyChange(const PropertyInfo* prp);
	};


};