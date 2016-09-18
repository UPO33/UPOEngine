#pragma once

#include "UBasic.h"
#include "UName.h"
#include "UArray.h"

namespace UPO
{
	typedef void(*Proc_OnModuleLoaded) (void);
	typedef void(*Proc_OnModuleUnoaded) (void);

	static const char* MODULE_LOADED_FUNCTION_NAME = "OnModuleLoaded";	//is called when a module is loaded
	static const char* MODULE_UNLOADED_FUNCTION_NAME = "OnModuleUnloaded";	//is called when a module is unloaded


	//////////////////////////////////////////////////////////////////////////
	class UAPI Module
	{
		friend class ModuleSys;
		friend class MetaSys;

		Name		mName;
		void*		mHandle = nullptr;

	public:
		Name GetName() const { return mName; }
	};


	//////////////////////////////////////////////////////////////////////////
	class UAPI ModuleSys
	{
		friend class Module;

		TArray<Module*>		mLoadedModules;
		Module*				mCurrentLoadingModule = nullptr;
		Module*				mCurrentUnloadingModule = nullptr;

	public:
		static ModuleSys* Get();

		ModuleSys();
		~ModuleSys();

		Module* LoadModule(const char* moduleName);
		bool UnloadModule(Module* module);
		void UnloadAll();
		bool IsLoaded(const char* moduleName);
		Module* FindByName(const char* moduleName);

		const TArray<Module*>& GetLoadedModules() const { return mLoadedModules; }
		Module* GetCurrentLoadingModule() const { return mCurrentLoadingModule; }
		Module* GetCurrentUnloadingModule() const { return mCurrentUnloadingModule; }
	};
};
