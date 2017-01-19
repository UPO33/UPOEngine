#pragma once

#include "UAssetViewer.h"
#include "UMainViewport.h"
#include "../UPOEngine/Engine/UCanvas.h"


namespace UPOEd
{
	//////////////////////////////////////////////////////////////////////////
	class AssetViewer_Texture2D : public AssetWindowBase
	{
		World*	mWorld = nullptr;
		MainViewport*	mViewport = nullptr;
		PropertyBrowserDW* mPropertyBrowser = nullptr;

	public:


		AssetViewer_Texture2D(QWidget* parent) : AssetWindowBase(parent)
		{

			WorldInitParam wip;
			wip.mWorldType = EWorldType::EEditor;
			mWorld = GEngine()->CreateWorld(wip);

			mViewport = new MainViewport(this);
			mViewport->setMinimumSize(100, 100);
			setCentralWidget(mViewport);

			GameWindowCreationParam gwcp;
			gwcp.mCreateCanvas = true;
			mViewport->mOptions.mShowFPS = false;
			mViewport->mOptions.mRenderGrid = true;
			mViewport->InitAndReg(gwcp);
			mViewport->SetWorld(mWorld);

			mPropertyBrowser = new PropertyBrowserDW(this);
			this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, mPropertyBrowser);
		}
		~AssetViewer_Texture2D()
		{
			if (mViewport) mViewport->ReleaseAndUnreg();
			mViewport = nullptr;
			GEngine()->DeleteWorld(mWorld);
		}
		void AttachAsset(Asset* asset) override
		{
			AssetWindowBase::AttachAsset(asset);
			if (mPropertyBrowser)
				mPropertyBrowser->AttachObject(asset);
		}
		void Tick() override
		{
			if (!mAttachedAsset) AttachAsset(nullptr);
				
			if (mPropertyBrowser) mPropertyBrowser->Tick();

			if (mViewport)
			{
				CanvasTextureItem item;
				item.mTexture = mAttachedAsset->Cast<ATexture2D>();
				item.mPosition = Vec2(0, 0);
				item.mSize.mX = mViewport->width();
				item.mSize.mY = mViewport->height();
				mViewport->mCanvas->Draw(item);
			}
		}
	};
};