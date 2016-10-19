#include "UAssetBrowser.h"
#include "UAssetViewer.h"

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
		((AssetBrowserWidget*)this->widget())->Tick();
	}
	//////////////////////////////////////////////////////////////////////////
	AssetBrowserWidget::AssetBrowserWidget(QWidget* parent /*= nullptr*/) : QWidget(parent)
	{
		setLayout(new QVBoxLayout);

		mFilter = new QLineEdit(this);
		mFilter->setClearButtonEnabled(true);
		layout()->addWidget(mFilter);

		mAssets = new QListWidget(this);
		layout()->addWidget(mAssets);


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

		auto assetsClass = Asset::GetClassInfoStatic()->GetSubClasses();
		for (size_t i = 0; i < assetsClass.Length(); i++)
		{
			if (assetsClass[i] == nullptr) continue;

			if (assetsClass[i]->GetAttributes().HasAttrib(EAtrribID::EAT_Instanceable))
			{
				QAction* actCreate = new QAction(ToQString(assetsClass[i]->GetName()), this);

				//create asset event -------------------
				connect(actCreate, &QAction::triggered, this, [&](bool) {
					QAction* theSender = (QAction*)sender();
					Name assetClassName = ToName(((QAction*)sender())->text());
					auto assetClass = MetaSys::Get()->FindClass(assetClassName);
					if (assetClass)
					{
						AssetEntry* newAsset = AssetSys::Get()->CreateAsset(assetClass, GetUniqueAssetName(assetClass));
						if (newAsset)
						{
							AssetBrowserItem* newAssetItem = new AssetBrowserItem(newAsset, mAssets);
							mAssets->addItem(newAssetItem);
						}
					}
				});

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

			if (SearchCheck(assetName, filter))
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
	Name AssetBrowserWidget::GetUniqueAssetName(ClassInfo* assetClass) const
	{
		for (size_t i = 0; i < 9999; i++)
		{
			QString str = ToQString(assetClass->GetName()).remove(':') + QString::number(i);
			Name name = ToName(str);
			if (!AssetSys::Get()->FindAsset(name))
			{
				return name;
			}
		}
		return Name();
	}

	//////////////////////////////////////////////////////////////////////////
	AssetBrowserItem::AssetBrowserItem(AssetEntry* asset, QListWidget* list) : QListWidgetItem(list)
	{
		UASSERT(asset);

		mAsset = asset;

		setText(ToQString(asset->GetName()));
		setFlags(flags() | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		setToolTip(ToQString(asset->GetClassName()));
	}



};