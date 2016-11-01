#pragma once

#include "UCommonEd.h"


class Ui_AssetBrowser;
class Ui_AssetBrowserAssetItem;

namespace UPOEd
{

	class AssetBrowserItem : public QListWidgetItem
	{
	public:
		AssetEntry* mAsset;
		AssetBrowserItem(AssetEntry* asset, QListWidget* list);
		void AttachAsset(AssetEntry* asset);
	};

	//////////////////////////////////////////////////////////////////////////
	class AssetBrowserItem2 : public QTreeWidgetItem
	{
	public:
		Name mAssetName;	//myassets/myAsset.asset
		bool mIsFolder;
		QString mAbsoluteFileName; 
		QString mFileName;	//myAsset.asset
		AssetEntry* mAsset = nullptr;

		AssetBrowserItem2(const QString& assetName, const QString& absoluteFileName, const QString& fileName, bool isFolder, AssetBrowserItem2* parent)
			: QTreeWidgetItem(parent)
		{
			mAssetName = ToName(assetName);
			mAbsoluteFileName = absoluteFileName;
			mFileName = fileName;
			mIsFolder = isFolder;

			setText(0, fileName);
			setFlags(flags() | Qt::ItemFlag::ItemIsEditable);

			InitIcon();
		}
		void SetAsset(AssetEntry* asset)
		{
			mAsset = asset;
			InitIcon();
		}
		void InitIcon();
	};
	//////////////////////////////////////////////////////////////////////////
	class AssetBrowserWidget2 : public QWidget
	{
		Ui_AssetBrowser* ui;
	public:
		AssetBrowserWidget2(QWidget* parent = nullptr);

		void InitFolders();
		void AddDirContents(const QString& dir, AssetBrowserItem2* parent, const QString& prefix);
		bool FilterCheck(const QString& str);
	};

	//////////////////////////////////////////////////////////////////////////
	class AssetBrowserWidget : public QWidget
	{
		QLineEdit* mFilter;
		QListWidget* mAssets;

		QAction* mActionImport;
		QList<QAction*> mActionsCreateAsset;

		bool mEditingIntetAssetCreation = false;
		ClassInfo* mPendingCreationAssetClass = nullptr;
		AssetBrowserItem* mPendingCreationAssetItem = nullptr;

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
		void EVImportAsset(bool);
		void EVCreateAsset(bool);
		void EVItemEditingFinished(const QString& newText);

		virtual void dragEnterEvent(QDragEnterEvent *event) override;
		virtual void dropEvent(QDropEvent *event) override;

	};
	//////////////////////////////////////////////////////////////////////////
	class AssetBrowserDW : public QDockWidget
	{
	public:
		

		AssetBrowserDW(QWidget* parent = nullptr);

		void Tick();
	};



};