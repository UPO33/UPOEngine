#include "UEntityTest.h"
#include "../Meta/UMeta.h"
#include "UInput.h"
#include "UWorld.h"
#include "UCanvas.h"

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

	TArray<CanvasTextureItem> gTextureItems;

	void EntityTest::OnTick()
	{
		Parent::OnTick();

		if (Input::IsKeyReleased(EKeyCode::EKC_Any))
			ULOG_MESSAGE("");

		if (Input::IsKeyPressed(EKeyCode::EKC_MouseLeft))
		{
			ULOG_MESSAGE("mouse clickead at %", Input::GetMousePosition());
			CanvasTextureItem item;
			item.mPosition = Input::GetMousePosition() - Vec2(100);
			item.mSize = Vec2(200);
			gTextureItems.Add(item);
		}

		if (auto canvas = GetWorld()->GetCanvas())
		{
			for (auto& item : gTextureItems)
			{
				canvas->Draw(item);
			}
		}
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