#pragma once

#include "UVector.h"


namespace UPO
{
	struct UAPI alignas(16) Plane
	{
		float mX, mY, mZ, mD;

		Plane() {}
		Plane(InitZero) { mX = mY = mZ = mD = 0; }

		Plane(float x, float y, float z, float d) : mX(x), mY(y), mZ(z), mD(d) {}
		Plane(const Vec3& normalizedNormal, float distance)
		{
			mX = normalizedNormal.mX;
			mY = normalizedNormal.mY;
			mZ = normalizedNormal.mZ;
			mD = distance;
		}
		explicit Plane(Vec4& v) : mX(v.mX), mY(v.mY), mZ(v.mZ), mD(v.mW) {}
		Plane& operator =(const Vec4& v)
		{
			mX = v.mX;
			mY = v.mY;
			mZ = v.mZ;
			mD = v.mW;
			return *this;
		}
		Plane(const Vec3& point, const Vec3& normal)
		{
			//normal must be normalized
			mX = normal.mX;
			mY = normal.mY;
			mZ = normal.mZ;
			mD = -(normal | point);
		}
		Plane(const Vec3& point0, const Vec3& point1, const Vec3& point2);

		void Normalize()
		{
			float lenSQ = mX * mX + mY * mY + mZ * mZ;
			float rsqrt = RSqrt(lenSQ);
			mX *= rsqrt;
			mY *= rsqrt;
			mZ *= rsqrt;
			mD *= rsqrt;
		}
		
		//This function can be useful in finding the signed distance from a point to a plane. 
		float DotCoord(const Vec3& v3) const 
		{ 
			return (mX * v3.mX) + (mY * v3.mY) + (mZ * v3.mZ) + mD;
		}
		//This function is useful for calculating the angle between the normal vector of the plane, and another normal vector.
		float DotNormal(const Vec3& v3) const {
			return (mX * v3.mX) + (mY * v3.mY) + (mZ * v3.mZ);
		}
		Vec3 GetNormal() const { return Vec3(mX, mY, mZ); }
		Vec3 ProjectPoint(const Vec3& point)
		{
			return point - Vec3(mX, mY, mZ) * DotCoord(point);
		}

		float& operator [] (unsigned i) { return ((float*)this)[i]; }
		float operator [] (unsigned i) const { return ((float*)this)[i]; }
	};
	inline float Dot(const Plane& a, const Plane& b)
	{
		return Dot((const Vec4&)a, (const Vec4&)b);
	}

	inline float UPlanePointSignedDistance(const Vec3& planeNormal, const Vec3& planePoint, const Vec3& point)
	{
		return planeNormal | (point - planePoint);
	}
}
