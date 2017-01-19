#pragma once

#include "UCommonEd.h"

#include "UPropertyBrowser.h"

namespace UPOEd
{
	//////////////////////////////////////////////////////////////////////////
	class AssetViewer;
	class AssetWindowBase;
	class AssetViewer_Default;

	//////////////////////////////////////////////////////////////////////////
	class AssetViewer : public QMainWindow
	{
		friend AssetWindowBase;

		static QList<AssetWindowBase*>	SOpenAssets;

	public:
		static void Tick();
		static void OpenAsset(AssetEntry* asset);
		//return the viewer if any
		static AssetWindowBase* GetViewerOfAsset(AssetEntry* entry);
		static AssetWindowBase* MakeCorrespondingView(const ClassInfo* assetClass, QWidget* parentWidget = nullptr);
	};

	//////////////////////////////////////////////////////////////////////////
	class AssetWindowBase : public QMainWindow
	{
		friend AssetViewer;

		Object* mRefHelper; // only is used as loader reference

	protected:
		TObjectPtr<Asset> mAttachedAsset;
	public:
		AssetWindowBase(QWidget* parent = nullptr);
		~AssetWindowBase();
		virtual void AttachAsset(Asset* asset);
		virtual void Tick() {}
		virtual void closeEvent(QCloseEvent *event) override;
	};

	//////////////////////////////////////////////////////////////////////////
	class AssetViewer_Default : public AssetWindowBase
	{
		PropertyBrowserWidget* mPropertyBrowser;
	public:
		AssetViewer_Default(QWidget* parent = nullptr);
		virtual void Tick() override;
		virtual void AttachAsset(Asset*) override;
		virtual void closeEvent(QCloseEvent *event) override;

	};


};