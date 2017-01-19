#pragma once

#include "UAssetViewer.h"

#include "../UPOEngine/Engine/UEntityStaticMesh.h"

namespace UPOEd
{
	class AssetViewer_StaticMesh : public AssetWindowBase
	{
		World*					mWorld = nullptr;
		EntityStaticMesh*		mEntityInWorld = nullptr;
		MainViewport*			mViewport = nullptr;
		PropertyBrowserDW*		mPropertyBrowser = nullptr;

	public:

		
		AssetViewer_StaticMesh(QWidget* parent) : AssetWindowBase(parent)
		{
			InitWorld();

			mViewport = new MainViewport(this);
			mViewport->setMinimumSize(300, 300);
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
		~AssetViewer_StaticMesh()
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

			if (mEntityInWorld)
				mEntityInWorld->SetMesh(asset ? asset->Cast<AStaticMesh>() : nullptr);
		}
		void Tick() override
		{
			if (mPropertyBrowser) mPropertyBrowser->Tick();
			if (mViewport) mViewport->Tick();

			if (auto pb = mWorld->GetPrimitiveBatch())
			{

			}
		}
		void InitWorld()
		{
			WorldInitParam wip;
			wip.mWorldType = EWorldType::EEditor;
			mWorld = GEngine()->CreateWorld(wip);
			mEntityInWorld = mWorld->CreateEntity<EntityStaticMesh>(nullptr);
			mEntityInWorld->SetMesh(mAttachedAsset->Cast<AStaticMesh>());

		}
	};

};