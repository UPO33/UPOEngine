#pragma once

#include "UAsset.h"
#include "../Core/UBound.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class EntityStaticMesh;
	class AStaticMesh;

	//////////////////////////////////////////////////////////////////////////
	class UAPI AStaticMeshRS
	{
	public:
		class GFXVertexBuffer*	mVertexBuffer;
		class GFXIndexBuffer*	mIndexBuffer;
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
		struct VertexType
		{
			Vec3 mPosition;
			Vec3 mNormal;
			Vec2 mUV;
		};

		typedef unsigned short IndexType;

	private:
		Buffer				mContent;
		AStaticMeshRS*		mRS = nullptr;
		AABB				mBound;
		bool				mFlipUV = false;
		bool				mGenerateSmoothNormal = false;
		TArray<VertexType>	mVertices;
		TArray<IndexType>	mIndices;

		/*
		Material*	mMaterial;
		*/

		void UpdateBound();
		virtual void OnCreate() override;
		virtual void OnDestroy() override;

	public:
		AStaticMeshRS* GetRS() const { return mRS; }


		void MetaBeforePropertyChange(const PropertyInfo* prp);
		void MetaAfterPropertyChange(const PropertyInfo* prp);
	};


};