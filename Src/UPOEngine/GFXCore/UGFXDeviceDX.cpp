#pragma once

#include "UGFXDeviceDX.h"



namespace UPO
{
	UAPI GFXDeviceDX* gGFXDX = nullptr;

	//////////////////////////////////////////////////////////////////////////
	HRESULT __stdcall ShaderIncludeer::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
	{
		char fullFilename[256];
		ShaderMgr::FixShaderPath(pFileName, fullFilename);
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
	HRESULT __stdcall ShaderIncludeer::Close(LPCVOID pData)
	{
		if(pData) MemFree((void*)pData);
		return S_OK;
	}

};