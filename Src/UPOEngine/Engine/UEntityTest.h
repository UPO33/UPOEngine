#pragma once

#include "UEntity.h"


namespace UPO
{
	class UAPI EntityTest : public Entity
	{
		UCLASS(EntityTest, Entity)
	public:

		EntityTest();
		~EntityTest();

		void OnBeginPlay() override;
		void OnEndPlay(EEndPlayReason) override;
		void OnTick() override;

		void OnConstruct() override;
		void OnDestroy() override;

	};

	class EntityTestRS
	{
		
	};
};
