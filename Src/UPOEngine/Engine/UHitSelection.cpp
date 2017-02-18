#include "UHitSelection.h"
#include "../Core/UFreeListAllocator.h"
#include "../GFXCore/UGFXDevice.h"
#include "UEntity.h"

namespace UPO
{
	
	FreeListAllocator gHitProxyAllocator { (int)HPBase::MaxSize, 128 };

	static const unsigned ReadTextureSize = 4;

	TinyLock	gLockHitProxyAllocator;

	void* HPBase::operator new(size_t size)
	{
		UASSERT(size < HPBase::MaxSize);
		USCOPE_LOCK(gLockHitProxyAllocator);
		return gHitProxyAllocator.Alloc();
	}

	void HPBase::operator delete(void* memory)
	{
		USCOPE_LOCK(gLockHitProxyAllocator);
		gHitProxyAllocator.Free(memory);
	}

	HitSelectionCanvas::HitSelectionCanvas(Vec2I size)
	{
		mSize = size;
		mTakenHit = nullptr;
		mProxiesIndexGTWrite = 0;

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

	HPBaseRef HitSelectionCanvas::GetTakenHit()
	{
		return mTakenHit;
	}

	void HitSelectionCanvas::SetMousePosition(Vec2I mousePosition)
	{

	}

	void HitSelectionCanvas::Resize(Vec2I newSize)
	{
		mSize = newSize;
	}

	unsigned HitSelectionCanvas::RegisterRTProxy(HPBaseRef hp)
	{
		if (hp)
			return mRTProxies.Add(hp);
		else
			return 0;
	}

	unsigned HitSelectionCanvas::RegisterProxy(HPBaseRef hp)
	{
		USCOPE_LOCK(mSwapLock);
		if (hp)
			return mProxies[mProxiesIndexGTWrite].Add(hp) + MaxRTProxy;
		else
			return MaxRTProxy;
	}


	void HitSelectionCanvas::ClearRTProxeis()
	{
// 		for (HPBase* item : mRTProxies)
// 			SafeDelete(item);
		

		//begin proxies
		USCOPE_LOCK(mSwapLock);
		mProxies[mProxiesIndexGTWrite ^ 1].RemoveAll();
		mProxies[mProxiesIndexGTWrite ^ 1].Add(nullptr);

		mProxiesIndexGTWrite ^= 1;

		mRTProxies.RemoveAll();
		mRTProxies.Add(nullptr);
	}
	void HitSelectionCanvas::ClearProxeis()
	{
		ClearRTProxeis();
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


		HPBaseRef takenHit = nullptr;
		for (unsigned i = 0; i < result.mNumIDs; i++)
		{
			int priority = INT_MIN;
			unsigned hid = result.mIDs[i];

			if(hid == 0) continue;

			TArray<HPBaseRef>& hitProxies = mRTProxies;
			if (hid >= MaxRTProxy)
			{
				hitProxies = mProxies[mProxiesIndexGTWrite ^ 1];
				hid -= MaxRTProxy;
			}

			if (hitProxies.IsIndexValid(hid))
			{
				if (hitProxies[hid] && hitProxies[hid]->mPriority > priority)
				{
					takenHit = hitProxies[hid];
					priority = hitProxies[hid]->mPriority;
				}
			}
		}

		mTakenHit = takenHit;
	}

};