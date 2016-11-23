#pragma once

#include "UPropertyBrowserWidgets.h"


namespace UPOEd
{
	//////////////////////////////////////////////////////////////////////////
	class PropertyBrowserWidget : public QWidget
	{
		TObjectPtr<Object> mObject = nullptr;

		QLineEdit*		mFilter;
		QTreeWidget*	mTree;

	public:
		PropertyBrowserWidget(QWidget* parent = nullptr);
		QWidget* GetWidgetForProp(const PBBaseProp::Param& param);
		bool MetaClassNeedSubProperties(const ClassInfo* ci);

		QTreeWidget* GetTree() const { return mTree; }
		//////////////////////////////////////////////////////////////////////////
		PBBaseProp* AddTreeItem(const PropertyInfo* prp, void* instance, QTreeWidgetItem* parentItem, int arrayIndex = -1);
		PBBaseProp* CreateProperty(PBBaseProp* parent);
		//////////////////////////////////////////////////////////////////////////
		void AttachObject(Object* object);
		//////////////////////////////////////////////////////////////////////////
		void AddPropertiesOfClass(ClassInfo* theClass, void* instance, QTreeWidgetItem* parentItem);
		//////////////////////////////////////////////////////////////////////////
		void Tick();
		//////////////////////////////////////////////////////////////////////////
		void TickTreeItem(QTreeWidgetItem* item);
		//////////////////////////////////////////////////////////////////////////
		void ReFillTree();
	};


	//////////////////////////////////////////////////////////////////////////
	class PropertyBrowserDW : public QDockWidget
	{
		PropertyBrowserWidget* mWidget;

	public:
		PropertyBrowserDW(QWidget* parent = nullptr);
		void Tick();
		void AttachObject(Object* object);
	};
};
