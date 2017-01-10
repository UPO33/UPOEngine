#pragma once

#include "UGFXDevice.h"

namespace UPO
{
	namespace GlobalResources
	{

		UAPI void LoadGFXResources();
		UAPI void UnloadGFXResources();

		UAPI GFXDepthStencilState* GetDepthStencilState(const GFXDepthStencilStateDesc&);
		UAPI GFXRasterizerState* GetRasterizerState(const GFXRasterizerStateDesc&);
		UAPI GFXBlendState* GetBlendState(const GFXBlendStateDesc&);
		UAPI GFXSamplerState* GetSamplerState(const GFXSamplerStateDesc&);
		UAPI GFXInputLayout* GetInputLayout(const GFXInputLayoutDesc&);
		UAPI GFXTexture2D* GetDefaultTexture2D();
		UAPI GFXSamplerState* GetDefaultSampler();

	}
};