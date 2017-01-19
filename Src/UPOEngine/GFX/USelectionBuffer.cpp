#include "USelectionBuffer.h"


namespace UPO
{

	SelectionBuffer::SelectionBuffer(Vec2I size)
	{
		mIDBuffer = nullptr;
		mIDBufferRTV = nullptr;
		mIDOut = nullptr;

		{
			GFXTexture2DDesc desc;
			desc.mFlag = ETextureFlag::ERenderTargatable | ETextureFlag::EShaderResourceView;
			desc.mWidth = size.mX;
			desc.mHeight = size.mY;
			desc.mFormat = EPixelFormat::R32_UINT;
			mIDBuffer = gGFX->CreateTexture2D(desc);
			UASSERT(mIDBuffer);
			gGFX->ClearRenderTarget(mIDBuffer->GetRenderTargetView(), Color(3.5f));
		}
		{
			GFXTexture2DDesc desc;
			desc.mFlag = ETextureFlag::ECPURead;
			desc.mFormat = EPixelFormat::R32_UINT;
			desc.mWidth = 1;
			desc.mHeight = 1;
			mIDOut = gGFX->CreateTexture2D(desc);
			UASSERT(mIDBuffer);
		}
	}

	SelectionBuffer::~SelectionBuffer()
	{
		SafeDelete(mIDBufferRTV);
		SafeDelete(mIDBuffer);
	}

	void SelectionBuffer::Catch()
	{
		gGFX->CopySubresourceRegion(mIDOut, 0, 0, 0, mIDBuffer, 0, mCursorPosition.mX, 1, mCursorPosition.mY, 1);
		unsigned bytesPerRow = 0;
		void* data = gGFX->Map(mIDOut, EMapFlag::ERead, 0, bytesPerRow);
		gGFX->Unmap(mIDOut, 0);
		unsigned theID = *((unsigned*)data);
		ULOG_MESSAGE("%  %", bytesPerRow, theID);
	}

	void SelectionBuffer::SetCurdorPosition(Vec2I position)
	{
		mCursorPosition = position;
	}

	void SelectionBuffer::Resize(Vec2I newSize)
	{
	}

};