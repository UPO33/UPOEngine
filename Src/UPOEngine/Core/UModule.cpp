#include "UModule.h"

#ifdef UPLATFORM_WIN
#include <windows.h>
#endif
#ifdef UPLATFORM_LINUX
#include <dlfcn.h>
#endif

namespace UPO
{
#ifdef UPLATFORM_WIN
	void* ULoadModule(const char* filename)
	{
		return reinterpret_cast<HMODULE>(::LoadLibraryA(filename));
	}
	bool UFreeModule(void* handle)
	{
		return ::FreeLibrary(reinterpret_cast<HMODULE>(handle)) != 0;
	}
	void* UGetProcAddress(void* handle, const char* name)
	{
		return (void*)::GetProcAddress(reinterpret_cast<HMODULE>(handle), name);
	}
#endif

#ifdef UPLATFORM_LINUX
	void* ULoadModule(const char* filename)
	{
		return dlopen(filename, RTLD_GLOBAL);
	}
	bool UFreeModule(void* handle)
	{
		//On success, dlclose() returns 0; on error, it returns a nonzerovalue.
		return dlclose(handle) == 0;
	}
	void* UGetProcAddress(void* handle, const char* name)
	{
		return (void*)dlsym(handle, name);
	}
#endif


	//////////////////////////////////////////////////////////////////////////
	ModuleSys* ModuleSys::Get()
	{
		static ModuleSys Ins;
		return &Ins;
	}

	//////////////////////////////////////////////////////////////////////////
	ModuleSys::ModuleSys()
	{
		mCurrentLoadingModule = nullptr;
		mCurrentUnloadingModule = nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	ModuleSys::~ModuleSys()
	{
		UnloadAll();
	}
	//////////////////////////////////////////////////////////////////////////
	Module* ModuleSys::LoadModule(const char* moduleName)
	{
		if (moduleName == nullptr) return nullptr;

		if (Module* foundModule = FindByName(moduleName)) // currently exist ?
			return foundModule;

		mCurrentLoadingModule = new Module;
		mCurrentLoadingModule->mName = moduleName;

		ULOG_MESSAGE("loading module [%] started...", moduleName);


		if (void* handle = ULoadModule(moduleName))
		{
			mCurrentLoadingModule->mHandle = handle;
			Proc_OnModuleLoaded loadProc = (Proc_OnModuleLoaded) UGetProcAddress(handle, MODULE_LOADED_FUNCTION_NAME);
			if (loadProc)
				loadProc();

			mLoadedModules.Add(mCurrentLoadingModule);
			Module* ret = mCurrentLoadingModule;
			mCurrentLoadingModule = nullptr;
			ULOG_SUCCESS("Module [%] successfully loaded", moduleName);
			return ret;
		}
		else
		{
			delete mCurrentLoadingModule;
			mCurrentLoadingModule = nullptr;

			ULOG_ERROR("Failed to load modue [%]", moduleName);
			return nullptr;
		}

		

	}
	//////////////////////////////////////////////////////////////////////////
	bool ModuleSys::UnloadModule(Module* module)
	{
		if (module == nullptr) return false;

		size_t findIndex = mLoadedModules.Find(module);
		if (findIndex == ~0) return false;

		mCurrentUnloadingModule = module;

		Proc_OnModuleUnoaded unloadProc = (Proc_OnModuleUnoaded) UGetProcAddress(module->mHandle, MODULE_UNLOADED_FUNCTION_NAME);
		if (unloadProc) 
			unloadProc();

		if (UFreeModule(module->mHandle))
		{
			mLoadedModules.RemoveAtSwap(findIndex);
			ULOG_SUCCESS("Module % unloaded successfully", module->mName.CStr());
			delete module;
			mCurrentUnloadingModule = nullptr;
			return true;
		}
		else
		{
			ULOG_ERROR("Failed to unload module %", module->mName.CStr());
			mCurrentUnloadingModule = nullptr;
			return false;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void ModuleSys::UnloadAll()
	{
		//maybe a module needs some other modules and can't be unloaded, because of it we repeat process many time
		for (int i = 0; i < 32; i++)
		{
			TArray<Module*> modules = mLoadedModules;
			for (size_t j = 0; j < modules.Length(); j++)
			{
				UnloadModule(modules[j]);
			}
		}
		mLoadedModules.Empty();
	}
	//////////////////////////////////////////////////////////////////////////
	bool ModuleSys::IsLoaded(const char* moduleName)
	{
		return FindByName(moduleName) != nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	Module* ModuleSys::FindByName(const char* moduleName)
	{
		if (moduleName == nullptr) return nullptr;

		for (size_t i = 0; i < mLoadedModules.Length(); i++)
		{
			if (mLoadedModules[i]->mName == moduleName)
				return mLoadedModules[i];
		}
		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////

};