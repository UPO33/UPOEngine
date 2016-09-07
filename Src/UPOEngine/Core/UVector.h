#include "UBasic.h"

namespace UPO
{
	static const float VECTOR_ESPILON = FLOAT_EPSILON;

	//////////////////////////////////////////////////////////////////////////
	struct Vec2;
	struct Vec3;
	struct Vec4;


	//////////////////////////////////////////////////////////////////////////
	struct UAPI Vec2
	{
		float mX, mY;

		Vec2() {}
		Vec2(float xy) { mX = mY = xy; }
		Vec2(float x, float y) { mX = x; mY = y; }

		inline float& operator [] (size_t i) { return ((float*)this)[i]; }
		inline const float& operator [] (size_t i) const { return ((float*)this)[i]; }

		inline Vec2 operator + (const Vec2& v) const { return Vec2(mX + v.mX, mY + v.mY); }
		inline Vec2 operator - (const Vec2& v) const { return Vec2(mX - v.mX, mY - v.mY); }
		inline Vec2 operator * (const Vec2& v) const { return Vec2(mX * v.mX, mY * v.mY); }
		inline Vec2 operator / (const Vec2& v) const { return Vec2(mX / v.mX, mY / v.mY); }

		void StoreTo(float* out) const
		{
			out[0] = mX;
			out[1] = mY;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	struct UAPI Vec3
	{
		union
		{
			struct
			{
				float mX, mY, mZ;
			};
			struct
			{
				float mR, mG, mB;
			};

			float mXYZ[3];
			float mRGB[3];
		};


		Vec3() {}

		Vec3(float xyz) { mX = mY = mZ = xyz; }
		Vec3(float x, float y, float z) { mX = x; mY = y; mZ = z; }
		inline Vec3(const Vec4& v);

		Vec3& operator = (const Vec3& v)
		{
			mX = v.mX;		mY = v.mY;		mZ = v.mZ;
			return *this;
		}
		Vec3& operator = (float f)
		{
			mX = mY = mZ = f;
			return *this;
		}

		inline Vec3& operator = (const Vec4& v);

		Vec3& operator += (float f)
		{
			mX += f;	mY += f;	mZ += f;
			return *this;
		}
		Vec3& operator -= (float f)
		{
			mX -= f;	mY -= f;	mZ -= f;
			return *this;
		}
		Vec3& operator *= (float f)
		{
			mX *= f;	mY *= f;	mZ *= f;
			return *this;
		}
		Vec3& operator /= (float f)
		{
			mX /= f;	mY /= f;	mZ /= f;
			return *this;
		}


		Vec3& operator += (const Vec3& other)
		{
			mX += other.mX;		mY += other.mY;		mZ += other.mZ;
			return *this;
		}
		Vec3& operator -= (const Vec3& other)
		{
			mX -= other.mX;		mY -= other.mY;		mZ -= other.mZ;
			return *this;
		}
		Vec3& operator *= (const Vec3& other)
		{
			mX *= other.mX;		mY *= other.mY;		mZ *= other.mZ;
			return *this;
		}
		Vec3& operator /= (const Vec3& other)
		{
			mX /= other.mX;		mY /= other.mY;		mZ /= other.mZ;
			return *this;
		}


		const Vec3 operator + (float f) const
		{
			return Vec3(mX + f, mY + f, mZ + f);
		}
		const Vec3 operator - (float f) const
		{
			return Vec3(mX - f, mY - f, mZ - f);
		}
		const Vec3 operator * (float f) const
		{
			return Vec3(mX * f, mY * f, mZ * f);
		}
		const Vec3 operator / (float f) const
		{
			return Vec3(mX / f, mY / f, mZ / f);
		}



		const Vec3 operator + (const Vec3& v) const
		{
			return Vec3(mX + v.mX, mY + v.mY, mZ + v.mZ);
		}
		const Vec3 operator - (const Vec3& v) const
		{
			return Vec3(mX - v.mX, mY - v.mY, mZ - v.mZ);
		}
		const Vec3 operator * (const Vec3& v) const
		{
			return Vec3(mX * v.mX, mY * v.mY, mZ * v.mZ);
		}
		const Vec3 operator / (const Vec3& v) const
		{
			return Vec3(mX / v.mX, mY / v.mY, mZ / v.mZ);
		}

		//negate
		Vec3 operator - () const { return Vec3(-mX, -mY, -mZ); }

		//dot product
		float operator | (const Vec3& v) const
		{
			return mX * v.mX + mY * v.mY + mZ + v.mZ;
		}
		//cross product
		//(swapping the position of any two arguments negates the result): If AxB == C then BxA == -C. 
		const Vec3 operator ^ (const Vec3& v) const
		{
			return Vec3(mY * v.mZ - mZ * v.mY,
				mZ * v.mX - mX * v.mZ,
				mX * v.mY - mY * v.mX);

		}
		//get normalize
		const Vec3 operator ~ () const
		{
			return GetNormalizeSafe();
		}
		inline float& operator [] (size_t i) { return ((float*)this)[i]; }
		inline const float operator [] (size_t i) const { return ((float*)this)[i]; }

		void StoreTo(float* out) const
		{
			out[0] = mX;
			out[1] = mY;
			out[2] = mZ;
		}

		///////////////////////comparisons
		bool operator > (float f) const
		{
			return	mX > f && mY > f && mZ > f;
		}
		bool operator >= (float f) const
		{
			return	mX >= f && mY >= f && mZ >= f;
		}
		bool operator > (const Vec3& v) const
		{
			return	mX > v.mX && mY > v.mY && mZ > v.mZ;
		}
		bool operator >= (const Vec3& v) const
		{
			return	mX >= v.mX && mY >= v.mY && mZ >= v.mZ;
		}
		bool operator < (float f) const
		{
			return	mX < f && mY < f && mZ < f;
		}
		bool operator < (const Vec3& v) const
		{
			return	mX < v.mX && mY < v.mY && mZ < v.mZ;
		}
		bool operator <= (float f) const
		{
			return	mX <= f && mY <= f && mZ <= f;
		}
		bool operator <= (const Vec3& v) const
		{
			return	mX <= v.mX && mY <= v.mY && mZ <= v.mZ;
		}

		bool operator == (float f) const
		{
			return Abs(mX - f) < VECTOR_ESPILON && Abs(mY - f) < VECTOR_ESPILON && Abs(mZ - f) < VECTOR_ESPILON;
		}
		bool operator == (const  Vec3& v) const
		{
			return Abs(mX - v.mX) < VECTOR_ESPILON && Abs(mY - v.mY) < VECTOR_ESPILON && Abs(mZ - v.mZ) < VECTOR_ESPILON;
		}
		bool operator != (float f) const
		{
			return Abs(mX - f) > VECTOR_ESPILON || Abs(mY - f) > VECTOR_ESPILON || Abs(mZ - f) > VECTOR_ESPILON;
		}
		bool operator != (const Vec3& v) const
		{
			return Abs(mX - v.mX) > VECTOR_ESPILON || Abs(mY - v.mY) > VECTOR_ESPILON || Abs(mZ - v.mZ) > VECTOR_ESPILON;
		}


		friend Vec3 operator * (float f, const Vec3& v)
		{
			return v * f;
		}

		float LengthSq() const { return (mX*mX + mY*mY + mZ*mZ); }
		float Length() const { return Sqrt(mX*mX + mY*mY + mZ*mZ); }

		void Normalize()
		{
			float s = RSqrt(mX*mX + mY*mY + mZ*mZ);
			mX *= s;	mY *= s;	mZ *= s;
		}
		Vec3 GetNormalize() const
		{
			Vec3 ret = *this;
			ret.Normalize();
			return ret;
		}
		void NormalizeSafe(const Vec3& errorValue = Vec3(0.0f))
		{
			float s = (mX*mX + mY*mY + mZ*mZ);
			if (s < VECTOR_ESPILON)
			{
				mX = errorValue.mX;		mY = errorValue.mY;		mZ = errorValue.mZ;
			}
			else
			{
				s = RSqrt(s);
				mX *= s;	mY *= s;	mZ *= s;
			}

		}
		Vec3 GetNormalizeSafe() const
		{
			Vec3 ret = *this;
			ret.GetNormalize();
			return ret;
		}
	};


	inline float Dot(const Vec3& a, const Vec3& b)
	{
		return a.mX * b.mX + a.mY * b.mY + a.mZ + b.mZ;
	}
	inline Vec3 Abs(const Vec3& v)
	{
		return Vec3(Abs(v.mX), Abs(v.mY), Abs(v.mZ));
	}
	inline Vec3 Min(const Vec3& v0, const Vec3& v1)
	{
		return Vec3(Min(v0.mX, v1.mX), Min(v0.mY, v1.mY), Min(v0.mZ, v1.mZ));
	}
	inline Vec3 Max(const Vec3& v0, const Vec3& v1)
	{
		return Vec3(Max(v0.mX, v1.mX), Max(v0.mY, v1.mY), Max(v0.mZ, v1.mZ));
	}
	inline Vec3 Lerp(const Vec3& v0, const Vec3& v1, float t)
	{
		return v0 * (1 - t) + v1 * t;
	}

	//////////////////////////////////////////////////////////////////////////
	struct UAPI Vec4
	{
		union
		{
			struct
			{
				float mX, mY, mZ, mW;
			};
			struct
			{
				float mR, mG, mB, mA;
			};

			float mXYZW[4];
			float mRGBA[4];
		};


		Vec4() {}

		Vec4(float xyzw)
		{
			mX = mY = mZ = mW = xyzw;
		}
		Vec4(float x, float y, float z, float w)
		{
			mX = x;		mY = y;		mZ = z;		mW = w;
		}
		Vec4(const Vec2& xy, const Vec2& zw)
		{
			mX = xy.mX;		mY = xy.mY;
			mZ = zw.mX;		mW = zw.mY;
		}
		Vec4(const Vec3& v3, float w = 1)
		{
			mX = v3.mX;		mY = v3.mY;		mZ = v3.mZ;		mW = w;
		}


		Vec4& operator += (float f)
		{
			mX += f;		mY += f;		mZ += f;		mW += f;
			return *this;
		}
		Vec4& operator -= (float f)
		{
			mX -= f;		mY -= f;		mZ -= f;		mW -= f;
			return *this;
		}
		Vec4& operator *= (float f)
		{
			mX *= f;		mY *= f;		mZ *= f;		mW *= f;
			return *this;
		}
		Vec4& operator /= (float f)
		{
			mX /= f;		mY /= f;		mZ /= f;		mW /= f;
			return *this;
		}


		Vec4& operator += (const Vec4& v)
		{
			mX += v.mX;		mY += v.mY;		mZ += v.mZ;		mW += v.mW;
			return *this;
		}
		Vec4& operator -= (const Vec4& v)
		{
			mX -= v.mX;		mY -= v.mY;		mZ -= v.mZ;		mW -= v.mW;
			return *this;
		}
		Vec4& operator *= (const Vec4& v)
		{
			mX *= v.mX;		mY *= v.mY;		mZ *= v.mZ;		mW *= v.mW;
			return *this;
		}
		Vec4& operator /= (const Vec4& v)
		{
			mX /= v.mX;		mY /= v.mY;		mZ /= v.mZ;		mW /= v.mW;
			return *this;
		}


		const Vec4 operator + (float f) const
		{
			return Vec4(mX + f, mY + f, mZ + f, mW + f);
		}
		const Vec4 operator - (float f) const
		{
			return Vec4(mX - f, mY - f, mZ - f, mW - f);
		}
		const Vec4 operator * (float f) const
		{
			return Vec4(mX * f, mY * f, mZ * f, mW * f);
		}
		const Vec4 operator / (float f) const
		{
			return Vec4(mX / f, mY / f, mZ / f, mW / f);
		}


		const Vec4 operator + (const Vec4& v) const
		{
			return Vec4(mX + v.mX, mY + v.mY, mZ + v.mZ, mW + v.mW);
		}
		const Vec4 operator - (const Vec4& v) const
		{
			return Vec4(mX - v.mX, mY - v.mY, mZ - v.mZ, mW - v.mW);
		}
		const Vec4 operator * (const Vec4& v) const
		{
			return Vec4(mX * v.mX, mY * v.mY, mZ * v.mZ, mW * v.mW);
		}
		const Vec4 operator / (const Vec4& v) const
		{
			return Vec4(mX / v.mX, mY / v.mY, mZ / v.mZ, mW / v.mW);
		}

		inline float& operator [] (size_t i) { return ((float*)this)[i]; }
		inline const float operator [] (size_t i) const { return ((float*)this)[i]; }

		void StoreTo(float* out) const
		{
			out[0] = mX;
			out[1] = mY;
			out[2] = mZ;
			out[3] = mW;
		}
		//dot product
		float operator | (const Vec4& v) const
		{
			return mX*v.mX + mY*v.mY + mZ*v.mZ + mW*v.mW;
		}
		Vec2 GetXY() const { return Vec2(mX, mY); }
		Vec2 GetZW() const { return Vec2(mZ, mW); }

		float LengthSq() const
		{
			return (mX * mX + mY * mY + mZ * mZ + mW * mW);
		}
		float Length() const
		{
			return Sqrt(Length());
		}
		float Length3Sq() const
		{
			return (mX * mX + mY * mY + mZ * mZ);
		}
		float Length3() const
		{
			return Sqrt(Length3Sq());
		}
	};

	inline Vec4 Lerp(const Vec4& v0, const Vec4& v1, float t)
	{
		return v0 * (1 - t) + v1 * t;
	}


	inline Vec3::Vec3(const Vec4& v)
	{
		mX = v.mX;		mY = v.mY;		mZ = v.mZ;
	}

	inline Vec3& Vec3::operator=(const Vec4& v)
	{
		mX = v.mX;		mY = v.mY;		mZ = v.mZ;
		return *this;
	}

	struct Color : public Vec4
	{
		Color(){}
		Color(float rgba) { mX = mY = mZ = mW = rgba; }
		Color(float r, float g, float b, float a) { mR = r;  mG = g;  mB = b;  mA = a; }
	};
};