#include "Test.h"
#include "RasterTech/systemclass.h"
#include "GFXCore/UGFXLauncherWin.h"

#include <SDL.h>

namespace UPO
{
	void FuncV() { ULOG_MESSAGE(""); };
	struct Test
	{
		void Func0() { ULOG_MESSAGE(""); return FuncV(); }
		int Func1(int) { ULOG_MESSAGE(""); return 8; }
	};
	struct Test2
	{
		void Func0() { ULOG_MESSAGE(""); return FuncV(); }
		int Func1(int) { ULOG_MESSAGE(""); return 8; }
	};
	int Func33(int) { ULOG_MESSAGE(""); return 8; }


	
	//////////////////////////////////////////////////////////////////////////
	UAPI void TestMain(int argc, const char** argv)
	{

		system("pause");
		return;
	}

};