#include "UGlobalShader.h"

namespace UPO
{
	TArray<GlobalShaderRegParam> gGlobalShaders;


	//////////////////////////////////////////////////////////////////////////
	void UAPI UGlobalShader_Reg(const GlobalShaderRegParam& entry)
	{
		gGlobalShaders.Add(entry);
	}

	//////////////////////////////////////////////////////////////////////////
	void UAPI UGlobalShader_CompileAll(TFP<void, const ShaderUniqueParam&> compileSuccessProc /*= nullptr*/)
	{
		UASSERT(gGFX);

		for (size_t i = 0; i < gGlobalShaders.Length(); i++)
		{
			auto& param = gGlobalShaders[i].mParam;
			UASSERT(param.mFileName && param.mEntryPoint);

			GFXShader* pShader = gGFX->CreateShader(param);
			if (pShader == nullptr)
			{
				ULOG_FATAL("compiling global shader failed");
				return;
			}
			pShader->IncRefCount(1);
			
			void** ppShader = gGlobalShaders[i].mPPShader;
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

			if (compileSuccessProc)
			{
				compileSuccessProc(param);
			}
		}
	}



	//////////////////////////////////////////////////////////////////////////
	void UAPI UGlobalShader_ReleaseAll()
	{
		for (size_t i = 0; i < gGlobalShaders.Length(); i++)
		{
			auto param = gGlobalShaders[i];
			UASSERT(param.mPPShader && *param.mPPShader);
			GFXShader* pShader = *(reinterpret_cast<GFXShader**>(param.mPPShader));
			pShader->IncRefCount(-1);
			delete pShader;
		}

		gGlobalShaders.Empty();
	}

};