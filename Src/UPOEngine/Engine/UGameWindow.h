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
		virtual void GetSize(Vec2I& out) {}
		virtual void* GetWinHandle() { return nullptr; }

		static GameWindow* New();
		static void Delete(GameWindow*);
	};
};