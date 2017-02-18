#pragma once

#include "UEntity.h"


namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class AMaterial;
	class AMaterialRS;


	//////////////////////////////////////////////////////////////////////////
	class UAPI EntityPrimitive : public Entity
	{
		UCLASS(EntityPrimitive, Entity)

	public:
		
		EntityPrimitive();
		~EntityPrimitive();

		bool				mVisible = true;
		bool				mCastShadow = true;
		bool				mReceiveShadow = true;
		bool				mMainPassEnable = true;
		bool				mCutomDepthStencilEnable = false;
		uint8				mCustomStencilValue = 0;
		TArray<AMaterial*>	mMaterials;

		virtual void SetMaterial(unsigned index, AMaterial* newMaterial);
		virtual void SetMaterials(const TArray<AMaterial*>& materials);
		virtual unsigned GetNumMaterial() const;
		virtual AMaterial* GetMaterial(unsigned index) const;
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI EntityPrimitiveRS : public EntityRS
	{
	public:
		unsigned					mCustomStencilValue = 0;
		unsigned					mHitID = 0;
		TArray<AMaterialRS*>		mMaterials;


		void OnFetch(unsigned) override;

	};
};