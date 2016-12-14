#include "UMatrix.h"

#include <DirectXMath.h>


namespace UPO
{
	const Matrix3 Matrix3::IDENTITY(Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1));
	const Matrix4 Matrix4::IDENTITY(Vec4(1, 0, 0, 0), Vec4(0, 1, 0, 0), Vec4(0, 0, 1, 0), Vec4(0, 0, 0, 1));
	

	Matrix4::Matrix4(const Matrix4& m)
	{
		mColumn[0] = m.mColumn[0];
		mColumn[1] = m.mColumn[1];
		mColumn[2] = m.mColumn[2];
		mColumn[3] = m.mColumn[3];
	}
	Matrix4& Matrix4::operator=(const Matrix4& m)
	{
		mColumn[0] = m.mColumn[0];
		mColumn[1] = m.mColumn[1];
		mColumn[2] = m.mColumn[2];
		mColumn[3] = m.mColumn[3];
		return *this;
	}
	Matrix4::Matrix4(const Vec4& c0, const Vec4& c1, const Vec4& c2, const Vec4& c3)
	{
		mColumn[0] = c0;	mColumn[1] = c1;	mColumn[2] = c2;	mColumn[3] = c3;
	}
	Matrix4::Matrix4(float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15)
	{
		mElements[0][0] = f0;		mElements[1][0] = f4;		mElements[2][0] = f8;		mElements[3][0] = f12;
		mElements[0][1] = f1;		mElements[1][1] = f5;		mElements[2][1] = f9;		mElements[3][1] = f13;
		mElements[0][2] = f2;		mElements[1][2] = f6;		mElements[2][2] = f10;		mElements[3][2] = f14;
		mElements[0][3] = f3;		mElements[1][3] = f7;		mElements[2][3] = f11;		mElements[3][3] = f15;
	}
	void Matrix4::MakeTranslation(const Vec3& position)
	{
		mElements[0][0] = 1;		mElements[1][0] = 0;		mElements[2][0] = 0;	 mElements[3][0] = position.mX;
		mElements[0][1] = 0;		mElements[1][1] = 1;		mElements[2][1] = 0;	 mElements[3][1] = position.mY;
		mElements[0][2] = 0;		mElements[1][2] = 0;		mElements[2][2] = 1;	 mElements[3][2] = position.mZ;
		mElements[0][3] = 0;		mElements[1][3] = 0;		mElements[2][3] = 0;	 mElements[3][3] = 1;
	}
	void Matrix4::MakeScale(const Vec3& scale)
	{
		mElements[0][0] = scale.mX;		mElements[1][0] = 0;			mElements[2][0] = 0;				mElements[3][0] = 0;
		mElements[0][1] = 0;			mElements[1][1] = scale.mY;		mElements[2][1] = 0;				mElements[3][1] = 0;
		mElements[0][2] = 0;			mElements[1][2] = 0;			mElements[2][2] = scale.mZ;			mElements[3][2] = 0;
		mElements[0][3] = 0;			mElements[1][3] = 0;			mElements[2][3] = 0;				mElements[3][3] = 1;
	}
	void Matrix4::MakeRotationX(float angleDegree)
	{
		float fs, fc;
		SinCosDeg(angleDegree, fs, fc);

		mColumn[0] = Vec4(1, 0, 0, 0);
		mColumn[1] = Vec4(0, fc, fs, 0);
		mColumn[2] = Vec4(0, -fs, fc, 0);
		mColumn[3] = Vec4(0, 0, 0, 1);
	}
	void Matrix4::MakeRotationY(float angleDegree)
	{
		float fs, fc;
		SinCosDeg(angleDegree, fs, fc);

		mColumn[0] = Vec4(fc, 0, -fs, 0);
		mColumn[1] = Vec4(0, 1, 0, 0);
		mColumn[2] = Vec4(fs, 0, fc, 0);
		mColumn[3] = Vec4(0, 0, 0, 1);
	}
	void Matrix4::MakeRotationZ(float angleDegree)
	{
		float fs, fc;
		SinCosDeg(angleDegree, fs, fc);

		mColumn[0] = Vec4(fc, fs, 0, 0);
		mColumn[1] = Vec4(-fs, fc, 0, 0);
		mColumn[2] = Vec4(0, 0, 1, 0);
		mColumn[3] = Vec4(0, 0, 0, 1);
	}
	void Matrix4::MakeRotationXYZ(const Vec3& angle)
	{
		float sinA, cosA, sinB, cosB, sinC, cosC;
		SinCosDeg(angle.mX, sinA, cosA);
		SinCosDeg(angle.mY, sinB, cosB);
		SinCosDeg(angle.mZ, sinC, cosC);

		mElements[0][0] = cosB * cosC;
		mElements[0][1] = sinA * sinB * cosC + cosA * sinC;
		mElements[0][2] = -cosA * sinB * cosC + sinA * sinC;
		mElements[0][3] = 0;

		mElements[1][0] = -cosB * sinC;
		mElements[1][1] = -sinA * sinB * sinC + cosA * cosC;
		mElements[1][2] = cosA * sinB * sinC + sinA * cosC;
		mElements[1][3] = 0;

		mElements[2][0] = sinB;
		mElements[2][1] = -sinA * cosB;
		mElements[2][2] = cosA * cosB;
		mElements[2][3] = 0;

		mElements[3][0] = 0;
		mElements[3][1] = 0;
		mElements[3][2] = 0;
		mElements[3][3] = 1;
	}
	void Matrix4::MakeRotationDir(const Vec3& directionZ)
	{
		Vec3 forward = directionZ;
		forward.Normalize();
		Vec3 up;
		if (Abs(forward.mX) < FLOAT_EPSILON && Abs(forward.mZ) < FLOAT_EPSILON)
		{
			if (forward.mY > 0) // forward vector is pointing +Y axis
				up = Vec3(0, 0, -1);
			else // forward vector is pointing -Y axis
				up = Vec3(0, 0, 1);
		}
		// in general, up vector is straight up
		else
		{
			up = Vec3(0, 1, 0);
		}


		Vec3 left = up ^ forward;
		left.Normalize();
		up = forward ^ left;
		up.Normalize();

		mColumn[0] = Vec4(left, 0);
		mColumn[1] = Vec4(up, 0);
		mColumn[2] = Vec4(forward, 0);
		mColumn[3] = Vec4(0, 0, 0, 1);
	}

	void Matrix4::MakeTranslationRotation(const Vec3& translation, const Vec3& rotation)
	{
		float sinA, cosA, sinB, cosB, sinC, cosC;
		SinCosDeg(rotation.mX, sinA, cosA);
		SinCosDeg(rotation.mY, sinB, cosB);
		SinCosDeg(rotation.mZ, sinC, cosC);

		mElements[0][0] = cosB * cosC;
		mElements[0][1] = sinA * sinB * cosC + cosA * sinC;
		mElements[0][2] = -cosA * sinB * cosC + sinA * sinC;
		mElements[0][3] = 0;

		mElements[1][0] = -cosB * sinC;
		mElements[1][1] = -sinA * sinB * sinC + cosA * cosC;
		mElements[1][2] = cosA * sinB * sinC + sinA * cosC;
		mElements[1][3] = 0;

		mElements[2][0] = sinB;
		mElements[2][1] = -sinA * cosB;
		mElements[2][2] = cosA * cosB;
		mElements[2][3] = 0;

		mElements[3][0] = translation.mX;
		mElements[3][1] = translation.mY;
		mElements[3][2] = translation.mZ;
		mElements[3][3] = 1;
	}

	void Matrix4::MakeTranslationRotationScale(const Vec3& tarnslation, const Vec3& rotation, Vec3& scale)
	{
		float sinA, cosA, sinB, cosB, sinC, cosC;
		SinCosDeg(rotation.mX, sinA, cosA);
		SinCosDeg(rotation.mY, sinB, cosB);
		SinCosDeg(rotation.mZ, sinC, cosC);

		mElements[0][0] = (cosB * cosC) * scale.mX;
		mElements[0][1] = (sinA * sinB * cosC + cosA * sinC) * scale.mX;
		mElements[0][2] = (-cosA * sinB * cosC + sinA * sinC) * scale.mX;
		mElements[0][3] = 0;

		mElements[1][0] = (-cosB * sinC) * scale.mY;
		mElements[1][1] = (-sinA * sinB * sinC + cosA * cosC) * scale.mY;
		mElements[1][2] = (cosA * sinB * sinC + sinA * cosC) * scale.mY;
		mElements[1][3] = 0;

		mElements[2][0] = (sinB)* scale.mZ;
		mElements[2][1] = (-sinA * cosB) * scale.mZ;
		mElements[2][2] = (cosA * cosB) * scale.mZ;
		mElements[2][3] = 0;

		mElements[3][0] = tarnslation.mX;
		mElements[3][1] = tarnslation.mY;
		mElements[3][2] = tarnslation.mZ;
		mElements[3][3] = 1;
	}

	void Matrix4::MakeFrustum(float fLeft, float fRight, float fBottom, float fTop, float fNear, float fFar)
	{
		float rl = (fRight - fLeft);
		float tb = (fTop - fBottom);
		float fn = (fFar - fNear);

		mElements[0][0] = (2 * fNear) / rl;
		mElements[0][1] = 0;
		mElements[0][2] = 0;
		mElements[0][3] = 0;

		mElements[1][0] = 0;
		mElements[1][1] = (2 * fNear) / tb;
		mElements[1][2] = 0;
		mElements[1][3] = 0;

		mElements[2][0] = (fRight + fLeft) / rl;
		mElements[2][1] = (fTop + fBottom) / tb;
		mElements[2][2] = -(fFar + fNear) / fn;
		mElements[2][3] = -1;

		mElements[3][0] = 0;
		mElements[3][1] = 0;
		mElements[3][2] = (-2 * fFar * fNear) / fn;
		mElements[3][3] = 0;
	}

	void Matrix4::MakePerspective(float fovDegree, float aspect, float zNear, float zFar)
	{
		float f = 1.0f / Tan(fovDegree * 0.5f * DEG2RAD);
		float nf = 1.0f / (zNear - zFar);

		mElements[0][0] = f / aspect;
		mElements[0][1] = 0;
		mElements[0][2] = 0;
		mElements[0][3] = 0;

		mElements[1][0] = 0;
		mElements[1][1] = f;
		mElements[1][2] = 0;
		mElements[1][3] = 0;

		mElements[2][0] = 0;
		mElements[2][1] = 0;
		mElements[2][2] = (zFar + zNear) * nf;
		mElements[2][3] = -1;

		mElements[3][0] = 0;
		mElements[3][1] = 0;
		mElements[3][2] = (2 * zFar * zNear) * nf;
		mElements[3][3] = 0;
	}

	void Matrix4::MakeOrtho(float width, float height, float zNear, float zFar)
	{
		MakeOrtho(width * -0.5f, width * 0.5f, height * -0.5f, height * 0.5f, zNear, zFar);
	}

	void Matrix4::MakeOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		float rl = (right - left);
		float tb = (top - bottom);
		float fn = (zFar - zNear);

		mElements[0][0] = 2 / rl;
		mElements[0][1] = 0;
		mElements[0][2] = 0;
		mElements[0][3] = 0;

		mElements[1][0] = 0;
		mElements[1][1] = 2 / tb;
		mElements[1][2] = 0;
		mElements[1][3] = 0;

		mElements[2][0] = 0;
		mElements[2][1] = 0;
		mElements[2][2] = -2 / fn;
		mElements[2][3] = 0;

		mElements[3][0] = -(right + left) / rl;
		mElements[3][1] = -(top + bottom) / tb;
		mElements[3][2] = -(zFar + zNear) / fn;
		mElements[3][3] = 1;
	}
	void Matrix4::GetTranspose(Matrix4& out) const
	{
		UASSERT(this != &out);
		out.mElements[0][0] = mElements[0][0];
		out.mElements[0][1] = mElements[1][0];
		out.mElements[0][2] = mElements[2][0];
		out.mElements[0][3] = mElements[3][0];

		out.mElements[1][0] = mElements[0][1];
		out.mElements[1][1] = mElements[1][1];
		out.mElements[1][2] = mElements[2][1];
		out.mElements[1][3] = mElements[3][1];

		out.mElements[2][0] = mElements[0][2];
		out.mElements[2][1] = mElements[1][2];
		out.mElements[2][2] = mElements[2][2];
		out.mElements[2][3] = mElements[3][2];

		out.mElements[3][0] = mElements[0][3];
		out.mElements[3][1] = mElements[1][3];
		out.mElements[3][2] = mElements[2][3];
		out.mElements[3][3] = mElements[3][3];
	}

	void Matrix4::Transpose()
	{
		Swap(mElements[0][1], mElements[1][0]);
		Swap(mElements[0][2], mElements[2][0]);
		Swap(mElements[0][3], mElements[3][0]);
		Swap(mElements[1][2], mElements[2][1]);
		Swap(mElements[1][3], mElements[3][1]);
		Swap(mElements[2][3], mElements[3][2]);
	}

	void Matrix4::Transpose3x3()
	{
		Swap(mElements[0][1], mElements[1][0]);
		Swap(mElements[0][2], mElements[2][0]);
		Swap(mElements[1][2], mElements[2][1]);
	}
	void Matrix4::TransformVec4(const Vec4& v, Vec4& out) const
	{
		UASSERT(&v != &out);

// 		out = GetColumn(0) * v.mX + GetColumn(1) * v.mY + GetColumn(2) * v.mZ + GetColumn(3) * v.mW;

		out.mX = (mElements[0][0] * v.mX) + (mElements[1][0] * v.mY) + (mElements[2][0] * v.mZ) + (mElements[3][0] * v.mW);
		out.mY = (mElements[0][1] * v.mX) + (mElements[1][1] * v.mY) + (mElements[2][1] * v.mZ) + (mElements[3][1] * v.mW);
		out.mZ = (mElements[0][2] * v.mX) + (mElements[1][2] * v.mY) + (mElements[2][2] * v.mZ) + (mElements[3][2] * v.mW);
		out.mW = (mElements[0][3] * v.mX) + (mElements[1][3] * v.mY) + (mElements[2][3] * v.mZ) + (mElements[3][3] * v.mW);
	}
	void Matrix4::TransformVec3W0(const Vec3& v, Vec3& out) const
	{
		UASSERT(&v != &out);

		out.mX = (mElements[0][0] * v.mX) + (mElements[1][0] * v.mY) + (mElements[2][0] * v.mZ);
		out.mY = (mElements[0][1] * v.mX) + (mElements[1][1] * v.mY) + (mElements[2][1] * v.mZ);
		out.mZ = (mElements[0][2] * v.mX) + (mElements[1][2] * v.mY) + (mElements[2][2] * v.mZ);
	}
	void Matrix4::TransformVec3W1(const Vec3& v, Vec3& out) const
	{
		UASSERT(&v != &out);

		out.mX = (mElements[0][0] * v.mX) + (mElements[1][0] * v.mY) + (mElements[2][0] * v.mZ) + (mElements[3][0]);
		out.mY = (mElements[0][1] * v.mX) + (mElements[1][1] * v.mY) + (mElements[2][1] * v.mZ) + (mElements[3][1]);
		out.mZ = (mElements[0][2] * v.mX) + (mElements[1][2] * v.mY) + (mElements[2][2] * v.mZ) + (mElements[3][2]);
	}

	void Matrix4::Mul(const Matrix4& a, const Matrix4& b, Matrix4& out)
	{
		out.mElements[0][0] = a.GetRow(0) | b.GetColumn(0);
		out.mElements[0][1] = a.GetRow(1) | b.GetColumn(0);
		out.mElements[0][2] = a.GetRow(2) | b.GetColumn(0);
		out.mElements[0][3] = a.GetRow(3) | b.GetColumn(0);

		out.mElements[1][0] = a.GetRow(0) | b.GetColumn(1);
		out.mElements[1][1] = a.GetRow(1) | b.GetColumn(1);
		out.mElements[1][2] = a.GetRow(2) | b.GetColumn(1);
		out.mElements[1][3] = a.GetRow(3) | b.GetColumn(1);

		out.mElements[2][0] = a.GetRow(0) | b.GetColumn(2);
		out.mElements[2][1] = a.GetRow(1) | b.GetColumn(2);
		out.mElements[2][2] = a.GetRow(2) | b.GetColumn(2);
		out.mElements[2][3] = a.GetRow(3) | b.GetColumn(2);

		out.mElements[3][0] = a.GetRow(0) | b.GetColumn(3);
		out.mElements[3][1] = a.GetRow(1) | b.GetColumn(3);
		out.mElements[3][2] = a.GetRow(2) | b.GetColumn(3);
		out.mElements[3][3] = a.GetRow(3) | b.GetColumn(3);
	}
	Vec3 Matrix4::GetScale() const
	{
		return Vec3(mColumn[0].Length3(), mColumn[1].Length3(), mColumn[2].Length3());
	}
	Vec3 Matrix4::GetRotationEuler() const
	{
		Vec3 out;
		out.mX = ::atan2(mElements[1][2], mElements[2][2]) * RAD2DEG;
		out.mY = ::atan2(-mElements[0][2], Sqrt(mElements[1][2] * mElements[1][2] + mElements[2][2] * mElements[2][2])) * RAD2DEG;
		out.mZ = ::atan2(mElements[0][1], mElements[0][0]) * RAD2DEG;
		return out;
	}
	void Matrix4::Invert3x3()
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
			/*SetIndentity();*/
			mElements[0][0] = 1;
			mElements[0][1] = 0;
			mElements[0][2] = 0;

			mElements[1][0] = 0;
			mElements[1][1] = 1;
			mElements[1][2] = 0;

			mElements[2][0] = 0;
			mElements[2][1] = 0;
			mElements[2][2] = 1;
		}

		// divide by the determinant
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

	void Matrix4::InvertAffine()
	{
#if 0
		// R^-1
		Matrix3 r(mElements[0][0], mElements[0][1], mElements[0][2],
			mElements[1][0], mElements[1][1], mElements[1][2],
			mElements[2][0], mElements[2][1], mElements[2][2]);

		r.Invert();

		mElements[0][0] = r.mElements[0][0] /*r[0]*/;  mElements[0][1] = r.mElements[0][1] /*r[1]*/;  mElements[0][2] = r.mElements[0][2]/*r[2]*/;
		mElements[1][0] = r.mElements[1][0] /*r[3]*/;  mElements[1][1] = r.mElements[1][1] /*r[4]*/;  mElements[1][2] = r.mElements[1][2]/*r[5]*/;
		mElements[2][0] = r.mElements[2][0] /*r[6]*/;  mElements[2][1] = r.mElements[2][1] /*r[7]*/;  mElements[2][2] = r.mElements[2][2]/*r[8]*/;
#else
		Invert3x3();
#endif
		// -R^-1 * T
		float x = mElements[3][0];
		float y = mElements[3][1];
		float z = mElements[3][2];

		//translation   =   r.Coulmn0 * x + r.Column1 * y  +  r.Column2 * z
		mElements[3][0] = -(mElements[0][0] * x + mElements[1][0] * y + mElements[2][0] * z);
		mElements[3][1] = -(mElements[0][1] * x + mElements[1][1] * y + mElements[2][1] * z);
		mElements[3][2] = -(mElements[0][2] * x + mElements[1][2] * y + mElements[2][2] * z);

		// last row should be unchanged (0,0,0,1)
		//mElements[0][3] = mElements[1][3] = mElements[2][3] = 0.0f; mElements[3][3] = 1.0f;
	}

	void Matrix4::InvertGeneral()
	{
		// get cofactors of minor matrices
		float cofactor0 = getCofactor(mElements[1][1], mElements[1][2], mElements[1][3], mElements[2][1], mElements[2][2], mElements[2][3], mElements[3][1], mElements[3][2], mElements[3][3]);
		float cofactor1 = getCofactor(mElements[1][0], mElements[1][2], mElements[1][3], mElements[2][0], mElements[2][2], mElements[2][3], mElements[3][0], mElements[3][2], mElements[3][3]);
		float cofactor2 = getCofactor(mElements[1][0], mElements[1][1], mElements[1][3], mElements[2][0], mElements[2][1], mElements[2][3], mElements[3][0], mElements[3][1], mElements[3][3]);
		float cofactor3 = getCofactor(mElements[1][0], mElements[1][1], mElements[1][2], mElements[2][0], mElements[2][1], mElements[2][2], mElements[3][0], mElements[3][1], mElements[3][2]);

		// get determinant
		float determinant = mElements[0][0] * cofactor0 - mElements[0][1] * cofactor1 + mElements[0][2] * cofactor2 - mElements[0][3] * cofactor3;
		if (Abs(determinant) <= FLOAT_EPSILON)
		{
			SetIndentity();
			return;
		}

		// get rest of cofactors for adj(M)
		float cofactor4 = getCofactor(mElements[0][1], mElements[0][2], mElements[0][3], mElements[2][1], mElements[2][2], mElements[2][3], mElements[3][1], mElements[3][2], mElements[3][3]);
		float cofactor5 = getCofactor(mElements[0][0], mElements[0][2], mElements[0][3], mElements[2][0], mElements[2][2], mElements[2][3], mElements[3][0], mElements[3][2], mElements[3][3]);
		float cofactor6 = getCofactor(mElements[0][0], mElements[0][1], mElements[0][3], mElements[2][0], mElements[2][1], mElements[2][3], mElements[3][0], mElements[3][1], mElements[3][3]);
		float cofactor7 = getCofactor(mElements[0][0], mElements[0][1], mElements[0][2], mElements[2][0], mElements[2][1], mElements[2][2], mElements[3][0], mElements[3][1], mElements[3][2]);

		float cofactor8 = getCofactor(mElements[0][1], mElements[0][2], mElements[0][3], mElements[1][1], mElements[1][2], mElements[1][3], mElements[3][1], mElements[3][2], mElements[3][3]);
		float cofactor9 = getCofactor(mElements[0][0], mElements[0][2], mElements[0][3], mElements[1][0], mElements[1][2], mElements[1][3], mElements[3][0], mElements[3][2], mElements[3][3]);
		float cofactor10 = getCofactor(mElements[0][0], mElements[0][1], mElements[0][3], mElements[1][0], mElements[1][1], mElements[1][3], mElements[3][0], mElements[3][1], mElements[3][3]);
		float cofactor11 = getCofactor(mElements[0][0], mElements[0][1], mElements[0][2], mElements[1][0], mElements[1][1], mElements[1][2], mElements[3][0], mElements[3][1], mElements[3][2]);

		float cofactor12 = getCofactor(mElements[0][1], mElements[0][2], mElements[0][3], mElements[1][1], mElements[1][2], mElements[1][3], mElements[2][1], mElements[2][2], mElements[2][3]);
		float cofactor13 = getCofactor(mElements[0][0], mElements[0][2], mElements[0][3], mElements[1][0], mElements[1][2], mElements[1][3], mElements[2][0], mElements[2][2], mElements[2][3]);
		float cofactor14 = getCofactor(mElements[0][0], mElements[0][1], mElements[0][3], mElements[1][0], mElements[1][1], mElements[1][3], mElements[2][0], mElements[2][1], mElements[2][3]);
		float cofactor15 = getCofactor(mElements[0][0], mElements[0][1], mElements[0][2], mElements[1][0], mElements[1][1], mElements[1][2], mElements[2][0], mElements[2][1], mElements[2][2]);

		// build inverse matrix = adj(M) / det(M)
		// adjugate of M is the transpose of the cofactor matrix of M
		float invDeterminant = 1.0f / determinant;
		mElements[0][0] = invDeterminant * cofactor0;
		mElements[0][1] = -invDeterminant * cofactor4;
		mElements[0][2] = invDeterminant * cofactor8;
		mElements[0][3] = -invDeterminant * cofactor12;

		mElements[1][0] = -invDeterminant * cofactor1;
		mElements[1][1] = invDeterminant * cofactor5;
		mElements[1][2] = -invDeterminant * cofactor9;
		mElements[1][3] = invDeterminant * cofactor13;

		mElements[2][0] = invDeterminant * cofactor2;
		mElements[2][1] = -invDeterminant * cofactor6;
		mElements[2][2] = invDeterminant * cofactor10;
		mElements[2][3] = -invDeterminant * cofactor14;

		mElements[3][0] = -invDeterminant * cofactor3;
		mElements[3][1] = invDeterminant * cofactor7;
		mElements[3][2] = -invDeterminant * cofactor11;
		mElements[3][3] = invDeterminant * cofactor15;
	}

	float Matrix4::Determinant()
	{
		return	mElements[0][0] * getCofactor(mElements[1][1], mElements[1][2], mElements[1][3], mElements[2][1], mElements[2][2], mElements[2][3], mElements[3][1], mElements[3][2], mElements[3][3]) -
			mElements[0][1] * getCofactor(mElements[1][0], mElements[1][2], mElements[1][3], mElements[2][0], mElements[2][2], mElements[2][3], mElements[3][0], mElements[3][2], mElements[3][3]) +
			mElements[0][2] * getCofactor(mElements[1][0], mElements[1][1], mElements[1][3], mElements[2][0], mElements[2][1], mElements[2][3], mElements[3][0], mElements[3][1], mElements[3][3]) -
			mElements[0][3] * getCofactor(mElements[1][0], mElements[1][1], mElements[1][2], mElements[2][0], mElements[2][1], mElements[2][2], mElements[3][0], mElements[3][1], mElements[3][2]);
	}

	void Matrix4::ToString(char* out) const
	{
		::sprintf_s(out, 128,
			"%12.6f  %12.6f  %12.6f  %12.6f\n"
			"%12.6f  %12.6f  %12.6f  %12.6f\n"
			"%12.6f  %12.6f  %12.6f  %12.6f\n"
			"%12.6f  %12.6f  %12.6f  %12.6f\n",
			mElements[0][0], mElements[1][0], mElements[2][0], mElements[3][0],
			mElements[0][1], mElements[1][1], mElements[2][1], mElements[3][1],
			mElements[0][2], mElements[1][2], mElements[2][2], mElements[3][2],
			mElements[0][3], mElements[1][3], mElements[2][3], mElements[3][3]);
	}

	bool Matrix4::IsEqual(const Matrix4& other, float epsilon /*= 0.00001f*/) const
	{
		for (size_t i = 0; i < 4; i++)
			for (size_t j = 0; j < 4; j++)
				if (Abs(mElements[i][j] - other.mElements[i][j]) > epsilon)
					return false;
		return true;
	}

	bool Matrix4::IsOrthogonal(float epsilon /*= 0.000001f*/)
	{
		//orthogonal matrix:
		//Each column of the matrix must be a unit vector.This fact implies that there can be no scale applied to the matrix.
		//The columns of the matrix must be mutually perpendicular.

// 		Matrix4 inv = *this;
// 		inv.InvertGeneral();
// 		Matrix4 trs = *this;
// 		trs.Transpose();
// 		return inv.IsEqual(trs, epsilon);
		return ((*this) * this->GetTranspose()).IsEqual(Matrix4::IDENTITY);
		
	}

	Matrix4& Matrix4::operator+=(const Matrix4& m)
	{
		for (size_t i = 0; i < 4; i++)
			for (size_t j = 0; j < 4; j++)
				mElements[i][j] += m.mElements[i][j];
		return *this;
	}





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

};