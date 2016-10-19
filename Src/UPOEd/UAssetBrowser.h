#pragma once

#include "UCommonEd.h"



namespace UPOEd
{
	class AssetBrowserItem : public QListWidgetItem
	{
	public:
		AssetEntry* mAsset;
		AssetBrowserItem(AssetEntry* asset, QListWidget* list);
	};

	class AssetBrowserWidget : public QWidget
	{
		QLineEdit* mFilter;
		QListWidget* mAssets;

		QAction* mActionImport;
		QList<QAction*> mActionsCreateAsset;

	public:
		AssetBrowserWidget(QWidget* parent = nullptr);
		
		void InitActions();

		void FillList(const QString& filter);

		void Tick();

		void EVContextMenuRequested(const QPoint&);
		void OpenAsset(QListWidgetItem* item);
		void DeleteAsset(QListWidgetItem* item);
		Name GetUniqueAssetName(ClassInfo* assetClass) const;
		AssetEntry* CreateAssetDefault(ClassInfo* assetClass, Name name);
	};

	class AssetBrowserDW : public QDockWidget
	{
	public:
		

		AssetBrowserDW(QWidget* parent = nullptr);

		void Tick();
	};



};