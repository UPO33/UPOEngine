#pragma once

#include "UWorld.h"


#define UUSE_ARRAYCHILD

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class World;
	class WorldRS;
	class WorldTicking;
	class Entity;
	class EntityRS;
	class EntityVisualizer;

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


// 		EEF_Tick0Enable,
// 		EEF_Tick1Enable,
// 		EEF_Tick2Enable,
		EEF_Default = EEF_Alive | EEF_Initilized
	};
	enum EEndPlayReason
	{
		EPR_EndPlay,
		EPR_Destroy,
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

		
		

		Name				mName;

		size_t				mIndexInWorld;
		size_t				mTickRegistered : 1;
		size_t				mTickPendingAdd : 1;
		size_t				mIsWorldTransformInvDirty : 1;
		size_t				mIsSelected : 1;

		Matrix4				mLocalTransform;
		Matrix4				mWorldTransform;
		Matrix4				mWorldTransformInv;
		AABB				mBound;

		EntityVisualizer*	mVisualizer;

		void* mRS;


		void UpdateChildrenTransform();
		void CalcLocalTrsFromWorldAndParent();
		void TagRenderDirty(unsigned flags);


	public:


		Entity();
		~Entity();

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
		Name GetName() const { return mName; }

		template<typename TVisualizerClass = EntityVisualizer> TVisualizerClass* GetVisualizer() const { return (TVisualizerClass*)mVisualizer; }

		//return pointer to render state if any
		template< typename TRSClass = EntityRS> TRSClass* GetRS() const { return (TRSClass*)mRS; }
		
		//return pointer to render state data
		void* GetRSMemory() const;

		const Matrix4& GetWorldTransform() const { return mWorldTransform; }
		const Matrix4& GetLocalTransform() const { return mLocalTransform; }
		const Matrix4& GetInvWorldTransform();

		Vec3 GetWorldPosition() const;
		Vec3 GetLocalPosition() const;
		Vec3 GetForward() const;
		Vec3 GetRight() const;
		Vec3 GetUp() const;

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
			return (!mTickPendingAdd && !mTickRegistered && mRS == nullptr);
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

		
		//////////////////////////////////////////////////////////////////////////
		virtual void SetSelected(bool selectet);

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

		void MetaBeforePropertyChange(const PropertyInfo* prp);
		void MetaAfterPropertyChange(const PropertyInfo* prp);

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
	enum EEntityRSFlag
	{
		ERS_Default = 0,

		ERS_CastShadow = 1,
		ERS_RecieveShadow = 1 << 1,
		ERS_Visible = 1 << 2,
		ERS_MainPassEnable = 1 << 3,
		ERS_CustomDepthEnable = 1 << 4,
		ERS_Static = 1 << 5,
		ERS_RenderDataValid = 1 << 6,
		ERS_Selected = 1 << 7,
	};


	//////////////////////////////////////////////////////////////////////////
	class UAPI EntityRS
	{
	public:
		Entity*		mGS;
		WorldRS*	mOwner;
		Flag		mRSFlag;
		unsigned	mPrivateIndex;
		
		EntityRS()
		{}
		EntityRS(InitZero) : mGS(nullptr), mOwner(nullptr), mRSFlag(0), mPrivateIndex(0)
		{}
		EntityRS(Entity* gs, WorldRS* wrs, EEntityRSFlag flag = ERS_Default) : mGS(gs), mOwner(wrs), mRSFlag(flag), mPrivateIndex(0)
		{}
		virtual ~EntityRS()
		{
			mGS->mRS = nullptr;
		}
		virtual void OnFetch(unsigned flag){}
		virtual unsigned GetHitID();
	};


	//////////////////////////////////////////////////////////////////////////
	class UAPI EntityVisualizer
	{
	public:
		Entity*		mOwner;

		EntityVisualizer(Entity* owner) : mOwner(owner) {}
		virtual ~EntityVisualizer() {}

		World* GetWorld() const { return mOwner->GetWorld(); }
		GameWindow* GetGameWindow() const { return mOwner->GetWorld()->GetGameWindow(); }

		template<typename TEntityClass = Entity> TEntityClass* GetOwner() const { return (TEntityClass*)mOwner; }

		virtual void OnVisualize(PrimitiveBatch*) {}

	};
};