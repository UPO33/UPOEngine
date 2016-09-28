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

	void Vec2::MetaSerialize(Stream& s)
	{
		s.RW(mX).RW(mY);
	}
	void Vec3::MetaSerialize(Stream& s)
	{
		s.RW(mX).RW(mY).RW(mZ);
	}
	void Vec4::MetaSerialize(Stream& s)
	{
		s.RW(mX).RW(mY).RW(mZ).RW(mW);
	}
void Color::MetaSerialize(Stream& s)
	{
		s.RW(mR).RW(mG).RW(mB).RW(mA);
	}
	void Color32::MetaSerialize(Stream& s)
	{
		s.RW(mRGBA);
	}

};