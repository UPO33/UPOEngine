#pragma once

#include "UGFXDevice.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class GameWindow;

	//////////////////////////////////////////////////////////////////////////
	class UAPI GFXContext
	{
	public:
		struct InitParam
		{
			void* mWindowHandle = nullptr;
			unsigned mWidth = 0;
			unsigned mHeight = 0;
		};
		
		GFXContext(){}
		virtual ~GFXContext(){}

		virtual bool Init(InitParam param) { return false; }
		virtual bool Init(GameWindow*) { return false; }
		virtual bool Resize(unsigned newW, unsigned newH) { return false; }
		virtual bool Release() { return false; }
		virtual bool PresentSwapChain() { return false; };
		virtual GameWindow* GetGameWindow() { return nullptr; }
		virtual GFXTexture2D* GetBackBuffer() { return nullptr; }
		virtual unsigned BackBufferWidth() { return 0; }
		virtual unsigned BackBufferHeight() { return 0; }

		static GFXContext* New();
		static void Delete(GFXContext*);
	};
};