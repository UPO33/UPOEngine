#pragma once

#include "UCommonEd.h"

#include "UPropertyBrowser.h"

namespace UPOEd
{
	class AssetViewer : public QMainWindow
	{
		Asset* mAttachedAsset = nullptr;
	public:
		AssetViewer(QWidget* parent = nullptr);
		
		virtual void AttachAsset(Asset* asset) { mAttachedAsset = asset; }
		virtual void Tick() {}

		static AssetViewer* Current;

		static void OpenAsset(AssetEntry* asset);
		static void Close();

		static AssetViewer* MakeCorrespondingView(Asset* asset, QWidget* parentWidget = nullptr);
	};

	//////////////////////////////////////////////////////////////////////////
	class AssetViewer_Default : public AssetViewer
	{
		PropertyBrowserWidget* mPropertyBrowser;
	public:
		AssetViewer_Default(QWidget* parent = nullptr) : AssetViewer(parent)
		{
			mPropertyBrowser = new PropertyBrowserWidget(this);
			setCentralWidget(mPropertyBrowser);
		}
		virtual void Tick() override;
		virtual void AttachAsset(Asset*) override;
		virtual void closeEvent(QCloseEvent *event) override;

	};
};