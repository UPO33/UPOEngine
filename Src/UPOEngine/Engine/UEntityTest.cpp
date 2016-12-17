#include "UEntityTest.h"
#include "../Meta/UMeta.h"
#include "UInput.h"

namespace UPO
{


	UCLASS_BEGIN_IMPL(EntityTest)
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

	}

	void EntityTest::OnEndPlay(EEndPlayReason)
	{
		ULOG_MESSAGE("");
	}

	void EntityTest::OnTick()
	{
		Parent::OnTick();

		if (Input::IsKeyReleased(EKeyCode::EKC_Any))
			ULOG_MESSAGE("");
	}

	void EntityTest::OnConstruct()
	{
		SetTickEnable(true);
	}

	void EntityTest::OnDestroy()
	{
		ULOG_MESSAGE("");

	}

};