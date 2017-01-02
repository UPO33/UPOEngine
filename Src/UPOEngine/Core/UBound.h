#pragma once

#include "UPlane.h"


namespace UPO
{
	struct UAPI alignas(16) AABB
	{
		Vec3 mMin, mMax;

		AABB(){}
		AABB(InitZero) { mMin = mMax = Vec3(0); }
		AABB(const Vec3& min, const Vec3& max) : mMin(min), mMax(max) {}

		void MakeFromPoints(const Vec3* points, unsigned numPoints);

		inline Vec3 GetCenter() const
		{
			return (mMin + mMax) * 0.5f;
		}
		inline Vec3 GetExtent() const
		{
			return (mMax - mMin) * 0.5f;
		}
		inline bool IsPointInside(const Vec3& point) const
		{
			return point > mMin && point < mMax;
		}
		int Intersect(const Plane&);
	};

	struct SphereBound
	{
		Vec3	mPosition;
		float	mRadius;
	};

	struct UAPI alignas(16) BoxSphereBound
	{
		Vec3	mCenter;
		float	mRadiusSq;
		Vec3	mExtent;
	};
};