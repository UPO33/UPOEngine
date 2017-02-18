#pragma once

#include "UAsset.h"
#include "UWorldTimer.h"
#include "UWorldTicking.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class AWorldSetting;
	class WorldRS;
	class Entity;
	class EntityCamera;
	class GameWindow;
	class Canvas;
	class PrimitiveBatch;
	class InputState;
	class AudioDevice;

	//////////////////////////////////////////////////////////////////////////
	struct WorldTickResult
	{
		unsigned	mNumCreatedEntity = 0;
		unsigned	mNumDestroyedEntity = 0;
		unsigned	mNumTickedEntity = 0;
		float		mConsumedTimeMS = 0;

	};
	//////////////////////////////////////////////////////////////////////////
	enum EWorldFlag
	{
		EWF_Alive,

		EWF_Default = EWF_Alive,

		EWF_Max
	};
	//////////////////////////////////////////////////////////////////////////
	enum class EWorldType
	{
		EGame,
		EEditor,
		EPlayInEditor,
	};

	//////////////////////////////////////////////////////////////////////////
	struct EntityCreationParam
	{
		ClassInfo*	mClass = nullptr;
		Entity*		mParent = nullptr;
		Transform	mWorldTransform = Transform::IDENTITY;

		EntityCreationParam(ClassInfo* theClass = nullptr, Entity* parent = nullptr) 
			: mClass(theClass), mParent(parent), mWorldTransform(Transform::IDENTITY) {}

	};

	typedef TArray<Entity*>	EntityArray;

	//////////////////////////////////////////////////////////////////////////
	class UAPI WorldEditorData
	{
	public:
		World*		mOwner;
		Vec3		mCameraPosition;
		Vec3		mCameraRotation;
		float		mCameraFOV;
		float		mNearZPlane;
		float		mFazZPlane;

		WorldEditorData(World*);
		~WorldEditorData();

		void MoveGlobal(Vec3 offset);
		void RotateGlobal(Vec3 offset);
		void MoveLocal(Vec3 offset);

		void Tick();
	};

	//////////////////////////////////////////////////////////////////////////
	struct WorldInitParam
	{
		EWorldType		mWorldType = EWorldType::EEditor;
		Name			mName = "World";
		bool			mStartPlaying = false;
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI World : public Asset
	{
		UCLASS(World, Asset)

		friend Entity;
		friend WorldRS;

	private:
		EntityArray				mEntities;
		bool					mIsPlaying = false;
		bool					mIsPaused = false;
		bool					mIsFirstTick = false;
		bool					mIsInTick = false;
		bool					mDoExitPlay = false;
		AWorldSetting*			mWorldSetting = nullptr;

		unsigned				mNumDestroyedEntity = 0;
		unsigned				mNumTickSinceLastDestroy = 0;
		

		bool mDoBeginPlay = false;
		bool mDoEndPlay = false;
		
		Entity*		mRootEntity;


		unsigned MAX_DESTROYED_ENTITY = 16;

		WorldTicking		mTicking;
		WorldTimer			mTimer;

		float	mDeltaTime;
		float	mDeltaTimeReal;
		float	mDeltaScale;
		float	mSecondsSincePlay;
		float	mSecondsSincePlayReal;

		bool	mIsGamePaused = false;

		TArray<Entity*>		mEntitiesPendingKill;
		
		WorldRS*			mRS = nullptr;
		

		EntityArray*		mEntitiesPendingAddToRS;
		EntityArray*		mEntitiesPendingDestroyFromRS;
		EntityArray			mEntitiesRenderDataDirty;

		Event				mFetchCompleted;
		Event				mTickEndEvent;
		EWorldType			mWorldType;
		EntityCamera*		mFreeCamera = nullptr;
		

	public:
		TDelegateMulti<void, Entity*>	mOnEntityCreated;
		TDelegateMulti<void, Entity*>	mOnEntityDestroyed;
		bool							mIsAlive = true;
		int								mPriority = 1;
		GameWindow*						mMainWindow = nullptr;
		PrimitiveBatch*					mPrimitiveBatch = nullptr;
		bool							mHasAduio = true;
		AudioDevice*					mAudioDevice = nullptr;

	private:
		void PushToPendingAddToRS(Entity* ent);
		void PushToPendingKill(Entity* ent);


		void KillEntity(Entity*);
		void CheckPendingKills();

	public:
		WorldTicking* GetTicking() { return &mTicking; }
		WorldTimer* GetTimer() { return &mTimer; }

		World(const WorldInitParam& param = WorldInitParam());
		~World();
		
		WorldRS* GetRS() const { return mRS; }

		void SetPlaying();
		void PausePlaying();
		void ResumePlaying();
		void StopPlaying();
		bool IsPlaying() const { return mIsPlaying; }

		void PauseGame();
		void ResumeGame();

		void SetDeltaScale(float scale) { mDeltaScale = scale; }
		float GetDeltaScale() const { return mDeltaScale; }
		float GetDeltaTime() const { return mDeltaTime; }
		float GetDeltaTimeUnscaled() const { return mDeltaTimeReal; }
		float GetSecondsSincePlay() const { return mSecondsSincePlay; }
		float GetSecondsSincePlayUnscaled() const { return mSecondsSincePlayReal; }

		EntityCamera* GetFreeCamera() const { return mFreeCamera; }

		//////////////////////////////////////////////////////////////////////////
		Entity* CreateEntity(EntityCreationParam& param);

		template <typename TEntityClass> TEntityClass* CreateEntity(Entity* parent)
		{
			return (TEntityClass*)CreateEntity(EntityCreationParam(TEntityClass::GetClassInfoStatic(), parent));
		}

		//////////////////////////////////////////////////////////////////////////
		void SingleTick(WorldTickResult& result);
		void Tick(float delta);
		void CheckPerformBeginPlay();
		void CheckPerformEndPlay();
		void PerformTick();
		void KillDestroyedEntities();

		void PushToLimbo(Entity* deadEntity);
		//children of the entity considered dead
		//void GoToCemetery(Entity* entity);
		void IncCemetery() { mNumDestroyedEntity++; }

		Entity* GetRootEntity();

		void Release();

		void CreateRS();
		void DestroyRS();

		World* Duplicate();

		bool ProjectWorldToScreen(const Vec3& worldPos, Vec2& outScreenPos, EntityCamera* camera);
		bool ProjectScreenToWorld(const Vec2& screenPos, Vec3& outWorldPos, EntityCamera* camera);

		GameWindow* GetGameWindow() const { return mMainWindow; }
		Canvas* GetCanvas() const;
		PrimitiveBatch* GetPrimitiveBatch() const;
		InputState* GetInputState() const;

		EWorldType GetType() const { return mWorldType; }
	};
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
// 	class EntityCamera
// 	{
// 	public:
// 		float mFOV = 70;
// 		Vector3 mPosition;
// 		Quaternion mRotation;
// 		Vector3 mRotationXYZ;
// 		float mMovementSpeed = 10;
// 		Renderer* mRenderer = nullptr;
// 
// 		EntityCamera(Renderer* renderer) : mRenderer(renderer)
// 		{
// 			mPosition = Vector3(0, 0, 20);
// 		}
// 
// 
// 		void Tick() 
// 		{
// 			Vector3 vInput;
// 			if (Input::IsKeyDown('W'))vInput.z = -1;
// 			if (Input::IsKeyDown('S'))vInput.z = 1;
// 			if (Input::IsKeyDown('D'))vInput.x = 1;
// 			if (Input::IsKeyDown('A'))vInput.x = -1;
// 			if (Input::IsKeyDown('E'))vInput.y = 1;
// 			if (Input::IsKeyDown('Q'))vInput.y = -1;
// 			int mouseX, mouseY;
// 			if(Input::IsMouseDown(EMouseButton::EMB_Right))
// 			{
// 				Input::GetMouseVelocity(mouseX, mouseY);
// 				mRotationXYZ.y -= mouseX * DEG2RAD;
// 				mRotationXYZ.x -= mouseY * DEG2RAD;
// 			}
// 			mFOV -= Sign(Input::GetMouseWheelDelta()) * 4;
// 			mRotation = Quaternion::CreateFromYawPitchRoll(mRotationXYZ.y, mRotationXYZ.x, mRotationXYZ.z);
// 
// 			Vector3 vMovement = Vector3::Transform(vInput, mRotation);
// 			mPosition += vMovement * mMovementSpeed;
// 		}
// 		Vector3 GetPosition() const { return mPosition; }
// 		Vector3 GetForward() const 
// 		{
// 			Vector3 forward = Vector3::Transform(Vector3::Forward, mRotation); 
// 			forward.Normalize();
// 			return forward;
// 		}
// 		Matrix GetViewMatrix()
// 		{
// 			//return Matrix::CreateLookAt(mPosition, Vector3(0,0,0), Vector3::Up);
// 
// 			Vector3 forward = Vector3::Transform(Vector3::Forward, mRotation);
// 			Vector3 up = Vector3::Transform(Vector3::Up, mRotation);
// 			
// 			return Matrix::CreateWorld(mPosition, forward, up).Invert().Transpose();
// 		}
// 		Matrix GetProjMatrix()
// 		{
// 			return Matrix::CreatePerspectiveFieldOfView(mFOV * DEG2RAD, mRenderer->GetAspectRatio(), 0.1, 10000).Transpose();
// 		}
// 		Matrix GetViewProj()
// 		{
// 			return GetProjMatrix() * GetViewMatrix();
// 		}
// 	};

	//////////////////////////////////////////////////////////////////////////
};