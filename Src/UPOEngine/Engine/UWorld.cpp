#include "UWorld.h"
#include "UComponent.h"
#include "UEntity.h"
#include "UEntityCamera.h"
#include "UEntityStaticMesh.h"
#include "UEntityFreeCamera.h"
#include "UWorldRS.h"
#include "UGameWindow.h"
#include "UInput.h"
#include "UCanvas.h"
#include "../Meta/UMeta.h"
#include "UPrimitiveBatch.h"
#include "UAudioDevice.h"


namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	//returns the size of render state corresponding to the entity, zero if entity has not render state
	size_t UGetEntityClassRSSize(const ClassInfo* entityClass)
	{
		if (entityClass->IsBaseOf(EntityStaticMesh::GetClassInfoStatic())) return sizeof(EntityStaticMeshRS);

		if (entityClass->IsBaseOf(EntityCamera::GetClassInfoStatic())) return sizeof(EntityCameraRS);
		
		return 0;
	}


	EntityRS* UGetEntityRS(Entity* ent, size_t sizeOfRS)
	{
		if (sizeOfRS == 0) return 0;
		

	}
	//////////////////////////////////////////////////////////////////////////
	Entity* World::CreateEntity(EntityCreationParam& param)
	{
		ClassInfo* entityClass = param.mClass;
		if (!entityClass->IsBaseOf(Entity::GetClassInfoStatic()) || entityClass->IsAbstract())
		{
			ULOG_ERROR("invalid class!");
			return nullptr;
		}

		//both entity and rs must be 16 byte aligned

		size_t rsSize = UGetEntityClassRSSize(entityClass);
		Entity* newEntity = nullptr;
		if (rsSize)
		{
			size_t rsOffset = entityClass->GetSize() + UCACHE_ALIGN;
			newEntity = (Entity*)GObjectSys()->NewObject(entityClass, rsOffset + rsSize);
			//newEntity->mRS = (void*)(((size_t)newEntity) + rsOffset);
		}
		else
		{
			newEntity = NewObject<Entity>(entityClass);
		}
		UASSERT(newEntity);

		//set new name for entity
		{
			static String StrEntityName = "Entity00000000000";
			static Name NameEntity = "Entity";
			static unsigned EntityCreateCounter = 0;

			StrEntityName.SetFormatted("%s_%i", param.mClass->GetName().CStr(), EntityCreateCounter++);
			newEntity->mName = StrEntityName;
		}

		newEntity->Init(param.mParent, this);
		newEntity->OnConstruct();

		if (mIsPlaying)
		{
			newEntity->FlagSet(EEF_BeginPlayWasCalled);
			newEntity->OnBeginPlay();
		}

		PushToPendingAddToRS(newEntity);

		mOnEntityCreated.InvokeAll(newEntity);

		return newEntity;
	}
	//////////////////////////////////////////////////////////////////////////
	void World::SingleTick(WorldTickResult& result)
	{
		
	}
	//////////////////////////////////////////////////////////////////////////
	void World::Tick(float delta)
	{
		if (!mIsAlive)return;

		mDeltaTimeReal = delta;
		mDeltaTime = delta * mDeltaScale;

		CheckPendingKills();

		mIsInTick = true;

		const bool bTickableWorld = mWorldType == EWorldType::EGame || mWorldType == EWorldType::EPlayInEditor;

		if (mIsPlaying && bTickableWorld)
		{
			CheckPerformBeginPlay();
			
			mTimer.Tick(mDeltaTime);
			mTicking.Tick(mDeltaTime);
			

			mSecondsSincePlay += mDeltaTime;
			mSecondsSincePlayReal += mDeltaTimeReal;
		}
		else
		{
			CheckPerformEndPlay();
		}

		mIsInTick = false;

		mPrimitiveBatch->Tick(mDeltaTime);

		mNumTickSinceLastDestroy++;

		if(mFreeCamera)
		{
			mFreeCamera->OnTick();
			
			if (mHasAduio && mAudioDevice)
				mAudioDevice->SetListenerPositionOrientation(mFreeCamera->GetWorldPosition(), mFreeCamera->GetForward(), mFreeCamera->GetUp());
		}


		//visualizers
		if(GetPrimitiveBatch() && GetGameWindow())
		{
			PrimitiveBatch* pb = GetPrimitiveBatch();

			for(Entity* ent : mEntities)
			{
				if (ent && ent->IsAlive() && ent->GetVisualizer())
				{
					ent->GetVisualizer()->OnVisualize(pb);
					pb->SetHitProxy(nullptr);
				}
			}
		}


	}

	void World::CheckPerformBeginPlay()
	{
		if (!mDoBeginPlay) return;
		mDoBeginPlay = false;
		
		size_t len = mEntities.Length();
		for (size_t i = 0; i < len; i++)
		{
			Entity* ent = mEntities[i];
			if (ent && ent->FlagTest(EEF_Alive))
			{
				if (!ent->FlagTest(EEF_BeginPlayWasCalled))
				{
					ent->FlagSet(EEF_BeginPlayWasCalled);
					ent->OnBeginPlay();
				}
			}
		}
	}

	void World::CheckPerformEndPlay()
	{
		if (!mDoEndPlay) return;
		mDoEndPlay = false;

		size_t len = mEntities.Length();
		for (size_t i = 0; i < len; i++)
		{
			Entity* ent = mEntities[i];
			if (ent && ent->FlagTest(EEF_Alive))
			{
				if (ent->FlagTest(EEF_BeginPlayWasCalled))
				{
					ent->FlagClear(EEF_BeginPlayWasCalled);
					ent->OnEndPlay(EEndPlayReason::EPR_EndPlay);
				}
			}
		}

		mSecondsSincePlay = 0;
	}

	void World::PerformTick()
	{
		mTimer.Tick(mDeltaTime);
		mTicking.Tick(mDeltaTime);
	}

	void World::KillDestroyedEntities()
	{
		if(mNumTickSinceLastDestroy > 60 && mNumDestroyedEntity)
		{
			mEntities.RemoveIf([&](Entity* ent) {
				if (ent->FlagTest(EEF_Alive))
				{
					return false;
				}
				else
				{
					DeleteObject(ent);
					return true;
				}
			});
			

			

			mNumDestroyedEntity = 0;
			mNumTickSinceLastDestroy = 0;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void World::PushToLimbo(Entity* deadEntity)
	{
// 		mNumTickSinceLastDestroy = 0;
// 		mNumDestroyedEntity++;
// 		mEntitiesPendingKill.Add(deadEntity->mIndexInWorld);
// 
// 		Entity* entity = nullptr;
// 		if (entity->IsReadyToDie())
// 		{
// 			DeleteObject(entity);
// 		}
	}


	Entity* World::GetRootEntity()
	{
		return mRootEntity;
	}

	void World::Release()
	{
		mTimer.StopAll();
		
	}

	void World::CreateRS()
	{
		mRS = new WorldRS(this);
	}

	void World::DestroyRS()
	{
		if (mRS) delete mRS;
		mRS = nullptr;
	}

	World* World::Duplicate()
	{
		return nullptr;
	}

	Canvas* World::GetCanvas() const
	{
		if (mMainWindow && mMainWindow->mCanvas) return mMainWindow->mCanvas;
		return nullptr;
	}

	PrimitiveBatch* World::GetPrimitiveBatch() const
	{
		return mPrimitiveBatch;
	}

	InputState* World::GetInputState() const
	{
		if (mMainWindow && mMainWindow->mInputState) return mMainWindow->mInputState;
	}

	void World::PushToPendingAddToRS(Entity* ent)
	{
		mEntitiesPendingAddToRS->Add(ent);
	}

	void World::PushToPendingKill(Entity* ent)
	{
		UASSERT(ent);

		mEntitiesPendingKill.Add(ent);
		if(ent->mRS) mEntitiesPendingDestroyFromRS->Add(ent);
	}



	void World::KillEntity(Entity* entity)
	{
		ULOG_MESSAGE("kiling entity [%]", entity->mName.CStr());
		DeleteObject(entity);
	}

	void World::CheckPendingKills()
	{
		int	numPendingKill = (int)mEntitiesPendingKill.Length();
		int index = 0;
		while (index < numPendingKill)
		{
			Entity* entity = mEntitiesPendingKill[index];
			if (entity && entity->IsReadyToDie())
			{
				//remove from entities list
				mEntities.LastElement()->mIndexInWorld = entity->mIndexInWorld;
				mEntities.RemoveAtSwap(entity->mIndexInWorld);
				//remove from pending kill list
				mEntitiesPendingKill.RemoveAtSwap(index);
				KillEntity(entity);
				numPendingKill--;
			}
			else
			{
				index++;
			}
		}
	}



	World::World(const WorldInitParam& param) :
		mTickEndEvent(false, false),
		mFetchCompleted(false, false)
	{
		mDeltaScale = 1;
		mDeltaTimeReal = mDeltaTime = 1.0f / 60.0f;
		mSecondsSincePlay = mSecondsSincePlayReal = 0;

		mWorldType = param.mWorldType;


		mEntitiesPendingAddToRS = new EntityArray;
		mEntitiesPendingDestroyFromRS = new EntityArray;

		{
			mFreeCamera = NewObject<EntityFreeCamera>();
			mFreeCamera->mWorld = this;
		}

		EnqueueRenderCommandAndWait([this]() {
			CreateRS();
		});


		mRootEntity = NewObject<Entity>();
		mRootEntity->mWorld = this;
		mRootEntity->mIndexInWorld = 0;
		mRootEntity->mName = "Root";

		mPrimitiveBatch = new PrimitiveBatch(this);

		if (param.mStartPlaying) SetPlaying();
	}

	//////////////////////////////////////////////////////////////////////////
	World::~World()
	{
		EnqueueRenderCommandAndWait([this]() {
			DestroyRS();
		});


		DeleteObject(mFreeCamera);
		DeleteObject(mRootEntity);
	}
	void World::SetPlaying()
	{
		UASSERT(!mIsInTick);
		if (mIsPlaying) return;

		mIsPlaying = true;
		mDoBeginPlay = true;
	}

	void World::PausePlaying()
	{
		UASSERT(!mIsInTick);
		if (mIsPlaying && !mIsPaused)
		{
			mIsPaused = true;
		}
	}

	void World::ResumePlaying()
	{
		UASSERT(!mIsInTick);
		if (mIsPlaying && mIsPaused)
		{
			mIsPaused = false;
		}
	}

	void World::StopPlaying()
	{
		UASSERT(!mIsInTick);
		if (!mIsPlaying) return;

		mIsPlaying = false;
		mDoEndPlay = true;
	}

	void World::PauseGame()
	{

	}

	void World::ResumeGame()
	{

	}

	

	UCLASS_BEGIN_IMPL(World)
	UCLASS_END_IMPL(World)

	WorldEditorData::WorldEditorData(World* world)
	{
		mOwner = world;
		mCameraPosition = Vec3::ZERO;
		mCameraRotation = Vec3::ZERO;
		mCameraFOV = 60;
		mNearZPlane = 0.1f;
		mFazZPlane = 100000;
	}

	WorldEditorData::~WorldEditorData()
	{

	}

	void WorldEditorData::MoveGlobal(Vec3 offset)
	{
		mCameraPosition += offset;
	}

	void WorldEditorData::RotateGlobal(Vec3 offset)
	{
		mCameraRotation += offset;
	}

	void WorldEditorData::MoveLocal(Vec3 offset)
	{
		mCameraPosition += Quat::MakeEuler(mCameraRotation).Rotate(offset);
	}

	void WorldEditorData::Tick()
	{

	}

};