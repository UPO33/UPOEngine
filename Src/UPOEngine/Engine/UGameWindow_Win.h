#include "UGameWindow.h"

#ifdef UPLATFORM_WIN
#include <windows.h>
#include <windowsx.h>
#include <WinUser.h>

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class UAPI GameWindowWin : public GameWindow
	{
		TinyLock mSizeLock;
		Vec2I mSize;

		HINSTANCE mAppHandle = nullptr;
		bool mFullScreen;
		wchar_t* mWindowName;

	public:
		

		
		//renderer read this every frame so that it must be tread safe
		void GetWinSize(Vec2I& out) override;
		void OnCreateWindow() override;
		void OnDestroyWindow() override;

		//////////////////////////////////////////////////////////////////////////
		LRESULT MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
	};

	


};

#endif	//UPLATFORM_WIN