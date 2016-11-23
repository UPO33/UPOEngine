#pragma once

#include "UAsset.h"
#include "../GFXCore/UGFXDevice.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class UAPI StaticMesh : public Asset
	{
		UCLASS(StaticMesh, Asset)

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

		GFXVertexBuffer*	mVertexBuffer = nullptr;
		GFXIndexBuffer*		mIndexBuffer = nullptr;
		unsigned			mVertexCount = 0;
		unsigned			mIndexCount = 0;
		AABB				mBound;
		bool				mFlipUV = false;
		bool				mGenerateSmoothNormal = false;

		/*
		Material*	mMaterial;
		*/

	protected:
		virtual void OnInit() override;
		virtual void OnInitRS() override;
		virtual void OnRelease() override;
		virtual void OnReleaseRS() override;
	};


};