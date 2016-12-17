#pragma once

#include "UVector.h"





inline float getCofactor(float m0, float m1, float m2,
	float m3, float m4, float m5,
	float m6, float m7, float m8)
{
	return m0 * (m4 * m8 - m5 * m7) -
		m1 * (m3 * m8 - m5 * m6) +
		m2 * (m3 * m7 - m4 * m6);
}

namespace UPO
{
	struct alignas(16) Quat
	{
		float mX, mY, mZ, mW;

	};

// 	struct Transform
// 	{
// 		Quat	mRotation;
// 		Vec3	mPosition;
// 		Vec3	mScale;
// 
// 	};

	//////////////////////////////////////////////////////////////////////////3x3 column major matrix
	struct UAPI Matrix3
	{
		static const Matrix3 IDENTITY;
		union
		{
			Vec3 mColumn[3];
			float mElements[3][3];	//[column][row]
		};

		Matrix3() {}

		Matrix3(float f0, float f1, float f2,	//column0
				float f3, float f4, float f5,	//column1
				float f6, float f7, float f8);	//column2
		Matrix3(float all);
		Matrix3(const Vec3& c0, const Vec3& c1, const Vec3& c2);
		Matrix3(const Matrix3& copy);
		Matrix3& operator = (const Matrix3& copy);

		void SetIndentity() { *this = IDENTITY; }

		void TransformVector(const Vec3& v, Vec3& out) const;
		Vec3 TransformVector(const Vec3& v) const
		{
			Vec3 ret;
			TransformVector(v, ret);
			return ret;
		}
		void GetTranspose(Matrix3& out) const;
		void Transpose();

		const Vec3& GetColumn(unsigned index) const;
		Vec3 GetRow(unsigned index) const;
		// If the determinant is not 0, then the matrix is invertible
		float Determinant() const;
		void Invert();

		Matrix3 operator * (const Matrix3& m) const
		{
			Matrix3 ret;
			Mul(*this, m, ret);
			return ret;
		}
		Matrix3& operator *= (const Matrix3& m)
		{
			Matrix3 tmp;
			Mul(*this, m, tmp);
			*this = tmp;
			return *this;
		}
		static void Mul(const Matrix3& a, const Matrix3& b, Matrix3& out);
	};


	//////////////////////////////////////////////////////////////////////////
	//4x4 column major Matrix
	struct UAPI Matrix4
	{
		static const Matrix4 IDENTITY;

		union
		{
			Vec4	mColumn[4];
			float	mElements[4][4];	//[column][row]
		};

		Matrix4(){}

		Matrix4(float f0, float f1, float f2, float f3,			//column0
				float f4, float f5, float f6, float f7,			//column1
				float f8, float f9, float f10, float f11,		//column2
				float f12, float f13, float f14, float f15);	//column3

		Matrix4(const Vec4& c0, const Vec4& c1, const Vec4& c2, const Vec4& c3);
		Matrix4(const Matrix4& m);
		Matrix4& operator = (const Matrix4& m);

		void SetIndentity() { *this = IDENTITY; }
		void MakeTranslation(float x, float y, float z) { MakeTranslation(Vec3(x, y, z)); }
		void MakeTranslation(const Vec3& position);
		void MakeScale(float scale) { MakeScale(Vec3(scale)); }
		void MakeScale(float x, float y, float z) { MakeScale(Vec3(x, y, z)); }
		void MakeScale(const Vec3& scale);
		void MakeRotationX(float angleDegree);
		void MakeRotationY(float angleDegree);
		void MakeRotationZ(float angleDegree);
		void MakeRotationXYZ(float x, float y, float z) { MakeRotationXYZ(Vec3(x, y, z)); }
		void MakeRotationXYZ(const Vec3& angle);
		void MakeRotationDir(const Vec3& directionZ);
		void MakeTranslationRotation(const Vec3& translation, const Vec3& rotation);
		void MakeTranslationRotationScale(const Vec3& tarnslation, const Vec3& rotation, Vec3& scale);
		void MakeFrustum(float fLeft, float fRight, float fBottom, float fTop, float fNear, float fFar);
		void MakePerspective(float fovDegree, float aspect, float zNear, float zFar);
		void MakeOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
		void MakeOrtho(float width, float height, float zNear, float zFar);
		const Vec4& GetColumn(unsigned index) const
		{
			UASSERT(index < 4);
			return mColumn[index];
		}
		Vec4 GetRow(unsigned index) const
		{
			UASSERT(index < 4);
			return Vec4(mElements[0][index], mElements[1][index], mElements[2][index], mElements[3][index]);
		}
		void GetTranspose(Matrix4& out) const;
		Matrix4 GetTranspose() const
		{
			Matrix4 ret;	
			GetTranspose(ret);
			return ret;
		}
		void Transpose();
		//transpose rotation part
		void Transpose3x3();
		void TransformVec4(const Vec4& v, Vec4& out) const;
		Vec4 TransformVec4(const Vec4& v) const
		{
			Vec4 ret;
			TransformVec4(v, ret);
			return ret;
		}
		void TransformVec3W0(const Vec3& v, Vec3& out) const;
		Vec3 TransformVec3W0(const Vec3& v) const
		{
			Vec3 ret;
			TransformVec3W0(v, ret);
			return ret;
		}
		void TransformVec3W1(const Vec3& v, Vec3& out) const;
		Vec3 TransformVec3W1(const Vec3& v) const
		{
			Vec3 ret;
			TransformVec3W1(v, ret);
			return ret;
		}
		static void Mul(const Matrix4& a, const Matrix4& b, Matrix4& out);
		Vec3 GetTranslation() const { return Vec3(mColumn[3]); }
		Vec3 GetScale() const;
		Vec3 GetRotationEuler() const;
		void Invert3x3();
		void InvertAffine();
		void InvertGeneral();
		float Determinant();
		Matrix4 operator * (const Matrix4& m) const
		{
			Matrix4 ret;
			Mul(*this, m, ret);
			return ret;
		}
		Matrix4& operator *= (const Matrix4& m)
		{
			Matrix4 tmp;
			Mul(*this, m, tmp);
			*this = tmp;
			return *this;
		}
		void ToString(char* out) const;
		bool IsEqual(const Matrix4& other, float epsilon = 0.00001f) const;
		//transpose of an orthogonal matrix is equal to its inverse
		//Translation, rotation, and reflection are the only orthogonal transformations
		bool IsOrthogonal(float epsilon = 0.000001f);
		Matrix4& operator += (const Matrix4& m);
	};

	typedef Matrix4 Transform;
};