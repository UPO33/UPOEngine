#pragma once

#include "UEntityCamera.h"

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

		EntityFreeCamera();

		void OnTick() override;
		void FocusAt(const AABB&);
	};


};