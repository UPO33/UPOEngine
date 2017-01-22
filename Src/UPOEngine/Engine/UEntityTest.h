#pragma once

#include "UEntity.h"
#include "UCanvas.h"
#include "UStaticMesh.h"
#include "UTexture2D.h"
#include "UPhysMaterial.h"
#include "UMaterial.h"

namespace UPO
{

	class UAPI EntityTest : public Entity
	{
		UCLASS(EntityTest, Entity)
	public:

		Vec2 mQuadPos = Vec2(0.1f);
		Vec2 mQuadSize = Vec2(0.2f);
		Color32 mColor = Color32::RED;
		ATexture2D*	mTexture2D = nullptr;
		TObjectPtr<ATexture2D> mTex2DPtr;
		TArray<ATexture2D*>	mTextures;
		AStaticMesh*	mMesh = nullptr;
		APhysMaterial* mPhysMat = nullptr;
		String mString = "Str";
		Name mName;
		Texture2DSamplerInfo mTestSampler;

		EntityTest();
		~EntityTest();

		void OnBeginPlay() override;
		void OnEndPlay(EEndPlayReason) override;
		void OnTick() override;

		void OnConstruct() override;
		void OnDestroy() override;

		TArray<CanvasTextureItem> mTextureItems;

		void MetaAfterPropertyChange(const PropertyInfo* prp);
		void MetaBeforePropertyChange(const PropertyInfo* prp);
	};

	class EntityTestRS
	{
		
	};
};
