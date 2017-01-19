#include "UWorld.h"
#include "UComponent.h"
#include "UEntity.h"
#include "UEntityCamera.h"
#include "UEntityStaticMesh.h"
#include "UWorldRS.h"
#include "UGameWindow.h"
#include "UInput.h"
#include "UCanvas.h"
#include "../Meta/UMeta.h"
#include "../GFX/UPrimitiveBatch.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class UAPI EntityFreeCamera : public EntityCamera
	{
		UCLASS(EntityFreeCamera, EntityCamera)

		Vec3	mPosition = Vec3::ZERO;
		Vec3	mRotationEuler = Vec3::ZERO;
		float	mMoveSpeed = 100;
		Vec3	mFocusPoint;
		Vec3	mFocusPointRotation;
		float	mFocusDist = 30;
		
		EntityFreeCamera()
		{
			mPosition = Vec3(30, 30, -30);
			mRotationEuler = Matrix4::SRotationDir(Vec3::ZERO - mPosition).GetRotationEuler();
		}
		void OnTick() override
		{
			float moveSpeed = mMoveSpeed * GetWorld()->GetDeltaTime();
			auto input = GetWorld()->GetInputState();
			Vec2 mouseVelocity = input->GetMouseVelocity();

			if (input->IsKeyPressed(EKC_LeftCtrl))	
			{
				Vec3 vDir = Quat::MakeEuler(mRotationEuler).Rotate(Vec3(0,0, mFocusDist));
				mFocusPoint = mPosition + vDir;
				
				mFocusPointRotation = Matrix4::SRotationDir(mPosition - mFocusPoint).GetRotationEuler();
			}
			if (input->IsKeyDown(EKC_LeftCtrl))
			{
				mFocusDist -= Clamp(input->GetMouseWheelDelta(), -5, 5);
				mFocusDist = Clamp(mFocusDist, 10, 10000);

				if(input->IsKeyDown(EKC_MouseRight))
				{
					mFocusPointRotation.mY += mouseVelocity.mX;
					mFocusPointRotation.mX -= mouseVelocity.mY;

					Vec3 v = Quat::MakeEuler(mFocusPointRotation) * Vec3(0, 0, mFocusDist);
					Matrix4 matR, matT;
					matT.MakeTranslation(v);
					mPosition = mFocusPoint + v;
					matR.MakeRotationDir(mFocusPoint - mPosition);
					mRotationEuler = matR.GetRotationEuler();
					SetWorldTransform(matR * matT);
				}
// 				Vec3 v = Quat::MakeEuler(mFocusPointRotation) * Vec3(0, 0, mFocusDist);
// 				Matrix4 matR, matT;
// 				matT.MakeTranslation(v);
// 				mPosition = mFocusPoint + v;
// 				matR.MakeRotationDir(mFocusPoint - mPosition);
// 				mRotationEuler = matR.GetRotationEuler();
				
			}
			else
			{
				float upWorld = input->IsKeyDown(EKC_E) ? 1 : (input->IsKeyDown(EKC_Q) ? -1 : 0);
				float rightLocal = input->IsKeyDown(EKC_D) ? 1 : (input->IsKeyDown(EKC_A) ? -1 : 0);
				float forwardLocal = input->IsKeyDown(EKC_W) ? 1 : (input->IsKeyDown(EKC_S) ? -1 : 0);

				if (input->IsKeyDown(EKC_MouseMiddle))
				{
					upWorld -= Clamp(mouseVelocity.mY, -1, 1);
					rightLocal = Clamp(mouseVelocity.mX, -1, 1);
				}


				if (input->IsKeyDown(EKC_LeftShift)) moveSpeed *= 2;

				if (input->IsKeyDown(EKC_MouseRight))
				{
					mRotationEuler.mY += mouseVelocity.mX;
					mRotationEuler.mX += mouseVelocity.mY;
					int fovDelta = Clamp(input->GetMouseWheelDelta(), -1, 1) * 5;
					mFieldOfView -= fovDelta;
					mFieldOfView = Clamp(mFieldOfView, 40, 120);

					String strFov;
					strFov.SetFormatted("FOV %f", mFieldOfView);
					if (fovDelta) GetWorld()->GetCanvas()->AddDebugString(strFov);

				}




				mPosition.mY += upWorld * moveSpeed;

				mPosition += Quat::MakeEuler(mRotationEuler).Rotate(Vec3(rightLocal, 0, forwardLocal)) * moveSpeed;

				SetWorldTransform(mPosition, mRotationEuler);
			}

			
		}
	};

	UCLASS_BEGIN_IMPL(EntityFreeCamera)
	UCLASS_END_IMPL(EntityFreeCamera)



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
			newEntity->mRS = (void*)(((size_t)newEntity) + rsOffset);
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
		mDeltaTimeUnscaled = delta;
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
			mSecondsSincePlayUnscaled += mDeltaTimeUnscaled;
		}
		else
		{
			CheckPerformEndPlay();
		}

		mIsInTick = false;

		mNumTickSinceLastDestroy++;

		if(mFreeCamera)
		{
			mFreeCamera->OnTick();
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

	void World::PushToPendingDestroyFromRS(Entity* ent)
	{
		mEntitiesPendingDestroyFromRS->Add(ent);
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
		mDeltaTimeUnscaled = mDeltaTime = 1.0f / 60.0f;
		mSecondsSincePlay = mSecondsSincePlayUnscaled = 0;

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

		mPrimitiveBatch = new PrimitiveBatch();

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