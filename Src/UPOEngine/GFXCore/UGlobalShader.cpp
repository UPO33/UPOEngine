#include "UGlobalShader.h"

namespace UPO
{
	struct GlobalShadersContext
	{
		TArray<GlobalShaderRegParam>	mShaders;
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
		if (!gGlobalShadersContext)return;


		for (GlobalShaderRegParam& param : gGlobalShadersContext->mShaders)
		{
			UASSERT(param.mPPShader && *param.mPPShader);
			GFXShader* pShader = *(reinterpret_cast<GFXShader**>(param.mPPShader));
			pShader->IncRefCount(-1);
			delete pShader;
		}

		delete gGlobalShadersContext;
	}




	//////////////////////////////////////////////////////////////////////////
	void UAPI UGlobalShader_ReleaseAll()
	{

	}


};