#include "UVector.h"
#include "../Meta/UMeta.h"

namespace UPO
{
	UCLASS_BEGIN_IMPL(Vec2)
	UCLASS_END_IMPL(Vec2)


	UCLASS_BEGIN_IMPL(Vec3)
	UCLASS_END_IMPL(Vec3)

	
	UCLASS_BEGIN_IMPL(Vec4)
	UCLASS_END_IMPL(Vec4)



	UCLASS_BEGIN_IMPL(Color)
	UCLASS_END_IMPL(Color)



	UCLASS_BEGIN_IMPL(Color32)
	UCLASS_END_IMPL(Color32)

	//////////////////////////////////////////////////////////////////////////
	const Color32 Color32::BLACK(0, 0, 0, 255);
	const Color32 Color32::WHITE(255, 255, 255, 255);
	const Color32 Color32::RED(255, 0, 0, 255);
	const Color32 Color32::GREEN(0, 255, 0, 255);
	const Color32 Color32::BLUE(0, 0, 255, 255);
	const Color32 Color32::YELLOW(255, 255, 0, 255);

	void Vec2::ToString(char* outBuffer, unsigned bufferSize) const
	{
		UASSERT(outBuffer && bufferSize);
		sprintf_s(outBuffer, bufferSize, "{%f, %f}", mX, mY);
	}

	String Vec2::ToString() const
	{
		char buffer[40];
		ToString(buffer, 40);
		return String(buffer);
	}

	void Vec2::MetaSerialize(Stream& s)
	{
		s.RW(mX).RW(mY);
	}

	void Vec3::ToString(char* outBuffer, unsigned bufferSize) const
	{
		UASSERT(outBuffer && bufferSize);
		sprintf_s(outBuffer, bufferSize, "{%f, %f, %f}", mX, mY, mZ);
	}

	String Vec3::ToString() const
	{
		char buffer[80];
		ToString(buffer, 80);
		return String(buffer);
	}

	void Vec3::MetaSerialize(Stream& s)
	{
		s.RW(mX).RW(mY).RW(mZ);
	}
	void Vec4::MetaSerialize(Stream& s)
	{
		s.RW(mX).RW(mY).RW(mZ).RW(mW);
	}

	void Vec4::ToString(char* outBuffer, unsigned bufferSize) const
	{
		UASSERT(outBuffer && bufferSize);
		sprintf_s(outBuffer, bufferSize, "{%f, %f, %f, %f}", mX, mY, mZ, mW);
	}

	String Vec4::ToString() const
	{
		char buffer[120];
		ToString(buffer, 120);
		return String(buffer);
	}

	void Color::MetaSerialize(Stream& s)
	{
		s.RW(mR).RW(mG).RW(mB).RW(mA);
	}

	void Color::ToString(char* outBuffer, unsigned bufferSize) const
	{
		UASSERT(outBuffer && bufferSize);
		sprintf_s(outBuffer, bufferSize, "{%f, %f, %f, %f}", mR, mG, mB, mA);
	}

	String Color::ToString() const
	{
		char buffer[120];
		ToString(buffer, 120);
		return String(buffer);
	}

	void Color32::ToString(char* outBuffer, unsigned bufferSize) const
	{
		UASSERT(outBuffer && bufferSize);
		sprintf_s(outBuffer, bufferSize, "{%i, %i, %i, %i}", mRGBA[0], mRGBA[1], mRGBA[2], mRGBA[3]);
	}

	String Color32::ToString() const
	{
		char buffer[80];
		ToString(buffer, 80);
		return String(buffer);
	}

	void Color32::MetaSerialize(Stream& s)
	{
		s.RW(mRGBA);
	}


	StringStreamOut& operator<<(StringStreamOut& stream, const Vec2& v)
	{
		stream << "{" << v.mX << ", " << v.mY << "}";
		return stream;
	}
	StringStreamOut& operator<<(StringStreamOut& stream, const Vec3& v)
	{
		stream << "{" << v.mX << ", " << v.mY << ", " << v.mZ << "}";
		return stream;
	}
	StringStreamOut& operator<<(StringStreamOut& stream, const Vec4& v)
	{
		stream << "{" << v.mX << ", " << v.mY << ", " << v.mZ << ", " << v.mW << "}";
		return stream;
	}
	StringStreamOut& operator<<(StringStreamOut& stream, const Color& c)
	{
		stream << "{" << c.mR << ", " << c.mG << ", " << c.mB << ", " << c.mA << "}";
		return stream;
	}
	StringStreamOut& operator<<(StringStreamOut& stream, Color32 c)
	{
		stream << "{" << c.mRGBA[0] << ", " << c.mRGBA[1] << ", " << c.mRGBA[2] << ", " << c.mRGBA[3] << "}";
		return stream;
	}
	StringStreamOut& operator<<(StringStreamOut& stream, Vec2I v)
	{
		stream << "{" << v.mX << ", " << v.mY << "}";
		return stream;
	}


};