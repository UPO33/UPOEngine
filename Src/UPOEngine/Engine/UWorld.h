#pragma once

#include "UEntity.h"
#include "UAsset.h"
#include "UWorldTimer.h"
#include "UWorldTicking.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class AWorldSetting;

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
		EWF_Max
	};
	//////////////////////////////////////////////////////////////////////////
	struct EntityCreationParam
	{
		ClassInfo*	mClass;
		Entity*		mParent;
	};
	//////////////////////////////////////////////////////////////////////////
	class UAPI World : public Asset
	{
		UCLASS(World, Asset)

		friend Entity;

	private:
		TArray<Entity*>			mEntities;
		bool					mIsPlaying = false;
		bool					mIsFirstTick = false;
		WorldTickResult			mCurTickResult;
		float					mSecondsSincePlay = 0;
		TimeCounterHigh			mTimerSincePlay;
		bool					mIsInTick = false;
		bool					mDoExitPlay = false;
		AWorldSetting*			mWorldSetting = nullptr;

		unsigned				mNumDestroyedEntity = 0;
		unsigned				mNumTickSinceLastDestroy = 0;

		bool mDoBeginPlay = false;
		bool mDoEndPlay = false;
		
		Entity*		mRootEntity;
		TArray<Entity*>		mRootEntities;

		TArray<Entity*>		mStaticEntities;

		unsigned MAX_DESTROYED_ENTITY = 16;

		WorldTicking		mTicking;
		WorldTimer			mTimer;

	public:
		WorldTicking& GetTicking() { return mTicking; }
		WorldTimer& GetTimer() { return mTimer; }

		void SetPlaying(bool playing);

		void AddEntityToList(Entity* ent);
		//////////////////////////////////////////////////////////////////////////
		Entity* CreateEntity(EntityCreationParam& param);
		//////////////////////////////////////////////////////////////////////////
		void SingleTick(WorldTickResult& result);

		void PerformBeginPlay();
		void PerformEndPlay();
		void PerformTick();
		void KillDestroyedEntities();

		void PushToLimbo(Entity* deadEntity);
		//children of the entity considered dead
		//void GoToCemetery(Entity* entity);
		void IncCemetery() { mNumDestroyedEntity++; }

		Entity* GetRootEntity();
		const TArray<Entity*>& GetRootEntities() const { return mRootEntities; }
	};
	//////////////////////////////////////////////////////////////////////////
// 	class Renderer;


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