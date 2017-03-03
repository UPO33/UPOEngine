#include "UPropertyBrowserWidgets.h"
#include "UPropertyBrowser.h"
#include "UAssetBrowser.h"
#include "UMainWindow.h"

#include "ui_TransformProperty.h"

namespace UPOEd
{
	static const unsigned DEFAUL_WIDGETS_SPACE = 2;
	static const unsigned DEFAULT_MARGIN = 2;


	PBTArrayProp::PBTArrayProp(const PBBaseProp::Param& param, QWidget* parent) : PBBaseProp(param, parent)
	{
		UASSERT(mParam.mPropertyInfo->GetType() == EPropertyType::EPT_TArray);
		
		bool isConstantLength = GetPropertyInfo()->HasAttrib(EAttribID::EAT_ArrayConstantLength);
		
		if(!isConstantLength)
		{
			mBtnAdd = new QPushButton();
			mBtnAdd->setText("+");
			layout()->addWidget(mBtnAdd);
			mBtnRemoveAll = new QPushButton;
			mBtnRemoveAll->setText("C");
			layout()->addWidget(mBtnRemoveAll);
		}

		
		ReMakeElements();
		
		if(!isConstantLength)
		{
			connect(mBtnAdd, &QPushButton::clicked, this, [&](bool) {
				// 			WidgetValueChanged();
				// 			mNumNewElement++;
				// 
				ValueAs<SerArray>().AddDefault(1, GetPropertyInfo()->TemplateArgType(), GetPropertyInfo()->TemplateArgTypeInfo());

				// 			size_t index = mChildren.Add();
				// 			mChildren[index] = GetOwner()->AddTreeItem(mParam.mPropertyInfo, mParam.mInstance, mParam.mTreeItem, index);
							//PropertyChanged();
				ReMakeElements();
			});

			connect(mBtnRemoveAll, &QPushButton::clicked, this, [&](bool) {
				// 			WidgetValueChanged();
				// 			mDoClear = true;

				ValueAs<SerArray>().RemoveAll(GetPropertyInfo()->TemplateArgType(), GetPropertyInfo()->TemplateArgTypeInfo());

				for (size_t i = 0; i < mChildren.Length(); i++)
				{
					delete mChildren[i]->mParam.mTreeItem;
				}
				mChildren.RemoveAll();

				//PropertyChanged();
			});
		}

	}

	void PBTArrayProp::ReMakeElements()
	{
		for (size_t i = 0; i < mChildren.Length(); i++)
		{
			delete mChildren[i]->mParam.mTreeItem;
		}
		
		mLength = ValueAs<SerArray>().mLength;

		mChildren.RemoveAll();
		mChildren.AddDefault(mLength);

		for (int i = 0; i < mLength; i++)
		{
			mChildren[i] = GetOwner()->AddTreeItem(mParam.mRootObject, mParam.mPropertyInfo, mParam.mInstance, mParam.mTreeItem, i);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void PBTArrayProp::UpdatePropertyValue()
	{
// 		if (mDoClear)
// 		{
// 			mLength = 0;
// 			ValueAs<SerArray>().RemoveAll(GetPropertyInfo()->TemplateArgType(), GetPropertyInfo()->TemplateArgTypeInfo());
// 			ReMakeElements();
// 		}
// 		else if (mNumNewElement)
// 		{
// 			ValueAs<SerArray>().AddDefault(mNumNewElement, GetPropertyInfo()->TemplateArgType(), GetPropertyInfo()->TemplateArgTypeInfo());
// 			mLength += mNumNewElement;
// 			ReMakeElements();
// 		}
// 		mDoClear = false;
// 		mNumNewElement = 0;
		
	}
	//////////////////////////////////////////////////////////////////////////
	void PBTArrayProp::UpdateWidgetValue()
	{
// 		if (mLength != ValueAs<SerArray>().mLength)
// 		{
// 			ReMakeElements();
// 		}
	}

	void PBBaseProp::WidgetValueChanged()
	{
		if(!mGameIsEditing)
			mWidgetValueChanged = true;
	}

	PBBaseProp::PBBaseProp(const Param& param, QWidget* parent) : QWidget(parent), mParam(param)
	{
		setLayout(new QHBoxLayout);
		layout()->setMargin(DEFAULT_MARGIN);
		layout()->setSpacing(DEFAUL_WIDGETS_SPACE);

		//set disabled if has EAT_Uneditable attribute
		if (param.mPropertyInfo->GetAttributes().HasAttrib(EAttribID::EAT_Uneditable))
			this->setDisabled(true);
		//set tooltip to name of property's type
		if (param.mArrayIndex != -1)
			setToolTip(ToQString(param.mPropertyInfo->TemplateArgTypeName()));
		else
			setToolTip(ToQString(param.mPropertyInfo->GetTypeName()));
	}

	PropertyInfo* PBBaseProp::GetPropertyInfo() const
	{
		return mParam.mPropertyInfo;
	}

	TypeInfo* PBBaseProp::GetTypeInfo() const
	{
		if (mParam.mArrayIndex == -1) return mParam.mPropertyInfo->GetTypeInfo();
		else return mParam.mPropertyInfo->TemplateArgTypeInfo();
	}

	UPO::EPropertyType PBBaseProp::GetType() const
	{
		if (mParam.mArrayIndex == -1) return mParam.mPropertyInfo->GetType();
		else return mParam.mPropertyInfo->TemplateArgType();
	}

	PropertyBrowserWidget* PBBaseProp::GetOwner() const
	{
		return mParam.mOwner;
	}

	void* PBBaseProp::Map() const
	{
		void* pValue = mParam.mPropertyInfo->Map(mParam.mInstance);
		if (mParam.mArrayIndex != -1 && mParam.mPropertyInfo->GetType() == EPropertyType::EPT_TArray) // is TArray element ?
		{
			SerArray* arr = (SerArray*)pValue;
			pValue = arr->GetElement(mParam.mArrayIndex, mParam.mPropertyInfo->TemplateArgType(), mParam.mPropertyInfo->TemplateArgTypeInfo());
		}
		return pValue;
	}



	// 	void PBBaseProp::PropertyChanged()
// 	{
// 		if (mParam.mPropertyInfo->GetOwner()->HasMetaPropertyChanged())
// 				mParam.mPropertyInfo->GetOwner()->CallMetaPropertyChanged(mParam.mInstance, mParam.mPropertyInfo);
// 		if (mParam.mParentProperty)
// 			mParam.mParentProperty->PropertyChanged();
// 	}

	void PBBaseProp::BeforePropertyChange()
	{
		TArray<ClassInfo*> classes;
		mParam.mRootObject->GetClassInfo()->GetInheritedClasses(classes, true);
		//mParam.mPropertyInfo->GetOwner()->GetInheritedClasses(classes, true);
		for (ClassInfo* item : classes)
		{
			if (item->Has_MetaBeforePropertyChange())
				item->Call_MetaBeforePropertyChange(mParam.mInstance, mParam.mPropertyInfo);
		}
		
		mParam.mOwner->mOnMetaBeforePropertyChange.InvokeAll(mParam.mPropertyInfo);

		if (mParam.mParentProperty)
			mParam.mParentProperty->BeforePropertyChange();
	}

	void PBBaseProp::AfterPropertyChange()
	{
		TArray<ClassInfo*> classes;
		mParam.mRootObject->GetClassInfo()->GetInheritedClasses(classes, true);
		//mParam.mPropertyInfo->GetOwner()->GetInheritedClasses(classes, true);

		for (ClassInfo* item : classes)
		{
			if (item->Has_MetaAfterPropertyChange())
				item->Call_MetaAfterPropertyChange(mParam.mInstance, mParam.mPropertyInfo);
		}

		mParam.mOwner->mOnMetaAfterPropertyChange.InvokeAll(mParam.mPropertyInfo);

		if (mParam.mParentProperty)
			mParam.mParentProperty->AfterPropertyChange();
	}

	void PBBaseProp::Tick()
	{
		if (mWidgetValueChanged)
		{
			BeforePropertyChange();
			UpdatePropertyValue();
			AfterPropertyChange();
		}
		else
		{
			mGameIsEditing = true;
			UpdateWidgetValue();
			mGameIsEditing = false;
		}
		mWidgetValueChanged = false;
	}

	void PBBaseProp::UpdatePropertyValue()
	{

	}

	void PBBaseProp::UpdateWidgetValue()
	{

	}

	void PBBaseProp::ResetToDefault()
	{
		//can't reset a TArray elements
		if (mParam.mArrayIndex != -1)return;

		if (Object* obj = mParam.mRootObject)
		{
			Object* defaulObj = NewObject(obj->GetClassInfo());
			ULOG_MESSAGE("obj %", defaulObj->GetClassInfo()->GetName());
			auto offset = ((size_t)mParam.mInstance) - ((size_t)(mParam.mRootObject.Get()));
			auto origInstance = mParam.mInstance;
			mParam.mInstance = (void*)(((size_t)defaulObj) + offset);
			mParam.mRootObject = defaulObj;
			UpdateWidgetValue();
			mWidgetValueChanged = true;
			mParam.mInstance = origInstance;
			mParam.mRootObject = obj;
			DeleteObject(defaulObj);
			
			
		}
	}

	PBTransformProp::PBTransformProp(const PBBaseProp::Param& param, QWidget* parent) : PBBaseProp(param, parent)
	{
		delete layout();
		ui = new Ui_TransformProperty;
		ui->setupUi(this);
		
		mTransform = ValueAs<Transform>();
		SetWidgetsValue(mTransform);
	

		QDoubleSpinBox* spinBoxes[] = { ui->mPX, ui->mPY, ui->mPZ,	 ui->mRX, ui->mRY, ui->mRZ,		ui->mSX, ui->mSY, ui->mSZ };
		for (QDoubleSpinBox* spinbox : spinBoxes)
		{
			connect(spinbox, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [&](double value) {
				WidgetValueChanged();
			});
		}
	}

	void PBTransformProp::UpdatePropertyValue()
	{
		Vec3 position = Vec3(ui->mPX->value(), ui->mPY->value(), ui->mPZ->value());
		Vec3 rotation = Vec3(ui->mRX->value(), ui->mRY->value(), ui->mRZ->value());
		Vec3 scale = Vec3(ui->mSX->value(), ui->mSY->value(), ui->mSZ->value());

		Transform newTransform;
		newTransform.MakeTransform(position, rotation, scale);

		mTransform = newTransform;

		ValueAs<Transform>() = newTransform;
	}

	void PBTransformProp::UpdateWidgetValue()
	{
		auto& transform = ValueAs<Transform>();

		if (transform.IsEqual(mTransform, 0.0001)) return;

		mTransform = transform;
		SetWidgetsValue(mTransform);
	}

	void PBTransformProp::SetWidgetsValue(const Transform& transform)
	{
		Vec3 position = transform.GetTranslation();
		Vec3 rotation = transform.GetRotationEuler();
		Vec3 scale = transform.GetScale();

		ui->mPX->setValue(position.mX);
		ui->mPY->setValue(position.mY);
		ui->mPZ->setValue(position.mZ);

		ui->mRX->setValue(rotation.mX);
		ui->mRY->setValue(rotation.mY);
		ui->mRZ->setValue(rotation.mZ);

		ui->mSX->setValue(scale.mX);
		ui->mSY->setValue(scale.mY);
		ui->mSZ->setValue(scale.mZ);
	}

	PBObjectProp::PBObjectProp(const PBBaseProp::Param& param, QWidget* parent) : PBBaseProp(param, parent)
	{
		setAcceptDrops(true);

		mBtn = new QCommandLinkButton(this);
		mBtn->setIcon(QIcon());
		if (auto icon = GetIcon(this->GetObjectClass())) mBtn->setIcon(*icon);


		UpdateWidgetValue();

		connect(mBtn, &QCommandLinkButton::clicked, this, [this](bool) {
			ULOG_MESSAGE("click");
			if (auto obj = GetValueObject())
			{
				if (auto asset = obj->Cast<Asset>()) //is asset?
				{
				}
				else if (auto entity = obj->Cast<Entity>()) // is entity?
				{

				}
			}
		});

		layout()->addWidget(mBtn);
	}

	void PBObjectProp::UpdatePropertyValue()
	{
		SetValueObject(mObject);
	}

	void PBObjectProp::UpdateWidgetValue()
	{
		if (Object* obj = GetValueObject())
		{
			mObject = obj;
			if(obj->GetClassInfo()->IsBaseOf(GetObjectClass()))
			{
				if (auto asset = obj->Cast<Asset>())
				{
					mBtn->setText(ToQString(asset->GetName()));
				}
				else if (auto entity = obj->Cast<Entity>())
				{
					mBtn->setText(ToQString(entity->GetName()));
				}
			}
		}
		else
		{
			mBtn->setText("null");
			mObject = nullptr;
		}
	}

	void PBObjectProp::dragEnterEvent(QDragEnterEvent *event)
	{
		ULOG_WARN("%", event->source()->parent()->metaObject()->className());
		
		mObjectToAssign = nullptr;
		if (auto assetBrowser = qobject_cast<AssetBrowserWidget*>(event->source()->parent()))
		{
			if (auto curItem = (AssetBrowserItem*)assetBrowser->GetAssetsTreeWidget()->currentItem())
			{
				if (auto droppedCLass = curItem->mAssetEntry->GetClassInfo())
				{
					if (droppedCLass->IsBaseOf(GetObjectClass()))
					{
						curItem->mAssetEntry->LoadNow(this->mParam.mRootObject);
						mObjectToAssign = curItem->mAssetEntry->GetInstance();
						event->setDropAction(Qt::DropAction::IgnoreAction);
						event->accept();
						return;
					}

				}

			}
		}
		event->ignore();
	}

	void PBObjectProp::dropEvent(QDropEvent *event)
	{
		ULOG_WARN("");
		if (mObjectToAssign)
		{
			mObject = mObjectToAssign;
			WidgetValueChanged();
			mObjectToAssign = nullptr;
		}
		event->ignore();
		
	}

	void PBObjectProp::dragMoveEvent(QDragMoveEvent *event)
	{
		//ULOG_MESSAGE("");
		event->accept();
	}

	const ClassInfo* PBObjectProp::GetObjectClass()
	{
		//is not array element ?
		if (mParam.mArrayIndex == -1)
		{
			if (mParam.mPropertyInfo->GetType() == EPropertyType::EPT_ObjectPoniter)
			{
				if (auto ti = mParam.mPropertyInfo->GetTypeInfo())
					return ti->Cast<ClassInfo>();
			}
			else if (mParam.mPropertyInfo->GetType() == EPropertyType::EPT_TObjectPtr)
			{
				if (auto ti = mParam.mPropertyInfo->TemplateArgTypeInfo())
					return ti->Cast<ClassInfo>();
			}
			else
				ULOG_FATAL("");
		}
		else
		{
			return mParam.mPropertyInfo->TemplateArgTypeInfo()->Cast<ClassInfo>();
		}
		return nullptr;
// 		return GetTypeInfo()->Cast<ClassInfo>();
// 
// 		bool isTObjectPtr = GetType() == EPropertyType::EPT_TObjectPtr;
// 		if (isTObjectPtr)
// 			return GetTypeInfo()->Cast<ClassInfo>()->TemplateArgTypeInfo()->Cast<ClassInfo>();
// 		else
// 			return GetTypeInfo()->GetTypeInfo()->Cast<ClassInfo>();
	}

	Object* PBObjectProp::GetValueObject()
	{
		if (GetType() == EPropertyType::EPT_TObjectPtr)
			return ValueAs<ObjectPtr>().Get();
		else if (GetType() == EPropertyType::EPT_ObjectPoniter)
			return ValueAs<Object*>();
		else
			UASSERT(false);
	}

	void PBObjectProp::SetValueObject(Object* obj)
	{
		if (GetType() == EPropertyType::EPT_TObjectPtr)
			ValueAs<ObjectPtr>() = obj;
		else if (GetType() == EPropertyType::EPT_ObjectPoniter)
			ValueAs<Object*>() = obj;
		else
			UASSERT(false);

// 		if (obj)
// 		{
// 			if (auto asset = obj->Cast<Asset>())
// 			{
// 				mBtn->setText(ToQString(asset->GetName()));
// 			}
// 			else if (auto entity = obj->Cast<Entity>())
// 			{
// 				mBtn->setText(ToQString(entity->GetName()));
// 			}
// 		}
// 		else
// 		{
// 			mBtn->setText("null");
// 		}
	}


};