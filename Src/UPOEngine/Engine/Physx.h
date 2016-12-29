#pragma once

#include <PxPhysics.h>
#include <PxPhysicsAPI.h>
#include <extensions\PxExtensionsAPI.h>

#include "../Core/UBasic.h"

namespace UPO
{
	class DefaulPhysFoundation : public physx::PxFoundation
	{

	};
	class UPXAllocator : public physx::PxAllocatorCallback
	{
		virtual void* allocate(size_t size, const char* typeName, const char* filename, int line) override
		{
			
		}

		virtual void deallocate(void* ptr) override
		{
		}

	};
	class UPXErrorCallback : public physx::PxErrorCallback
	{
		virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override
		{
		}
	};


	class Physx
	{
		physx::PxPhysics*			mPhysicsSDK;
		physx::PxFoundation*		mFoundation;
		physx::PxTolerancesScale	mTolerancesScale;
		physx::PxProfileZoneManager* mProfileZoneManager;
		physx::PxCooking*			mCooking;
		// 		physx::PxDefaultErrorCallback	mErrorCallback;
		// 		physx::PxDefaultAllocator		mAllocator;

		UPXAllocator		mAllocator;
		UPXErrorCallback	mErrorCallback;

		void Init()
		{
			mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mAllocator, mErrorCallback);
			UASSERT(mFoundation);

			{
				//adjust mTolerancesScale
			}

			mPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mTolerancesScale);
			UASSERT(mPhysicsSDK);

			bool recordMemoryAllocations = true;
			mProfileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager(mFoundation);
			UASSERT(mProfileZoneManager);

			mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, physx::PxCookingParams(mTolerancesScale));
			UASSERT(mCooking);

			UASSERT(PxInitExtensions(*mPhysicsSDK));

		}
		void Release()
		{
			PxCloseExtensions();

			if (mCooking) mCooking->release();
			if (mProfileZoneManager) mProfileZoneManager->release();
			if (mPhysicsSDK) mPhysicsSDK->release();
			if (mFoundation) mFoundation->release();
		}



	};
	

};