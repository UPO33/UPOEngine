#pragma once

#include "UCommonEd.h"


namespace UPOEd
{
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
		QLineEdit* mFilter;
		QTreeWidget* mTree;
		World*	mAttachedWorld;
		EntityBrowserItem* mSelectedEntity;

	public:
		EntityBrowserWidget(QWidget* parent =  nullptr);
		~EntityBrowserWidget();
		void ReFillTree();
		void AttachWorld(World* world);
		void AddToTree(EntityBrowserItem* parentItem);
		void EVContextMenuRequested(const QPoint& pos);
		void SelectEntity(EntityBrowserItem* item);
		void Tick();

	};
	//////////////////////////////////////////////////////////////////////////
	class EntityBrowserDW : public QDockWidget
	{
	public:
		World* mWorld;
		EntityBrowserDW(QWidget* parent = nullptr);

		void AttachWorld(World* world);
		void Tick();
	};
};