#pragma once

#include "UGFXDevice.h"
#include <bitset>
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

#define UGLOBAL_SHADER_DECL2(varibleName, shaderClass) 	struct ZZC##varibleName : public GlobalShaderData\
	{\
	typedef shaderClass ShaderClassT;\
	ZZC##varibleName(const char* filename, const char* entrypoint, EShaderType type, const DefinitionList& definitions = DefinitionList())\
	: GlobalShaderData(filename, entrypoint, type, definitions){}\
	shaderClass* Get(const ShaderConstantsCombined& definitions) { return (shaderClass*)_Get(definitions); }\
	ZZC##varibleName(){}\
	};\
	extern UAPI ZZC##varibleName* varibleName;\




#define UGLOBAL_SHADER_IMPL2(varibleName, filename, entrypoint, ...)\
	UAPI ZZC##varibleName* varibleName = new ZZC##varibleName(filename, entrypoint, ZZC##varibleName::ShaderClassT::EnumType, __VA_ARGS__);
	

#define UGLOBAL_SHADER_DECLIMPL2(varibleName, shaderClass, filename, entrypoint, ...)\
	UGLOBAL_SHADER_DECL2(varibleName, shaderClass);\
	UGLOBAL_SHADER_IMPL2(varibleName, filename, entrypoint, __VA_ARGS__);




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

	static const size_t MaxShaderConstant = 256;

	//////////////////////////////////////////////////////////////////////////
	struct UAPI ShaderConstant
	{
		size_t	mIndex;

	public:
		ShaderConstant(size_t index = 0) : mIndex(index) {}
		ShaderConstant(const char* name, const char* value);
		~ShaderConstant();

		Name GetName() const;
		Name GetValue() const;
	};

	//////////////////////////////////////////////////////////////////////////
	struct UAPI ShaderConstantsCombined
	{
		std::bitset<MaxShaderConstant>	mBits;

		ShaderConstantsCombined() : mBits(0)
		{
		}
		ShaderConstantsCombined(ShaderConstant sc) : mBits(0)
		{
			mBits.set(sc.mIndex);
		}
		ShaderConstantsCombined& operator |= (const ShaderConstantsCombined& rh)
		{
			mBits |= rh.mBits;
			return *this;
		}
		ShaderConstantsCombined& operator |= (ShaderConstant rh)
		{
			mBits.set(rh.mIndex);
			return *this;
		}
		ShaderConstantsCombined operator | (const ShaderConstantsCombined& rh) const
		{
			ShaderConstantsCombined ret(*this);
			ret |= rh;
			return ret;
		}
		ShaderConstantsCombined operator | (ShaderConstant rh) const
		{
			ShaderConstantsCombined ret(*this);
			ret |= rh;
			return ret;
		}
		void GetShaderConstants(ShaderConstant out[MaxShaderConstant], size_t& outCount) const;
		bool Has(ShaderConstant sc) const
		{
			return mBits.test(sc.mIndex);
		}
		bool operator == (const ShaderConstantsCombined& rh) const
		{
			//TODO SIMD optimizations possible
			return mBits == rh.mBits;
		}
		bool operator != (const ShaderConstantsCombined& rh) const
		{
			return !this->operator==(rh);
		}
	};


	//////////////////////////////////////////////////////////////////////////
	struct UAPI GlobalShaderData
	{
		TArray<ShaderConstantsCombined>		mShadersConstants;
		TArray<GFXShader*>					mShaders;

		typedef std::initializer_list<std::pair<const char*, const char*>> DefinitionList;

		Name			mFilename;
		Name			mEntrypoint;
		EShaderType		mShaderType;
		DefinitionList	mDefinitions;
		

		GlobalShaderData(){}
		~GlobalShaderData();
		GlobalShaderData(const char* filename, const char* entrypoint, EShaderType type, const DefinitionList& definitions = DefinitionList());
		GFXShader* _Get(const ShaderConstantsCombined& definitions);

	};
	
};