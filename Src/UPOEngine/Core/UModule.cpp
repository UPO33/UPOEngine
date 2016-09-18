#include "UModule.h"

#ifdef UPLATFORM_WIN
#include <windows.h>
#endif

namespace UPO
{


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

		ULOG_MESSAGE("loading module [%s] started...", moduleName);

#ifdef UPLATFORM_WIN
		HMODULE hModuleHandle = ::LoadLibraryA(moduleName);
		if (hModuleHandle != NULL)
		{
			mCurrentLoadingModule->mHandle = (void*)hModuleHandle;
			Proc_OnModuleLoaded loadProc = (Proc_OnModuleLoaded)::GetProcAddress(hModuleHandle, MODULE_LOADED_FUNCTION_NAME);
			if (loadProc)
				loadProc();

			mLoadedModules.Add(mCurrentLoadingModule);
			Module* ret = mCurrentLoadingModule;
			mCurrentLoadingModule = nullptr;
			ULOG_SUCCESS("Module [%s] successfully loaded", moduleName);
			return ret;
		}
		else
		{
			delete mCurrentLoadingModule;
			mCurrentLoadingModule = nullptr;

			ULOG_ERROR("Failed to load modue [%s]", moduleName);
			return nullptr;
		}
#endif
		

	}
	//////////////////////////////////////////////////////////////////////////
	bool ModuleSys::UnloadModule(Module* module)
	{
		if (module == nullptr) return false;

		size_t findIndex = mLoadedModules.Find(module);
		if (findIndex == ~0) return false;

		mCurrentUnloadingModule = module;

#ifdef UPLATFORM_WIN
		HMODULE handle = (HMODULE)module->mHandle;
		Proc_OnModuleUnoaded unloadProc = (Proc_OnModuleUnoaded) ::GetProcAddress(handle, MODULE_UNLOADED_FUNCTION_NAME);
		if (unloadProc) unloadProc();

		if (::FreeLibrary(handle))
		{
			mLoadedModules.RemoveAtSwap(findIndex);
			ULOG_SUCCESS("Module %s unloaded successfully", module->mName.CStr());
			delete module;
			mCurrentUnloadingModule = nullptr;
			return true;
		}
		else
		{
			ULOG_ERROR("Failed to unload module %s", module->mName.CStr());
			mCurrentUnloadingModule = nullptr;
			return false;
		}
#endif
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