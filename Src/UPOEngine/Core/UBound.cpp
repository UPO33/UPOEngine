#include "UBound.h"

namespace UPO
{

// 	void AABB::MakeFromPoints(const Vec3* points, unsigned numPoints)
// 	{
// 		UASSERT(points && numPoints >= 2);
// 
// 		Vec3 vMin = Min(points[0], points[1]);
// 		Vec3 vMax = Max(points[0], points[1]);
// 
// 		for (unsigned i = 2; i < numPoints; i++)
// 		{
// 			vMin = Min(vMin, points[i]);
// 			vMax = Max(vMax, points[i]);
// 		}
// 
// 		mMin = vMin;
// 		mMax = vMax;
// 	}

	template<typename T> T& ElemAtStride(const void* array, unsigned index, unsigned stride)
	{
		  return *((T*)(((char*)array) + index * stride));
	}
	AABB AABB::MakeFromPoints(const void* points, unsigned numPoints, unsigned stride)
	{
		UASSERT(points && stride > 0 && numPoints >= 2);

		Vec3 vMin = Min(ElemAtStride<Vec3>(points, 0, stride), ElemAtStride<Vec3>(points, 1, stride));
		Vec3 vMax = Max(ElemAtStride<Vec3>(points, 0, stride), ElemAtStride<Vec3>(points, 1, stride));

		for (unsigned i = 2; i < numPoints; i++)
		{
			vMin = Min(vMin, ElemAtStride<Vec3>(points, i, stride));
			vMax = Max(vMax, ElemAtStride<Vec3>(points, 1, stride));
		}

		return AABB(vMin, vMax);
	}

	int AABB::Intersect(const Plane& plane)
	{
		Vec3 vec1, vec2;

		for (unsigned i = 0; i < 3; i++)
		{
			if (plane[i] >= 0)
			{
				vec1[i] = mMin[i];
				vec2[i] = mMax[i];
			}
			else
			{
				vec1[i] = mMax[i];
				vec2[i] = mMax[i];
			}
		}

		if (plane.DotCoord(vec2) > 0)
		{
			return 1;
		}
		if (plane.DotCoord(vec1) > 0)
		{
			return 2;
		}
		return 0;
// 		float posSide = (normal.X * vec2.X) + (normal.Y * vec2.Y) + (normal.Y * vec2.Y) + planeDistance;
// 		if (posSide > 0)
// 		{
// 			//box is completely on positive side of plane
// 			return;
// 		}
// 		float negSide = (normal.X * vec1.X) + (normal.Y * vec1.Y) + (normal.Y * vec1.Y) + planeDistance;
// 		if (negSide < 0)
// 		{
// 			//box is completely on negative side of plane
// 			return;
// 		}
// 		//if you get this far, box is currently intersecting the plane.
	}

};