#include "UShaderMgr.h"

#ifdef UPLATFORM_WIN

#include "UGFXDeviceDX.h"

namespace UPO
{
#if 0
	struct UAPI ShaderMgr
	{
		static D3D_SHADER_MACRO* GetGlobalDefinesDX()
		{
			return nullptr;
		}

		static D3D_SHADER_MACRO* GetDefinesDX(const ShaderMacroDefinition* defs)
		{
			
		}
		static void FixShaderPath(const char* shaderFilename, char outFullFilename[256])
		{
			sprintf_s(outFullFilename, 256, "..\\Content\\Shaders\\%s", shaderFilename);
		}
		static bool GetByteCode(const char* filename, const char* entryPoint, EShaderType type, const ShaderMacroDefinition* defs, Buffer& outByteCodes)
		{
			outByteCodes = Buffer();

			if (!filename || !entryPoint) return false;

			char fullFileName[256];
			FixShaderPath(filename, fullFileName);

			Buffer fileContent;
			if (!File::OpenReadFull(fullFileName, fileContent))
			{
				ULOG_ERROR("there is no such a shader file %s", filename);
				return false;
			}

			ShaderIncludeer inc;
			ID3D10Blob* shaderByteCode = nullptr;
			ID3D10Blob* shaderError = nullptr;
			if (SUCCEEDED(D3DCompile(fileContent.Data(), fileContent.Size(), fullFileName, GetGlobalDefines()
				, /*D3D_COMPILE_STANDARD_FILE_INCLUDE*/ &inc, entryPoint, ShaderTypeToDXProfile(type), D3D10_SHADER_ENABLE_STRICTNESS,
				0, &shaderByteCode, &shaderError)))
			{
				outByteCodes = Buffer(shaderByteCode->GetBufferSize(), shaderByteCode->GetBufferPointer());
				return true;
			}
			else
			{
				if (shaderError)
					ULOG_ERROR("Compiling shader failed %s %s : %s", filename, entryPoint, ((char*)shaderError->GetBufferPointer()));
				else
					ULOG_ERROR("Mising Shader File : %s", filename);
				return false;
			}
		}

	};

#endif

	void ShaderFileToRelativePath(const char* shaderFilename, char outFullFilename[UMAX_PATH])
	{
		sprintf_s(outFullFilename, UMAX_PATH, "..\\Shaders\\%s", shaderFilename);
	}
	//////////////////////////////////////////////////////////////////////////
	class ShaderIncludeer : public ID3DInclude
	{
	public:
		//////////////////////////////////////////////////////////////////////////
		HRESULT __stdcall Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
		{
			char fullFilename[UMAX_PATH];
			ShaderFileToRelativePath(pFileName, fullFilename);
			Buffer buffer;
			if (File::OpenReadFull(fullFilename, buffer))
			{
				void* data = MemAlloc(buffer.Size());
				MemCopy(data, buffer.Data(), buffer.Size());

				*ppData = data;
				*pBytes = buffer.Size();
				return S_OK;
			}
			else
			{
				*ppData = nullptr;
				*pBytes = 0;
				return S_FALSE;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		HRESULT __stdcall Close(LPCVOID pData)
		{
			if (pData) MemFree((void*)pData);
			return S_OK;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	class ShaderMgrDX : public ShaderMgr
	{
		//////////////////////////////////////////////////////////////////////////
		D3D_SHADER_MACRO* GetFinalDefinitionsDX(const ShaderMacroDefinition* customDefinitions)
		{
			static D3D_SHADER_MACRO MacroBuffersDX[256];
			static TArray<ShaderMacroDefinition> Macros;


			Macros.RemoveAll();
			Macros += GetGlobalDefinitions();

			//adding custom definitions
			if(customDefinitions)
			{
				while (customDefinitions->mName)
				{
					auto found = Macros.FindLambda([&](const ShaderMacroDefinition& elem) {
						return elem.mName == customDefinitions->mName;
					});

					if (found != ~0) // found ?
					{
						ULOG_ERROR("shader macro confilict [%]", customDefinitions->mName);
					}
					else
					{
						Macros.Add(*customDefinitions);
					}
					customDefinitions++;
				}
			}

			//convert to DX type
			{
				for (size_t i = 0; i < Macros.Length(); i++)
				{
					MacroBuffersDX[i].Name = Macros[i].mName;
					MacroBuffersDX[i].Definition = Macros[i].mDefinition;
				}
				//must be null terminated
				MacroBuffersDX[Macros.Length()] = { nullptr, nullptr };
			}

			return MacroBuffersDX;
		}
		//////////////////////////////////////////////////////////////////////////
		D3D_SHADER_MACRO* GetFinalDefinitionsDX(const TArray<ShaderMacroDefinition>& customDefinitions)
		{
			TArray<ShaderMacroDefinition> defs = customDefinitions;
			defs.Add(ShaderMacroDefinition());
			return GetFinalDefinitionsDX(defs.Elements());
		}
		//////////////////////////////////////////////////////////////////////////
		bool GetShaderCode(const ShaderUniqueParam& param, Buffer& outShaderCode) override
		{
			outShaderCode = Buffer();

			if (!param.mFileName || !param.mEntryPoint) return false;

			char fullFileName[FILENAME_MAX];
			ShaderFileToRelativePath(param.mFileName, fullFileName);

			Buffer fileContent;
			if (!File::OpenReadFull(fullFileName, fileContent))
			{
				ULOG_ERROR("there is no such a shader file %", param.mFileName);
				return false;
			}

			ShaderIncludeer inc;
			ID3D10Blob* shaderByteCode = nullptr;
			ID3D10Blob* shaderError = nullptr;
			if (SUCCEEDED(D3DCompile(fileContent.Data(), fileContent.Size(), fullFileName, GetFinalDefinitionsDX(param.mCustomDefinitions)
				, /*D3D_COMPILE_STANDARD_FILE_INCLUDE*/ &inc, param.mEntryPoint, ShaderTypeToDXProfile(param.mType), 
				D3D10_SHADER_ENABLE_STRICTNESS, 0, &shaderByteCode, &shaderError)))
			{
				outShaderCode = Buffer(shaderByteCode->GetBufferSize(), shaderByteCode->GetBufferPointer());
				return true;
			}
			else
			{
				if (shaderError)
					ULOG_ERROR("failed to compile shader [%] [%] [%]    %", param.mFileName, param.mEntryPoint, EnumToStr(param.mType), ((char*)shaderError->GetBufferPointer()));
				else
					ULOG_ERROR("Mising Shader File [%]", param.mFileName);
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		bool GetShaderCode(Name filename, Name entrypoint, EShaderType type, const TArray<ShaderMacroDefinition>& definitions, Buffer& outShaderCode) override
		{
			outShaderCode = Buffer();

			char fullFileName[FILENAME_MAX];
			ShaderFileToRelativePath(filename, fullFileName);

			Buffer fileContent;
			if (!File::OpenReadFull(fullFileName, fileContent))
			{
				return false;
			}

			ShaderIncludeer inc;
			ID3D10Blob* shaderByteCode = nullptr;
			ID3D10Blob* shaderError = nullptr;
			if (SUCCEEDED(D3DCompile(fileContent.Data(), fileContent.Size(), fullFileName, GetFinalDefinitionsDX(definitions)
				, /*D3D_COMPILE_STANDARD_FILE_INCLUDE*/ &inc, entrypoint, ShaderTypeToDXProfile(type),
				D3D10_SHADER_ENABLE_STRICTNESS, 0, &shaderByteCode, &shaderError)))
			{
				outShaderCode = Buffer(shaderByteCode->GetBufferSize(), shaderByteCode->GetBufferPointer());
				return true;
			}
			else
			{
				if (shaderError)
					ULOG_ERROR("failed to compile shader [%] [%] [%]    %", filename, entrypoint, EnumToStr(type), ((char*)shaderError->GetBufferPointer()));
				else
					ULOG_ERROR("Mising Shader File [%]", filename);
				return false;
			}
		}
	};

	//////////////////////////////////////////////////////////////////////////
	ShaderMgr* ShaderMgr::Get()
	{
		static TInstance<ShaderMgrDX> Ins;
		return Ins;
	}
};
#endif