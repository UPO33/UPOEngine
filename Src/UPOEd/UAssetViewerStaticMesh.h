#pragma once

#include "UAssetViewer.h"

#include "../UPOEngine/Engine/UEntityStaticMesh.h"
#include "../UPOEngine/Engine/UEntityCamera.h"
#include "../UPOEngine/Engine/UEntityFreeCamera.h"

#include "../UPOEngine/GFX//UPrimitiveBatch.h"

namespace UPOEd
{
	class AssetViewer_StaticMesh : public AssetWindowBase
	{
		World*					mWorld = nullptr;
		EntityStaticMesh*		mEntityInWorld = nullptr;
		RenderViewportWidget*			mViewport = nullptr;
		PropertyBrowserDW*		mPropertyBrowser = nullptr;

	public:

		void Test()
		{

		}
		AssetViewer_StaticMesh(QWidget* parent) : AssetWindowBase(parent)
		{
			InitWorld();

			mViewport = new RenderViewportWidget(this);
			mViewport->setMinimumSize(300, 300);
			setCentralWidget(mViewport);

			GameWindowCreationParam gwcp;
			gwcp.mCreateCanvas = true;
			mViewport->mOptions.mShowFPS = false;
			mViewport->mOptions.mRenderGrid = true;
			mViewport->InitAndReg(gwcp);
			mViewport->SetWorld(mWorld);


			mPropertyBrowser = new PropertyBrowserDW(this);

			mPropertyBrowser->GetWidget()->mOnMetaAfterPropertyChange.BindLambda([this](const PropertyInfo* prp)
			{
				mEntityInWorld->SetMaterial(mAttachedAsset->Cast<AStaticMesh>()->GetDefaultMaterial());
				mEntityInWorld->MetaAfterPropertyChange(prp);
			});

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
			{
				AStaticMesh* attachedAsset = asset ? asset->Cast<AStaticMesh>() : nullptr;

				mEntityInWorld->SetMesh(attachedAsset);
				FocusOnEntity();
			}

		}
		void Tick() override
		{
			if (mPropertyBrowser) mPropertyBrowser->Tick();
			if (mViewport) mViewport->Tick();

			if (auto pb = mWorld->GetPrimitiveBatch())
			{
				pb->DrawWireMesh(mAttachedAsset ? mAttachedAsset->Cast<AStaticMesh>() : nullptr, Transform::IDENTITY, Color32::RED, 0);
			}
		}
		
		void InitWorld()
		{
			WorldInitParam wip;
			wip.mWorldType = EWorldType::EEditor;
			mWorld = GEngine()->CreateWorld(wip);
			mEntityInWorld = mWorld->CreateEntity<EntityStaticMesh>(nullptr);
			mEntityInWorld->SetMesh(nullptr);
		}
		void FocusOnEntity()
		{
			AStaticMesh* attachedMesh = mEntityInWorld->GetMesh();

			if (attachedMesh)
			{
				mWorld->GetFreeCamera()->Cast<EntityFreeCamera>()->FocusAt(attachedMesh->GetBound());
			}
		}
		void OnAfterPropertyChanged( PropertyInfo* prp)
		{
			mEntityInWorld->MetaAfterPropertyChange(nullptr);
		}
	};

};