#include "UPropertyBrowser.h"

namespace UPOEd
{
	


	PropertyBrowserWidget::PropertyBrowserWidget(QWidget* parent /*= nullptr*/) : QWidget(parent)
	{
		setMinimumSize(QSize(300, 300));

		setLayout(new QVBoxLayout);
		mFilter = new QLineEdit(this);
		layout()->addWidget(mFilter);

		connect(mFilter, &QLineEdit::textChanged, this, [&]()
		{
			ReFillTree();
		});

		mTree = new PropertyBrowserTreeWidget(this);
		layout()->addWidget(mTree);
		mTree->setColumnCount(2);
		mTree->setAlternatingRowColors(true);
		mTree->setIndentation(10);
		//mTree->header()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
// 			mTree->setHeaderHidden(true);
		
		QStringList columnsName;
		columnsName << "Property" << "Value";
		mTree->setHeaderLabels(columnsName);
		
// 		mTree->header()->resizeSection(0, 150);
	}
	//////////////////////////////////////////////////////////////////////////
	QWidget* PropertyBrowserWidget::GetWidgetForProp(const PBBaseProp::Param& param)
	{
		auto type = param.mArrayIndex == -1 ? param.mPropertyInfo->GetType() : param.mPropertyInfo->TemplateArgType();
		auto typeinfo = param.mArrayIndex == -1 ? param.mPropertyInfo->GetTypeInfo() : param.mPropertyInfo->TemplateArgTypeInfo();

		switch (type)
		{
		case UPO::EPT_Unknown:
			UASSERT(false);
			break;
		case UPO::EPT_bool:
			return new PBBoolProp(param, mTree);
		case UPO::EPT_int8:
		case UPO::EPT_uint8:
		case UPO::EPT_int16:
		case UPO::EPT_uint16:
		case UPO::EPT_int32:
		case UPO::EPT_uint32:
		case UPO::EPT_int64:
		case UPO::EPT_uint64:
		case UPO::EPT_float:
		case UPO::EPT_double:
			return new PBNumberProp(param, mTree);
		case UPO::EPT_enum:
			return new PBEnumProp(param, mTree);
		case UPO::EPT_TArray:
			return new PBTArrayProp(param, mTree);

		case UPO::EPT_TObjectPtr:
		case UPO::EPT_ObjectPoniter:
		{
			if (param.mPropertyInfo->HasAttrib(EAttribID::EAT_ShowProperties))
				return new PBBaseProp(param, mTree);
			else
				return new PBObjectProp(param, mTree);
		}

		case UPO::EPT_MetaClass:
		{
			UASSERT(typeinfo);
			ClassInfo* ci = typeinfo->Cast<ClassInfo>();

			if (ci == Vec2::GetClassInfoStatic())	return new PBVecProp(param, mTree);
			if (ci == Vec3::GetClassInfoStatic())	return new PBVecProp(param, mTree);
			if (ci == Vec4::GetClassInfoStatic())	return new PBVecProp(param, mTree);

			if (ci == Color::GetClassInfoStatic())	 return new PBColorProp(param, mTree);
			if (ci == Color32::GetClassInfoStatic()) return new PBColorProp(param, mTree);

			if (ci == String::GetClassInfoStatic()) return new PBStringProp(param, mTree);
			if (ci == Name::GetClassInfoStatic())	return new PBNameProp(param, mTree);

			if (ci == Transform::GetClassInfoStatic()) return new PBTransformProp(param, mTree);

			//^^^^^^^^^^^^^^^ new propertywidgets here ^^^^^^^
		}
		}
		return new PBBaseProp(param, mTree);
	}
	//////////////////////////////////////////////////////////////////////////
	bool PropertyBrowserWidget::MetaClassNeedSubProperties(const ClassInfo* ci)
	{
		return true;
		if (ci == Vec2::GetClassInfoStatic()
			|| ci == Vec3::GetClassInfoStatic()
			|| ci == Vec4::GetClassInfoStatic()
			|| ci == Color::GetClassInfoStatic()
			|| ci == Color32::GetClassInfoStatic()
			|| ci == String::GetClassInfoStatic()
			|| ci == Name::GetClassInfoStatic()
			) return false;

		return true;
	}
	bool PrpFilterCheck(const PropertyInfo* prp, const QString& filter)
	{
		bool b = false;
		if (prp->GetType() == EPropertyType::EPT_MetaClass)
		{
			ClassInfo* cls = (ClassInfo*)prp->GetTypeInfo();
			for (size_t i = 0; i < cls->NumProperty(); i++)
			{
				if (PrpFilterCheck(cls->GetProperty(i), filter))
					b = true;
			}
			
		}
		if(!b)
			return UPOEd::FilterCheck(filter, ToQString(prp->GetName()));
	}
	PBBaseProp* PropertyBrowserWidget::AddTreeItem(Object* root, const PropertyInfo* prp, void* instance, QTreeWidgetItem* parentItem, int arrayIndex /*= -1*/)
	{
		QTreeWidgetItem* newItem = new QTreeWidgetItem(parentItem);
		if (arrayIndex == -1)
		{
			//set property name
			newItem->setText(0, prp->GetLegibleName().CStr());
			/////comment
			Attrib attrComment;
			if (prp->GetAttributes().GetAttrib(EAttribID::EAT_Comment, attrComment))
			{
				newItem->setToolTip(0, QString(attrComment.GetString().CStr()));
			}
		}
		else
		{
			newItem->setText(0, QString::asprintf("[%d]", arrayIndex));
		}



		mTree->addTopLevelItem(newItem);


		PBBaseProp* parentItemWidget = nullptr;
		if (parentItem)
		{
			parentItemWidget = (PBBaseProp*)mTree->itemWidget(parentItem, 1);
			//UASSERT(parentItemWidget);
		}



		PBBaseProp::Param param;
		param.mRootObject = root;
		param.mInstance = instance;
		param.mParentProperty = parentItemWidget;
		param.mPropertyInfo = (PropertyInfo*)prp;
		param.mTreeItem = newItem;
		param.mOwner = this;
		param.mArrayIndex = arrayIndex;

		EPropertyType prpType = prp->GetType();
		if (arrayIndex != -1)
			prpType = prp->TemplateArgType();

		const TypeInfo* prpTypeInfo = prp->GetTypeInfo();
		if (arrayIndex != -1)
			prpTypeInfo = prp->TemplateArgTypeInfo();

		void* pValue = prp->Map(instance);
		if (arrayIndex != -1)
			pValue = ((SerArray*)pValue)->GetElement(arrayIndex, prpType, prpTypeInfo);



		QWidget* widget = GetWidgetForProp(param);
		mTree->setItemWidget(newItem, 1, widget);







		switch (prpType)
		{
		case UPO::EPT_Unknown:
			break;
		case UPO::EPT_bool:
			break;
		case UPO::EPT_int8:
		case UPO::EPT_uint8:
		case UPO::EPT_int16:
		case UPO::EPT_uint16:
		case UPO::EPT_int32:
		case UPO::EPT_uint32:
		case UPO::EPT_int64:
		case UPO::EPT_uint64:
		case UPO::EPT_float:
		case UPO::EPT_double:
		case UPO::EPT_enum:
			break;
		case UPO::EPT_TArray:
			break;
		case UPO::EPT_TObjectPtr:
		case UPO::EPT_ObjectPoniter:
		{
			Object* obj = nullptr;
			if (prpType == EPT_ObjectPoniter)
				obj = *((Object**)pValue);
			else
				obj = ((ObjectPtr*)pValue)->Get();

			if (prp->HasAttrib(EAttribID::EAT_ShowProperties) && obj)
			{
				AddPropertiesOfClass(obj, obj->GetClassInfo(), obj, newItem);
			}
		}
		break;
		case UPO::EPT_MetaClass:
		{
			ClassInfo* ci = prpTypeInfo->Cast<ClassInfo>();
			UASSERT(ci);
			// 				ULOG_WARN("CI %s", ci->GetName().CStr());
			// 				if(MetaClassNeedSubProperties(ci))
			{
				AddPropertiesOfClass(root, ci, pValue, newItem);
			}
		}
		break;
		default:
			break;
		}
		return (PBBaseProp*)widget;
	}


	void PropertyBrowserWidget::AttachObject(Object* object)
	{
		mObject = object;
		ReFillTree();
	}
	//////////////////////////////////////////////////////////////////////////
	void PropertyBrowserWidget::AddPropertiesOfClass(Object* root, ClassInfo* theClass, void* instance, QTreeWidgetItem* parentItem)
	{
		SClassChain classChain;
		theClass->GetClassChain(classChain, false, true);
		for (unsigned iClass = 0; iClass < classChain.mNumClass; iClass++)
		{
			auto cls = classChain.mClasses[iClass];
			
			if(cls->HasAttrib(EAttribID::EAT_Hidden)) continue;

			for (unsigned iProperty = 0; iProperty < cls->NumProperty(); iProperty++)
			{
				auto prp = cls->GetProperty(iProperty);

				if (prp->HasAttrib(EAttribID::EAT_Hidden)) continue;

				if(PrpFilterCheck(prp, mFilter->text()))
					AddTreeItem(root, prp, instance, parentItem);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void PropertyBrowserWidget::Tick()
	{
		//if object remove 
		if (!mObject) 
		{
			ReFillTree();
		}
		else if(mObject && mObject->mNeedsPropertyBrowserRefresh)
		{
			mObject->mNeedsPropertyBrowserRefresh = false;
			ReFillTree();
		}

		QTreeWidgetItem* root = mTree->invisibleRootItem();
		TickTreeItem(root);
	}

	void PropertyBrowserWidget::TickTreeItem(QTreeWidgetItem* item)
	{
		QWidget* widget = mTree->itemWidget(item, 1);
		if (widget)
		{
			((PBBaseProp*)widget)->Tick();
		}
		for (int i = 0; i < item->childCount(); i++)
		{
			TickTreeItem(item->child(i));
		}
	}

	void PropertyBrowserWidget::ReFillTree()
	{
		mTree->clear();

		if (Object* obj = mObject)
		{
			AddPropertiesOfClass(obj, obj->GetClassInfo(), obj, nullptr);
		}
		mTree->expandAll();
	}

// 	void PropertyBrowserWidget::mousePressEvent(QMouseEvent *event)
// 	{
// 		ULOG_WARN("");
// 		event->accept();
// 		QWidget::mousePressEvent(event);
// 	}
// 
// 	void PropertyBrowserWidget::mouseReleaseEvent(QMouseEvent *event)
// 	{
// 		ULOG_WARN("");
// 		event->accept();
// 		QWidget::mouseReleaseEvent(event);
// 	}

	PropertyBrowserDW::PropertyBrowserDW(QWidget* parent /*= nullptr*/) : QDockWidget(parent)
	{
		mWidget = new PropertyBrowserWidget();
		setWidget(mWidget);
		this->setWindowTitle("Properties");
	}

	void PropertyBrowserDW::Tick()
	{
		mWidget->Tick();
	}

	void PropertyBrowserDW::AttachObject(Object* object)
	{
		mWidget->AttachObject(object);
	}

	PropertyBrowserTreeWidget::PropertyBrowserTreeWidget(QWidget* parent /*= nullptr*/) : QTreeWidget(parent)
	{

	}

	void PropertyBrowserTreeWidget::mousePressEvent(QMouseEvent *event)
	{
		ULOG_WARN("");
		QTreeWidget::mousePressEvent(event);
	}

	void PropertyBrowserTreeWidget::mouseReleaseEvent(QMouseEvent *event)
	{
		ULOG_WARN("");
		if (event->button() == Qt::MouseButton::MiddleButton)
		{
			if (QTreeWidgetItem* treeItem = this->itemAt(event->pos()))
			{
				if (PBBaseProp* widgetPrp = (PBBaseProp*)(this->itemWidget(treeItem, 1)))
				{
					widgetPrp->ResetToDefault();
					ULOG_WARN("asdasd");
				}
			}
		}
		QTreeWidget::mouseReleaseEvent(event);
	}

};