
#include "UAssetViewer.h"

namespace UPOEd
{

	AssetViewer::AssetViewer(QWidget* parent /*= nullptr*/) : QMainWindow(parent)
	{
		auto toolbar = this->addToolBar("toolbar");

		QAction* saveAction = toolbar->addAction("Save");
		connect(saveAction, &QAction::triggered, this, [&](bool checked) {
			if (mAttachedAsset)
				mAttachedAsset->Save();
		});

		QAction* reloatAction = toolbar->addAction("Reload");
		connect(reloatAction, &QAction::triggered, [](bool checked) {
			ULOG_MESSAGE("TODO");
		});
	}

	AssetViewer* AssetViewer::Current = nullptr;

	void AssetViewer::OpenAsset(AssetEntry* assetEntry)
	{
		UASSERT(assetEntry);

		if (Current)
		{
			Current->close();
			delete Current;
			Current = nullptr;
		}

		assetEntry->LoadNow(nullptr);
		if (auto asset = assetEntry->GetInstance())
		{
			Current = MakeCorrespondingView(asset);
			Current->AttachAsset(asset);
			Current->show();
		}
		else
		{
			ULOG_ERROR("failed to load asset [%]", assetEntry->GetName());
		}
	}

	void AssetViewer::Close()
	{

	}

	AssetViewer* AssetViewer::MakeCorrespondingView(Asset* asset, QWidget* parentWidget)
	{
		return new AssetViewer_Default(parentWidget);
	}

	void AssetViewer_Default::Tick()
	{
		mPropertyBrowser->Tick();
	}

	void AssetViewer_Default::AttachAsset(Asset* asset)
	{
		AssetViewer::AttachAsset(asset);

		this->setWindowTitle(ToQString(asset->GetName()));

		mPropertyBrowser->AttachObject(asset);

		
	}

	void AssetViewer_Default::closeEvent(QCloseEvent *event)
	{
		mPropertyBrowser->AttachObject(nullptr);

		AssetViewer::closeEvent(event);

		event->accept();
	}

};