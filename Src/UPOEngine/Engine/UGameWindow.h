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
		virtual void Tick() {}
		virtual unsigned GetWidth() { return 0; }
		virtual unsigned GetHeight() { return 0; }

		static GameWindow* Get();
	};
};