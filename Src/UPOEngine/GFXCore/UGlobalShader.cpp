#include "UGlobalShader.h"
#include "UShaderMgr.h"
#include "../Core/ULargeInteger.h"

#include <bitset>

namespace UPO
{
	struct GlobalShadersContext
	{
		//DEPRECATED
		TArray<GlobalShaderRegParam>	mShaders;

		TArray<GlobalShaderData*>		mGlobalShaders;
	};

	GlobalShadersContext* gGlobalShadersContext = nullptr;

	void GlobalShaders::Register(const GlobalShaderRegParam& param)
	{
		if (!gGlobalShadersContext) gGlobalShadersContext = new GlobalShadersContext;

		gGlobalShadersContext->mShaders.Add(param);
	}

	void GlobalShaders::CompileAll()
	{
		ULOG_MESSAGE("compiling global sghaders...");

		if (!gGlobalShadersContext) return;


		for(GlobalShaderRegParam& regParam : gGlobalShadersContext->mShaders)
		{
			auto& param = regParam.mParam;
			UASSERT(param.mFileName && param.mEntryPoint);

			GFXShader* pShader = gGFX->CreateShader(param);
			if (pShader == nullptr)
			{
				ULOG_FATAL("compiling global shader failed");
				return;
			}
			pShader->IncRefCount(1);

			void** ppShader = regParam.mPPShader;
			switch (param.mType)
			{
			case EShaderType::EVertex: *(reinterpret_cast<GFXVertexShader**>(ppShader)) = (GFXVertexShader*)pShader;
				break;
			case EShaderType::EHull: *(reinterpret_cast<GFXHullShader**>(ppShader)) = (GFXHullShader*)pShader;
				break;
			case EShaderType::EDomain: *(reinterpret_cast<GFXDomainShader**>(ppShader)) = (GFXDomainShader*)pShader;
				break;
			case EShaderType::EGeometry: *(reinterpret_cast<GFXGeometryShader**>(ppShader)) = (GFXGeometryShader*)pShader;
				break;
			case EShaderType::EPixel: *(reinterpret_cast<GFXPixelShader**>(ppShader)) = (GFXPixelShader*)pShader;
				break;
			case EShaderType::ECompute: *(reinterpret_cast<GFXComputeShader**>(ppShader)) = (GFXComputeShader*)pShader;
				break;
			}
		}
	}

	void GlobalShaders::ReleaseAll()
	{
		//DEPRECATED
#if 0
		if (!gGlobalShadersContext)return;


		for (GlobalShaderRegParam& param : gGlobalShadersContext->mShaders)
		{
			UASSERT(param.mPPShader && *param.mPPShader);
			GFXShader* pShader = *(reinterpret_cast<GFXShader**>(param.mPPShader));
			pShader->IncRefCount(-1);
			delete pShader;
		}

		delete gGlobalShadersContext;
#endif // 

		if (gGlobalShadersContext)
		{
			for (auto item : gGlobalShadersContext->mGlobalShaders)
			{
				delete item;
			}

			gGlobalShadersContext->mGlobalShaders.RemoveAll();
		}
		
	}






	struct ShaderConstantHash
	{
		static const size_t	NumBit = 32;
		static const size_t NumDW = 16;

		unsigned	mDW[NumDW];

		uint256		mBits;

		ShaderConstantHash()
		{
			for (size_t i = 0; i < NumDW; i++) mDW[i] = 0;
		}
		~ShaderConstantHash()
		{

		}
		bool operator == (const ShaderConstantHash& rh) const
		{
			for (size_t i = 0; i < NumDW; i++)
				if (mDW[i] != rh.mDW[i]) return false;
			return true;
		}
		bool operator != (const ShaderConstantHash& rh) const
		{
			return !this->operator==(rh);
		}

		ShaderConstantHash& operator |= (const ShaderConstant& sc)
		{
			auto index = sc.mIndex;
			mDW[index / NumBit] |= 1 << (index % NumBit);
		}
	};

	



	struct ShaderConstantExt
	{
		Name	mName;
		Name	mValue;
	};

	
	size_t					gShadersConstantCounter = 0;
	ShaderConstantExt		gShadersConstantExt[MaxShaderConstant];
	
	size_t UFindShaderConstant(Name name, Name value)
	{
		for (size_t i = 0; i < MaxShaderConstant; i++)
		{
			if (gShadersConstantExt[i].mName == name && gShadersConstantExt[i].mValue == value)
				return i;
		}
		return InvalidIndex;
	}

	ShaderConstant::ShaderConstant(const char* name, const char* value)
	{
		Name theName = name;
		Name theValue = value;

		size_t index = UFindShaderConstant(theName, theValue);
		if (index != InvalidIndex)
		{
			mIndex = index;
		}
		else
		{
			mIndex = ++gShadersConstantCounter;
			UASSERT(gShadersConstantCounter < MaxShaderConstant);
		}

		gShadersConstantExt[mIndex].mName = name;
		gShadersConstantExt[mIndex].mValue = value;
	}

	ShaderConstant::~ShaderConstant()
	{

	}

	Name ShaderConstant::GetName() const
	{
		return gShadersConstantExt[mIndex].mName;
	}

	Name ShaderConstant::GetValue() const
	{
		return gShadersConstantExt[mIndex].mValue;
	}

	void ShaderConstantsCombined::GetShaderConstants(ShaderConstant out[MaxShaderConstant], size_t& outCount) const
	{
		outCount = mBits.count();
		size_t counter = 0;
		for (size_t i = 0; i < MaxShaderConstant; i++)
		{
			if (mBits.test(i))
			{
				out[counter] = ShaderConstant(i);
				counter++;
			}
		}
	}


	//////////////////////////////////////////////////////////////////////////
	GFXShader* GlobalShaderData::_Get(const ShaderConstantsCombined& definitions)
	{
		auto index = mShadersConstants.Find(definitions);
		if (index != InvalidIndex)
		{
			return mShaders[index];
		}
		else
		{
			ShaderConstant shaderConstants[MaxShaderConstant];
			size_t numConstants = 0;
			definitions.GetShaderConstants(shaderConstants, numConstants);
			
			TArray<ShaderMacroDefinition> arrayDefinitions;
			for (size_t i = 0; i < numConstants; i++)
			{
				arrayDefinitions.Add(ShaderMacroDefinition(shaderConstants[i].GetName(), shaderConstants[i].GetValue()));
			}
			for (auto item : mDefinitions)
			{
				arrayDefinitions.Add(ShaderMacroDefinition(item.first, item.second));
			}


			Buffer shaderBytes;
			if (!ShaderMgr::Get()->GetShaderCode(mFilename, mEntrypoint, mShaderType, arrayDefinitions, shaderBytes))
			{
				ULOG_FATAL("compiling global shader failed, no such a file [%][%][%]", mFilename, mEntrypoint, EnumToStr(mShaderType));
				return nullptr;
			}
			GFXShader* compiledShader = gGFX->CreateShader(shaderBytes, mShaderType, mFilename);
			if (!compiledShader)
			{
				ULOG_FATAL("failed to compile shader [%][%][%]", mFilename, mEntrypoint, EnumToStr(mShaderType));
				return nullptr;
			}

			mShadersConstants.Add(definitions);
			mShaders.Add(compiledShader);
			return compiledShader;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	GlobalShaderData::GlobalShaderData(const char* filename, const char* entrypoint, EShaderType type, const DefinitionList& definitions)
	{
		mFilename = filename;
		mEntrypoint = entrypoint;
		mShaderType = type;
		mDefinitions = definitions;

		if (gGlobalShadersContext == nullptr)
			gGlobalShadersContext = new GlobalShadersContext;
		
		gGlobalShadersContext->mGlobalShaders.Add(this);
	}

	//////////////////////////////////////////////////////////////////////////
	GlobalShaderData::~GlobalShaderData()
	{
		for (auto item : mShaders)
		{
			if (item) delete item;
		}
		mShaders.RemoveAll();
		mShadersConstants.RemoveAll();
	}

};