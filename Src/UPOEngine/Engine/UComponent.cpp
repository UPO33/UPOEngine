#include "UComponent.h"

namespace UPO
{
#if 0
	Component::Component()
	{
		static Name NameComponent = "Component";

		mParent = nullptr;
		mNumChild = 0;

		mChildHeadComp = mDownComp = mUpComp = nullptr;

		mName = NameComponent;

	}

	Component* Component::GetParent() const
	{
		return mParent;
	}

	Component* Component::GetChild(unsigned index) const
	{
		Component* iter = mChildHeadComp;
		while (iter)
		{
			if (index == 0) break;

			iter = iter->mDownComp;
		}
		return iter;
	}

	void Component::AttachTo(Component* newParent)
	{
		if (FlagTest(EEF_Alive | EEF_Registered | EEF_Initilized))
		{
			if (mParent == newParent) return;

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
				if (newParent->IsSubsetOf(this))
				{
					ULOG_ERROR("a component cant be attachet to its child");
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
	//////////////////////////////////////////////////////////////////////////
	bool Component::IsSubsetOf(Component* parent) const
	{
		UASSERT(parent);

		Component* iter = mParent;
		while (iter)
		{
			if (iter == parent) return true;
			iter = iter->mParent;
		}
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	//remove a child from list, child should be exist
	void Component::RemoveChildFromList(Component* child)
	{
		UASSERT(child);
		if (mChildHeadComp == child)
		{
			UASSERT(child->mUpComp == nullptr);	//head child must not have up component
			if (child->mDownComp)
			{
				mChildHeadComp = child->mDownComp;
				mChildHeadComp->mUpComp = nullptr;
				child->mDownComp = nullptr;
			}
			else
			{
				mChildHeadComp = nullptr;
			}
			child->mParent = nullptr;
		}
		else
		{
			if (child->mUpComp)
				child->mUpComp->mDownComp = child->mDownComp;
			if (child->mDownComp)
				child->mDownComp->mUpComp = child->mUpComp;

			child->mUpComp = nullptr;
			child->mDownComp = nullptr;
			child->mParent = nullptr;
		}
		mNumChild--;
	}
	//////////////////////////////////////////////////////////////////////////
	void Component::AddChildToList(Component* child)
	{
		UASSERT(child);
		UASSERT(child->mUpComp == nullptr && child->mDownComp == nullptr);
		if (mChildHeadComp)
		{
			UASSERT(mChildHeadComp->mUpComp == nullptr);
			
			child->mDownComp = mChildHeadComp;
			mChildHeadComp->mUpComp = child;
			mChildHeadComp = child;
		}
		else
		{
			mChildHeadComp = child;
		}
		mNumChild++;
	}

	void Component::Tick()
	{
		if (FlagTest(EEntityFlag::EEF_Tickable | EEntityFlag::EEF_Alive | EEntityFlag::EEF_BeginPlayWasCalled))
			OnTick();

		DoOnChilChild([](Component* child) {
			child->Tick();
		});
	}

	void Component::RegisterToWorld(World* world)
	{
		UASSERT(mWorld == nullptr);
		mWorld = world;
	}

	void Component::OnParentChanged()
	{
		ULOG_MESSAGE("");
	}

	void Component::Destroy()
	{
		Destroy_Pass0();
		Destroy_Pass1();
	}

	void Component::Destroy_Pass0()
	{
		if (FlagTestAnClear(EEntityFlag::EEF_Alive))
		{
			DoOnChilChild([](Component* child) {
				child->Destroy_Pass0();
			});
		}
	}
	void Component::Destroy_Pass1()
	{
		if(!FlagTest(EEntityFlag::EEF_OnDestroyWasCalled))
		{
			FlagSet(EEntityFlag::EEF_OnDestroyWasCalled);

			if (FlagTest(EEntityFlag::EEF_BeginPlayWasCalled))
				OnEndPlay();

			OnDestroy();

			DoOnChilChild([](Component* child) {
				child->Destroy_Pass1();
			});
		}
	}
	//////////////////////////////////////////////////////////////////////////
	unsigned Component::NumChild() const
	{
		return mNumChild;
	}
#endif
};