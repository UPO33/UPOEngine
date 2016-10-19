#include "UPropertyBrowserWidgets.h"
#include "UPropertyBrowser.h"

namespace UPOEd
{

	PBTArrayProp::PBTArrayProp(const PBBaseProp::Param& param, QWidget* parent) : PBBaseProp(param, parent)
	{
		UASSERT(mParam.mPropertyInfo->GetType() == EPropertyType::EPT_TArray);

		mBtnAdd = new QPushButton();
		mBtnAdd->setText("+");
		layout()->addWidget(mBtnAdd);
		mBtnRemoveAll = new QPushButton;
		mBtnRemoveAll->setText("C");
		layout()->addWidget(mBtnRemoveAll);

		
		ReMakeElements();
		
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
			mChildren[i] = GetOwner()->AddTreeItem(mParam.mPropertyInfo, mParam.mInstance, mParam.mTreeItem, i);
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

	void PBBaseProp::PropertyChanged()
	{
		if (mParam.mPropertyInfo->GetOwner()->HasMetaPropertyChanged())
				mParam.mPropertyInfo->GetOwner()->CallMetaPropertyChanged(mParam.mInstance, mParam.mPropertyInfo);
		if (mParam.mParentProperty)
			mParam.mParentProperty->PropertyChanged();
	}

};