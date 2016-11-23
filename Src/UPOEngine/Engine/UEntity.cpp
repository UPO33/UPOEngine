#include "UEntity.h"
#include "../Meta/UMeta.h"
#include "UWorld.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	Entity::Entity()
	{
		static Name NameEntity = "Entity";

		mParent = nullptr;
		mNumChild = 0;

		mChildHead = mDownEntity = mUpEntity = nullptr;

		mName = NameEntity;

	}
	//////////////////////////////////////////////////////////////////////////
	Entity* Entity::GetParent() const
	{
		return mParent;
	}
	//////////////////////////////////////////////////////////////////////////
	Entity* Entity::GetChild(unsigned index) const
	{
		Entity* iter = mChildHead;
		while (iter)
		{
			if (index == 0) break;

			iter = iter->mDownEntity;
		}
		return iter;
	}
	//////////////////////////////////////////////////////////////////////////
	void Entity::AttachTo(Entity* newParent)
	{
		if (mParent == newParent) return;

		if (FlagTest(EEF_Alive | EEF_Registered | EEF_Initilized))
		{
			if (newParent == nullptr) // detach?
			{
				if (mParent)
				{
					mParent->RemoveChildFromList(this);
					
					OnParentChanged();
				}
			}
			else
			{
				if(newParent->FlagTest(EEF_Alive | EEF_Registered | EEF_Initilized) && GetWorld() == newParent->GetWorld())
				{
					if (newParent->IsSubsetOf(this))
					{
						ULOG_ERROR("a Entity cant be attachet to its child");
						return;
					}

					if (mParent) // already has parent ?
						mParent->RemoveChildFromList(this);

					mParent = newParent;
					mParent->AddChildToList(this);
					OnParentChanged();
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	bool Entity::IsSubsetOf(Entity* parent) const
	{
		UASSERT(parent);

		Entity* iter = mParent;
		while (iter)
		{
			if (iter == parent) return true;
			iter = iter->mParent;
		}
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	//remove a child from list, child should be exist
	void Entity::RemoveChildFromList(Entity* child)
	{
#ifndef UUSE_ARRAYCHILD
		UASSERT(child);
		if (mChildHead == child)
		{
			UASSERT(child->mUpEntity == nullptr);	//head child must not have up Entity
			if (child->mDownEntity)
			{
				mChildHead = child->mDownEntity;
				mChildHead->mUpEntity = nullptr;
				child->mDownEntity = nullptr;
			}
			else
			{
				mChildHead = nullptr;
			}
			child->mParent = nullptr;
		}
		else
		{
			if (child->mUpEntity)
				child->mUpEntity->mDownEntity = child->mDownEntity;
			if (child->mDownEntity)
				child->mDownEntity->mUpEntity = child->mUpEntity;

			child->mUpEntity = nullptr;
			child->mDownEntity = nullptr;
			child->mParent = nullptr;
		}
		mNumChild--;

#else
		mChildren.LastElement()->mIndexInParent = child->mIndexInParent;
		mChildren.RemoveAtSwap(child->mIndexInParent);
#endif
	}
	//////////////////////////////////////////////////////////////////////////
	void Entity::AddChildToList(Entity* child)
	{
#ifndef UUSE_ARRAYCHILD
		UASSERT(child);
		UASSERT(child->mUpEntity == nullptr && child->mDownEntity == nullptr);
		if (mChildHead)
		{
			UASSERT(mChildHead->mUpEntity == nullptr);

			child->mDownEntity = mChildHead;
			mChildHead->mUpEntity = child;
			mChildHead = child;
		}
		else
		{
			mChildHead = child;
		}
		mNumChild++;

#else
		mIndexInParent = mChildren.Add(child);
#endif
	}

	void Entity::Tick()
	{
		if (FlagTest(EEntityFlag::EEF_Tickable | EEntityFlag::EEF_Alive | EEntityFlag::EEF_BeginPlayWasCalled))
			OnTick();

		DoOnChilChild([](Entity* child) {
			child->Tick();
		});
	}

	//////////////////////////////////////////////////////////////////////////
	void Entity::Init(Entity* parent, World* world)
	{
		mWorld = world;

		FlagSet(EEF_Alive | EEF_Initilized);

		if (parent)
		{
			mParent = parent;
			mParent->AddChildToList(this);
		}
		else
		{
			mWorld->mRootEntities.Add(this);
		}
		mWorld->mEntities.Add(this);
	}

	void Entity::RegisterToWorld(World* world)
	{
		UASSERT(mWorld == nullptr);
		mWorld = world;
	}

	void Entity::OnParentChanged()
	{
		ULOG_MESSAGE("");
	}



	//////////////////////////////////////////////////////////////////////////
	void Entity::Destroy()
	{
		if (FlagTestAnClear(EEF_Alive))
		{
			//detaching from parent
			{
				if (mParent) mParent->RemoveChildFromList(this);
				mParent = nullptr;
			}

			GetWorld()->PushToLimbo(this);
			//this pass sets children flag and adds them to limbo
			Destroy_Pass0();
			//this pass calls related destroy functions
			Destroy_Pass1();
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Entity::Destroy_Pass0()
	{
		DoOnChilChild([](Entity* child) {
			if (child->FlagTestAnClear(EEF_Alive))
			{
				child->GetWorld()->PushToLimbo(child);
				child->Destroy_Pass0();
			}
		});
	}
	//////////////////////////////////////////////////////////////////////////
	void Entity::Destroy_Pass1()
	{
		if (!FlagTest(EEF_OnDestroyWasCalled))
		{
			FlagSet(EEF_OnDestroyWasCalled);

			if (FlagTest(EEF_BeginPlayWasCalled))
				OnEndPlay(EPR_Destroy);

			OnDestroy();

			DoOnChilChild([](Entity* child) {
				child->Destroy_Pass1();
			});
		}
	}
	//////////////////////////////////////////////////////////////////////////
	unsigned Entity::NumChild() const
	{
#ifndef UUSE_ARRAYCHILD
		return mNumChild;
#else
		return mChildren.Length();
#endif
	}


	UCLASS_BEGIN_IMPL(Entity)
	UCLASS_END_IMPL(Entity)
};