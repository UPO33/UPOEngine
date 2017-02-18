#include "UEntityBrowser.h"
#include "UMainWindow.h"

#include "../UPOEngine/Engine/UEntityCamera.h"


namespace UPOEd
{
	//////////////////////////////////////////////////////////////////////////
	void UGetSpawnableEntitiesClass(TArray<ClassInfo*>& out)
	{
		GMetaSys()->GetAllTypes().ForEach([&out](TypeInfo* type)
		{
			if (type && type->IsClass())
			{
				ClassInfo* ci = (ClassInfo*)type;
				if (ci->IsBaseOf<Entity>() && !ci->IsAbstract() && ci->HasAttrib(EAttribID::EAT_Instanceable))
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

		mTree = new EntityBrowserTreeWidget(this);
		layout()->addWidget(mTree);
		mTree->setHeaderHidden(true);
		mTree->setIndentation(10);
		mTree->setAlternatingRowColors(true);
		mTree->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
		mTree->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
		mTree->setDragEnabled(true);
		mTree->setDragEnabled(true);
		mTree->setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);
		

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
		mSelectedEntity = nullptr;
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
		QMenu* menu_Create = nullptr;
		if(mIsEntityCreationPermitted)
		{
			menu_Create = new QMenu(menu);
			menu_Create->setTitle("&Create");
		}

		QPoint globalPoint = mTree->mapToGlobal(pos);
		auto selectedItem = (EntityBrowserItem*)mTree->itemAt(pos);
		if (selectedItem) // r we on item?
		{
			TObjectPtr<Entity> selectedEntity = selectedItem->mEntity;
			
			menu->addAction("&Destroy", this, [selectedEntity, this]() {
				if (selectedEntity) selectedEntity->Destroy();
				ReFillTree();
			});

		}
		else
		{

		}
		
		if(mIsEntityCreationPermitted)	
		{
			menu->addMenu(menu_Create);
			TArray<ClassInfo*> entityClasses;
			UGetSpawnableEntitiesClass(entityClasses);
			for (ClassInfo* entityClass : entityClasses)
			{
				QIcon* classIcon = GetIcon(entityClass);
				QAction* actionCreateEntity = menu_Create->addAction(classIcon ? *classIcon : QIcon(), ToQString(entityClass->GetName()));
				connect(actionCreateEntity, &QAction::triggered, this, [this, entityClass, selectedItem](bool) {
					CreateEntity(entityClass, selectedItem);
				});
			}
		}

		menu->popup(globalPoint);


	}

	void EntityBrowserWidget::SelectEntity(EntityBrowserItem* item, bool applyOnPropertyBrowser)
	{
		if (mSelectedEntity && mSelectedEntity->mEntity)
			mSelectedEntity->mEntity->SetSelected(false);

		mSelectedEntity = item;
		if (mSelectedEntity && mSelectedEntity->mEntity)
			mSelectedEntity->mEntity->SetSelected(true);

		if(applyOnPropertyBrowser)
		{
			gMainWindow->mPropertyBrowser->AttachObject(item->mEntity);
		}
	}

	//calls a lambda on items recursively. [](QTreeWidgetItem*){}
	template<typename Lambda> void UDoOnChildren(QTreeWidgetItem* root, const Lambda& proc)
	{
		for (size_t i = 0; i < root->childCount(); i++)
		{
			proc(root->child(i));
			UDoOnChildren(root->child(i), proc);
		}
	}

	void EntityBrowserWidget::SelectEntity(Entity* entity, bool applyOnPropertyBrowser)
	{
		if (mSelectedEntity && mSelectedEntity->mEntity)
			mSelectedEntity->mEntity->SetSelected(false);
			
		if (!entity)
		{
			mSelectedEntity = nullptr;
			mTree->setCurrentItem(nullptr);
			return;
		}

		UDoOnChildren(mTree->invisibleRootItem(), [&](QTreeWidgetItem* item) {
			if (EntityBrowserItem* entItem = (EntityBrowserItem*)item)
			{
				if (entItem->mEntity.Get() == entity)
				{
					mTree->setCurrentItem(entItem);
					
					mSelectedEntity = entItem;
					mSelectedEntity->mEntity->SetSelected(true);

					if(applyOnPropertyBrowser)
						gMainWindow->mPropertyBrowser->AttachObject(entity);
				}
			}
		});

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

	
	void EntityBrowserWidget::CreateEntity(ClassInfo* entityClass, EntityBrowserItem* parentMaybeNull)
	{
		EntityCreationParam ecp;
		ecp.mClass = entityClass;
		ecp.mParent = nullptr;
		//if any entity is selected create at its transformation
		if (parentMaybeNull)
		{
			ecp.mParent = parentMaybeNull->mEntity;
			ecp.mWorldTransform = parentMaybeNull->mEntity->GetWorldTransform();
		}
		else if (EntityCamera* edCamera = mAttachedWorld->GetFreeCamera())
		{
			Vec3 swapnPosition = edCamera->GetWorldPosition() + edCamera->GetForward() * 50;
			ecp.mWorldTransform.MakeTranslation(swapnPosition);
		}

		mAttachedWorld->CreateEntity(ecp);
		ReFillTree();
	}

	// 	void EntityBrowserWidget::mousePressEvent(QMouseEvent *event)
// 	{
// 		QWidget::mousePressEvent(event);
// 		ULOG_MESSAGE("");
// // 		mTree->setItemSelected(nullptr, 0);
// // 		mTree->clearFocus();
// 	}

	EntityBrowserTreeWidget::EntityBrowserTreeWidget(QWidget* parent /*= nullptr*/) : QTreeWidget(parent)
	{
		this->setIndentation(10);
		this->setDragEnabled(true);
		this->setDragDropMode(QAbstractItemView::NoDragDrop);
	}

	EntityBrowserTreeWidget::~EntityBrowserTreeWidget()
	{

	}

	void EntityBrowserTreeWidget::dropEvent(QDropEvent *event)
	{
		EntityBrowserItem* curItem = (EntityBrowserItem*)this->currentItem();
		Entity* curEntity = curItem ? curItem->mEntity.Get() : nullptr;

		if (auto tb = qobject_cast<EntityBrowserTreeWidget*>(event->source()))
		{
			if (!curEntity)return;

			if (EntityBrowserItem* item = (EntityBrowserItem*)this->itemAt(event->pos()))
			{
				if (item != curItem && item->mEntity)
				{
					if (!item->mEntity->IsSubsetOf(curEntity))
					{
						curEntity->AttachTo(item->mEntity);
						((EntityBrowserWidget*)this->parent())->ReFillTree();

						event->accept();
						return;
					}
				}

			}
			else
			{
				curEntity->AttachTo(nullptr);
				((EntityBrowserWidget*)this->parent())->ReFillTree();
				event->accept();
				return;
			}
		}
		event->ignore();
	}

	void EntityBrowserTreeWidget::dragEnterEvent(QDragEnterEvent *event)
	{
		ULOG_MESSAGE("%", event->source()->metaObject()->className());
		ULOG_MESSAGE("%", ToString(this->currentItem()->text(0)));

		if (auto tb = qobject_cast<EntityBrowserTreeWidget*>(event->source()))
		{
			if (this->itemAt(event->pos()))
			{
				event->accept();
				return;
			}
		}

		event->accept();


	}

	void EntityBrowserTreeWidget::dragMoveEvent(QDragMoveEvent *event)
	{
		EntityBrowserItem* curItem = (EntityBrowserItem*)this->currentItem();
		Entity* curEntity = curItem ? curItem->mEntity.Get() : nullptr;

		if (auto tb = qobject_cast<EntityBrowserTreeWidget*>(event->source()))
		{
			if (EntityBrowserItem* item = (EntityBrowserItem*)this->itemAt(event->pos()))
			{
				if (item != curItem && item->mEntity)
				{
					if (!item->mEntity->IsSubsetOf(curEntity))
					{
						event->accept();
						return;
					}
				}
			}
			else
			{
				event->accept();
				return;
			}
		}
		event->ignore();
		
	}

	void EntityBrowserTreeWidget::dragLeaveEvent(QDragLeaveEvent *event)
	{
		ULOG_MESSAGE("");
	}

	void EntityBrowserTreeWidget::keyPressEvent(QKeyEvent* event)
	{
		QTreeWidget::keyPressEvent(event);
		event->accept();
	}

	void EntityBrowserTreeWidget::keyReleaseEvent(QKeyEvent* event)
	{
		QTreeWidget::keyReleaseEvent(event);
		event->accept();
	}

};