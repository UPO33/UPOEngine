#pragma once

#include "../Core/UCore.h"

namespace UPO
{ 
	//////////////////////////////////////////////////////////////////////////
	class UAPI GameWindow
	{
	public:
		virtual void Init() {}
		virtual void Release() {}
		virtual bool Tick() { return false; }
		virtual unsigned GetWidth() { return 0; }
		virtual unsigned GetHeight() { return 0; }

		virtual void* GetWinHandle() { return nullptr; }

		static GameWindow* New();
		static void Delete(GameWindow*);
	};
};