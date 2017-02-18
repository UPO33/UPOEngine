#include "UEntityPrimitive.h"
#include "UMaterial.h"
#include "../Meta/UMeta.h"

namespace UPO
{


	
	UCLASS_BEGIN_IMPL(EntityPrimitive)
	UPROPERTY(mVisible)
	UPROPERTY(mCastShadow)
	UPROPERTY(mReceiveShadow)
	UPROPERTY(mMainPassEnable)
	UPROPERTY(mCutomDepthStencilEnable)
	UPROPERTY(mCustomStencilValue)
	UPROPERTY(mMaterials, UATTR_ArrayConstantLength())
	UCLASS_END_IMPL(EntityPrimitive);






	EntityPrimitive::EntityPrimitive()
	{

	}

	EntityPrimitive::~EntityPrimitive()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	void EntityPrimitive::SetMaterial(unsigned index, AMaterial* newMaterial)
	{
		if (mMaterials.IsIndexValid(index))
		{
			if (mMaterials[index] == newMaterial)return;

			mMaterials[index] = newMaterial;
			TagRenderMiscDirty();
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void EntityPrimitive::SetMaterials(const TArray<AMaterial*>& materials)
	{
		for (size_t i = 0; i < mMaterials.Length(); i++)
		{
			if (materials.IsIndexValid(i))
				mMaterials[i] = materials[i];
			else
				mMaterials[i] = nullptr;
		}
		TagRenderMiscDirty();
	}

	//////////////////////////////////////////////////////////////////////////
	unsigned EntityPrimitive::GetNumMaterial() const
	{
		return mMaterials.Length();
	}
	//////////////////////////////////////////////////////////////////////////
	AMaterial* EntityPrimitive::GetMaterial(unsigned index) const
	{
		if (mMaterials.IsIndexValid(index))
			return mMaterials[index];
		return nullptr;
	}


	void EntityPrimitiveRS::OnFetch(unsigned flag)
	{
		EntityRS::OnFetch(flag);

		if(flag & EEF_RenderDataMiscDirty)
		{
			mRSFlag.Clear(ERS_Visible | ERS_CastShadow | ERS_RecieveShadow | ERS_MainPassEnable | ERS_CustomDepthEnable | ERS_Selected);

			UASSERT(mGS);
			EntityPrimitive* gs = (EntityPrimitive*)mGS;


			if (gs->mVisible) mRSFlag.Set(ERS_Visible);
			if (gs->mCastShadow) mRSFlag.Set(ERS_CastShadow);
			if (gs->mReceiveShadow) mRSFlag.Set(ERS_RecieveShadow);
			if (gs->mMainPassEnable) mRSFlag.Set(ERS_MainPassEnable);
			if (gs->mCutomDepthStencilEnable) mRSFlag.Set(ERS_CustomDepthEnable);
			if (gs->mIsSelected) mRSFlag.Set(ERS_Selected);

			mCustomStencilValue = gs->mCustomStencilValue;

			//materials
			{
				mMaterials.RemoveAll();
				mMaterials.AddUnInit(gs->mMaterials.Length());

				for (size_t i = 0; i < gs->mMaterials.Length(); i++)
				{
					mMaterials[i] = gs->mMaterials[i] ? gs->mMaterials[i]->GetRS() : nullptr;
				}
			}
		}
	}


};