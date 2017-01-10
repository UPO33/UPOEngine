#include "UGlobalResources.h"
#include "../Misc/UApp.h"
#include "../Engine/UEngineBase.h"
#include "../Engine/UAssetSys.h"


namespace UPO
{
	namespace GlobalResources
	{
		//////////////////////////////////////////////////////////////////////////
		struct GFXResources
		{
			TArray<GFXDepthStencilState*>	mDepthStencils;
			TArray<GFXRasterizerState*>		mRasterizers;
			TArray<GFXBlendState*>			mBlendStates;
			TArray<GFXInputLayout*>			mInputLayouts;
			TArray<GFXSamplerState*>		mSamlers;

			GFXDevice*				mDevice = nullptr;

			GFXTexture2D*			mDefaultTexture2D = nullptr;
			GFXSamplerState*		mDefaultSampler = nullptr;

			GFXResources(GFXDevice* device)
			{
// 				UASSERT(IsRenderThread());

				mDevice = device;

				LoadDefaultTexture2D();

				//default smapler state
				{
					mDefaultSampler = Get(GFXSamplerStateDesc());
				}
			}
			~GFXResources()
			{
// 				UASSERT(IsRenderThread());

				for (auto iter : mDepthStencils) delete iter;
				for (auto iter : mRasterizers) delete iter;
				for (auto iter : mBlendStates) delete iter;
				for (auto iter : mInputLayouts) delete iter;

				mDepthStencils.RemoveAll();
				mRasterizers.RemoveAll();
				mBlendStates.RemoveAll();
				mInputLayouts.RemoveAll();

				if (mDefaultTexture2D) delete mDefaultTexture2D;
				mDefaultTexture2D = nullptr;
			}

			//////////////////////////////////////////////////////////////////////////
			void LoadDefaultTexture2D()
			{
				UASSERT(mDefaultTexture2D == nullptr);

				Buffer textureContent;
				bool fileRead = GAssetSys()->ReadFileFull("DefaultTexture2D.dds", textureContent);
				UASSERT(fileRead);


				mDefaultTexture2D = mDevice->CreateTexture2DFromMemory(textureContent);

				UASSERT(mDefaultTexture2D);
			}

			GFXDepthStencilState* Get(const GFXDepthStencilStateDesc& param)
			{
				UASSERT(IsRenderThread());

				for (GFXDepthStencilState* item : mDepthStencils)
				{
					if (item->GetDesc() == param)	//found?
						return item;
				}

				if(GFXDepthStencilState* newState = mDevice->CreateDepthStencilState(param))
				{
					mDepthStencils.Add(newState);
					return newState;
				};

				return nullptr;
			}
			//////////////////////////////////////////////////////////////////////////
			GFXRasterizerState* Get(const GFXRasterizerStateDesc& param)
			{
				UASSERT(IsRenderThread());

				for (GFXRasterizerState* item : mRasterizers)
				{
					if (item->GetDesc() == param)	//found?
						return item;
				}

				if (GFXRasterizerState* newState = mDevice->CreateRasterizerState(param))
				{
					mRasterizers.Add(newState);
					return newState;
				}
				return nullptr;
			}
			//////////////////////////////////////////////////////////////////////////
			GFXBlendState* Get(const GFXBlendStateDesc& param)
			{
				UASSERT(IsRenderThread());

				for (GFXBlendState* item : mBlendStates)
				{
					if (item->GetDesc() == param)	//found?
						return item;
				}

				if (GFXBlendState* newState = mDevice->CreateBlendState(param))
				{
					mBlendStates.Add(newState);
					return newState;
				}
				return nullptr;
			}
			//////////////////////////////////////////////////////////////////////////
			GFXSamplerState* Get(const GFXSamplerStateDesc& param)
			{
				UASSERT(IsRenderThread());

				for (GFXSamplerState* item : mSamlers)
				{
					if (item->GetDesc() == param)	//found?
						return item;
				}

				if (GFXSamplerState* newState = mDevice->CreateSamplerState(param))
				{
					mSamlers.Add(newState);
					return newState;
				}
				return nullptr;
			}
			//////////////////////////////////////////////////////////////////////////
			GFXInputLayout* Get(const GFXInputLayoutDesc& param)
			{
				UASSERT(IsRenderThread());

				for (GFXInputLayout* item : mInputLayouts)
				{
					if (item->GetDesc() == param)	//found?
						return item;
				}

				if (GFXInputLayout* newState = mDevice->CreateInputLayout(param))
				{
					mInputLayouts.Add(newState);
					return newState;
				}
				return nullptr;
			}
		};

		//////////////////////////////////////////////////////////////////////////
		GFXResources* gGFXResources = nullptr;


		//////////////////////////////////////////////////////////////////////////
		UAPI void LoadGFXResources()
		{
			UASSERT(gGFXResources == nullptr);
			gGFXResources = new GFXResources(gGFX);
		}
		//////////////////////////////////////////////////////////////////////////
		UAPI void UnloadGFXResources()
		{
			if (gGFXResources) delete gGFXResources;
			gGFXResources = nullptr;
		}
		//////////////////////////////////////////////////////////////////////////
		UAPI GFXDepthStencilState* GetDepthStencilState(const GFXDepthStencilStateDesc& param)
		{
			UASSERT(gGFXResources);
			return gGFXResources->Get(param);
		}
		UAPI GFXRasterizerState* GetRasterizerState(const GFXRasterizerStateDesc& param)
		{
			UASSERT(gGFXResources);
			return gGFXResources->Get(param);
		}
		UAPI GFXBlendState* GetBlendState(const GFXBlendStateDesc& param)
		{
			UASSERT(gGFXResources);
			return gGFXResources->Get(param);
		}
		UAPI GFXSamplerState* GetSamplerState(const GFXSamplerStateDesc& param)
		{
			UASSERT(gGFXResources);
			return gGFXResources->Get(param);
		}
		UAPI GFXInputLayout* GetInputLayout(const GFXInputLayoutDesc& param)
		{
			UASSERT(gGFXResources);
			return gGFXResources->Get(param);
		}
		UAPI GFXTexture2D* GetDefaultTexture2D()
		{
			UASSERT(gGFXResources);
			return gGFXResources->mDefaultTexture2D;
		}
		UAPI GFXSamplerState* GetDefaultSampler()
		{
			UASSERT(gGFXResources);
			return gGFXResources->mDefaultSampler;
		}

	};
	






};