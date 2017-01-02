#include "UAssetBrowser.h"
#include "UAssetViewer.h"
#include "UAssetConverter.h"
#include "ui_asset_browser.h"

#include <QDirIterator>
#include "../UPOEngine/Engine/UEngine.h"


namespace UPOEd
{
	//////////////////////////////////////////////////////////////////////////
	AssetBrowserDW::AssetBrowserDW(QWidget* parent /*= nullptr*/) : QDockWidget(parent)
	{
		this->setWindowTitle("Asset Browser");
		this->setMinimumSize(QSize(300, 300));
		this->setWidget(new AssetBrowserWidget);
	}
	//////////////////////////////////////////////////////////////////////////
	void AssetBrowserDW::Tick()
	{
		//((AssetBrowserWidget*)this->widget())->Tick();
	}
	//////////////////////////////////////////////////////////////////////////
#if 0 
	void AssetBrowserWidget::EVItemEditingFinished(const QString& newText)
	{
		if (mEditingIntetAssetCreation) // we were setting the name of new asset?
		{
			mEditingIntetAssetCreation = false;

			UASSERT(mPendingCreationAssetClass && mPendingCreationAssetItem);
			Name nameOfNewAsset = ToName(newText);
			if (!AssetSys::Get()->FindAsset(nameOfNewAsset))
			{
				AssetEntry* newAsset = AssetSys::Get()->CreateAsset(mPendingCreationAssetClass, nameOfNewAsset);
				if (newAsset)
				{
					mPendingCreationAssetItem->AttachAsset(newAsset);
					mPendingCreationAssetClass = nullptr;
					ULOG_SUCCESS("asset [%] created", nameOfNewAsset);
					return;
				}
			}

			ULOG_ERROR("failed to create new asset [%]", nameOfNewAsset);
			delete mPendingCreationAssetItem;
			mPendingCreationAssetItem = nullptr;
			mPendingCreationAssetClass = nullptr;
		}
	}
	AssetBrowserWidget::AssetBrowserWidget(QWidget* parent /*= nullptr*/) : QWidget(parent)
	{
		this->setAcceptDrops(true);
		setLayout(new QVBoxLayout);
		

		mFilter = new QLineEdit(this);
		mFilter->setClearButtonEnabled(true);
		layout()->addWidget(mFilter);

		mAssets = new QListWidget(this);
		layout()->addWidget(mAssets);
		mAssets->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
// 		connect(mAssets, &QListWidget::currentTextChanged, this, [&](const QString& str)
// 		{
// 			ULOG_MESSAGE("%s", ToString(str).CStr());
// 		});
		

		connect(mAssets->itemDelegate(), &QAbstractItemDelegate::commitData, this, [&](QWidget* widget) {

			QString strNewText = reinterpret_cast<QLineEdit*>(widget)->text();
			EVItemEditingFinished(strNewText);
		});

		FillList(QString());

		connect(mAssets, &QListWidget::itemDoubleClicked, this, [&](QListWidgetItem* item) {
			OpenAsset(item);
		});

		connect(mFilter, &QLineEdit::textChanged, this, [&](const QString& str) {
			FillList(str);
		});


		mAssets->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
		connect(mAssets, &QListWidget::customContextMenuRequested, this, &AssetBrowserWidget::EVContextMenuRequested);
		
		InitActions();

	}
	//////////////////////////////////////////////////////////////////////////
	void AssetBrowserWidget::InitActions()
	{
		mActionImport = new QAction("Import", this);
		mActionImport->setToolTip("import a new asset");
		connect(mActionImport, &QAction::triggered, this, &AssetBrowserWidget::EVImportAsset);

		auto assetsClass = Asset::GetClassInfoStatic()->GetSubClasses();
		for (size_t i = 0; i < assetsClass.Length(); i++)
		{
			if (assetsClass[i] == nullptr) continue;

			if (assetsClass[i]->GetAttributes().HasAttrib(EAtrribID::EAT_Instanceable))
			{
				QAction* actCreate = new QAction(ToQString(assetsClass[i]->GetName()), this);

				connect(actCreate, &QAction::triggered, this, &AssetBrowserWidget::EVCreateAsset);
				mActionsCreateAsset << actCreate;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void AssetBrowserWidget::FillList(const QString& filter)
	{
		mAssets->clear();

		auto& assets = AssetSys::Get()->GetAssets();
		for (size_t i = 0; i < assets.Length(); i++)
		{
			QString assetName = ToQString(assets[i]->GetName());
			QString assetClassName = ToQString(assets[i]->GetClassName());

			if (SearchCheck(filter, assetName))
			{
				auto* newAssetItem = new AssetBrowserItem(assets[i], mAssets);
				mAssets->addItem(newAssetItem);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void AssetBrowserWidget::Tick()
	{

	}

	void AssetBrowserWidget::EVContextMenuRequested(const QPoint& point)
	{
		QMenu menu;

		QPoint globalPoint = mAssets->mapToGlobal(point);


		if (QListWidgetItem* selectedItem = mAssets->itemAt(point))
		{
			QAction* actOpen = menu.addAction("Open");
			QAction* actDelete = menu.addAction("Delete");

			connect(actOpen, &QAction::triggered, this, [&](bool) {
				this->OpenAsset(selectedItem);
				QColorDialog* cd = new QColorDialog;
				
			});

			connect(actDelete, &QAction::triggered, this, [&](bool) {
				this->DeleteAsset(selectedItem);
			});
			

			menu.addAction(actOpen);
			menu.addAction(actDelete);
		}
		else
		{

		}

		menu.addSeparator();
		menu.addAction(mActionImport);

		QMenu menuCreate;
		menuCreate.setTitle("Create");
		menuCreate.addActions(mActionsCreateAsset);
		menu.addMenu(&menuCreate);

		QAction* chosenAction = menu.exec(globalPoint);
		
	}
	//////////////////////////////////////////////////////////////////////////
	void AssetBrowserWidget::OpenAsset(QListWidgetItem* item)
	{
		UASSERT(item);

		if (auto assetToLoad = AssetSys::Get()->FindAsset(ToName(item->text())))
		{
			AssetViewer::OpenAsset(assetToLoad);
		}
		else
		{
			ULOG_ERROR("failed to load asset");
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void AssetBrowserWidget::DeleteAsset(QListWidgetItem* item)
	{
		UASSERT(item);
		
		delete item;

		ULOG_ERROR("TODO asset have to be released and removed, btw first of all Pointers to the asset should be zero and ...");
	}
	//////////////////////////////////////////////////////////////////////////
	Name AssetBrowserWidget::GetUniqueAssetName(ClassInfo* optAssetClass) const
	{
		for (size_t i = 0; i < 9999; i++)
		{
			QString strAssetName;
			if (optAssetClass)
				strAssetName = ToQString(optAssetClass->GetName()).remove(':') + QString::number(i);
			else
				strAssetName = QString("Asset_") + QString::number(i);

			Name name = ToName(strAssetName);
			if (!AssetSys::Get()->FindAsset(name)) // isn't there any asset with that name ?
			{
				return name;
			}
		}
		return Name();
	}
	//////////////////////////////////////////////////////////////////////////
	void AssetBrowserWidget::EVImportAsset(bool)
	{
		QString strChosenFile = QFileDialog::getOpenFileName(this, "Import Asset");

		if (strChosenFile.isNull() || strChosenFile.isEmpty()) return;

		
	}
	//////////////////////////////////////////////////////////////////////////
	//context menu create asset
	void AssetBrowserWidget::EVCreateAsset(bool)
	{
		QAction* theSender = (QAction*)sender();
		Name assetClassName = ToName(((QAction*)sender())->text());
		auto assetClass = MetaSys::Get()->FindClass(assetClassName);
		if (assetClass)
		{
			mPendingCreationAssetClass = assetClass;
			mEditingIntetAssetCreation = true;

			AssetBrowserItem* newAssetItem = new AssetBrowserItem(nullptr, mAssets);
			mPendingCreationAssetItem = newAssetItem;

			mAssets->addItem(newAssetItem);
			newAssetItem->setSelected(true);
			mAssets->editItem(newAssetItem);
		}
	}

	void AssetBrowserWidget::dragEnterEvent(QDragEnterEvent *e)
	{
		if (e->mimeData()->hasUrls()) {
			e->acceptProposedAction();
		}
	}
	void AssetBrowserWidget::dropEvent(QDropEvent *e)
	{
		foreach(const QUrl &url, e->mimeData()->urls()) {
			QString fileName = url.toLocalFile();
			qDebug() << "Dropped file:" << fileName;
			ULOG_MESSAGE("dropped file %", fileName.toStdString().c_str());
			
		}
	}
	//////////////////////////////////////////////////////////////////////////
	AssetBrowserItem::AssetBrowserItem(AssetEntry* asset, QListWidget* list) : QListWidgetItem(list)
	{
		setFlags(flags() | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		AttachAsset(asset);
	}
	//////////////////////////////////////////////////////////////////////////
	void AssetBrowserItem::AttachAsset(AssetEntry* asset)
	{
		mAsset = asset;

		if (mAsset)
		{
			setText(ToQString(asset->GetName()));
			setToolTip(ToQString(asset->GetClassName()));
		}
		else
		{
			setText("Asset");
		}
	}
#endif // 

	AssetBrowserWidget::AssetBrowserWidget(QWidget* parent /*= nullptr*/) : QWidget(parent)
	{
		ui = new Ui_AssetBrowser;
		ui->setupUi(this);

		setAcceptDrops(true);

		connect(ui->assetsTree, &QTreeWidget::itemDoubleClicked, this, [&](QTreeWidgetItem* _item, int) {
			AssetBrowserItem* item = (AssetBrowserItem*)_item;
			if (item->mAssetEntry && !item->mAssetEntry->IsFolder())
			{
				OpenAsset(item->mAssetEntry);
			}
		});


		connect(ui->assetsTree, &QTreeWidget::customContextMenuRequested, this, &Self::EVContextMenuRequested);
		connect(ui->assetsTree->itemDelegate(), &QAbstractItemDelegate::commitData, this, [&](QWidget* widget) {

			QString strNewText = reinterpret_cast<QLineEdit*>(widget)->text();
			EVItemEditingFinished(strNewText);
		});

		connect(ui->filterLineEdit, &QLineEdit::textChanged, this, &Self::EVFilterTextChanged);

		ReFillTree();
		ReInitActions();

		mActionSaveAll = new QAction("Save All", this);
		connect(mActionSaveAll, &QAction::triggered, this, &Self::EVSaveAll);

		connect(ui->btnSave, &QPushButton::clicked, this, &Self::EVSaveAll);

		mActionShowAssetInExplorer = new QAction("Show In Explorer", this);

// 		auto itemProj = new QTreeWidgetItem(ui->foldersTree);
// 		itemProj->setText(0, "Project");
// 		auto itemEngine = new QTreeWidgetItem(ui->foldersTree);
// 		itemEngine->setText(0, "Engine");
// 
// 		ui->foldersTree->addTopLevelItem(itemProj);
// 		ui->foldersTree->addTopLevelItem(itemEngine);

		{
// 			AssetBrowserItem2* itemEngine = new AssetBrowserItem2(nullptr, nullptr);
// 			itemEngine->setText(0, "Engine");
// 			ui->assetsTree->addTopLevelItem(itemEngine);
			

// 			auto itemParent = itemEngine;
// 			AssetEntry* iter = 
// 			for (size_t i = 0; i < iter->mChildren.Length(); i++)
// 			{
// 				auto item = new AssetBrowserItem2(iter->mChildren[i], itemParent);
// 				ui->assetsTree->addTopLevelItem(item);
// 				itemParent = item;
// 			}
		}


// 		{
// 
// 
// 			AssetBrowserItem2* itemProject = new AssetBrowserItem2(nullptr, nullptr);
// 			itemProject->setText(0, "Project");
// 			ui->assetsTree->addTopLevelItem(itemProject);
// 			auto itemParent = itemProject;
// 			AssetEntry* iter = GAssetSys()->EngineEntry();
// 			for (size_t i = 0; i < iter->mChildren.Length(); i++)
// 			{
// 				auto item = new AssetBrowserItem2(iter->mChildren[i], itemParent);
// 				ui->assetsTree->addTopLevelItem(item);
// 				itemParent = item;
// 			}
// 
// 			
// 		}


		//InitFolders();
	}

	void AssetBrowserWidget::ReInitActions()
	{
		mAssetCreationActions.clear();

		auto& assetsClass = Asset::GetClassInfoStatic()->GetSubClasses();
		for (auto assetClass : assetsClass)
		{
			if(assetClass == nullptr) continue;

			if (assetClass->HasAttrib(EAtrribID::EAT_Instanceable)) //is instanceable?
			{

				QAction* actCreate = new QAction(ToQString(assetClass->GetName()), this);
				
				if (QIcon* icon = GetIcon(assetClass))
					actCreate->setIcon(*icon);

				connect(actCreate, &QAction::triggered, this, &Self::EVCreateAsset);
				mAssetCreationActions << actCreate;
			}
		}
	}

	void AssetBrowserWidget::AddEntiryToTree(AssetEntry* entry, AssetBrowserItem* parentItem)
	{
		AssetBrowserItem* item = parentItem;
		if (!entry->mFilename.IsEmpty())
		{
			if(FilterCheck(ToQString(entry->GetName())))
			{
				item = new AssetBrowserItem(entry, parentItem);
				ui->assetsTree->addTopLevelItem(item);
			}
		}
		for (size_t i = 0; i < entry->mChildren.Length(); i++)
		{
			AddEntiryToTree(entry->mChildren[i], item);
		}
	
	}




//deprecated
// 	void AssetBrowserWidget2::AddDirContents(const QString& path, AssetBrowserItem2* parentItem, const QString& prefix)
// 	{
// 		QDirIterator iter(path, QDir::Dirs | QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
// 		
// 		while (iter.hasNext()) 
// 		{
// 			//eg: ../content/file.txt
// 			auto filepath = iter.next();
// 			QFileInfo fileInfo = iter.fileInfo();
// 
// 			QString filename = fileInfo.fileName();		//eg file.txt
// 			QString assetName = prefix.isEmpty() ? filename : prefix + '/' + filename;
// 			QString absFilePath = fileInfo.absoluteFilePath();
// 
// 			if (fileInfo.isFile())
// 			{
// 				if (FilterCheck(assetName))
// 				{
// 					if (GAssetSys()->CheckFile(ToString(absFilePath))) // is asset file ?
// 					{
// 						AssetBrowserItem2* newItem = new AssetBrowserItem2(assetName, absFilePath, filename, false, parentItem);
// 						ui->assetsTree->addTopLevelItem(newItem);
// 					}
// 				}
// 			}
// 			else
// 			{
// 				AssetBrowserItem2* newItem = new AssetBrowserItem2(assetName, absFilePath, filename, true, parentItem);
// 				ui->assetsTree->addTopLevelItem(newItem);
// 				AddDirContents(filepath, newItem, assetName);
// 			}
// 		}
// 	}

	bool AssetBrowserWidget::FilterCheck(const QString& str)
	{
		return UPOEd::FilterCheck(ui->filterLineEdit->text(), str);
	}

	void AssetBrowserWidget::ReFillTree()
	{
		ui->assetsTree->clear();

		AddEntiryToTree(GAssetSys()->EngineEntry(), nullptr);
		AddEntiryToTree(GAssetSys()->ProjectEntry(), nullptr);
		
		ui->assetsTree->expandAll();
	}

	void AssetBrowserWidget::EVFilterTextChanged(const QString&)
	{
		ReFillTree();
		ui->assetsTree->expandAll();
	}

	void AssetBrowserWidget::EVSaveAll(bool)
	{
		GAssetSys()->SaveAllDirtyAssets();
	}
	//////////////////////////////////////////////////////////////////////////
	void AssetBrowserWidget::EVContextMenuRequested(const QPoint& point)
	{
		QMenu menu;
		QMenu menu_Create;

		QAction* actionRename = nullptr;
		QAction* actionDelete = nullptr;

		QPoint globalPoint = ui->assetsTree->mapToGlobal(point);

		{
			menu.addAction(mActionSaveAll);
		}

		AssetBrowserItem* selectedItem = (AssetBrowserItem*)ui->assetsTree->itemAt(point);
		if (selectedItem) // r we on item?
		{
			if(selectedItem->mAssetEntry->IsFolder()) //item is folder?
			{
				menu_Create.setTitle("Create");
				menu_Create.addActions(mAssetCreationActions);
				menu.addMenu(&menu_Create);

				
			}

			menu.addAction(mActionShowAssetInExplorer);
			actionRename = menu.addAction("Rename");
			actionDelete = menu.addAction("Delete");
		}
		else
		{
			
		}

		/////////////////////executing and handling selected menu
		QAction* chosenAction = menu.exec(globalPoint);
		if (mAssetCreationActions.contains(chosenAction)) // asset creation selected?
		{
			Name assetClassName = ToName(chosenAction->text());
			auto assetClass = GMetaSys()->FindClass(assetClassName);
			if (assetClass)
			{
				CreateDefaulAssetFile(assetClass, selectedItem);
			}
			
		}
		else if (chosenAction == mActionShowAssetInExplorer) // show in explorer?
		{
			UShowInExplorer(selectedItem->mAssetEntry->GetFullFileName());
		}
		else if (chosenAction == actionRename) // rename ?
		{
			ui->assetsTree->editItem(selectedItem);
		}
		else if (chosenAction == actionDelete) //delete ?
		{
			if (!selectedItem->mAssetEntry->IsFolder())
			{
			
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void AssetBrowserWidget::EVCreateAsset(bool)
	{
// 		//note: sender type is QAtion and its text is class's name
// 
// 		QAction* theSender = (QAction*)sender();
// 		Name assetClassName = ToName(((QAction*)sender())->text()); 
// 		auto assetClass = GMetaSys()->FindClass(assetClassName);
// 		if (assetClass)
// 		{
// 			mPendingCreationAssetClass = assetClass;
// 			mEditingIntetAssetCreation = true;
// 
// 			AssetBrowserItem* newAssetItem = new AssetBrowserItem(nullptr,);
// 			mPendingCreationAssetItem = newAssetItem;
// 
// 			mAssets->addItem(newAssetItem);
// 			newAssetItem->setSelected(true);
// 			mAssets->editItem(newAssetItem);
// 		}
	}

	void AssetBrowserWidget::EVItemEditingFinished(const QString& newText)
	{
		if (mEditingIntetAssetCreation) // we were setting the name of new asset?
		{
			mEditingIntetAssetCreation = false;

			UASSERT(mPendingCreationAssetClass && mPendingCreationAssetItem);

			AssetBrowserItem* folder = (AssetBrowserItem*)mPendingCreationAssetItem->parent();


			if (AssetEntry* newAsset = GAssetSys()->CreateDefaulAssetFile(folder->mAssetEntry, ToString(newText), mPendingCreationAssetClass))
			{
					mPendingCreationAssetItem->SetAsset(newAsset);
					mPendingCreationAssetItem = nullptr;
					mPendingCreationAssetClass = nullptr;
					return;
			}
			else
			{
				delete mPendingCreationAssetItem;
				mPendingCreationAssetItem = nullptr;
				mPendingCreationAssetClass = nullptr;
			}


		}
		else
		{
			AssetBrowserItem* item = (AssetBrowserItem*)ui->assetsTree->currentItem();
			item->mAssetEntry->Rename(ToString(newText));
			//RenameAsset(mRenamingItem, newText);
		}
		ULOG_ERROR("");
	}
	//////////////////////////////////////////////////////////////////////////
	void AssetBrowserWidget::dragEnterEvent(QDragEnterEvent *e)
	{
		if (e->mimeData()->hasUrls())
		{
			auto urls = e->mimeData()->urls();
			if (urls.size() == 1)
			{
				QString filename = urls[0].toLocalFile();
				if (!QDir(filename).exists()) // is not folder?
				{
					String ext = PathGetExt(ToString(filename));
					if(AssetConverter::ExtIsSupported(ext))
						e->acceptProposedAction();
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void AssetBrowserWidget::dropEvent(QDropEvent *ev)
	{
// 		foreach(const QUrl &url, e->mimeData()->urls()) {
// 			QString fileName = url.toLocalFile();
// 			qDebug() << "Dropped file:" << fileName;
// 			ULOG_MESSAGE("dropped file %s", fileName.toStdString().c_str());
// 
// 		}
		QPoint reletivePos = ui->assetsTree->mapFromGlobal(mapToGlobal(ev->pos()));	//(event->pos() is relative to widget
		AssetBrowserItem* item = (AssetBrowserItem*)ui->assetsTree->itemAt(reletivePos);
		if (item && item->mAssetEntry && item->mAssetEntry->IsFolder()) //r we dropping on folder ?
		{
			String fullfilename = ToString(ev->mimeData()->urls()[0].toLocalFile());
			AssetConverter::Convert(fullfilename, item->mAssetEntry);
			ReFillTree();
		}



	}

	void AssetBrowserWidget::dragMoveEvent(QDragMoveEvent *event)
	{	
		QPoint reletivePos = ui->assetsTree->mapFromGlobal(mapToGlobal(event->pos()));	//(event->pos() is relative to widget
		AssetBrowserItem* item = (AssetBrowserItem*) ui->assetsTree->itemAt(reletivePos);
		if(item && item->mAssetEntry && item->mAssetEntry->IsFolder()) //dropping on folder ?
		{
			event->setDropAction(Qt::DropAction::CopyAction);
			ui->assetsTree->expandItem(item);
			ui->assetsTree->setCurrentItem(item);
			//item->setTextColor(0, QColor(255, 0, 0));
			//item->setBackgroundColor(0, QColor(1,0,0));
			//ULOG_MESSAGE("%s", item->text(0).toStdString().c_str());
			
			event->accept();
		}
		else
		{
			event->ignore();
		}
	}

	void AssetBrowserWidget::OpenAsset(AssetEntry* asset)
	{
		AssetViewer::OpenAsset(asset);
	}

	bool AssetBrowserWidget::CreateDefaulAssetFile(ClassInfo* assetClass, AssetBrowserItem* folder)
	{
		mPendingCreationAssetClass = assetClass;
		mEditingIntetAssetCreation = true;

		AssetBrowserItem* newAssetItem = new AssetBrowserItem(nullptr, folder);
		newAssetItem->setText(0, "new asset");
		mPendingCreationAssetItem = newAssetItem;
		
		ui->assetsTree->addTopLevelItem(newAssetItem);
		//newAssetItem->setSelected(true);
		ui->assetsTree->setCurrentItem(newAssetItem);
		ui->assetsTree->expandItem(folder);
		ui->assetsTree->editItem(newAssetItem);

		return false;
	}

	bool AssetBrowserWidget::RenameAsset(AssetBrowserItem* item)
	{
		return false;
	}

	AssetBrowserItem::AssetBrowserItem(AssetEntry* entry, AssetBrowserItem* parent) : QTreeWidgetItem(parent)
	{
		mAssetEntry = entry;
		if (mAssetEntry)
		{
			setText(0, ToQString(mAssetEntry->GetFileName()));
			if (auto ci = mAssetEntry->GetClassInfo())
			{
				setToolTip(0, ToQString(ci->GetName()));
			}
		}

		setFlags(flags() | Qt::ItemFlag::ItemIsEditable);

		InitIcon();
	}

	void AssetBrowserItem::InitIcon()
	{
		static QIcon FolderIcon = QIcon("../Icons/AssetBrowserFolder.png");
		static QIcon AssetIcon = QIcon("../Icons/AssetBrowserAsset.png");

		if (mAssetEntry)
		{
			QIcon* icon = GetIcon(mAssetEntry->GetClassInfo());
			setIcon(0, icon ? *icon : mAssetEntry->mIsFolder ? FolderIcon : AssetIcon);
		}

		
	}

};
