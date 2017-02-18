#pragma once

#include "UAsset.h"
#include "UTexture2D.h"
#include "../GFXCore/UShaderMgr.h"
#include "../GFXCore/UGlobalShader.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class AMaterial;
	class AMaterialRS;
	class APhysMaterial;


	static unsigned const MaterialMaxTexture = 8;



	enum EMaterialFlag
	{
		EMF_Default = 0,
		EMF_Wireframe = 1,
		EMF_TwoSided = 2,

		EMF_Max = EMF_Wireframe | EMF_TwoSided
	};
	//////////////////////////////////////////////////////////////////////////
	class MaterialShaderData;

	//////////////////////////////////////////////////////////////////////////
	class UAPI AMaterialRS
	{
		friend AMaterial;

		GFXConstantBuffer*				mCBufferCommon;
		ATexture2DRS*					mTextures[MaterialMaxTexture];
		size_t							mNumTexture;
		MaterialShaderData*				mShadersData;
		Flag							mFlag;
		uint32							mMaterialConstantsFlags;
		void*							mPtrPODParamBuffer;
		void*							mPtrPODParamBufferRS;
		size_t							mPODParamBufferSize;
		ATexture2D**					mTexturesAsProperty[MaterialMaxTexture];
		Name							mShaderFileName;
		AMaterial*						mGS;

		void ReleaseShadersData();
		void InitShadersData();

	public:

		TArray<ShaderMacroDefinition>	mCommonDefinitions;

		AMaterialRS(AMaterial*);
		~AMaterialRS();

		GFXConstantBuffer* GetCommonBuffer() const { return mCBufferCommon; }
		Flag GetFlag() const { return mFlag; }

		//bind textures , samplers and params
		void Bind();

		GFXShaderBound& GetShaderBound(const ShaderConstantsCombined&);
	};


	//////////////////////////////////////////////////////////////////////////
	class UAPI AMaterial : public Asset
	{
		UCLASS(AMaterial, Asset)

		friend AMaterialRS;

	protected:
		AMaterialRS*		mRS;
		APhysMaterial*		mPhysMaterial;

		//must be initiliZe in derived ctor once
		void*							mPtrPODParamBuffer;
		size_t							mPODParamBufferSize;
		Name							mShaderFilename;
		TArray<ShaderMacroDefinition>	mCommonDefinitions;


		bool				mWireframe;
		bool				mTwoSided;

		virtual void OnCreate() override;
		virtual void OnDestroy() override;

		bool IsConstantProperty(const PropertyInfo*) const;
		

	public:
		AMaterial();
		~AMaterial();

		template<typename T = AMaterialRS> T* GetRS() const { return (T*)mRS; }

		void MetaAfterPropertyChange(const PropertyInfo*);
	
		void TextureChanged();
		void ParamChanged();
		void MiscChanged();

		void GetConstants(TArray<ShaderMacroDefinition>& out);


		static AMaterial* GetDefault();
		static AMaterial* GetDefaultWire();
	};



	//////////////////////////////////////////////////////////////////////////
	class UAPI AMaterialTest : public AMaterial
	{
		UCLASS(AMaterialTest, AMaterial)

	public:
		ATexture2D*		mTextureDiffuse = nullptr;
		ATexture2D*		mTextureNormal = nullptr;
		bool			mApplyUVTransformation = false;


		struct
		{
			Color			mDiffuseColor = Color::WHITE;
			Vec2			mUVOffset = Vec2::ZERO;
			Vec2			mUVScale = Vec2::ONE;

		} mParam;


		AMaterialTest();
		~AMaterialTest();

	};
};