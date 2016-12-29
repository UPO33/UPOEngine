#include "UEntity.h"
#include "../Meta/UMeta.h"
#include "UWorld.h"
#include "UEntityStaticMesh.h"

namespace UPO
{

	void Entity::UpdateChildrenTransform()
	{
		mWorldTransform = mParent->mWorldTransform * mLocalTransform;
		mIsWorldTransformInvDirty = true;
		TagRenderDataDirty(ERD_Transform);
	}

	void Entity::CalcLocalTrsFromWorldAndParent()
	{
		if (IsRoot())
		{
			mLocalTransform = mWorldTransform;
		}
		else
		{
			mLocalTransform = mParent->GetInvWorldTransform() * mWorldTransform;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	Entity::Entity()
	{
		static String StrEntityName = "Entity00000000000";
		static Name NameEntity = "Entity";
		static unsigned EntityCTorCounter = 0;

		mParent = nullptr;
		
#ifndef UUSE_ARRAYCHILD
		mNumChild = 0;
		mChildHead = mDownEntity = mUpEntity = nullptr;
#endif
		mEntityFlag = EEF_Default;

		StrEntityName.SetFormatted("%s_%i", GetClassInfo()->GetName().CStr(), EntityCTorCounter++);

		mName = StrEntityName;

		mTickRegistered = false;
		mTickPendingAdd = false;
		mIsWorldTransformInvDirty = false;

		mWorldTransform = mLocalTransform = mWorldTransformInv = Transform::IDENTITY;
	}

	void Entity::TagRenderDataDirty(unsigned flag)
	{
		UASSERT(flag == EEF_RenderDataDirty || flag == EEF_RenderDataTransformDirty);

		if (!FlagTest(EEF_RenderDataDirty))
			mWorld->mEntitiesRenderDataDirty.Add(this);
		FlagSet(flag);
	}

	//////////////////////////////////////////////////////////////////////////
	bool Entity::IsRoot() const
	{
		return mParent == mWorld->mRootEntity;
	}
	//////////////////////////////////////////////////////////////////////////
	Entity* Entity::GetParent() const
	{
		if (mParent == mWorld->mRootEntity) return nullptr;
		return mParent;
	}
	//////////////////////////////////////////////////////////////////////////
	Entity* Entity::GetChild(unsigned index) const
	{
#ifdef UUSE_ARRAYCHILD
		return mChildren[index];
#else
		Entity* iter = mChildHead;
		while (iter)
		{
			if (index == 0) break;

			index--;
			iter = iter->mDownEntity;
		}
		return iter;
#endif
	}
	//////////////////////////////////////////////////////////////////////////
	void Entity::AttachTo(Entity* newParent)
	{
		if (this == mWorld->mRootEntity) return;
		if (mParent == newParent) return;

		if (FlagTest(EEF_Alive | EEF_Registered))
		{
			UASSERT(mParent);

			if (newParent == nullptr) // detach?
			{
				if (IsRoot()) return;	//currently is detached

				newParent = mWorld->mRootEntity;
			}


			if (newParent->FlagTest(EEF_Alive | EEF_Registered) && GetWorld() == newParent->GetWorld())
			{
				if (newParent->IsSubsetOf(this))
				{
					ULOG_ERROR("Entity [%s] cant be attachet to its child", mName.CStr());
					return;
				}

				
				mParent->RemoveChildFromList(this);
				mParent = newParent;
				mParent->AddChildToList(this);

				CalcLocalTrsFromWorldAndParent();

				OnParentChanged();
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
	const Matrix4& Entity::GetInvWorldTransform()
	{
		if (mIsWorldTransformInvDirty)
		{
			mWorldTransformInv = mWorldTransform;
			mWorldTransformInv.InvertAffine();
		}
		return mWorldTransformInv;
	}
	//////////////////////////////////////////////////////////////////////////
	void Entity::SetLocalTransform(const Transform& localTrs)
	{
		mLocalTransform = localTrs;
		if (IsRoot())
		{
			mWorldTransform = localTrs;
		}
		else
		{
			mWorldTransform = mParent->mWorldTransform * localTrs;
		}
		mIsWorldTransformInvDirty = true;
		TagRenderDataDirty(ERD_Transform);
		UpdateChildrenTransform();
	}
	//////////////////////////////////////////////////////////////////////////
	void Entity::SetWorldTransform(const Transform& worldTrs)
	{
		mWorldTransform = worldTrs;
		mIsWorldTransformInvDirty = true;
		CalcLocalTrsFromWorldAndParent();
		TagRenderDataDirty(ERD_Transform);
		UpdateChildrenTransform();
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
		mChildren.RemoveShift(mChildren.Find(child));
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
		mChildren.Add(child);
#endif
	}

	void Entity::Tick()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	void Entity::Init(Entity* parent, World* world)
	{
		mWorld = world;

		mParent = (parent && parent->IsAlive()) ? parent : world->mRootEntity;
		mParent->AddChildToList(this);

		
		mIndexInWorld = (unsigned)mWorld->mEntities.Add(this);
	}

	void Entity::OnConstruct()
	{

	}
	void Entity::OnBeginPlay()
	{
		mWorld->GetTicking()->AdjustEntityTick(this);
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
	void Entity::SetTickEnable(bool enable)
	{
		if (!IsAlive() || IsTickEnable() == enable) return;
		enable ? FlagSet(EEF_Tickable) : FlagClear(EEF_Tickable);

		if (FlagTest(EEF_BeginPlayWasCalled))
		{
			mWorld->GetTicking()->AdjustEntityTick(this);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void Entity::Destroy()
	{
		UASSERT(mWorld->mRootEntity != this);

		if (FlagTestAnClear(EEF_Alive))
		{
			//for this entity and its children
			//		clear EEF_Alive flag, add to pending kill array
			//for this entity and its children
			//		call OnDestroy and...

			//detaching from parent
			{
				if (mParent) mParent->RemoveChildFromList(this);
				mParent = nullptr;
			}

			mWorld->mEntitiesPendingKill.Add(this);

			mWorld->PushToPendingDestroyFromRS(this);


			//GetWorld()->PushToLimbo(this);
			//this pass sets children flag and adds them to limbo
			Destroy_Pass0();
			//this pass calls related destroy functions
			Destroy_Pass1();

			mWorld->mOnEntityDestroyed.InvokeAll(this);

			ULOG_MESSAGE("Entity [%s] Destroyed", mName.CStr());
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Entity::Destroy_Pass0()
	{
// 		for (size_t i = 0; i < mChildren.Length(); i++)
// 		{
// 			Entity* child = mChildren[i];
// 			if (child->FlagTestAnClear(EEF_Alive))
// 			{
// 				child->mWorld->mEntitiesPendingKill.Add(child);
// 				child->mWorld->PushToPendingDestroyFromRS(child);
// 				//child->GetWorld()->PushToLimbo(child);
// 				child->Destroy_Pass0();
// 			}
// 		}
		DoOnChilChild([](Entity* child) {
			if (child->FlagTestAnClear(EEF_Alive))
			{
				child->mWorld->mEntitiesPendingKill.Add(child);
				child->mWorld->PushToPendingDestroyFromRS(child);
				//child->GetWorld()->PushToLimbo(child);
// 				child->Destroy_Pass0();
			}
		});
	}
	//////////////////////////////////////////////////////////////////////////
	//call OnDestroyed and OnEndPlay if required
	void Entity::Destroy_Pass1()
	{
		if (!FlagTest(EEF_OnDestroyWasCalled))
		{
			if (FlagTest(EEF_BeginPlayWasCalled))
				OnEndPlay(EPR_Destroy);

			FlagSet(EEF_OnDestroyWasCalled);
			OnDestroy();

// 			for (size_t i = 0; i < mChildren.Length(); i++)
// 			{
// 				mChildren[i]->Destroy_Pass1();
// 			}
			DoOnChilChild([](Entity* child) {
				if (!child->FlagTest(EEF_OnDestroyWasCalled))
				{
					if (child->FlagTest(EEF_BeginPlayWasCalled))
						child->OnEndPlay(EPR_Destroy);

					child->FlagSet(EEF_OnDestroyWasCalled);
					child->OnDestroy();
				}});
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
		UPROPERTY(mName, UATTR_Hidden())
		UPROPERTY(mEntityFlag, UATTR_Hidden())
		UPROPERTY(mTestVec3)
		UPROPERTY(mLocalTransform)
	UCLASS_END_IMPL(Entity)



};