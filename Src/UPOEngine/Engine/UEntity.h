#pragma once

#include "../Object/UObject.h"
#include "../Core/UCore.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class World;

	//////////////////////////////////////////////////////////////////////////
	enum EEntityFlag
	{
		EEF_Alive,
		EEF_Initilized,
		EEF_Tickable,
		EEF_BeginPlayWasCalled,
		EEF_IsArchiving,
		EEF_Registered,
		EEF_Default = EEF_Alive | EEF_Initilized
	};
	
	//////////////////////////////////////////////////////////////////////////
	class UAPI Entity : public Object
	{
		UCLASS(Entity, Object)


		World*			mWorld;
		Flag			mEntityFlag;

	public:
		World* GetWorld() const { return mWorld; }
		
		void Destroy();

		bool IsAlive() const { return mEntityFlag.Test(EEF_Alive); }
		bool IsRegistered() const { return mWorld != nullptr; }

		void FlagSet(unsigned flag)
		{
			mEntityFlag.Set(flag);
		}
		bool FlagTest(unsigned flag)
		{
			return mEntityFlag.Test(flag);
		}
		bool FlagTestAnClear(EEntityFlag flag)
		{
			return mEntityFlag.TestAndClear(flag);
		}

		virtual void OnConstruct() {};
		virtual void OnBeginPlay() {};
		virtual void OnEndPlay() {};
		virtual void OnTick() {};
		virtual void OnDestroy() {};
	};
};