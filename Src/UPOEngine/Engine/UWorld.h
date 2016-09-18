#pragma once

#include "UEntity.h"

namespace UPO
{
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
		Name		mName;
		Vec3		mPosition;
	};
	//////////////////////////////////////////////////////////////////////////
	class UAPI World : public Object
	{
		UCLASS(World, Object)

	private:
		TArray<Entity*>			mEntities;
		bool					mIsPlaying = false;
		bool					mIsFirtsTick = false;
		WorldTickResult			mCurTickResult;
		float					mSecondsSincePlay = 0;
		TimeCounterHigh			mTimerSincePlay;
		bool					mIsInTick = false;
		bool					mDoExitPlay = false;
	public:
		void Play() 
		{
			if (mIsPlaying) return;

			mIsPlaying = true;
			mIsFirtsTick = true;
			mTimerSincePlay.Start();
		}
		void ExitPlay()
		{
			if (!mIsPlaying) return;

			mIsPlaying = false;
		}
		void AddEntityToList(Entity* ent)
		{
			mEntities.Add(ent);
		}
		//////////////////////////////////////////////////////////////////////////
		Entity* CreateEntity(EntityCreationParam& param);
		//////////////////////////////////////////////////////////////////////////
		void SingleTick(WorldTickResult& result);
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