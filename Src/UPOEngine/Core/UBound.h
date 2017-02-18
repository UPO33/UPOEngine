#pragma once

#include "UPlane.h"
#include "UMatrix.h"

namespace UPO
{
	struct UAPI alignas(16) AABB
	{
		Vec3 mMin, mMax;

		AABB(){}
		AABB(InitZero) { mMin = mMax = Vec3(0); }
		AABB(const Vec3& min, const Vec3& max) : mMin(min), mMax(max) {}

		static AABB MakeFromPoints(const void* points, unsigned numPoints, unsigned stride);

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
		int Intersect(const Plane&) const;

		Vec3 GetCorner(bool positiveX, bool positiveY, bool positiveZ) const
		{
			Vec3 ret;
			ret.mX = positiveX ? mMax.mX : mMin.mX;
			ret.mY = positiveY ? mMax.mY : mMin.mY;
			ret.mZ = positiveZ ? mMax.mZ : mMin.mZ;
			return ret;
		}

		static AABB TransformBound(const AABB&, const Transform&);
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