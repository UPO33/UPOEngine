#pragma once

#include "UAsset.h"

namespace physx
{
	class PxMaterial;
};


namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	enum class EPhysMaterialCombine
	{
		EAverage,
		EMin,
		EMultiply,
		EMax
	};

	//////////////////////////////////////////////////////////////////////////
	class APhysMaterial : public Asset
	{
		UCLASS(APhysMaterial, Asset)

	public:
		float	mStaticFriction = 1;
		float	mDynamicFriction = 1;
		float	mRestitution = 1;

		EPhysMaterialCombine	mFrictionCombineMode = EPhysMaterialCombine::EAverage;
		EPhysMaterialCombine	mRestitutionCombineMode = EPhysMaterialCombine::EAverage;

	protected:
		physx::PxMaterial*		mPxMaterial = nullptr;

		void CreatePxMaterial();

		void OnInit() override;
		void OnRelease() override;

	};

};