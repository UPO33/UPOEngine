#include "UTypeInfo.h"
#include "../Core/UStream.h"
#include "../Meta/UMeta.h"

namespace UPO
{
	UCLASS_BEGIN_IMPL(TypeInfo)
	UCLASS_END_IMPL(TypeInfo)

// 	void Attrib::Serialize(Stream& stream)
// 	{
// 		if (stream.IsReader())
// 		{
// 			stream.RW(mID);
// 			switch (mID)
// 			{
// 			case EAtrribID::EAT_Comment:
// 			{
// 				mString.Serialize(stream);
// 			}
// 			break;
// 			case EAtrribID::EAT_Range:
// 				stream.RW(mValue0).RW(mValue1);
// 				break;
// 			}
// 		}
// 		else
// 		{
// 			stream.RW(mID);
// 			switch (mID)
// 			{
// 			case EAtrribID::EAT_Comment:
// 			{
// 				mString.Serialize(stream);
// 			}
// 			break;
// 			case EAtrribID::EAT_Range:
// 				stream.RW(mValue0).RW(mValue1);
// 				break;
// 			}
// 		}
// 	}

	AttribPack::AttribPack(Attrib a0, Attrib a1, Attrib a2, Attrib a3, Attrib a4, Attrib a5, Attrib a6, Attrib a7)
	{
		mAttributes[0] = a0;
		mAttributes[1] = a1;
		mAttributes[2] = a2;
		mAttributes[3] = a3;
		mAttributes[4] = a4;
		mAttributes[5] = a5;
		mAttributes[6] = a6;
		mAttributes[7] = a7;
		mNumAttributes = 8;
	}
	AttribPack::AttribPack(Attrib a0, Attrib a1, Attrib a2, Attrib a3, Attrib a4, Attrib a5, Attrib a6)
	{
		mAttributes[0] = a0;
		mAttributes[1] = a1;
		mAttributes[2] = a2;
		mAttributes[3] = a3;
		mAttributes[4] = a4;
		mAttributes[5] = a5;
		mAttributes[6] = a6;
		mNumAttributes = 7;
	}
	AttribPack::AttribPack(Attrib a0, Attrib a1, Attrib a2, Attrib a3, Attrib a4, Attrib a5)
	{
		mAttributes[0] = a0;
		mAttributes[1] = a1;
		mAttributes[2] = a2;
		mAttributes[3] = a3;
		mAttributes[4] = a4;
		mAttributes[5] = a5;
		mNumAttributes = 6;
	}
	AttribPack::AttribPack(Attrib a0, Attrib a1, Attrib a2, Attrib a3, Attrib a4)
	{
		mAttributes[0] = a0;
		mAttributes[1] = a1;
		mAttributes[2] = a2;
		mAttributes[3] = a3;
		mAttributes[4] = a4;
		mNumAttributes = 5;
	}
	AttribPack::AttribPack(Attrib a0, Attrib a1, Attrib a2, Attrib a3)
	{
		mAttributes[0] = a0;
		mAttributes[1] = a1;
		mAttributes[2] = a2;
		mAttributes[3] = a3;
		mNumAttributes = 4;
	}
	AttribPack::AttribPack(Attrib a0, Attrib a1, Attrib a2)
	{
		mAttributes[0] = a0;
		mAttributes[1] = a1;
		mAttributes[2] = a2;
		mNumAttributes = 3;
	}
	AttribPack::AttribPack(Attrib a0, Attrib a1)
	{
		mAttributes[0] = a0;
		mAttributes[1] = a1;
		mNumAttributes = 2;
	}
	AttribPack::AttribPack(Attrib a0)
	{
		mAttributes[0] = a0;
		mNumAttributes = 1;
	}
	AttribPack::AttribPack()
	{
		mNumAttributes = 0;
	}

	bool AttribPack::HasAttrib(EAttribID id) const
	{
		for (unsigned i = 0; i < mNumAttributes; i++)
			if (mAttributes[i].GetID() == id) return true;
		return false;
	}

	bool AttribPack::GetAttrib(EAttribID id, Attrib& out) const
	{
		for (unsigned i = 0; i < mNumAttributes; i++)
		{
			if (mAttributes[i].GetID() == id)
			{
				out = mAttributes[i];
				return true;
			}
		}
		return false;
	}

	unsigned AttribPack::NumAttributes() const
	{
		return mNumAttributes;
	}

};