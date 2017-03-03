
#include "UAssetViewer.h"
#include "UMainWindow.h"
#include "UAssetViewerTexture.h"
#include "UAssetViewerStaticMesh.h"

namespace UPOEd
{
	//asset windows currently open
	QList<AssetWindowBase*> AssetViewer::SOpenAssets;

	void AssetViewer::Tick()
	{
		for (AssetWindowBase* aw : SOpenAssets)
			aw->Tick();
	}





	void AssetViewer::OpenAsset(AssetEntry* assetEntry)
	{
		UASSERT(assetEntry);
		
		//is already open ? set focus
		if (AssetWindowBase* av = GetViewerOfAsset(assetEntry))
		{
			av->activateWindow();
			return;
		}

		AssetWindowBase* av = MakeCorrespondingView(assetEntry->GetClassInfo(), gMainWindow);
		assetEntry->LoadNow(av->mRefHelper);
		if (auto asset = assetEntry->GetInstance())
		{
			av->AttachAsset(asset);
			av->show();
		}
		else
		{
			delete av;
			ULOG_ERROR("failed to load asset [%]", assetEntry->GetName());
		}
	}



	AssetWindowBase* AssetViewer::GetViewerOfAsset(AssetEntry* assetEntry)
	{
		if (!assetEntry) return nullptr;

		if (Asset* ins = assetEntry->GetInstance())
		{

			for (AssetWindowBase* av : SOpenAssets)
			{
				if (av->mAttachedAsset.Get() == ins)
				{
					return av;
				}
			}
		}

		return nullptr;
	}

	AssetWindowBase* AssetViewer::MakeCorrespondingView(const ClassInfo* assetClass, QWidget* parentWidget)
	{
		if (assetClass->IsBaseOf<ATexture2D>())
			return new AssetViewer_Texture2D(parentWidget);
		if (assetClass->IsBaseOf<AStaticMesh>())
			return new AssetViewer_StaticMesh(parentWidget);

		return new AssetViewer_Default(parentWidget);
	}



	AssetViewer_Default::AssetViewer_Default(QWidget* parent /*= nullptr*/) : AssetWindowBase(parent)
	{
		mPropertyBrowser = new PropertyBrowserWidget(this);
		setCentralWidget(mPropertyBrowser);
	}

	void AssetViewer_Default::Tick()
	{
		mPropertyBrowser->Tick();
	}

	void AssetViewer_Default::AttachAsset(Asset* asset)
	{
		AssetWindowBase::AttachAsset(asset);
		mPropertyBrowser->AttachObject(asset);
	}

	void AssetViewer_Default::closeEvent(QCloseEvent *event)
	{
		mPropertyBrowser->AttachObject(nullptr);

		AssetWindowBase::closeEvent(event);

		event->accept();
	}

	AssetWindowBase::AssetWindowBase(QWidget* parent /*= nullptr*/) : QMainWindow(parent/*, commented for dbg Qt::WindowStaysOnTopHint*/)
	{
		setAttribute(Qt::WA_DeleteOnClose);
		mRefHelper = NewObject<Object>();
		AssetViewer::SOpenAssets << this;

		auto toolbar = this->addToolBar("toolbar");
		
		QAction* saveAction = toolbar->addAction("Save");
		connect(saveAction, &QAction::triggered, this, [&](bool checked) {
			if (mAttachedAsset)
				mAttachedAsset->Save();
		});

		{
			QSettings settingIns;
			QVariant variantRect = settingIns.value(QString(this->metaObject()->className()) + QString("_Geom"));
			if(variantRect.isValid())
				this->restoreGeometry(variantRect.toByteArray());
		}
	}

	AssetWindowBase::~AssetWindowBase()
	{
		AssetViewer::SOpenAssets.removeOne(this);
		if (mRefHelper) DeleteObject(mRefHelper);
		mRefHelper = nullptr;
	}

	void AssetWindowBase::AttachAsset(Asset* asset)
	{
		mAttachedAsset = asset;
		if(asset)
			setWindowTitle(ToQString(asset->GetName()));
	}

	void AssetWindowBase::closeEvent(QCloseEvent *event)
	{
		{
			QSettings settingIns;
			settingIns.setValue(QString(this->metaObject()->className()) + QString("_Geom"), QVariant(this->saveGeometry()));
		}

		AssetViewer::SOpenAssets.removeOne(this);
		if (mRefHelper) DeleteObject(mRefHelper);
		mRefHelper = nullptr;
		event->accept();
	}

};

