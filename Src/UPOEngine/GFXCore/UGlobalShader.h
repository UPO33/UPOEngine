#pragma once

#include "UGFXDevice.h"

/*
declare a global shader, the global shaders will be compiled on engine startup
how to use:
.h:
	UDECL_GLOBAL_SHADER(gMyVertexShader, GFXVertexShader);
.cpp:
	UIMPL_GLOBAL_SHADER(gMyVertexShader, "MyVertesShader.hlsl", "VSMain", {"MY_DEFINITION", "float3(0,0,0)" }, {"MY_PI", "3.1415" } );
*/
#define UDECL_GLOBAL_SHADER(varibleName, shaderClass) typedef shaderClass ZZT##varibleName; extern UAPI shaderClass* varibleName;


#define UIMPL_GLOBAL_SHADER(varibleName, filename, entrypoint, ...)\
	UAPI ZZT##varibleName* varibleName = nullptr;\
	struct Z_##varibleName {\
		Z_##varibleName()\
		{\
			GlobalShaderRegParam regParam = { reinterpret_cast<void**>(&varibleName), { filename, entrypoint, ZZT##varibleName::EnumType, { __VA_ARGS__ }} };\
			UGlobalShader_Reg(regParam);\
		}\
	} ZZ_##varibleName;\



namespace UPO
{
	struct GlobalShaderRegParam
	{
		void** mPPShader;
		ShaderUniqueParam mParam;
	};



	void UAPI UGlobalShader_Reg(const GlobalShaderRegParam&);
	void UAPI UGlobalShader_CompileAll(TFP<void, const ShaderUniqueParam&> compileSuccessProc = nullptr);
	void UAPI UGlobalShader_ReleaseAll();


};