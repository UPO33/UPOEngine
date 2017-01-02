#include "UEntityBrowser.h"
#include "UMainWindow.h"


namespace UPOEd
{
	void GetSpawnableEntitiesClass(TArray<ClassInfo*>& out)
	{
		GMetaSys()->GetAllTypes().ForEach([&out](TypeInfo* type)
		{
			if (type && type->IsClass())
			{
				ClassInfo* ci = (ClassInfo*)type;
				if (ci->IsBaseOf(Entity::GetClassInfoStatic()) && !ci->IsAbstract())
				{
					out.Add(ci);
				}
			}
		});
	}

	EntityBrowserDW::EntityBrowserDW(QWidget* parent) : QDockWidget(parent)
	{
		this->setAllowedAreas(Qt::DockWidgetArea::LeftDockWidgetArea | Qt::DockWidgetArea::RightDockWidgetArea);
		this->setWindowTitle("Entities");
		this->setContentsMargins(0, 0, 0, 0);
		this->setWidget(new EntityBrowserWidget);
	}

	void EntityBrowserDW::AttachWorld(World* world)
	{
		((EntityBrowserWidget*)this->widget())->AttachWorld(world);
	}

	void EntityBrowserDW::Tick()
	{
		((EntityBrowserWidget*)this->widget())->Tick();
	}

	EntityBrowserWidget::EntityBrowserWidget(QWidget* parent) : QWidget(parent)
	{
		mAttachedWorld = nullptr;
		mSelectedEntity = nullptr;


		setLayout(new QVBoxLayout);
		mFilter = new QLineEdit(this);
		mFilter->setClearButtonEnabled(true);
		layout()->addWidget(mFilter);

		mTree = new QTreeWidget(this);
		layout()->addWidget(mTree);
		mTree->setHeaderHidden(true);
		mTree->setAlternatingRowColors(true);
		mTree->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
		mTree->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

		setMinimumSize(QSize(300, 300));

		setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
		connect(mTree, &QTreeWidget::customContextMenuRequested, this, &EntityBrowserWidget::EVContextMenuRequested);
		
		connect(mTree, &QTreeWidget::itemSelectionChanged, this, [this]() {
			ULOG_MESSAGE("n %", mTree->selectedItems().length());
		});

		connect(mTree, &QTreeWidget::itemClicked, this, [this](QTreeWidgetItem* item, int)
		{
			SelectEntity((EntityBrowserItem*)item);
		});

	}

	EntityBrowserWidget::~EntityBrowserWidget()
	{

	}

	void EntityBrowserWidget::ReFillTree()
	{
		ULOG_MESSAGE("");
		mTree->clear();
		AddToTree(nullptr);
		mTree->expandAll();
	}



	EntityBrowserItem::EntityBrowserItem(Entity* entity, EntityBrowserItem* parent) : QTreeWidgetItem(parent)
	{
		mEntity = entity;
		setFlags(flags() | Qt::ItemFlag::ItemIsEditable);
		if (mEntity)
		{
			this->setText(0, ToQString(mEntity->mName));
			this->setToolTip(0, ToQString(mEntity->GetClassInfo()->GetName()));
			if (auto icon = GetIcon(entity->GetClassInfo()))
				this->setIcon(0, *icon);
		}
	}

	EntityBrowserItem::~EntityBrowserItem()
	{
	}

	void EntityBrowserWidget::AttachWorld(World* world)
	{
		mAttachedWorld = world;
		ReFillTree();
	}

	void EntityBrowserWidget::AddToTree(EntityBrowserItem* parentItem)
	{
		Entity* entity = nullptr;
		if (parentItem)
			entity = parentItem->mEntity;
		else 
			entity = mAttachedWorld->GetRootEntity();
		

		ULOG_MESSAGE("nchild %", entity->NumChild());
		for (unsigned iChild = 0; iChild < entity->NumChild(); iChild++)
		{
			Entity* child = entity->GetChild(iChild);
			if (child->IsAlive())	//TODO Add filter check
			{
				EntityBrowserItem* newItem = new EntityBrowserItem(child, parentItem);
				mTree->addTopLevelItem(newItem);
				AddToTree(newItem);
			}
		};
	}
	//tree context menu
	void EntityBrowserWidget::EVContextMenuRequested(const QPoint& pos)
	{
		QMenu* menu = new QMenu(this);
		QPoint globalPoint = mTree->mapToGlobal(pos);
		auto selectedItem = (EntityBrowserItem*)mTree->itemAt(pos);
		if (selectedItem) // r we on item?
		{
			TObjectPtr<Entity> selectedEntity = selectedItem->mEntity;
			
			menu->addAction("Destroy", this, [selectedEntity, this]() {
				if (selectedEntity) selectedEntity->Destroy();
				ReFillTree();
			});

		}
		else
		{

		}

		menu->popup(globalPoint);


	}

	void EntityBrowserWidget::SelectEntity(EntityBrowserItem* item)
	{
		if (mSelectedEntity == item) return;

		mSelectedEntity = item;
		if (Entity* ent = item->mEntity)
		{
			gMainWindow->mPropertyBrowser->AttachObject(ent);
		}
	}

	void EntityBrowserWidget::Tick()
	{
// 		if (mAttachedWorld)
// 		{
// 			if (mAttachedWorld->IsPlaying())
// 			{
// 				
// 			}
// 		}
	}

	// 	void EntityBrowserWidget::mousePressEvent(QMouseEvent *event)
// 	{
// 		QWidget::mousePressEvent(event);
// 		ULOG_MESSAGE("");
// // 		mTree->setItemSelected(nullptr, 0);
// // 		mTree->clearFocus();
// 	}

};