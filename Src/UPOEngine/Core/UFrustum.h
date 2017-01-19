#pragma once
#include "UPlane.h"
#include "UMatrix.h"
#include "UBound.h"

namespace UPO
{


	struct UAPI alignas(16) Frustum
	{
		enum 
		{
			ENear, EFar, ERight, ELeft, ETop, EBottom
		};

		Plane	mPlanes[6];	//The planes are defined such that the normal points towards the inside of the view frustum.

		bool IsInside(const Vec3& point) const;
		bool IsInside(const SphereBound& sphere);
		bool IsInside(const AABB& box);
		void MakeFromProjection(const Matrix4& projection);

	};


};