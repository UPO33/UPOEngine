#pragma once

#include "UEntity.h"
#include "UCanvas.h"

namespace UPO
{

	class UAPI EntityTest : public Entity
	{
		UCLASS(EntityTest, Entity)
	public:

		Vec2 mQuadPos = Vec2(0.1f);
		Vec2 mQuadSize = Vec2(0.2f);
		Color32 mColor = Color32::RED;

		EntityTest();
		~EntityTest();

		void OnBeginPlay() override;
		void OnEndPlay(EEndPlayReason) override;
		void OnTick() override;

		void OnConstruct() override;
		void OnDestroy() override;

		TArray<CanvasTextureItem> mTextureItems;
	};

	class EntityTestRS
	{
		
	};
};
