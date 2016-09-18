#pragma once

#include "UAsset.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class UTexture2D : public Asset
	{
		UCLASS(UTexture2D, Asset)

	private:
		Buffer		mContent;
		unsigned	mWidth;
		unsigned	mHeight;

	};
};