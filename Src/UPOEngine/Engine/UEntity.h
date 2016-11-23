#pragma once

#include "../Object/UObject.h"
#include "../Core/UCore.h"

#define UUSE_ARRAYCHILD

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
		EEF_OnDestroyWasCalled,
		EEF_InHell,
		EEF_Default = EEF_Alive | EEF_Initilized
	};
	enum EEndPlayReason
	{
		EPR_EndPlay,
		EPR_Destroy,
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI Entity : public Object
	{
		UCLASS(Entity, Object)


		friend World;

		World*			mWorld;
		Flag			mEntityFlag;

		Entity*		mParent;
		Entity*		mChildHead;
		Entity*		mUpEntity;
		Entity*		mDownEntity;

		TArray<Entity*>	mChildren;
		unsigned		mIndexInParent;

		Name		mName;
		unsigned	mNumChild;
		unsigned	mIndexInWorld;

	public:
		Entity();

		Entity* GetParent() const;
		unsigned NumChild() const;
		Entity* GetChild(unsigned index) const;
		void AttachTo(Entity* newParent);
		
		bool IsSubsetOf(Entity* parent) const;

		World* GetWorld() const { return mWorld; }

		bool IsAlive() const { return mEntityFlag.Test(EEF_Alive); }
		bool IsRegistered() const { return mWorld != nullptr; }

		void FlagSet(unsigned flag)
		{
			mEntityFlag.Set(flag);
		}
		void FlagClear(EEntityFlag flag)
		{
			mEntityFlag.Clear(flag);
		}
		bool FlagTest(unsigned flag)
		{
			return mEntityFlag.Test(flag);
		}
		bool FlagTestAnClear(EEntityFlag flag)
		{
			return mEntityFlag.TestAndClear(flag);
		}
		//is called right after constructor, world is valid in this function
		virtual void OnConstruct() {};
		virtual void OnBeginPlay() {};
		virtual void OnEndPlay(EEndPlayReason) {};
		virtual void OnTick() {};
		virtual void OnDestroy() {};

		virtual void OnRegisterToWorld() {};

		virtual void RegisterToWorld(World* world);

		virtual void OnParentChanged();


		void Destroy();

	private:
		void Destroy_Pass0();
		void Destroy_Pass1();

		void RemoveChildFromList(Entity* child);
		void AddChildToList(Entity* child);

		void Tick();

		void Init(Entity* parent, World* world);

		template<typename Lambda> void DoOnChilChild(Lambda proc)
		{
#ifndef UUSE_ARRAYCHILD
			Entity* iter = mChildHead;
			while (iter)
			{
				proc(iter);
				DoOnChilChild(proc);
				iter = iter->mDownEntity;
			}
		}
#else
			for (size_t i = 0; i < mChildren.Length(); i++)
			{
				proc(mChildren[i]);
				DoOnChilChild(proc);
			}
#endif
		};
	};
};