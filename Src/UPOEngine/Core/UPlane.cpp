#include "UPlane.h"

// #include <SimpleMath.h>
// #include <DirectXMath.h>

namespace UPO
{
// 	 namespace DXM = DirectX::SimpleMath;



	Plane::Plane(const Vec3& point0, const Vec3& point1, const Vec3& point2)
	{
		Vec3 n = ~(point1 - point0) ^ (point2 - point0);
		mX = n.mX;
		mY = n.mY;
		mZ = n.mZ;
		mD = -(n | point0);
	}


}