#pragma once

#include "UCommonEd.h"


namespace UPOEd
{
	//////////////////////////////////////////////////////////////////////////
	class EntityBrowserWidget : public QWidget
	{
		QLineEdit* mFilter;
		QTreeWidget* mTree;
		TObjectPtr<World>	mAttachedWorld;
	public:
		EntityBrowserWidget(QWidget* parent =  nullptr);
		~EntityBrowserWidget();
		void ReFillTree();
	};
	//////////////////////////////////////////////////////////////////////////
	class EntityBrowserDW : public QDockWidget
	{
	public:
		World* mWorld;
		EntityBrowserDW(QWidget* parent = nullptr);

		void AttachWorld(World* world);
	};
};