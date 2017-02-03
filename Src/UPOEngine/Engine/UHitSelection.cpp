#include "UHitSelection.h"
#include "../Core/UFreeListAllocator.h"
#include "../GFXCore/UGFXDevice.h"
#include "UEntity.h"

namespace UPO
{
	
	FreeListAllocator gHitProxyAllocator { (int)HitProxyBase::MaxSize, 128 };

	static const unsigned ReadTextureSize = 4;

	void* HitProxyBase::operator new(size_t size)
	{
		UASSERT(size < HitProxyBase::MaxSize);
		return gHitProxyAllocator.Alloc();
	}

	void HitProxyBase::operator delete(void* memory)
	{
		gHitProxyAllocator.Free(memory);
	}

	HitSelectionCanvas::HitSelectionCanvas(Vec2I size)
	{
		mSize = size;
		mTakenHit = nullptr;

		{
			GFXTexture2DDesc desc;
			desc.mFlag = ETextureFlag::ECPURead;
			desc.mFormat = EPixelFormat::R32_UINT;
			desc.mWidth = ReadTextureSize;
			desc.mHeight = ReadTextureSize;
			mReadTexture = gGFX->CreateTexture2D(desc);
			UASSERT(mReadTexture);
		}
	}

	HitSelectionCanvas::~HitSelectionCanvas()
	{

	}

	HitProxyBase* HitSelectionCanvas::GetTakenHit()
	{
		return nullptr;
	}

	void HitSelectionCanvas::SetMousePosition(Vec2I mousePosition)
	{

	}

	void HitSelectionCanvas::Resize(Vec2I newSize)
	{
		mSize = newSize;
	}

	unsigned HitSelectionCanvas::RegisterProxy(HitProxyBase* hp)
	{
		return mProxies.Add(hp);
	}

	void HitSelectionCanvas::ClearProxeis()
	{
		for (HitProxyBase* item : mProxies)
			SafeDelete(item);
		
		mProxies.RemoveAll();
	}

	struct IDCatchResult
	{
		unsigned	mIDs[ReadTextureSize * ReadTextureSize];
		unsigned	mNumIDs = 0;

	};

	void UGetIDS(void* mappedIDTexture, Vec2I leftTop, Vec2I rightBottom, unsigned* outIDs)
	{

	}
	void HitSelectionCanvas::CatchAt(Vec2 pos, GFXTexture2D* idBuffer)
	{
		UASSERT(IsRenderThread());

		Vec2I leftTop = (Vec2I)Clamp(pos - ReadTextureSize / 2, Vec2::ZERO, Vec2(mSize));
		Vec2I rightBottom = (Vec2I)Clamp(pos + ReadTextureSize / 2, Vec2::ZERO, Vec2(mSize));
		
		gGFX->CopySubresourceRegion(mReadTexture, 0, Vec2I(0), idBuffer, 0, leftTop, rightBottom);


		unsigned bytesPerRow = 0;
		byte* mapped = (byte*)gGFX->Map(idBuffer, EMapFlag::ERead, 0, bytesPerRow);
		unsigned id = *((unsigned*)mapped);
		

		IDCatchResult result;

		for (int i = leftTop.mX; i < rightBottom.mX; i++)
		{
			for (int j = leftTop.mY; j < rightBottom.mY; j++)
			{
				byte* dt = mapped + bytesPerRow * j + sizeof(unsigned) * i;
				result.mIDs[result.mNumIDs++] = *(unsigned *)(dt);
			}
		}

		gGFX->Unmap(idBuffer, 0);


		HitProxyBase* takenHit = nullptr;
		for (unsigned i = 0; i < result.mNumIDs; i++)
		{
			int priority = INT_MIN;
			unsigned hid = result.mIDs[i];
			if (mProxies.IsIndexValid(hid))
			{
				if (mProxies[hid] && mProxies[hid]->mPriority > priority)
				{
					takenHit = mProxies[hid];
					priority = mProxies[hid]->mPriority;
				}
			}
		}

		mTakenHit = takenHit;
		if (mTakenHit)
		{
			if (HPObject* hpObj = mTakenHit->Cast<HPObject>())
			{
				ULOG_MESSAGE("hpo");
				if (auto ent = UCast<Entity>(hpObj->mObject))
				{
					ULOG_MESSAGE("%", ent->GetName());
				}
			}
		}
	}

};