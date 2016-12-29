#pragma once
#include "UBasic.h"

namespace UPO
{
	static const float VECTOR_ESPILON = FLOAT_EPSILON;
	static const float VECTOR_COMPARISON_DELTA = 0.000001f;

	//////////////////////////////////////////////////////////////////////////
	struct Vec2;
	struct Vec3;
	struct Vec4;
	struct Color;
	struct Color32;


	//////////////////////////////////////////////////////////////////////////
	struct UAPI Vec2
	{
		UCLASS(Vec2, void)

		float mX, mY;

		inline Vec2() {}
		inline Vec2(float xy) { mX = mY = xy; }
		inline Vec2(float x, float y) { mX = x; mY = y; }
		inline Vec2(const Vec2& xy) { mX = xy.mX;	mY = xy.mY; }
		inline Vec2(const Vec3&);
		
		inline float& operator [] (size_t i) { return ((float*)this)[i]; }
		inline const float& operator [] (size_t i) const { return ((float*)this)[i]; }

		inline Vec2& operator = (const Vec2& xy) { mX = xy.mX;	mY = xy.mY;	return *this; }
		inline Vec2& operator = (float xy) { mX = mY = xy;  return *this; }
		inline Vec2& operator = (const Vec3&);

		inline Vec2 operator + (const Vec2& v) const { return Vec2(mX + v.mX, mY + v.mY); }
		inline Vec2 operator - (const Vec2& v) const { return Vec2(mX - v.mX, mY - v.mY); }
		inline Vec2 operator * (const Vec2& v) const { return Vec2(mX * v.mX, mY * v.mY); }
		inline Vec2 operator / (const Vec2& v) const { return Vec2(mX / v.mX, mY / v.mY); }

		inline Vec2 operator + (float s) const { return Vec2(mX + s, mY + s); }
		inline Vec2 operator - (float s) const { return Vec2(mX - s, mY - s); }
		inline Vec2 operator * (float s) const { return Vec2(mX * s, mY * s); }
		inline Vec2 operator / (float s) const { return Vec2(mX / s, mY / s); }


		inline Vec2& operator += (const Vec2& v) { mX += v.mX;	mY += v.mY;		return *this; }
		inline Vec2& operator -= (const Vec2& v) { mX -= v.mX;	mY -= v.mY;		return *this; }
		inline Vec2& operator *= (const Vec2& v) { mX *= v.mX;	mY *= v.mY;		return *this; }
		inline Vec2& operator /= (const Vec2& v) { mX /= v.mX;	mY /= v.mY;		return *this; }

		inline Vec2& operator += (float s) { mX += s;	mY += s;	return *this; }
		inline Vec2& operator -= (float s) { mX -= s;	mY -= s;	return *this; }
		inline Vec2& operator *= (float s) { mX *= s;	mY *= s;	return *this; }
		inline Vec2& operator /= (float s) { mX /= s;	mY /= s;	return *this; }

		friend Vec2 operator * (float f, const Vec2& v)
		{
			return v * f;
		}
		

		bool operator > (float s) const { return mX > s && mY > s; }
		bool operator >= (float s) const { return mX >= s && mY >= s; }
		bool operator < (float s) const { return mX < s && mY < s; }
		bool operator <= (float s) const { return mX <= s && mY <= s; }

		bool operator > (const Vec2& v) const { return mX > v.mX && mY > v.mY; }
		bool operator >= (const Vec2& v) const { return mX >= v.mX && mY >= v.mY; }
		bool operator < (const Vec2& v) const { return mX < v.mX && mY < v.mY; }
		bool operator <= (const Vec2& v) const { return mX <= v.mX && mY <= v.mY; }


		bool IsNearlyEqual(const Vec2& v, float delta = VECTOR_COMPARISON_DELTA) const
		{
			return Abs(mX - v.mX) < delta && Abs(mY - v.mY) < delta;
		}

		
		//negate
		Vec2 operator - () const { return Vec2(-mX, -mY); }
		//dot product
		float operator | (const Vec2& v) const { return mX * v.mX + mY * v.mY; }

		float LengthSq() const { return mX * mX + mY * mY; }
		float Length() const { return Sqrt(mX * mX + mY *mY); }

		void Normalize()
		{
			*this *= RSqrt(LengthSq());
		}
		void NormalizeSafe(const Vec2& errorValue = Vec2(0))
		{
			float len = LengthSq();
			if (len <= VECTOR_ESPILON)
			{
				*this = errorValue;
			}
			else
			{
				*this *= RSqrt(len);
			}
		}
		Vec2 GetNormalized() const
		{
			Vec2 ret = *this;
			ret.Normalize();
			return ret;
		}
		Vec2 GetNormalizedSafe(const Vec2& errorValue = Vec2(0)) const
		{
			Vec2 ret = *this;
			ret.NormalizeSafe(errorValue);
			return ret;
		}
		void StoreTo(float* out) const
		{
			out[0] = mX;
			out[1] = mY;
		}
		//get normalized
		Vec2 operator ~ () const { return GetNormalizedSafe(); }

		void MetaSerialize(Stream&);
	};

	inline float Dot(const Vec2& a, const Vec2& b)
	{
		return a | b;
	}
	inline Vec2 Abs(const Vec2& v)
	{
		return Vec2(Abs(v.mX), Abs(v.mY));
	}
	inline Vec2 Min(const Vec2& v0, const Vec2& v1)
	{
		return Vec2(Min(v0.mX, v1.mX), Min(v0.mY, v1.mY));
	}
	inline Vec2 Max(const Vec2& v0, const Vec2& v1)
	{
		return Vec2(Max(v0.mX, v1.mX), Max(v0.mY, v1.mY));
	}
	inline Vec2 Lerp(const Vec2& v0, const Vec2& v1, float t)
	{
		return v0 * (1 - t) + v1 * t;
	}
	inline Vec2 Clamp(const Vec2& value, float min, float max)
	{
		return Vec2(Clamp(value.mX, min, max), Clamp(value.mY, min, max));
	}
	inline Vec2 Clamp(const Vec2& value, const Vec2& min, const Vec2& max)
	{
		return Vec2(Clamp(value.mX, min.mX, max.mX), Clamp(value.mY, min.mY, max.mY));
	}




	//////////////////////////////////////////////////////////////////////////
	struct UAPI Vec3
	{
		UCLASS(Vec3, void)

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


		inline Vec3() {}
		inline Vec3(float xyz) { mX = mY = mZ = xyz; }
		inline Vec3(float x, float y, float z) { mX = x; mY = y; mZ = z; }
		inline Vec3(const Vec3& v3) { mX = v3.mX;		mY = v3.mY;		mZ = v3.mZ; }
		inline Vec3(const Vec2& v2, float z) { mX = v2.mX;		mY = v2.mY;		mZ = z; }
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
			return mX * v.mX + mY * v.mY + mZ * v.mZ;
		}
		//cross product
		//(swapping the position of any two arguments negates the result): If AxB == C then BxA == -C. 
		const Vec3 operator ^ (const Vec3& v) const
		{
			return Vec3(mY * v.mZ - mZ * v.mY,
				mZ * v.mX - mX * v.mZ,
				mX * v.mY - mY * v.mX);

		}
		//get normalized
		const Vec3 operator ~ () const
		{
			return GetNormalizedSafe();
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
		bool IsNearlyEqual(const Vec3& v, float delta = VECTOR_COMPARISON_DELTA) const
		{
			return Abs(mX - v.mX) < delta && Abs(mY - v.mY) < delta && Abs(mZ - v.mZ) < delta;
		}

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
			*this *= RSqrt(LengthSq());
		}
		Vec3 GetNormalized() const
		{
			Vec3 ret = *this;
			ret.Normalize();
			return ret;
		}
		void NormalizeSafe(const Vec3& errorValue = Vec3(0))
		{
			float s = LengthSq();
			if (s <= VECTOR_ESPILON)
			{
				*this = errorValue;
			}
			else
			{
				*this *= RSqrt(s);
			}
		}
		Vec3 GetNormalizedSafe() const
		{
			Vec3 ret = *this;
			ret.GetNormalized();
			return ret;
		}
		void MetaSerialize(Stream&);
	};


	inline float Dot(const Vec3& a, const Vec3& b)
	{
		return a.mX * b.mX + a.mY * b.mY + a.mZ * b.mZ;
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
	inline Vec3 Clamp(const Vec3& value, float min, float max)
	{
		return Vec3(Clamp(value.mX, min, max), Clamp(value.mY, min, max), Clamp(value.mZ, min, max));
	}
	inline Vec3 Clamp(const Vec3& value, const Vec3& min, const Vec3& max)
	{
		return Vec3(Clamp(value.mX, min.mX, max.mX), Clamp(value.mY, min.mY, max.mY), Clamp(value.mZ, min.mZ, max.mZ));
	}


	//////////////////////////////////////////////////////////////////////////
	struct UAPI Vec4
	{
		UCLASS(Vec4, void)

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


		inline Vec4() {}
		inline Vec4(float xyzw)
		{
			mX = mY = mZ = mW = xyzw;
		}
		inline Vec4(float x, float y, float z, float w)
		{
			mX = x;		mY = y;		mZ = z;		mW = w;
		}
		inline Vec4(const Vec2& xy, const Vec2& zw)
		{
			mX = xy.mX;		mY = xy.mY;
			mZ = zw.mX;		mW = zw.mY;
		}
		inline Vec4(const Vec2& xy, float z, float w)
		{
			mX = xy.mX;		mY = xy.mY;		mZ = z;		mW = w;
		}
		inline Vec4(float x, float y, const Vec2& zw)
		{
			mX = x;		mY = y;		mZ = zw.mX;		mW = zw.mY;
		}
		inline Vec4(const Vec3& xyz, float w)
		{
			mX = xyz.mX;		mY = xyz.mY;		mZ = xyz.mZ;		mW = w;
		}
		inline Vec4(float x, const Vec3& yzw)
		{
			mX = x;		mY = yzw.mX;		mZ = yzw.mY;	mW = yzw.mZ;
		}
		inline Vec4(const Vec4& xyzw)
		{
			mX = xyzw.mX;		mY = xyzw.mY;		mZ = xyzw.mZ;		mW = xyzw.mW;
		}



		inline Vec4& operator = (const Vec4& xyzw)
		{
			mX = xyzw.mX;		mY = xyzw.mY;		mZ = xyzw.mZ;		mW = xyzw.mW;
			return *this;
		}
		inline Vec4& operator = (float xyzw)
		{
			mX = mY = mZ = mW = xyzw;
			return *this;
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

		void MetaSerialize(Stream&);
	};
	inline Vec4 Abs(const Vec4& v)
	{
		return Vec4(Abs(v.mX), Abs(v.mY), Abs(v.mZ), Abs(v.mW));
	}
	inline Vec4 Min(const Vec4& v0, const Vec4& v1)
	{
		return Vec4(Min(v0.mX, v1.mX), Min(v0.mY, v1.mY), Min(v0.mZ, v1.mZ), Min(v0.mW, v1.mW));
	}
	inline Vec4 Max(const Vec4& v0, const Vec4& v1)
	{
		return Vec4(Max(v0.mX, v1.mX), Max(v0.mY, v1.mY), Max(v0.mZ, v1.mZ), Max(v0.mW, v1.mW));
	}
	inline Vec4 Lerp(const Vec4& v0, const Vec4& v1, float t)
	{
		return v0 * (1 - t) + v1 * t;
	}
	inline Vec4 Clamp(const Vec4& value, float min, float max)
	{
		return Vec4(Clamp(value.mX, min, max), Clamp(value.mY, min, max), Clamp(value.mZ, min, max), Clamp(value.mW, min, max));
	}
	inline Vec4 Clamp(const Vec4& value, const Vec4& min, const Vec4& max)
	{
		return Vec4(Clamp(value.mX, min.mX, max.mX), Clamp(value.mY, min.mY, max.mY), Clamp(value.mZ, min.mZ, max.mZ), Clamp(value.mW, min.mW, max.mW));
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
	//TODO remove inheritance
	struct UAPI Color : public Vec4
	{
		UCLASS(Color, void)

		inline Color(){}
		inline Color(float rgba) { mR = mG = mB = mA = rgba; }
		inline Color(float rgb, float a) { mR = mG = mB = rgb;	mA = a; }
		inline Color(float r, float g, float b, float a) { mR = r;  mG = g;  mB = b;  mA = a; }
		inline Color(const Color32& color);

		void MetaSerialize(Stream&);
	};

	//////////////////////////////////////////////////////////////////////////
	struct UAPI Color32
	{
		UCLASS(Color32, void)

		union
		{
			unsigned mColor;
			uint8	mRGBA[4];
		};

		Color32() { mColor = 0; }
		Color32(uint8 rgba) 
		{
			mRGBA[0] = rgba;	mRGBA[1] = rgba;	mRGBA[2] = rgba;	mRGBA[3] = rgba;
		}
		Color32(uint8 r, uint8 g, uint8 b, uint8 a = 255)
		{
			mRGBA[0] = r;	mRGBA[1] = g;	mRGBA[2] = b;	mRGBA[3] = a;
		}
		Color32(const Color& color)
		{
			mRGBA[0] = (uint8)Clamp(color.mR * 255.0f, 0.0f, 1.0f);
			mRGBA[1] = (uint8)Clamp(color.mG * 255.0f, 0.0f, 1.0f);
			mRGBA[2] = (uint8)Clamp(color.mB * 255.0f, 0.0f, 1.0f);
			mRGBA[3] = (uint8)Clamp(color.mA * 255.0f, 0.0f, 1.0f);
		}

		unsigned GetR() const { return mRGBA[0]; }
		unsigned GetG() const { return mRGBA[1]; }
		unsigned GetB() const { return mRGBA[2]; }
		unsigned GetA() const { return mRGBA[3]; }

		static const Color32 BLACK;
		static const Color32 WHITE;
		static const Color32 RED;
		static const Color32 GREEN;
		static const Color32 BLUE;
		static const Color32 YELLOW;

		void MetaSerialize(Stream&);
	};

	//////////////////////////////////////////////////////////////////////////Vec2
	inline Vec2::Vec2(const Vec3& v3)
	{
		mX = v3.mX;			mY = v3.mY;
	}
	Vec2& Vec2::operator=(const Vec3& v3)
	{
		mX = v3.mX;		mY = v3.mY;		return *this;
	}


	Color::Color(const Color32& color)
	{
		const float s = 1.0f / 255.0f;
		mR = color.GetR() * s;
		mG = color.GetG() * s;
		mB = color.GetB() * s;
		mA = color.GetA() * s;
	}





	//////////////////////////////////////////////////////////////////////////
	struct UAPI AABB
	{
		Vec3 mMin, mMax;

		void InitMinMax(const Vec3& min, const Vec3& max)
		{
			mMin = min;		mMax = max;
		}
		
	};


	template <typename T> struct TVec2
	{
		T mX, mY;

		inline TVec2() {}
		inline TVec2(T xy) { mX = mY = xy; }
		inline TVec2(T x, T y) { mX = x; mY = y; }
		inline TVec2(const TVec2& v) { mX = v.mX; mY = v.mY; }

		inline T& operator [] (size_t i) { return ((T*)this)[i]; }
		inline const T& operator [] (size_t i) const { return ((T*)this)[i]; }

		inline TVec2& operator = (T xy) { mX = mY = xy;  return *this; }
		inline TVec2& operator = (const T& c) { mX = c.mX;	mY = c.mY;   return *this; }

		inline TVec2 operator + (const TVec2& v) const { return TVec2(mX + v.mX, mY + v.mY); }
		inline TVec2 operator - (const TVec2& v) const { return TVec2(mX - v.mX, mY - v.mY); }
		inline TVec2 operator * (const TVec2& v) const { return TVec2(mX * v.mX, mY * v.mY); }
		inline TVec2 operator / (const TVec2& v) const { return TVec2(mX / v.mX, mY / v.mY); }

		inline TVec2 operator + (T n) const { return TVec2(mX + n, mY + n); }
		inline TVec2 operator - (T n) const { return TVec2(mX - n, mY - n); }
		inline TVec2 operator * (T n) const { return TVec2(mX * n, mY * n); }
		inline TVec2 operator / (T n) const { return TVec2(mX / n, mY / n); }


		inline TVec2& operator += (const TVec2& v) { mX += v.mX;	mY += v.mY;		return *this; }
		inline TVec2& operator -= (const TVec2& v) { mX -= v.mX;	mY -= v.mY;		return *this; }
		inline TVec2& operator *= (const TVec2& v) { mX *= v.mX;	mY *= v.mY;		return *this; }
		inline TVec2& operator /= (const TVec2& v) { mX /= v.mX;	mY /= v.mY;		return *this; }

		inline TVec2& operator += (T n) { mX += n;	mY += n;	return *this; }
		inline TVec2& operator -= (T n) { mX -= n;	mY -= n;	return *this; }
		inline TVec2& operator *= (T n) { mX *= n;	mY *= n;	return *this; }
		inline TVec2& operator /= (T n) { mX /= n;	mY /= n;	return *this; }


		bool operator > (T n) const { return mX > n && mY > n; }
		bool operator >= (T n) const { return mX >= n && mY >= n; }
		bool operator < (T n) const { return mX < n && mY < n; }
		bool operator <= (T n) const { return mX <= n && mY <= n; }

		bool operator > (const TVec2& v) const { return mX > v.mX && mY > v.mY; }
		bool operator >= (const TVec2& v) const { return mX >= v.mX && mY >= v.mY; }
		bool operator < (const TVec2& v) const { return mX < v.mX && mY < v.mY; }
		bool operator <= (const TVec2& v) const { return mX <= v.mX && mY <= v.mY; }

		bool operator == (const TVec2& v) const { return mX == v.mX && mY == v.mY; }
		bool operator != (const TVec2& v) const { return !this->operator==(v); }

		TVec2 operator - () const { return TVec2(-mX, -mY); }

	};

	typedef TVec2<int> Vec2I;

};