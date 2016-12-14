#pragma once

#include "../Object/UObject.h"


namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class World;

	//////////////////////////////////////////////////////////////////////////
	class UAPI GameInstance : public Object
	{
		UCLASS(GameInstance, Object)

	public:
		virtual void OnGameStart(){}
		virtual void OnGameEnd() {}
	};
}