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
	//////////////////////////////////////////////////////////////////////////
	struct UAPI alignas(16) Quat
	{
		float mX, mY, mZ, mW;

		static const Quat IDENTITY;

		Quat(){}
		Quat(float x, float y, float z, float w) { mX = x; mY = y; mZ = z; mW = w; }
		
		Quat operator * (const Quat&) const;
		Quat& operator *= (const Quat&);

		
		void Conjugate();
		Quat GetConjugate() const;
		void Invert();
		Quat GetInverse() const;

		float MagnitudeSq() { return ((Vec4*)this)->LengthSq(); }
		float Magnitude() { return ((Vec4*)this)->Length(); }
		
		void Normalize() { return ((Vec4*)this)->Normalize(); }
		Vec3 Rotate(const Vec3& v) const;
		Vec3 RotateInv(const Vec3& v) const;
		static Quat MakeEuler(const Vec3& angles);

		Vec3 operator * (const Vec3& v) const { return Rotate(v); }
		explicit operator float*() const { return (float*)this; }
	};

	inline float Dot(const Quat& q0, const Quat& q1)
	{
		return Dot((const Vec4&)q0, (const Vec4&)q1);
	}


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
// 	struct UAPI Matrix4
// 	{
// 		UCLASS(Matrix4, void)
// 
// 		static const Matrix4 IDENTITY;
// 
// 		union
// 		{
// 			Vec4	mColumn[4];
// 			float	mElements[4][4];	//[column][row]
// 		};
// 
// 		Matrix4(){}
// 
// 		Matrix4(float f0, float f1, float f2, float f3,			//column0
// 				float f4, float f5, float f6, float f7,			//column1
// 				float f8, float f9, float f10, float f11,		//column2
// 				float f12, float f13, float f14, float f15);	//column3
// 
// 		Matrix4(const Vec4& c0, const Vec4& c1, const Vec4& c2, const Vec4& c3);
// 		Matrix4(const Matrix4& m);
// 		Matrix4& operator = (const Matrix4& m);
// 
// 		void SetIndentity() { *this = IDENTITY; }
// 		void MakeTranslation(float x, float y, float z) { MakeTranslation(Vec3(x, y, z)); }
// 		void MakeTranslation(const Vec3& position);
// 		void MakeScale(float scale) { MakeScale(Vec3(scale)); }
// 		void MakeScale(float x, float y, float z) { MakeScale(Vec3(x, y, z)); }
// 		void MakeScale(const Vec3& scale);
// 		void MakeRotationX(float angleDegree);
// 		void MakeRotationY(float angleDegree);
// 		void MakeRotationZ(float angleDegree);
// 		void MakeRotationXYZ(float x, float y, float z) { MakeRotationXYZ(Vec3(x, y, z)); }
// 		void MakeRotationXYZ(const Vec3& angle);
// 		void MakeRotationDir(const Vec3& directionZ);
// 		void MakeTranslationRotation(const Vec3& translation, const Vec3& rotation);
// 		void MakeTranslationRotationScale(const Vec3& tarnslation, const Vec3& rotation, Vec3& scale);
// 		void MakeFrustum(float fLeft, float fRight, float fBottom, float fTop, float fNear, float fFar);
// 		void MakePerspectiveRH(float fovDegree, float aspect, float zNear, float zFar);
// 		void MakePerspectiveLH(float fovDegree, float aspect, float zNear, float zFar);
// 		void MakeOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
// 		void MakeOrtho(float width, float height, float zNear, float zFar);
// 		const Vec4& GetColumn(unsigned index) const
// 		{
// 			UASSERT(index < 4);
// 			return mColumn[index];
// 		}
// 		Vec4 GetRow(unsigned index) const
// 		{
// 			UASSERT(index < 4);
// 			return Vec4(mElements[0][index], mElements[1][index], mElements[2][index], mElements[3][index]);
// 		}
// 		void GetTranspose(Matrix4& out) const;
// 		Matrix4 GetTranspose() const
// 		{
// 			Matrix4 ret;	
// 			GetTranspose(ret);
// 			return ret;
// 		}
// 		void Transpose();
// 		//transpose rotation part
// 		void Transpose3x3();
// 		void TransformVec4(const Vec4& v, Vec4& out) const;
// 		Vec4 TransformVec4(const Vec4& v) const
// 		{
// 			Vec4 ret;
// 			TransformVec4(v, ret);
// 			return ret;
// 		}
// 		void TransformVec3W0(const Vec3& v, Vec3& out) const;
// 		Vec3 TransformVec3W0(const Vec3& v) const
// 		{
// 			Vec3 ret;
// 			TransformVec3W0(v, ret);
// 			return ret;
// 		}
// 		void TransformVec3W1(const Vec3& v, Vec3& out) const;
// 		Vec3 TransformVec3W1(const Vec3& v) const
// 		{
// 			Vec3 ret;
// 			TransformVec3W1(v, ret);
// 			return ret;
// 		}
// 		static void Mul(const Matrix4& a, const Matrix4& b, Matrix4& out);
// 		Vec3 GetTranslation() const { return Vec3(mColumn[3]); }
// 		Vec3 GetScale() const;
// 
// 		void RemoveScaling();
// 
// 		Vec3 GetRotationEuler() const;
// 		void Invert3x3();
// 		void InvertAffine();
// 		void InvertGeneral();
// 		float Determinant();
// 		Matrix4 operator * (const Matrix4& m) const
// 		{
// 			Matrix4 ret;
// 			Mul(*this, m, ret);
// 			return ret;
// 		}
// 		Matrix4& operator *= (const Matrix4& m)
// 		{
// 			Matrix4 tmp;
// 			Mul(*this, m, tmp);
// 			*this = tmp;
// 			return *this;
// 		}
// 		void ToString(char* out) const;
// 		bool IsEqual(const Matrix4& other, float epsilon = 0.00001f) const;
// 		//transpose of an orthogonal matrix is equal to its inverse
// 		//Translation, rotation, and reflection are the only orthogonal transformations
// 		bool IsOrthogonal(float epsilon = 0.000001f);
// 		Matrix4& operator += (const Matrix4& m);
// 
// 		void MetaSerialize(Stream&);
// 
// 
// 			Vec3 GetRotationEuler0() const;
// 			Vec3 GetRotationEuler1() const;
// 	};

	struct UAPI alignas(16) Matrix4
	{
		UCLASS(Matrix4, void)

		static const Matrix4 IDENTITY;

		union 
		{
			Vec4	mRows[4];
			float	mElements[16];
		};
		

		Matrix4() {}

		Matrix4(const Vec4& r0, const Vec4& r1, const Vec4& r2, const Vec4& r3);
		Matrix4(const Matrix4& m);
		Matrix4& operator = (const Matrix4& m);

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
		void MakeRotationDir(const Vec3& forward);
		void MakeRotationQuat(const Quat&);
		void MakePerspective(float fovDegree, float aspect, float zNear, float zFar);
		void MakeOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
		void MakeOrtho(float width, float height, float zNear, float zFar);
		void MakeViewLookAt(const Vec3& eyePos, const Vec3& focusPos, const Vec3& up);
		//scale * rotation * translation
		void MakeTransform(const Vec3& translation, const Vec3& rotation, const Vec3& scale);
		void MakeTransform(const Vec3& translation, const Quat& rotation, const Vec3& scale);

		static Matrix4 SRotationDir(const Vec3& forward)
		{
			Matrix4 ret;	 ret.MakeRotationDir(forward);		return ret;
		}

		void GetTranspose(Matrix4& out) const;
		Matrix4 GetTranspose() const
		{
			Matrix4 ret;
			GetTranspose(ret);
			return ret;
		}
		void Transpose();

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
		//transpose of the product of a and b
		static void MulTranspose(const Matrix4& a, const Matrix4& b, Matrix4& out);

		Vec3 GetTranslation() const;
		Vec3 GetScale() const;

		Vec3 GetRight() const { return mRows[0]; }
		Vec3 GetUp() const { return mRows[1]; }
		Vec3 GetForward() const { return mRows[2]; }

		Vec4& GetRow(unsigned index) { return mRows[index]; }
		const Vec4& GetRow(unsigned index) const { return mRows[index]; }

		void RemoveScaling();

		Vec3 GetRotationEuler() const;
		//This function only uses the 3x3 portion of this matrix. if this matrix contains scales, shears,
		//or other non-rotation transformations in the 3x3 matrix, then the result is undefined.
		void GetRotationQuat(Quat& out) const;
		Quat GetRotationQuat() const
		{
			Quat ret;
			GetRotationQuat(ret);
			return ret;
		}

		float Determinant();
		void Invert();
		Matrix4 GetInverse() const;

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

		bool IsEqual(const Matrix4& other, float epsilon = 0.00001f) const;
		//transpose of an orthogonal matrix is equal to its inverse
		//Translation, rotation, and reflection are the only orthogonal transformations
		bool IsOrthogonal(float epsilon = 0.000001f);

		void MetaSerialize(Stream&);


	};

	typedef Matrix4 Transform;

	UAPI StringStreamOut& operator << (StringStreamOut&, const Quat&);
// 	UAPI StringStreamOut& operator << (StringStreamOut&, const Matrix4&);
// 	UAPI StringStreamOut& operator << (StringStreamOut&, const Matrix3&);

};