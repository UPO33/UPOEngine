#include "UEntityTest.h"
#include "../Meta/UMeta.h"
#include "UInput.h"
#include "UWorld.h"
#include "UCanvas.h"
#include "UGameWindow.h"
#include "../GFX/UPrimitiveBatch.h"

namespace UPO
{


	UCLASS_BEGIN_IMPL(EntityTest)
		UPROPERTY(mQuadPos)
		UPROPERTY(mQuadSize)
		UPROPERTY(mColor)
	UCLASS_END_IMPL(EntityTest)

	EntityTest::EntityTest()
	{
		
	}

	EntityTest::~EntityTest()
	{

	}

	void EntityTest::OnBeginPlay()
	{
		Parent::OnBeginPlay();
		ULOG_MESSAGE("");

		SetTickEnable(true);
		this->Invoke(0.5f, ~0U, [this]()
		{
			if (auto canvas = GetWorld()->GetCanvas())
			{

			}
		});
	}

	void EntityTest::OnEndPlay(EEndPlayReason)
	{
		ULOG_MESSAGE("");
		
	}

	

	void EntityTest::OnTick()
	{
		Parent::OnTick();

		if (auto canvas = GetWorld()->GetCanvas())
		{
			if (GetWorld()->GetInputState()->IsKeyPressed(EKC_MouseLeft))
			{
				canvas->AddDebugString(String("mouse clicked"), Color32::GREEN, 2);
			}
			for (auto& item : mTextureItems)
			{
				canvas->Draw(item);
			}
		}
// 		if (auto batch = GetWorld()->GetPrimitiveBatch())
// 		{
// 			for (int i = -64; i < 64; i++)
// 			{
// 				batch->DrawLine(Vec3(i * 5, 0, 100), Color32::BLUE, Vec3(i * 5, 0, -100), Color::BLUE);
// 				batch->DrawLine(Vec3(-100, 0, i * 5), Color::RED, Vec3(100, 0, i * 5), Color::RED);
// 			}
// 			batch->DrawLine(Vec3(0), Color32::RED, Vec3(10, 0, 0), Color32::RED);
// 			batch->DrawLine(Vec3(0), Color32::GREEN, Vec3(0, 10, 0), Color32::GREEN);
// 			batch->DrawLine(Vec3(0), Color32::BLUE, Vec3(0, 0, 10), Color32::BLUE);
// 
// 		}
	}

	void EntityTest::OnConstruct()
	{
		ULOG_MESSAGE("");
		SetTickEnable(true);
	}

	void EntityTest::OnDestroy()
	{
		ULOG_MESSAGE("");

	}

};