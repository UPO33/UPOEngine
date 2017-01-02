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
		Flag					mFlag;
		class GFXVertexBuffer*	mVertexBuffer;
		class GFXIndexBuffer*	mIndexBuffer;
		unsigned				mVertexCount;
		unsigned				mIndexCount;
		AABB					mBound;
		AStaticMesh*			mOwner;
	};


	//////////////////////////////////////////////////////////////////////////
	class UAPI AStaticMesh : public Asset
	{
		UCLASS(AStaticMesh, Asset)

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

		struct RSFinishedListener
		{
			ObjectPtr				mObject;
			TMFP<void, void*>		mProc;
		};
		/*
		Material*	mMaterial;
		*/
	public:
		AStaticMeshRS* GetRS() const { return mRS; }
		bool IsRSReady() const;
		void AddRSCompleteListener(EntityStaticMesh* receiver, TMFP<void, AStaticMeshRS*> proc);
		
	protected:
		virtual void OnInit() override;
		virtual void OnInitRS() override;
		virtual void OnRelease() override;
		virtual void OnReleaseRS() override;
	};


};