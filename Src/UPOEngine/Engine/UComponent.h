#pragma once

#include "UEntity.h"

namespace UPO
{
#if 0
	//////////////////////////////////////////////////////////////////////////
	class UAPI Component : public Entity
	{
		UCLASS(Component, Entity)

		friend World;

		Component*	mParent;
		Component*	mChildHeadComp;
		Component*  mUpComp;
		Component*  mDownComp;

		Name		mName;
		unsigned	mNumChild;
		unsigned	mIndexInWorld;

	public:
		Component();

		Component* GetParent() const;
		unsigned NumChild() const;
		Component* GetChild(unsigned index) const;
		void AttachTo(Component* newParent);

		bool IsSubsetOf(Component* parent) const;


		virtual void RegisterToWorld(World* world);

		virtual void OnParentChanged();

		void Destroy();

	private:
		void Destroy_Pass0();
		void Destroy_Pass1();

		void RemoveChildFromList(Component* child);
		void AddChildToList(Component* child);

		void Tick();

		template<typename Lambda> void DoOnChilChild(Lambda proc)
		{
			Component* iter = mChildHeadComp;
			while (iter)
			{
				proc(iter);
				iter = iter->mDownComp;
			}
		}
	};
#endif // 0

};