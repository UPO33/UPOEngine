#pragma once

#include "UAsset.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class UAPI AMaterialRS
	{

	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI AMaterial : public Asset
	{
		/*
		USMesh*		mPreviewMesh;
		*/
		AMaterialRS*	mRS = nullptr;
	public:
		AMaterialRS* GetRS() const { return mRS; }
	};
};