#pragma once

#include "UWorld.h"
#include "UEngineBase.h"


#define UUSE_ARRAYCHILD

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class World;
	class WorldRS;
	class WorldTicking;
	class EntityRS;

	//////////////////////////////////////////////////////////////////////////
	enum EEntityFlag
	{
		EEF_Alive = 1,
		EEF_Initilized = 1 << 1,
		EEF_Tickable = 1 << 2,
		EEF_BeginPlayWasCalled = 1 << 3,
		EEF_IsArchiving = 1 << 4,
		EEF_Registered = 1 << 5,
		EEF_OnDestroyWasCalled = 1 << 6,

		EEF_RenderDataDirty = 1 << 7,
		EEF_RenderDataTransformDirty = 1 << 8,
		EEF_RenderDataMiscDirty = 1 << 9,


		EEF_CastShadow = 1 << 10,
		EEF_RecieveShadow = 1 << 11,
		EEF_Visible = 1 << 12,
		EEF_MainPassEnable = 1 << 13,
		EEF_CustomDepthEnable = 1 << 14,
		EEF_Static = 1 << 15,
		EEF_RenderDataValid = 1 << 16,

// 		EEF_Tick0Enable,
// 		EEF_Tick1Enable,
// 		EEF_Tick2Enable,
		EEF_Default = EEF_Alive | EEF_Initilized | EEF_Visible | EEF_MainPassEnable | EEF_CastShadow | EEF_RecieveShadow
	};
	enum EEndPlayReason
	{
		EPR_EndPlay,
		EPR_Destroy,
	};
	enum ERendeDirty
	{
		ERD_Transform,
	};
	//////////////////////////////////////////////////////////////////////////
	class UAPI alignas(16) Entity : public Object
	{
		UCLASS(Entity, Object)


		friend World;
		friend WorldTicking;

		World*			mWorld;
		Flag			mEntityFlag;

		Entity*		mParent;
#ifndef UUSE_ARRAYCHILD
		Entity*		mChildHead;
		Entity*		mUpEntity;
		Entity*		mDownEntity;
		unsigned	mNumChild;
#else
		TArray<Entity*>	mChildren;
#endif

		
		

		Name		mName;
		
		unsigned	mIndexInWorld;
		unsigned	mTickRegistered : 1;
		unsigned	mTickPendingAdd : 1;
		unsigned	mIsWorldTransformInvDirty : 1;

		unsigned	mRenderDirtyFlag;
		bool		mStatic;

		Matrix4		mLocalTransform;
		Matrix4		mWorldTransform;
		Matrix4		mWorldTransformInv;
		AABB		mBound;

		void UpdateChildrenTransform();
		void CalcLocalTrsFromWorldAndParent();
		void TagRenderDirty(unsigned flags);
		void* mRS;

	public:

		Vec3	mTestVec3;

		Entity();

		void TagRenderDirty();	//all render state data is dirty
		void TagRenderTransformDirty();
		void TagRenderMiscDirty(); // miscellaneous such as mesh, material ,..

		bool IsRoot() const;
		Entity* GetParent() const;
		unsigned NumChild() const;
		Entity* GetChild(unsigned index) const;
		//attach this entity to another entity, set null to detach, keeps world transform
		void AttachTo(Entity* newParent);
		bool IsSubsetOf(Entity* parent) const;

		World* GetWorld() const { return mWorld; }

		//return pointer to render state if any
		template< typename TRSClass = EntityRS> TRSClass* GetRS() const { return (TRSClass*)mRS; }
		
		//return pointer to render state data
		void* GetRSMemory() const { return mRS; }

		const Matrix4& GetWorldTransform() const { return mWorldTransform; }
		const Matrix4& GetLocalTransform() const { return mLocalTransform; }
		const Matrix4& GetInvWorldTransform();
		const AABB& GetBound() const;

		void SetLocalTransform(const Transform&);
		void SetLocalTransform(const Vec3& location, const Vec3& rotation = Vec3::ZERO, const Vec3& scale = Vec3::ONE);
		void SetLocalTransform(const Vec3& location, const Quat& rotation = Quat::IDENTITY, const Vec3& scale = Vec3::ONE);

		void SetWorldTransform(const Transform&);
		void SetWorldTransform(const Vec3& location, const Vec3& rotation = Vec3::ZERO, const Vec3& scale = Vec3::ONE);
		void SetWorldTransform(const Vec3& location, const Quat& rotation = Quat::IDENTITY, const Vec3& scale = Vec3::ONE);

		bool IsAlive() const { return mEntityFlag.Test(EEF_Alive); }
		bool IsRegistered() const { return mWorld != nullptr; }

		bool IsReadyToDie() const
		{
			UASSERT(!FlagTest(EEF_Alive));
			return (!mTickPendingAdd && !mTickRegistered);
		}

		void FlagSet(unsigned flag)
		{
			mEntityFlag.Set(flag);
		}
		void FlagClear(unsigned flag)
		{
			mEntityFlag.Clear(flag);
		}
		bool FlagTest(unsigned flag) const
		{
			return mEntityFlag.Test(flag);
		}
		bool FlagTestAnClear(EEntityFlag flag)
		{
			return mEntityFlag.TestAndClear(flag);
		}
		//is called right after constructor, world is valid in this function
		virtual void OnConstruct();
		virtual void OnBeginPlay();
		virtual void OnEndPlay(EEndPlayReason) {};
		virtual void OnTick() {};
		virtual void OnDestroy() {};

		
		virtual void OnRegisterToWorld() {};

		virtual void RegisterToWorld(World* world);

		virtual void OnParentChanged();
		virtual void OnTransformChanged() {};
		virtual void OnCalcBound() { };

		bool IsTickEnable() const
		{ 
			return FlagTest(EEF_Tickable); 
		}
		void SetTickEnable(bool enable);
		void Destroy();

	private:
		void Destroy_Pass0();
		void Destroy_Pass1();

		void RemoveChildFromList(Entity* child);
		void AddChildToList(Entity* child);



		void Init(Entity* parent, World* world);

		template<typename Lambda> void DoOnChilChild(Lambda proc)
		{
#ifndef UUSE_ARRAYCHILD
			Entity* iter = mChildHead;
			while (iter)
			{
				proc(iter);
				iter->DoOnChilChild(proc);
				iter = iter->mDownEntity;
			}
#else
			for (size_t i = 0; i < mChildren.Length(); i++)
			{
				proc(mChildren[i]);
				mChildren[i]->DoOnChilChild(proc);
			}
#endif
		};

	public:
		template<typename TLambda> TimerHandle Invoke(float intervalSeconds, unsigned numRepeat, const TLambda& lambda)
		{
			return GetWorld()->GetTimer()->StartTimer(intervalSeconds, numRepeat, this, lambda);
		}
	};


	//////////////////////////////////////////////////////////////////////////
	class UAPI EntityRS
	{
	public:
		Entity*		mGS;
		WorldRS*	mOwner;
		Flag		mRSFlag;
		unsigned	mPrivateIndex;



		virtual void OnFetch(unsigned flag){}
	};
};