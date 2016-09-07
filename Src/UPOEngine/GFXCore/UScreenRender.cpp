#include "UScreenRender.h"

namespace UPO
{
	
	//////////////////////////////////////////////////////////////////////////
	ScreenRender* ScreenRender::Get()
	{
		static TInstance<ScreenRender> Ins;
		return Ins;
	}

// 	ID3D10Blob* CompileShader(const char* filename, const char* function, const char* profile = "vs_5_0")
// 	{
// 		ID3D10Blob* shader = nullptr;
// 		ID3D10Blob* error = nullptr;
// 		if (FAILED(D3DX11CompileFromFileA(filename, nullptr, nullptr, function, profile, 0, 0,
// 			nullptr, &shader, &error, nullptr)))
// 		{
// 			if (error)
// 			{
// 				ULOG_ERROR("Compiling shader failed %s %s : %s", filename, function, ((char*)error->GetBufferPointer()));
// 				return nullptr;
// 			}
// 			else
// 			{
// 				ULOG_ERROR("Mising Shader File");
// 				return nullptr;
// 			}
// 		}
// 		return shader;
// 	}
	void Test(ID3D11Device* device)
	{

		// Create a vertex buffer for a triangle.

		Vec2 triangleVertices[] =
		{
			Vec2(-0.5f, -0.5f),
			Vec2(0.0f, 0.5f),
			Vec2(0.5f, -0.5f),
		};

		D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
		vertexBufferDesc.ByteWidth = sizeof(Vec2)* 3;
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		ID3D11Buffer* vb;
		D3D11_SUBRESOURCE_DATA vertexBufferData;
		vertexBufferData.pSysMem = triangleVertices;
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;

		if (FAILED(device->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			&vb)))
		{
			ULOG_ERROR("sddddddddddddddddddd");
		}
	}

	ID3D11ShaderResourceView* gTex0;
	ID3D11InputLayout* gInputLayout;
	ID3D11VertexShader* gVShader;
	ID3D11PixelShader* gPShader;

	void ScreenRender::Init(GFXDevice* gfx)
	{
		mGFX = (GFXDeviceDX*)gfx;
		mVertexShader = mGFX->LoadShader("Canvas.hlsl", "VSMain", EShaderType::EST_VERTEX);
		mPixelShader = mGFX->LoadShader("Canvas.hlsl", "PSMain", EShaderType::EST_PIXEL);

		GFXVertexElement_Desc vElements[] =
		{
			GFXVertexElement_Desc("POSITION", EPixelFormat::EPT_R32G32_FLOAT),
			GFXVertexElement_Desc("UV", EPixelFormat::EPT_R32G32_FLOAT)
		};
		mLayout = mGFX->CreateInputLayout(vElements, ARRAYSIZE(vElements), mVertexShader);

		GFXVertexBuffer_Desc vbDesc;
		vbDesc.mUsage = EResourceUsage::EBU_DYNAMIC;
		vbDesc.mSize = sizeof(VertexType) * 32;
		mQuadBuffer = gfx->CreateVertexBuffer(vbDesc);

		GFXDepthStencilState_Desc dsDesc;
		dsDesc.mDepthEnable = false;
		dsDesc.mStencilEnable = false;
		mDepthStencilState = mGFX->CreateDepthStencilState(dsDesc);

		GFXSamplerState_Desc ssDesc;
		ssDesc.mAddressU = ETextureAddress::ETA_CLAMP;
		ssDesc.mAddressV = ETextureAddress::ETA_CLAMP;
		ssDesc.mFilter = ETextureFilter::ETF_MIN_MAG_MIP_LINEAR;
		mSampler = mGFX->CreateSamplerState(ssDesc);

		if (FAILED(D3DX11CreateShaderResourceViewFromFileW(mGFX->mDevice, L"building1.dds", NULL, NULL, &gTex0, NULL)))
		{
			ULOG_ERROR("failed to load texture");
		}
		
		D3D11_BLEND_DESC bd;
		
		mGFX->mDevice->CreateBlendState();

		GFXRasterizerState_Desc rs;
		rs.mDepthClipEnable = false;
		rs.mCullMode = ECM_NONE;
		mRaster = mGFX->CreateRasterizerState(rs);

		DirectX::TexMetadata meta;
		DirectX::ScratchImage img;
		if (SUCCEEDED(DirectX::LoadFromWICFile(L"thumb.png", 0, &meta, img)))
		{
		}


		unsigned pixels[32][32];
		for (size_t i = 0; i < 32; i++)
		{
			for (size_t j = 0; j < 32; j++)
			{
				pixels[i][j] = rand();
			}
		}
// 		GFXTexture2D_Desc tDesc;
// 		tDesc.mWidth = 32;
// 		tDesc.mHeight = 32;
// 		tDesc.mInitialData = pixels;
// 		tDesc.mFormat = EPixelFormat::EPT_R8G8B8A8_UINT;
// 		mTexture = mGFX->CreateTexture2D(tDesc);
	}

	//////////////////////////////////////////////////////////////////////////
	void ScreenRender::DrawTexture(GFXTexture2D* texture, Vec2 pos, Vec2 size, Vec2 uvTopLeft, Vec2 uvRightBottom)
	{
		VertexType* vertices = (VertexType*)mQuadBuffer->Map(EMapFlag::EMF_WRITE_DISCARD);
		//0,0
		vertices[0].mPosition = pos;
		vertices[0].mUV = uvTopLeft;
		//1,0
		vertices[1].mPosition.mX = pos.mX + size.mX;
		vertices[1].mPosition.mY = pos.mY;
		vertices[1].mUV.mX = uvRightBottom.mX;
		vertices[1].mUV.mY = uvTopLeft.mY;
		//0,1
		vertices[2].mPosition.mX = pos.mX;
		vertices[2].mPosition.mY = pos.mY + size.mY;
		vertices[2].mUV.mX = uvTopLeft.mX;
		vertices[2].mUV.mY = uvRightBottom.mY;

		//1,0
		vertices[3].mPosition.mX = pos.mX + size.mX;
		vertices[3].mPosition.mY = pos.mY;
		vertices[3].mUV.mX = uvRightBottom.mX;
		vertices[3].mUV.mY = uvTopLeft.mY;
		//1,1
		vertices[4].mPosition = pos + size;
		vertices[4].mUV = uvRightBottom;
		//0,1
		vertices[5].mPosition.mX = pos.mX;
		vertices[5].mPosition.mY = pos.mY + size.mY;
		vertices[5].mUV.mX = uvTopLeft.mX;
		vertices[5].mUV.mY = uvRightBottom.mY;

		mQuadBuffer->Unmap();

		mGFX->BinInputLayout(mLayout);
		mGFX->SetRasterizer(mRaster);
		mGFX->SetDepthStencilState(mDepthStencilState);
		mGFX->SetPrimitiveTopology(EPrimitiveTopology::EPT_TRIANGLELIST);
		mGFX->BinVertexBuffer(mQuadBuffer, sizeof(VertexType), 0);
		mGFX->BindShaders(mVertexShader, mPixelShader);
		mGFX->mDeviceContext->PSSetShaderResources(0, 1, &gTex0);
		mGFX->BindSamplerState(mSampler, 0, EST_PIXEL);
		mGFX->Draw(6, 0);
	}

	void ScreenRender::Frame()
	{
// 		DrawTexture(nullptr, Vec2(0.1), Vec2(0.5));
		DrawTexture(nullptr, Vec2(0.1, 0.7), Vec2(0.5));
		DrawTexture(nullptr, Vec2(0.1, 0.1), Vec2(0.2));
	}

};