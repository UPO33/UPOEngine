#pragma once

#include "UCommonEd.h"


namespace UPOEd
{

	//////////////////////////////////////////////////////////////////////////
	class EntityBrowserTreeWidget : public QTreeWidget
	{
		Q_OBJECT

	public:
		EntityBrowserTreeWidget(QWidget* parent = nullptr);
		~EntityBrowserTreeWidget();

		virtual void dropEvent(QDropEvent *event) override;

		virtual void dragEnterEvent(QDragEnterEvent *event) override;

		virtual void dragMoveEvent(QDragMoveEvent *event) override;

		virtual void dragLeaveEvent(QDragLeaveEvent *event) override;

	};

	//////////////////////////////////////////////////////////////////////////
	class EntityBrowserItem : public QTreeWidgetItem
	{
	public:
		TObjectPtr<Entity>	mEntity;

		EntityBrowserItem(Entity* entity, EntityBrowserItem* parent);
		~EntityBrowserItem();
	};

	//////////////////////////////////////////////////////////////////////////
	class EntityBrowserWidget : public QWidget
	{
		QLineEdit*				mFilter;
		QTreeWidget*			mTree;
		World*					mAttachedWorld;
		EntityBrowserItem*		mSelectedEntity;
		bool					mIsEntityCreationPermitted = true;

	public:
		EntityBrowserWidget(QWidget* parent =  nullptr);
		~EntityBrowserWidget();
		void ReFillTree();
		void AttachWorld(World* world);
		void AddToTree(EntityBrowserItem* parentItem);
		void EVContextMenuRequested(const QPoint& pos);
		void SelectEntity(EntityBrowserItem* item);
		void Tick();
		void CreateEntity(ClassInfo* entityClass, EntityBrowserItem* parentMaybeNull);
	};
	//////////////////////////////////////////////////////////////////////////
	class EntityBrowserDW : public QDockWidget
	{
	public:
		EntityBrowserDW(QWidget* parent = nullptr);

		void AttachWorld(World* world);
		void Tick();
	};
};