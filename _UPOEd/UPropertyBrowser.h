#pragma once

#include "UPropertyBrowserWidgets.h"
#include "../UPOEngine/Core/UDelegate.h"

namespace UPOEd
{
	//////////////////////////////////////////////////////////////////////////
	class PropertyBrowserTreeWidget : public QTreeWidget
	{
		Q_OBJECT
	public:
		PropertyBrowserTreeWidget(QWidget* parent = nullptr);
	protected:
		virtual void mousePressEvent(QMouseEvent *event) override;
		virtual void mouseReleaseEvent(QMouseEvent *event) override;

	};


	//////////////////////////////////////////////////////////////////////////
	class PropertyBrowserWidget : public QWidget
	{
		Q_OBJECT

		TObjectPtr<Object> mObject = nullptr;

		QLineEdit*		mFilter;
		QTreeWidget*	mTree;

	public:
		TDelegateMulti<void, const PropertyInfo*> mOnMetaBeforePropertyChange;
		TDelegateMulti<void, const PropertyInfo*> mOnMetaAfterPropertyChange;

		PropertyBrowserWidget(QWidget* parent = nullptr);
		QWidget* GetWidgetForProp(const PBBaseProp::Param& param);
		bool MetaClassNeedSubProperties(const ClassInfo* ci);

		QLineEdit* GetFilterWidget() const { return mFilter; }
		QTreeWidget* GetTreeWidget() const { return mTree; }
		//////////////////////////////////////////////////////////////////////////
		PBBaseProp* AddTreeItem(Object* root, const PropertyInfo* prp, void* instance, QTreeWidgetItem* parentItem, int arrayIndex = -1);
		//////////////////////////////////////////////////////////////////////////
		void AttachObject(Object* object);
		//////////////////////////////////////////////////////////////////////////
		void AddPropertiesOfClass(Object* root, ClassInfo* theClass, void* instance, QTreeWidgetItem* parentItem);
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
		PropertyBrowserWidget * GetWidget() { return mWidget; }
	};
};
