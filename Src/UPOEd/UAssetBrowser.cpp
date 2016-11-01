#include "UAssetBrowser.h"
#include "UAssetViewer.h"

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
		this->setWidget(new AssetBrowserWidget2);
	}
	//////////////////////////////////////////////////////////////////////////
	void AssetBrowserDW::Tick()
	{
		//((AssetBrowserWidget*)this->widget())->Tick();
	}
	//////////////////////////////////////////////////////////////////////////
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
					ULOG_SUCCESS("asset [%s] created", nameOfNewAsset.CStr());
					return;
				}
			}

			ULOG_ERROR("failed to create new asset [%s]", nameOfNewAsset.CStr());
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
			ULOG_MESSAGE("dropped file %s", fileName.toStdString().c_str());
			
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

	AssetBrowserWidget2::AssetBrowserWidget2(QWidget* parent /*= nullptr*/) : QWidget(parent)
	{
		ui = new Ui_AssetBrowser;
		ui->setupUi(this);

// 		auto itemProj = new QTreeWidgetItem(ui->foldersTree);
// 		itemProj->setText(0, "Project");
// 		auto itemEngine = new QTreeWidgetItem(ui->foldersTree);
// 		itemEngine->setText(0, "Engine");
// 
// 		ui->foldersTree->addTopLevelItem(itemProj);
// 		ui->foldersTree->addTopLevelItem(itemEngine);

		InitFolders();
	}
	QDir GetEngineContentDir()
	{
		QDir dir = "../Content/";
		return dir;
	}
	void AssetBrowserWidget2::InitFolders()
	{
		ui->assetsTree->clear();


		AddDirContents("../Content/", nullptr, QString());
	}

	void AssetBrowserWidget2::AddDirContents(const QString& path, AssetBrowserItem2* parentItem, const QString& prefix)
	{
		QDirIterator iter(path, QDir::Dirs | QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
		
		while (iter.hasNext()) 
		{
			//eg: ../content/file.txt
			auto filepath = iter.next();
			QFileInfo fileInfo = iter.fileInfo();

			QString filename = fileInfo.fileName();		//eg file.txt
			QString assetName = prefix.isEmpty() ? filename : prefix + '/' + filename;
			QString absFilePath = fileInfo.absoluteFilePath();

			if (fileInfo.isFile())
			{
				if (FilterCheck(assetName))
				{
					if (AssetSys::Get()->CheckFile(ToString(absFilePath))) // is asset file ?
					{
						AssetBrowserItem2* newItem = new AssetBrowserItem2(assetName, absFilePath, filename, false, parentItem);
						ui->assetsTree->addTopLevelItem(newItem);
					}
				}
			}
			else
			{
				AssetBrowserItem2* newItem = new AssetBrowserItem2(assetName, absFilePath, filename, true, parentItem);
				ui->assetsTree->addTopLevelItem(newItem);
				AddDirContents(filepath, newItem, assetName);
			}
		}
	}

	bool AssetBrowserWidget2::FilterCheck(const QString& str)
	{
		return SearchCheck(ui->filterLineEdit->text(), str);
	}

	void AssetBrowserItem2::InitIcon()
	{
		static QIcon FolderIcon = QIcon("../Icons/AssetBrowserFolder.png");
		static QIcon AssetIcon = QIcon("../Icon/AssetBrowserAsset.png");

		QIcon* icon = nullptr;
		if (mAsset)
		{
			if (ClassInfo* assetClass = mAsset->GetClassInfo())
			{
				Attrib attribIcon;
				if (assetClass->GetAttrib(EAT_Icon, attribIcon))
				{
					icon = GetIcon(ToQString(attribIcon.GetString()));
				}
			}
		}

		setIcon(0, icon ? *icon : mIsFolder ? FolderIcon : AssetIcon);
	}

};
