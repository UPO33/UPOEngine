#pragma once

#include "../Core/UBasic.h"
#include "../GFXCore/UGFXCore.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	class UAPI SelectionBuffer
	{
	public:
		GFXTexture2D*			mIDBuffer;
		GFXTexture2D*			mIDOut;
		GFXRenderTargetView*	mIDBufferRTV;
		Vec2I					mCursorPosition;
		SelectionBuffer(Vec2I size);
		~SelectionBuffer();

		//will catch required data for id at the cursor position
		void Catch();

		void SetCurdorPosition(Vec2I position);
		void Resize(Vec2I newSize);
	};
};