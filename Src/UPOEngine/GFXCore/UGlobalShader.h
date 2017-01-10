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
#define UGLOBAL_SHADER_DECL(varibleName, shaderClass) typedef shaderClass ZZT##varibleName; extern UAPI shaderClass* varibleName;


#define UGLOBAL_SHADER_IMPL(varibleName, filename, entrypoint, ...)\
	UAPI ZZT##varibleName* varibleName = nullptr;\
	struct Z_##varibleName {\
		Z_##varibleName()\
		{\
			UPO::GlobalShaderRegParam regParam = { reinterpret_cast<void**>(&varibleName), { filename, entrypoint, ZZT##varibleName::EnumType, { __VA_ARGS__ }} };\
			UPO::GlobalShaders::Register(regParam);\
		}\
	} ZZ_##varibleName;\


#define UGLOBAL_SHADER_DECLIMPL(varibleName, shaderClass, filename, entrypoint, ...) \
	UGLOBAL_SHADER_DECL(varibleName, shaderClass)\
	UGLOBAL_SHADER_IMPL(varibleName, filename, entrypoint, __VA_ARGS__)


namespace UPO
{
	struct GlobalShaderRegParam
	{
		void** mPPShader;
		ShaderUniqueParam mParam;
	};




	namespace GlobalShaders
	{
		UAPI void Register(const GlobalShaderRegParam&);
		UAPI void CompileAll();
		UAPI void ReleaseAll();
	};
};