#pragma once

#include "UGFXDevice.h"

#define USHADERS_FOLDER "..//Content//Shaders//"



namespace UPO
{


	//////////////////////////////////////////////////////////////////////////
	class UAPI ShaderMgr
	{
		TArray<ShaderMacroDefinition> mGlobalDefinitions;

	public:
		static ShaderMgr* Get();

		

		const TArray<ShaderMacroDefinition>& GetGlobalDefinitions() { return mGlobalDefinitions; }

		virtual bool GetShaderCode(const ShaderUniqueParam& param, Buffer& outShaderCode) = 0;

		
	};
};