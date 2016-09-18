#pragma once

#include "UBasic.h"

namespace UPO
{
	/*
	hierarchical tree node
	*/
	template <typename T> struct THTreeNode
	{
		T *mParent, *mChildHead, *mDown, *mUp;

		THTreeNode()
		{
			mParent = mChildHead = mDown = mUp = nullptr;
		}
		T* GetParent() const { return mParent; }
		T* GetChildHead() const { return mChildHead; }
		T* GetUp() const { return mUp; }
		T* GetDown() const { return mDown; }


		bool HasChild() const { return mChildHead != nullptr; }

		bool CanBeAttached() const
		{
			return mParent == nullptr && mUp == nullptr && mDown == nullptr;
		}
		void AttachTo(T* parent)
		{
			UASSERT(parent);
			UASSERT(CanBeAttached());

			if (parent->mChildHead)
			{
				mUp = nullptr;
				mDown = parent->mChildHead;
				mParent = parent;
				parent->mChildHead->mUp = this;
				parent->mChildHead = this;
			}
			else
			{
				parent->mChildHead = this;
				mParent = parent;
				mUp = nullptr;
				mDown = nullptr;
			}
		}
		void Detach()
		{
			if (mParent) //have parent ?
			{
				if (mParent->mChildHead == this) //node is child head?
				{
					UASSERT(mUp == nullptr); //when node is child head it must not has anything up
					if (mDown) // have anything down?
					{
						mParent->mChildHead = mDown;
						mDown->mUp = nullptr;
					}
					else
					{
						mParent->mChildHead = nullptr;
					}
				}
				else
				{
					if (mUp)
						mUp->mDown = mDown;
					if (mDown)
						mDown->mUp = mUp;
				}
				mParent = nullptr;
				mUp = nullptr;
				mDown = nullptr;
			}
		}
	};

#if 0
	void HTreeAttachTo(HTreeNode* nodeToAttach, HTreeNode* parent)
	{
		XASSERT(nodeToAttach->mParent == nullptr && nodeToAttach->mUp == nullptr && nodeToAttach->mDown == nullptr);
		XASSERT(parent);
		if (parent->mChildHead)
		{
			nodeToAttach->mUp = nullptr;
			nodeToAttach->mDown = parent->mChildHead;
			nodeToAttach->mParent = parent;
			parent->mChildHead->mUp = nodeToAttach;
			parent->mChildHead = nodeToAttach;
		}
		else
		{
			parent->mChildHead = nodeToAttach;
			nodeToAttach->mParent = parent;
			nodeToAttach->mUp = nullptr;
			nodeToAttach->mDown = nullptr;
		}
	}
	/*
	detach a node from its parent
	*/
	void HTreeDetachNode(HTreeNode* node)
	{
		if (node->mParent) //have parent ?
		{
			if (node->mParent->mChildHead == node) //node is child head?
			{
				XASSERT(node->mUp == nullptr); //when node is child head it must not has anything up
				if (node->mDown) // have anything down?
				{
					node->mParent->mChildHead = node->mDown;
					node->mDown->mUp = nullptr;
				}
				else
				{
					node->mParent->mChildHead = nullptr;
				}
			}
			else
			{
				if (node->mUp)
					node->mUp->mDown = node->mDown;
				if (node->mDown)
					node->mDown->mUp = node->mUp;
			}
			node->mParent = nullptr;
			node->mUp = nullptr;
			node->mDown = nullptr;
		}
	}
#endif
};