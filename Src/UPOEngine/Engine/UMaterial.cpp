#include "UMaterial.h"
#include "UPhysMaterial.h"
#include "../Meta/UMeta.h"
#include "../GFXCore/UGFXCore.h"
#include "UTexture2D.h"

namespace UPO
{
	UCLASS_BEGIN_IMPL(AMaterial, UATTR_Instanceable())
		UPROPERTY(mPhysMaterial)
		UPROPERTY(mWireframe)
		UPROPERTY(mTwoSided)
	UCLASS_END_IMPL(AMaterial)



// 	UCLASS_BEGIN_IMPL(MaterialTestParam)
// 		UPROPERTY(mTextures.mDiffuse)
// 		UPROPERTY(mTextures.mNormal)
// 		UPROPERTY(mParams.mDiffuseColor)
// 		UPROPERTY(mParams.mUVOffset)
// 		UPROPERTY(mParams.mUVScale)
// 	UCLASS_END_IMPL(MaterialTestParam)

	UCLASS_BEGIN_IMPL(AMaterialTest, UATTR_Instanceable())
		UPROPERTY(mTextureDiffuse)
		UPROPERTY(mTextureNormal)
		UPROPERTY(mApplyUVTransformation, UATTR_MaterialConstant("APPLY_UV_TRANSFORMATION"))
		UPROPERTY(mParam.mDiffuseColor)
		UPROPERTY(mParam.mUVOffset)
		UPROPERTY(mParam.mUVScale)
	UCLASS_END_IMPL(AMaterialTest)


	UGLOBAL_SHADER_DECLIMPL(gVSMaterialTest, GFXVertexShader, "MaterialTest.hlsl", "VSMain");
	UGLOBAL_SHADER_DECLIMPL(gPSMaterialTest, GFXPixelShader, "MaterialTest.hlsl", "PSMain");



	static const unsigned MaterialTextureStartSlot = 3;
	static const unsigned MaterialCommonConstantBufferSlot = 4;



	TArray<MaterialShaderData*>		gMaterialsShaders;

	//////////////////////////////////////////////////////////////////////////
	struct MaterialShaderData : public RefCountable
	{
		static const unsigned MaxShader = 256;

		size_t							mNumShaders = 0;
		ShaderConstantsCombined			mShadersConstants[MaxShader];
		GFXShaderBound					mShaders[MaxShader];
		Name							mFileName;
		TArray<ShaderMacroDefinition>	mCommonDefinitions;
		uint32							mMaterialConstantsHash = 0;
		TArray<ShaderMacroDefinition>	mMaterialConstants;


		MaterialShaderData()
		{
		}
		~MaterialShaderData()
		{
			for (size_t i = 0; i < mNumShaders; i++)
			{
				GFXShaderBound& shaderBound = mShaders[i];
				SafeDelete(shaderBound.mVertexShader);
				SafeDelete(shaderBound.mHullShader);
				SafeDelete(shaderBound.mDomainShader);
				SafeDelete(shaderBound.mGeometryShader);
				SafeDelete(shaderBound.mPixelShader);
			}
			mNumShaders = 0;
		}
		size_t Find(const ShaderConstantsCombined& sc) const
		{
			for (size_t i = 0; i < mNumShaders; i++)
			{
				if (mShadersConstants[i] == sc)
					return i;
			}
			return InvalidIndex;
		}
		GFXShaderBound& GetShaders(const ShaderConstantsCombined& sc)
		{
			auto index = Find(sc);
			if (index != InvalidIndex)
			{
				return mShaders[index];
			}
			else
			{
				TArray<ShaderMacroDefinition> definitions;
				ShaderConstant shaderContants[MaxShaderConstant];
				size_t	numShaderConstant = 0;
				sc.GetShaderConstants(shaderContants, numShaderConstant);
				//adding shader constants
				for (size_t i = 0; i < numShaderConstant; i++)
				{
					definitions.Add(ShaderMacroDefinition(shaderContants[i].GetName(), shaderContants[i].GetValue()));
				}

				definitions += mMaterialConstants;
				definitions += mCommonDefinitions;

				Buffer bytescodeVS;
				Buffer bytescodePS;
				
				{
					for (auto item : definitions)
					{
						if (item.mName)
							ULOG_MESSAGE("%", item.mName);
						else
							ULOG_MESSAGE("-");
					}
				}

				GFXShaderBound shaders;

				if (!ShaderMgr::Get()->GetShaderCode(mFileName, "VSMain", EShaderType::EVertex, definitions, bytescodeVS))
				{
				}
				if (!ShaderMgr::Get()->GetShaderCode(mFileName, "PSMain", EShaderType::EPixel, definitions, bytescodePS))
				{
				}

				shaders.mVertexShader = gGFX->CreateShader<GFXVertexShader>(bytescodeVS, mFileName);
				shaders.mPixelShader = gGFX->CreateShader<GFXPixelShader>(bytescodePS, mFileName);

				mShaders[mNumShaders] = shaders;
				mShadersConstants[mNumShaders] = sc;
				mNumShaders++;

				return mShaders[mNumShaders-1];
			}
		}
	};



	void AMaterial::OnCreate()
	{
		Parent::OnCreate();

		EnqueueRenderCommandAndWait([this]() {
			mRS = new AMaterialRS(this);
		});
		MetaAfterPropertyChange(nullptr);
	}

	AMaterial::AMaterial()
	{
		mRS = nullptr;
		mPhysMaterial = nullptr;
		mPtrPODParamBuffer = nullptr;
		mPODParamBufferSize = 0;
		mWireframe = false;
		mTwoSided = false;
	}

	AMaterial::~AMaterial()
	{

	}


	void AMaterial::MetaAfterPropertyChange(const PropertyInfo* prp)
	{
		ULOG_WARN("%", prp ? prp->GetName() : nullptr);
		TextureChanged();
		ParamChanged();
		MiscChanged();

		if (IsConstantProperty(prp))
		{
			EnqueueRenderCommandAndWait([this]() {
				mRS->ReleaseShadersData();
				mRS->InitShadersData();
			});
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void AMaterial::TextureChanged()
	{
		EnqueueRenderCommand([this]() {

			if (!mRS) return;

			for (unsigned i = 0; i < mRS->mNumTexture; i++)
			{
				if (ATexture2D* prprTex = *(mRS->mTexturesAsProperty[i]))
					mRS->mTextures[i] = prprTex->GetRS();
				else
					mRS->mTextures[i] = nullptr;
			}
		});
	}

	//////////////////////////////////////////////////////////////////////////
	void AMaterial::ParamChanged()
	{
		if (!mRS || !mRS->mPtrPODParamBuffer)return;

		MemCopy(mRS->mPtrPODParamBufferRS, mRS->mPtrPODParamBuffer, mRS->mPODParamBufferSize);
		
		EnqueueRenderCommand([this]() {

			void* mapped = gGFX->Map(mRS->GetCommonBuffer(), EMapFlag::EWriteDiscard);

			MemCopy(mapped, mRS->mPtrPODParamBufferRS, mRS->mPODParamBufferSize);

			gGFX->Unmap(mRS->GetCommonBuffer());

			ULOG_MESSAGE("buffer update");
		});
	}

	void AMaterial::MiscChanged()
	{
		EnqueueRenderCommand([this]() {

			if (!mRS) return;

			mRS->mFlag.Clear(EMaterialFlag::EMF_TwoSided | EMaterialFlag::EMF_Wireframe);

			if (mWireframe) mRS->mFlag.Set(EMaterialFlag::EMF_Wireframe);
			if (mTwoSided) mRS->mFlag.Set(EMaterialFlag::EMF_TwoSided);
		});
	}

	void AMaterial::OnDestroy()
	{
		Parent::OnDestroy();

		AMaterialRS* rs = mRS;

		EnqueueRenderCommand([rs]() {
			if (rs) delete rs;
		});
	}


	bool AMaterial::IsConstantProperty(const PropertyInfo* prp) const
	{
		return prp && prp->HasAttrib(EAttribID::EAT_MaterialConstant);
	}

	void AMaterial::GetConstants(TArray<ShaderMacroDefinition>& out)
	{

		for (auto& item : GetClassInfo()->GetProperties())
		{
			Attrib attr;
			if (item.GetAttrib(EAttribID::EAT_MaterialConstant, attr))
			{
				if (item.GetType() == EPropertyType::EPT_bool)
				{
					bool* valueBool = (bool*)item.Map(this);
					if(*valueBool)
						out.Add(ShaderMacroDefinition(attr.GetString(), ""));
				}
				
			}
		}
	}

	AMaterialRS::AMaterialRS(AMaterial* gs)
	{
		//zeroing
		{
			MemZero(mTextures, sizeof(mTextures));
			mNumTexture = 0;
			mCBufferCommon = nullptr;
			mPtrPODParamBuffer = nullptr;
			mPODParamBufferSize = 0;
			mPtrPODParamBufferRS = nullptr;
			mShadersData = nullptr;
			mGS = nullptr;
			mFlag = EMaterialFlag::EMF_Default;
		}

		UASSERT(gs);
		mGS = gs;
		mCommonDefinitions = gs->mCommonDefinitions;

		//finding textures properties
		auto& properties = gs->GetClassInfo()->GetProperties();
		for (auto& prp : properties)
		{
			if (prp.GetTypeInfo())
			{
				if (auto cinfo = prp.GetTypeInfo()->Cast<ClassInfo>())
				{
					if (cinfo->IsBaseOf<ATexture2D>())
					{
						mTexturesAsProperty[mNumTexture] = (ATexture2D**)prp.Map(gs);
						mNumTexture++;
					}
				}
			}
		}



		mPODParamBufferSize = gs->mPODParamBufferSize;
		mPtrPODParamBuffer = gs->mPtrPODParamBuffer;

		if(mPODParamBufferSize && mPtrPODParamBuffer)
		{
			mPtrPODParamBufferRS = MemAllocAligned(mPODParamBufferSize, 16);
			mCBufferCommon = gGFX->CreateConstantBuffer(mPODParamBufferSize);
			UASSERT(mCBufferCommon);
		}

		mShaderFileName = gs->mShaderFilename;
		InitShadersData();
	}

	AMaterialRS::~AMaterialRS()
	{
		SafeMemFreeAligned(mPtrPODParamBufferRS);
		SafeDelete(mCBufferCommon);
	}

	//////////////////////////////////////////////////////////////////////////
	void AMaterialRS::Bind()
	{

		GFXTexture2D*				textures[MaterialMaxTexture];
		GFXSamplerStateHandle		samplers[MaterialMaxTexture];

		for (unsigned i = 0; i < mNumTexture; i++)
		{
			textures[i] = nullptr;
			samplers[i] = nullptr;

			if (mTextures[i])
			{
				textures[i] = mTextures[i]->mTexture;
				samplers[i] = mTextures[i]->mSampler;
			}
		}
		
		gGFX->SetResourceView(textures, MaterialTextureStartSlot, mNumTexture, EShaderType::EPixel);
		gGFX->SetSamplerState(samplers, MaterialTextureStartSlot, mNumTexture, EShaderType::EPixel);

		gGFX->SetConstentBuffer(mCBufferCommon, MaterialCommonConstantBufferSlot, EShaderType::EVertex);
		gGFX->SetConstentBuffer(mCBufferCommon, MaterialCommonConstantBufferSlot, EShaderType::EPixel);

	}









	
	void AMaterialRS::ReleaseShadersData()
	{
		ULOG_MESSAGE("");
		SafeDelete(mShadersData);
	}

#ifdef ___
	void AMaterialRS::InitShadersData()
	{
		UASSERT(IsRenderThread());

		//has been compiled once ?
		for (TSmartPtr<MaterialShaders>& item : gMaterialShaders)
		{
			if (item->mShaderFilename == mShaderFileName)
			{
				mShaders = item;
				return;
			}
		}

		mShaders = new MaterialShaders;
		gMaterialShaders.Add(mShaders);

		//iterate and compile material's shaders for different binds
		for (unsigned iBind = 0; iBind < EMaterialShaderBind::EMS_Max; iBind++)
		{
			auto& desfinitions = UMaterialBindGetDefinitions((EMaterialShaderBind)iBind);

			ShaderUniqueParam vsParam;
			vsParam.mFileName = mShaderFileName;
			vsParam.mEntryPoint = "VSMain";
			vsParam.mType = EShaderType::EVertex;
			
			for (size_t iDef = 0; iDef < desfinitions.Length(); iDef++)
			{
				vsParam.mCustomDefinitions[iDef] = desfinitions[iDef];
			}

			ShaderUniqueParam psParam;
			psParam.mFileName = mShaderFileName;
			psParam.mEntryPoint = "PSMain";
			psParam.mType = EShaderType::EPixel;

			for (size_t iDef = 0; iDef < desfinitions.Length(); iDef++)
			{
				psParam.mCustomDefinitions[iDef] = desfinitions[iDef];
			}

			GFXVertexShader* vs = gGFX->CreateShader(vsParam)->As<GFXVertexShader>();
			GFXPixelShader* ps = gGFX->CreateShader(psParam)->As<GFXPixelShader>();
			
			
			mShaders->mBounds[iBind].mVertexShader = vs;
			mShaders->mBounds[iBind].mPixelShader = ps;

			if (!vs || !ps)
			{
				ULOG_ERROR("compiling material failed [%]", mGS->GetClassInfo()->GetName());
			}
		}

	}
#endif // ___

	template<typename Hasher> auto GetHash(Hasher& hasher, const TArray<ShaderMacroDefinition>& in)
	{
		for (auto& item : in)
		{
			item.GetHash(hasher);
		}
		return hasher.GetHash();
	}

	void AMaterialRS::InitShadersData()
	{
		ULOG_MESSAGE("");
		TArray<ShaderMacroDefinition> materialConstants;
		mGS->GetConstants(materialConstants);

		uint32 materialConstantsHash = GetHash(HasherCRC32(), materialConstants);

		for (MaterialShaderData* item : gMaterialsShaders)
		{
			if (item->mFileName == mShaderFileName && item->mMaterialConstantsHash == materialConstantsHash) //found?
			{
				this->mShadersData = item;
				return;
			}
		}
	
		{
			MaterialShaderData* newShaderData = new MaterialShaderData;
			newShaderData->mCommonDefinitions = mCommonDefinitions;
			newShaderData->mFileName = mShaderFileName;
			newShaderData->mMaterialConstants = materialConstants;
			newShaderData->mMaterialConstantsHash = materialConstantsHash;
			gMaterialsShaders.Add(newShaderData);

			mShadersData = newShaderData;
		}

	}


	GFXShaderBound& AMaterialRS::GetShaderBound(const ShaderConstantsCombined& sc)
	{
		UASSERT(mShadersData);

		return mShadersData->GetShaders(sc);
	}







	AMaterialTest::AMaterialTest()
	{
		mPODParamBufferSize = sizeof(mParam);
		mPtrPODParamBuffer = &mParam;
		mShaderFilename = "MaterialTest.hlsl";
	}

	AMaterialTest::~AMaterialTest()
	{

	}




// 	MaterialShaders::~MaterialShaders()
// 	{
// 		for (auto& iter : mBounds)
// 		{
// 			SafeDelete(iter.mVertexShader);
// 			SafeDelete(iter.mHullShader);
// 			SafeDelete(iter.mDomainShader);
// 			SafeDelete(iter.mGeometryShader);
// 			SafeDelete(iter.mPixelShader);
// 			SafeDelete(iter.mComputeShader);
// 		}
// 	}

};
