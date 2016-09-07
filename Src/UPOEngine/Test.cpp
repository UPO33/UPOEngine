#include "Test.h"
#include "RasterTech/systemclass.h"


namespace UPO
{

	UAPI void TestMain(int argc, const char** argv)
	{
		SystemClass* System;
		bool result;
		// Create the system object.
		System = new SystemClass;

		// Initialize and run the system object.
		result = System->Initialize();
		if (result)
		{
			System->Run();
		}

		// Shutdown and release the system object.
		System->Shutdown();
		delete System;
		System = 0;

		return;
	}

};