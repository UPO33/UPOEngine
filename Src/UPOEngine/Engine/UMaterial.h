#pragma once

#include "UAsset.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class APhysMaterial;
	class ATexture2D;

	//////////////////////////////////////////////////////////////////////////
	class UAPI AMaterialRS
	{

	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI AMaterial : public Asset
	{
		UCLASS(AMaterial, Asset)

		/*
		USMesh*		mPreviewMesh;
		*/
		AMaterialRS*	mRS = nullptr;
		APhysMaterial*	mPhysMaterial = nullptr;
	public:
		AMaterialRS* GetRS() const { return mRS; }
	};


	//////////////////////////////////////////////////////////////////////////
	class UAPI AMaterialTest : public AMaterial
	{
		UCLASS(AMaterialTest, AMaterial)

	public:
		ATexture2D*		mDiffuseTexture;
		Color			mDiffuseColor;

		AMaterialTest();
		~AMaterialTest();

		virtual void OnCreate() override;
		virtual void OnDestroy() override;

		void MetaAfterPropertyChange(const PropertyInfo*);
	};
};