#include "UMatrix.h"
#include "../Meta/UMeta.h"

#include <DirectXMath.h>
// #include <SimpleMath.h>
// #include <DirectXCollision.h>

namespace UPO
{
	const Matrix3 Matrix3::IDENTITY(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1));
	const Matrix4 Matrix4::IDENTITY(Vec4(1, 0, 0, 0), Vec4(0, 1, 0, 0), Vec4(0, 0, 1, 0), Vec4(0, 0, 0, 1));
	
	using namespace DirectX;

	UCLASS_BEGIN_IMPL(Matrix4)
	UCLASS_END_IMPL(Matrix4)
	
		Matrix3& Matrix3::operator=(const Matrix3& copy)
	{
		mElements[0][0] = copy.mElements[0][0];
		mElements[0][1] = copy.mElements[0][1];
		mElements[0][2] = copy.mElements[0][2];

		mElements[1][0] = copy.mElements[1][0];
		mElements[1][1] = copy.mElements[1][1];
		mElements[1][2] = copy.mElements[1][2];

		mElements[2][0] = copy.mElements[2][0];
		mElements[2][1] = copy.mElements[2][1];
		mElements[2][2] = copy.mElements[2][2];
		return *this;
	}

	Matrix3::Matrix3(const Matrix3& copy)
	{
		mElements[0][0] = copy.mElements[0][0];
		mElements[0][1] = copy.mElements[0][1];
		mElements[0][2] = copy.mElements[0][2];

		mElements[1][0] = copy.mElements[1][0];
		mElements[1][1] = copy.mElements[1][1];
		mElements[1][2] = copy.mElements[1][2];

		mElements[2][0] = copy.mElements[2][0];
		mElements[2][1] = copy.mElements[2][1];
		mElements[2][2] = copy.mElements[2][2];
	}

	Matrix3::Matrix3(float all)
	{
		mElements[0][0] = all;
		mElements[0][1] = all;
		mElements[0][2] = all;

		mElements[1][0] = all;
		mElements[1][1] = all;
		mElements[1][2] = all;

		mElements[2][0] = all;
		mElements[2][1] = all;
		mElements[2][2] = all;
	}

	Matrix3::Matrix3(float f0, float f1, float f2,
		float f3, float f4, float f5,
		float f6, float f7, float f8)
	{
		mElements[0][0] = f0;
		mElements[0][1] = f1;
		mElements[0][2] = f2;

		mElements[1][0] = f3;
		mElements[1][1] = f4;
		mElements[1][2] = f5;

		mElements[2][0] = f6;
		mElements[2][1] = f7;
		mElements[2][2] = f8;
	}
	Matrix3::Matrix3(const Vec3& c0, const Vec3& c1, const Vec3& c2)
	{
		mColumn[0] = c0;		mColumn[1] = c1;		mColumn[2] = c2;
	}
	void Matrix3::TransformVector(const Vec3& v, Vec3& out) const
	{
		UASSERT(&v != &out);

		// 		out = GetColumn(0) * v.mX + GetColumn(1) * v.mY + GetColumn(2) * v.mZ;

		out.mX = (mElements[0][0] * v.mX) + (mElements[1][0] * v.mY) + (mElements[2][0] * v.mZ);
		out.mY = (mElements[0][1] * v.mX) + (mElements[1][1] * v.mY) + (mElements[2][1] * v.mZ);
		out.mZ = (mElements[0][2] * v.mX) + (mElements[1][2] * v.mY) + (mElements[2][2] * v.mZ);
	}

	void Matrix3::GetTranspose(Matrix3& out) const
	{
		UASSERT(this != &out);
		out.mElements[0][0] = mElements[0][0];
		out.mElements[0][1] = mElements[1][0];
		out.mElements[0][2] = mElements[2][0];

		out.mElements[1][0] = mElements[0][1];
		out.mElements[1][1] = mElements[1][1];
		out.mElements[1][2] = mElements[2][1];

		out.mElements[2][0] = mElements[0][2];
		out.mElements[2][1] = mElements[1][2];
		out.mElements[2][2] = mElements[2][2];
	}

	void Matrix3::Transpose()
	{
		/*
		0	3	6
		1	4	7
		2	5	8

		std::swap(m[1],  m[3]);
		std::swap(m[2],  m[6]);
		std::swap(m[5],  m[7]);
		*/

		Swap(mElements[0][1], mElements[1][0]);
		Swap(mElements[0][2], mElements[2][0]);
		Swap(mElements[1][2], mElements[2][1]);
	}

	const Vec3& Matrix3::GetColumn(unsigned index) const
	{
		UASSERT(index < 3);
		return mColumn[index];
	}

	UPO::Vec3 Matrix3::GetRow(unsigned index) const
	{
		return Vec3(mElements[0][index], mElements[1][index], mElements[2][index]);
	}

	void Matrix3::Mul(const Matrix3& a, const Matrix3& b, Matrix3& out)
	{
		out.mElements[0][0] = a.GetRow(0) | b.GetColumn(0);
		out.mElements[0][1] = a.GetRow(1) | b.GetColumn(0);
		out.mElements[0][2] = a.GetRow(2) | b.GetColumn(0);

		out.mElements[1][0] = a.GetRow(0) | b.GetColumn(1);
		out.mElements[1][1] = a.GetRow(1) | b.GetColumn(1);
		out.mElements[1][2] = a.GetRow(2) | b.GetColumn(1);

		out.mElements[2][0] = a.GetRow(0) | b.GetColumn(2);
		out.mElements[2][1] = a.GetRow(1) | b.GetColumn(2);
		out.mElements[2][2] = a.GetRow(2) | b.GetColumn(2);
	}

	float Matrix3::Determinant() const
	{
		return	mElements[0][0] * (mElements[1][1] * mElements[2][2] - mElements[1][2] * mElements[2][1]) -
			mElements[0][1] * (mElements[1][0] * mElements[2][2] - mElements[1][2] * mElements[2][0]) +
			mElements[0][2] * (mElements[1][0] * mElements[2][1] - mElements[1][1] * mElements[2][0]);
	}

	void Matrix3::Invert()
	{
		float determinant;
		float tmp[9];

		tmp[0] = mElements[1][1] * mElements[2][2] - mElements[1][2] * mElements[2][1];
		tmp[1] = mElements[0][2] * mElements[2][1] - mElements[0][1] * mElements[2][2];
		tmp[2] = mElements[0][1] * mElements[1][2] - mElements[0][2] * mElements[1][1];
		tmp[3] = mElements[1][2] * mElements[2][0] - mElements[1][0] * mElements[2][2];
		tmp[4] = mElements[0][0] * mElements[2][2] - mElements[0][2] * mElements[2][0];
		tmp[5] = mElements[0][2] * mElements[1][0] - mElements[0][0] * mElements[1][2];
		tmp[6] = mElements[1][0] * mElements[2][1] - mElements[1][1] * mElements[2][0];
		tmp[7] = mElements[0][1] * mElements[2][0] - mElements[0][0] * mElements[2][1];
		tmp[8] = mElements[0][0] * mElements[1][1] - mElements[0][1] * mElements[1][0];

		// check determinant if it is 0
		determinant = mElements[0][0] * tmp[0] + mElements[0][1] * tmp[3] + mElements[0][2] * tmp[6];
		if (Abs(determinant) <= FLOAT_EPSILON)
		{
			SetIndentity();
			return;
		}

		float invDeterminant = 1.0f / determinant;
		mElements[0][0] = invDeterminant * tmp[0];
		mElements[0][1] = invDeterminant * tmp[1];
		mElements[0][2] = invDeterminant * tmp[2];
		mElements[1][0] = invDeterminant * tmp[3];
		mElements[1][1] = invDeterminant * tmp[4];
		mElements[1][2] = invDeterminant * tmp[5];
		mElements[2][0] = invDeterminant * tmp[6];
		mElements[2][1] = invDeterminant * tmp[7];
		mElements[2][2] = invDeterminant * tmp[8];
	}

	Matrix4::Matrix4(const Matrix4& m)
	{
		for (unsigned i = 0; i < 4; i++) mRows[i] = m.mRows[i];
	}

	Matrix4::Matrix4(const Vec4& r0, const Vec4& r1, const Vec4& r2, const Vec4& r3)
	{
		mRows[0] = r0;
		mRows[1] = r1;
		mRows[2] = r2;
		mRows[3] = r3;
	}

	Matrix4& Matrix4::operator=(const Matrix4& m)
	{
		for (unsigned i = 0; i < 4; i++) mRows[i] = m.mRows[i];
		return *this;
	}

	void Matrix4::MakeTranslation(const Vec3& position)
	{
		*((XMMATRIX*)this) = XMMatrixTranslation(position.mX, position.mY, position.mZ);
	}

	void Matrix4::MakeScale(const Vec3& scale)
	{
		*((XMMATRIX*)this) = XMMatrixScaling(scale.mX, scale.mY, scale.mZ);
	}

	void Matrix4::MakeRotationX(float angleDegree)
	{
		*((XMMATRIX*)this) = XMMatrixRotationX(angleDegree * DEG2RAD);
	}

	void Matrix4::MakeRotationY(float angleDegree)
	{
		*((XMMATRIX*)this) = XMMatrixRotationY(angleDegree * DEG2RAD);
	}

	void Matrix4::MakeRotationZ(float angleDegree)
	{
		*((XMMATRIX*)this) = XMMatrixRotationZ(angleDegree * DEG2RAD);
	}

	void Matrix4::MakeRotationXYZ(const Vec3& angle)
	{
		*((XMMATRIX*)this) = XMMatrixRotationRollPitchYaw(angle.mX * DEG2RAD, angle.mY * DEG2RAD, angle.mZ * DEG2RAD);
	}

	void Matrix4::MakeRotationDir(const Vec3& _forward)
	{
		Vec3 forward = _forward;
		forward.Normalize();
		Vec3 up;
		if (Abs(forward.mX) < FLOAT_EPSILON && Abs(forward.mZ) < FLOAT_EPSILON)
		{
			if (forward.mY > 0) // forward vector is pointing +Y axis
				up = Vec3(0, 0, -1);
			else // forward vector is pointing -Y axis
				up = Vec3(0, 0, 1);
		}
		else
		{
			up = Vec3(0, 1, 0);
		}

		Vec3 right = up ^ forward;
		right.Normalize();
		up =  forward ^ right;
		up.Normalize();

		mRows[0] = Vec4(right, 0);
		mRows[1] = Vec4(up, 0);
		mRows[2] = Vec4(forward, 0);
		mRows[3] = Vec4(0, 0, 0, 1);
	}

	void Matrix4::MakeRotationQuat(const Quat& rotation)
	{
		XMStoreFloat4x4A((XMFLOAT4X4A*)this, XMMatrixRotationQuaternion(XMLoadFloat4A((XMFLOAT4A*)&rotation)));
	}

	void Matrix4::MakePerspective(float fovDegree, float aspect, float zNear, float zFar)
	{
		*((XMMATRIX*)this) = XMMatrixPerspectiveFovLH(fovDegree * DEG2RAD, aspect, zNear, zFar);
	}

	void Matrix4::MakeOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		*((XMMATRIX*)this) = XMMatrixOrthographicOffCenterLH(left, right, bottom, top, zNear, zFar);
	}

	void Matrix4::MakeOrtho(float width, float height, float zNear, float zFar)
	{
		*((XMMATRIX*)this) = XMMatrixOrthographicLH(width, height, zNear, zFar);
	}

	void Matrix4::MakeViewLookAt(const Vec3& eyePos, const Vec3& focusPos, const Vec3& up)
	{
		XMStoreFloat4x4A((XMFLOAT4X4A*)this, XMMatrixLookAtLH(XMLoadFloat3((XMFLOAT3*)&eyePos), XMLoadFloat3((XMFLOAT3*)&focusPos), XMLoadFloat3((XMFLOAT3*)&up)));
	}

	void Matrix4::MakeTransform(const Vec3& translation, const Vec3& rotation, const Vec3& scale)
	{
		Matrix4 ms, mr, mt;
		ms.MakeScale(scale);
		mr.MakeRotationXYZ(rotation);
		mt.MakeTranslation(translation);
		*this = ms * mr * mt;
	}

	void Matrix4::MakeTransform(const Vec3& translation, const Quat& rotation, const Vec3& scale)
	{
		Matrix4 ms, mr, mt;
		ms.MakeScale(scale);
		mr.MakeRotationQuat(rotation);
		mt.MakeTranslation(translation);
		*this = ms * mr * mt;
	}

	void Matrix4::GetTranspose(Matrix4& out) const
	{
		XMStoreFloat4x4A((XMFLOAT4X4A*)out.mElements, XMMatrixTranspose(XMLoadFloat4x4A((XMFLOAT4X4A*)this)));
	}

	void Matrix4::Transpose()
	{
		*this = GetTranspose();
	}

	void Matrix4::TransformVec4(const Vec4& v, Vec4& out) const
	{
		((XMVECTOR&)out) = XMVector4Transform((XMVECTOR&)v, (XMMATRIX&)*this);
	}



	void Matrix4::TransformVec3W0(const Vec3& v, Vec3& out) const
	{
		Vec4 v4 = TransformVec4(Vec4(v, 0));
		out = v4;
	}

	void Matrix4::TransformVec3W1(const Vec3& v, Vec3& out) const
	{
		Vec4 v4 = TransformVec4(Vec4(v, 1));
		out = v4;
	}

	void Matrix4::Mul(const Matrix4& a, const Matrix4& b, Matrix4& out)
	{
		((XMMATRIX&)out) = XMMatrixMultiply((XMMATRIX&)a, (XMMATRIX&)b);
	}

	void Matrix4::MulTranspose(const Matrix4& a, const Matrix4& b, Matrix4& out)
	{
		((XMMATRIX&)out) = XMMatrixMultiplyTranspose((XMMATRIX&)a, (XMMATRIX&)b);
	}

	Vec3 Matrix4::GetTranslation() const
	{
		return Vec3(mRows[3]);
	}

	Vec3 Matrix4::GetScale() const
	{
		return Vec3(mRows[0].Length3(), mRows[1].Length3(), mRows[2].Length3());
	}

	void Matrix4::RemoveScaling()
	{
		for (unsigned i = 0; i < 3; i++) 
			((Vec3*)(mRows + i))->Normalize();
	}



	Vec3 Matrix4::GetRotationEuler() const
	{
// #define MatElem(x, y) mElements[x + y * 4]
#define MatElem(x, y) mElements[x * 4 + y]

		Vec3 out;
		out.mX = ::atan2(MatElem(1, 2), MatElem(2, 2)) * RAD2DEG;
		out.mY = ::atan2(-MatElem(0, 2), Sqrt(MatElem(1, 2) * MatElem(1, 2) + MatElem(2, 2) * MatElem(2, 2))) * RAD2DEG;
		out.mZ = ::atan2(MatElem(0, 1), MatElem(0, 0)) * RAD2DEG;
		return out;

#undef MatElem
	}
	
	
	void Matrix4::GetRotationQuat(Quat& out) const
	{
		XMStoreFloat4A((XMFLOAT4A*)&out, XMQuaternionRotationMatrix(XMLoadFloat4x4A((XMFLOAT4X4A*)this)));
	}

	float Matrix4::Determinant()
	{
		return XMVectorGetX(XMMatrixDeterminant(XMLoadFloat4x4A((XMFLOAT4X4A*)this)));
	}

	void Matrix4::Invert()
	{
		XMStoreFloat4x4A((XMFLOAT4X4A*)this, XMMatrixInverse(nullptr, XMLoadFloat4x4A((XMFLOAT4X4A*)this)));
	}

	Matrix4 Matrix4::GetInverse() const
	{
		Matrix4 ret;
		XMStoreFloat4x4A((XMFLOAT4X4A*)&ret, XMMatrixInverse(nullptr, XMLoadFloat4x4A((XMFLOAT4X4A*)this)));
		return ret;
	}

	bool Matrix4::IsEqual(const Matrix4& other, float epsilon /*= 0.00001f*/) const
	{
		for (unsigned i = 0; i < 16; i++)
		{
			if (Abs(mElements[i] - other.mElements[i]) > epsilon) return false;
		}
		return true;
	}

	bool Matrix4::IsOrthogonal(float epsilon /*= 0.000001f*/)
	{
		UASSERT(false);
		return false;
	}

	void Matrix4::MetaSerialize(Stream& stream)
	{
		stream.Bytes(this, sizeof(Matrix4));
	}




	

	Vec3 Quat::Rotate(const Vec3& v) const
	{
		const float vx = 2.0f*v.mX;
		const float vy = 2.0f*v.mY;
		const float vz = 2.0f*v.mZ;
		const float w2 = mW*mW - 0.5f;
		const float dot2 = (mX*vx + mY*vy + mZ*vz);
		return Vec3
		(
			(vx*w2 + (mY * vz - mZ * vy)*mW + mX*dot2),
			(vy*w2 + (mZ * vx - mX * vz)*mW + mY*dot2),
			(vz*w2 + (mX * vy - mY * vx)*mW + mZ*dot2)
		);
	}

	Vec3 Quat::RotateInv(const Vec3& v) const
	{
		const float vx = 2.0f*v.mX;
		const float vy = 2.0f*v.mY;
		const float vz = 2.0f*v.mZ;
		const float w2 = mW*mW - 0.5f;
		const float dot2 = (mX*vx + mY*vy + mZ*vz);
		return Vec3
		(
			(vx*w2 - (mY * vz - mZ * vy)*mW + mX*dot2),
			(vy*w2 - (mZ * vx - mX * vz)*mW + mY*dot2),
			(vz*w2 - (mX * vy - mY * vx)*mW + mZ*dot2)
		);
	}

	Quat Quat::MakeEuler(const Vec3& angles)
	{
		Quat ret;
		XMStoreFloat4A((XMFLOAT4A*)&ret, XMQuaternionRotationRollPitchYaw(angles.mX * DEG2RAD, angles.mY * DEG2RAD, angles.mZ * DEG2RAD));
		return ret;
	}

	Quat Quat::GetConjugate() const
	{
		return Quat(-mX, -mY, -mZ, mW);
	}

	void Quat::Invert()
	{
		float rLengthSq =  1.0f / MagnitudeSq();

		mX = -mX * rLengthSq;
		mY = -mY * rLengthSq;
		mZ = -mZ * rLengthSq;
		mW = mW * rLengthSq;
	}

	Quat Quat::GetInverse() const
	{
		Quat ret(*this);
		ret.Invert();
		return ret;
	}

	const Quat Quat::IDENTITY(0, 0, 0, 1);

	void Quat::Conjugate()
	{
		mX = -mX;
		mY = -mY;
		mZ = -mZ;
	}

	Quat& Quat::operator*=(const Quat& q)
	{
		XMStoreFloat4A((XMFLOAT4A*)this, XMQuaternionMultiply(XMLoadFloat4A((XMFLOAT4A*)this), XMLoadFloat4A((XMFLOAT4A*)&q)));
		return *this;
	}

	Quat Quat::operator*(const Quat& q) const
	{
		Quat ret;
		XMStoreFloat4A((XMFLOAT4A*)&ret ,XMQuaternionMultiply(XMLoadFloat4A((XMFLOAT4A*)this), XMLoadFloat4A((XMFLOAT4A*)&q)));
		return ret;
	}


	UAPI StringStreamOut& operator<<(StringStreamOut& stream, const Quat& q)
	{
		stream << "{" << q.mX << ", " << q.mY << ", " << q.mZ << ", " << q.mW << "}";
		return stream;
	}
// 
// 	UAPI StringStreamOut& operator<<(StringStreamOut& stream, const Matrix4& matrix)
// 	{
// // 		stream << matrix.mRows[0];
// // 		stream << "\n";
// // 		stream << matrix.mRows[1];
// // 		stream << "\n";
// // 		stream << matrix.mRows[2];
// // 		stream << "\n";
// // 		stream << matrix.mRows[3];
// 		return stream;
// 	}
// 
// 	UAPI StringStreamOut& operator<<(StringStreamOut& stream, const Matrix3& matrix)
// 	{
// 		UASSERT(false);
// 		return stream;
// 	}

};

