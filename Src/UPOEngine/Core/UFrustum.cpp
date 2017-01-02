#include "UFrustum.h"

namespace UPO
{

	bool Frustum::IsInside(const Vec3& point) const
	{
		for (unsigned i = 0; i < 6; i++)
		{
			if (mPlanes[i].DotCoord(point) < 0) return false;
		}
		return true;
	}

	bool Frustum::IsInside(const SphereBound& sphere)
	{
		for (unsigned i = 0; i < 6; i++)
		{
			float dist = mPlanes[i].DotCoord(sphere.mPosition);
			if (dist < -sphere.mRadius)
				return false;	
		}
		return true;
	}

	bool Frustum::IsInside(const AABB& box)
	{
		for (unsigned i = 0; i < 6; i++)
		{
			//The positive vertex is the vertex from the box that is further along the normal's direction
			//If the p-vertex is on the wrong side of the plane, the box can be immediately rejected
			Vec3 positiveVertex = box.mMin;
			if (mPlanes[i].mX >= 0) positiveVertex.mX = box.mMax.mX;
			if (mPlanes[i].mY >= 0) positiveVertex.mY = box.mMax.mY;
			if (mPlanes[i].mZ >= 0) positiveVertex.mZ = box.mMax.mZ;

			if (mPlanes[i].DotCoord(positiveVertex) < 0) // is outside ?
				return false;
		}
		return true;
	}

}