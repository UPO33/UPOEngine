#pragma once

#include "UCommonEd.h"
#include "../UPOEngine/Meta/UMeta.h"
#include "UColorSelectionDialog.h"

//////////////////////////////////////////////////////////////////////////
class Ui_TransformProperty;

namespace UPOEd
{
	//////////////////////////////////////////////////////////////////////////
	class PropertyBrowserWidget;

	//////////////////////////////////////////////////////////////////////////
	static const float PROPERTY_FLOAT_COMPARISON = 0.00001f;

	//////////////////////////////////////////////////////////////////////////
	//base widget for properties item, such as float, String, ...
	class PBBaseProp : public QWidget
	{
	public:
		

		struct Param
		{
			PropertyBrowserWidget* mOwner = 0;
			void* mInstance;	//the instance that has this property
			PropertyInfo* mPropertyInfo;
			TObjectPtr<Object> mRootObject;
			PBBaseProp*	mParentProperty;
			QTreeWidgetItem* mTreeItem;
			int	mArrayIndex = -1;	//-1 means its not array's element
		};

		Param mParam;

		bool mWidgetValueChanged = false;
		bool mGameIsEditing = false;

		void WidgetValueChanged();

		PBBaseProp(const Param& param, QWidget* parent);

		PropertyInfo* GetPropertyInfo() const;
		TypeInfo* GetTypeInfo() const;
		//get the type of property. if its TArray's Element the return value is type of array's element 
		EPropertyType GetType() const;
		PropertyBrowserWidget* GetOwner() const;

		//get pointer to value, for instance if its bool property the return value is actually bool*
		//if the property is element of TArray it return pointer to element as well
		void* Map() const;
		template <typename T> T& ValueAs()
		{
			return *((T*)Map());
		}

		template <typename T> T& GetValue()
		{
			return *((T*)Map());
		}

		template <typename T> void SetValue(T value)
		{
			*((T*)Map()) = value;
		}
		void BeforePropertyChange();
		void AfterPropertyChange();

		virtual void Tick();

		//if WidgetValueChanged() was called this is called to update property value from widget value
		virtual void UpdatePropertyValue();
		//update widget value from property, called every tick, its better to check whether property value have changed or not instead of 
		//setting the widget's value every tick
		virtual void UpdateWidgetValue();
	};

	//////////////////////////////////////////////////////////////////////////
	class PBBoolProp : public PBBaseProp
	{
		QCheckBox* mCheckBox;


	public:
		PBBoolProp(const PBBaseProp::Param& param, QWidget* parent) : PBBaseProp(param, parent)
		{
			mCheckBox = new QCheckBox(this);
			layout()->addWidget(mCheckBox);
			mCheckBox->setText("");
			mCheckBox->setCheckable(true);

			UpdateWidgetValue();

			connect(mCheckBox, &QCheckBox::stateChanged, this, [&](int state)
			{
				WidgetValueChanged();
			});
		}
		void UpdatePropertyValue() override
		{
			ValueAs<bool>() = mCheckBox->checkState() == Qt::CheckState::Checked;
		}
		void UpdateWidgetValue() override
		{
			mCheckBox->setChecked(ValueAs<bool>());
		}
	};

	class PBNumberProp : public PBBaseProp
	{
		static const int PRECISION_DECIMAL_FLOAT = 4;
		static const int PRECISION_DECIMAL_DOUBLE = 5;

		QDoubleSpinBox* mSpinBox;
		double mValue = 0;

	public:
		PBNumberProp(const PBBaseProp::Param& param, QWidget* parent) : PBBaseProp(param, parent)
		{
			float typeMin = 0;
			float typeMax = 0;
			int decimals = 0;
			switch (GetType())
			{
			case EPT_uint8: typeMin = 0;	typeMax = 0xFF;	break;
			case EPT_uint16: typeMin = 0;	typeMax = 0xFFff;	break;
			case EPT_uint32: typeMin = 0;	typeMax = 0xFFffFFff;	break;
			case EPT_uint64: typeMin = 0;	typeMax = 0xFFffFFff;	break;

			case EPT_int8: typeMin = SCHAR_MIN;	typeMax = SCHAR_MAX;	break;
			case EPT_int16: typeMin = SHRT_MIN;	typeMax = SHRT_MAX;	break;
			case EPT_int32: typeMin = INT_MIN;	typeMax = INT_MAX;	break;
			case EPT_int64: typeMin = INT_MIN;	typeMax = INT_MAX;	break;

			case EPT_float: typeMin = FLT_MIN;	typeMax = FLT_MAX; decimals = PRECISION_DECIMAL_FLOAT;	break;
			case EPT_double: typeMin = FLT_MIN;	typeMax = FLT_MAX; decimals = PRECISION_DECIMAL_DOUBLE;	break;
				break;
			}

			float min = typeMin;
			float max = typeMax;
			Attrib rangeAttr;
			if (GetPropertyInfo()->GetAttributes().GetAttrib(EAT_Range, rangeAttr))
			{
				min = Clamp(rangeAttr.GetValue0(), typeMin, typeMax);
				max = Clamp(rangeAttr.GetValue1(), typeMin, typeMax);
			}

			mSpinBox = new QDoubleSpinBox(this);
			layout()->addWidget(mSpinBox);

			mSpinBox->setMinimum(min);
			mSpinBox->setMaximum(max);
			mSpinBox->setDecimals(decimals);
			if (decimals == 0) //for integer single step is 1
				mSpinBox->setSingleStep(1);
			else
				mSpinBox->setSingleStep(Min((max - min)/500.0f, 1.0f));
			mSpinBox->setAccelerated(true);


			UpdateWidgetValue();


			connect(mSpinBox, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [&](double value) {
				WidgetValueChanged();
				mValue = value;
			});
		}
		~PBNumberProp()
		{
		}
		void UpdateWidgetValue() override
		{
			double curPrpValue = 0;
			switch (GetType())
			{
			case EPT_uint8: curPrpValue = (double)ValueAs<uint8>(); break;
			case EPT_uint16: curPrpValue = (double)ValueAs<uint16>(); break;
			case EPT_uint32: curPrpValue = (double)ValueAs<uint32>(); break;
			case EPT_uint64: curPrpValue = (double)ValueAs<uint64>(); break;

			case EPT_int8: curPrpValue = (double)ValueAs<int8>(); break;
			case EPT_int16: curPrpValue = (double)ValueAs<int16>(); break;
			case EPT_int32: curPrpValue = (double)ValueAs<int32>(); break;
			case EPT_int64: curPrpValue = (double)ValueAs<int64>(); break;

			case EPT_float: curPrpValue = (double)ValueAs<float>(); break;
			case EPT_double: curPrpValue = (double)ValueAs<double>(); break;
			}

			if (Abs(curPrpValue - mValue) >= PROPERTY_FLOAT_COMPARISON)
				mSpinBox->setValue(curPrpValue);
		}
		void UpdatePropertyValue() override
		{
			switch (GetType())
			{
			case EPT_uint8: ValueAs<uint8>() = mValue; break;
			case EPT_uint16: ValueAs<uint16>() = mValue; break;
			case EPT_uint32: ValueAs<uint32>() = mValue; break;
			case EPT_uint64: ValueAs<uint64>() = mValue; break;

			case EPT_int8: ValueAs<int8>() = mValue; break;
			case EPT_int16: ValueAs<int64>() = mValue; break;
			case EPT_int32: ValueAs<int64>() = mValue; break;
			case EPT_int64: ValueAs<int64>() = mValue; break;

			case EPT_float: ValueAs<float>() = mValue; break;
			case EPT_double: ValueAs<double>() = mValue; break;
			}
		}
	};

	//////////////////////////////////////////////////////////////////////////
	class PBStringProp : public PBBaseProp
	{
		QLineEdit* mLineEdit;
	public:
		PBStringProp(const PBBaseProp::Param& param, QWidget* parent) : PBBaseProp(param, parent)
		{
			mLineEdit = new QLineEdit(this);
			layout()->addWidget(mLineEdit);

			mLineEdit->setClearButtonEnabled(true);

			UpdateWidgetValue();

			connect(mLineEdit, &QLineEdit::textChanged, this, [&](const QString& str) {
				WidgetValueChanged();
			});


		}
		void UpdateWidgetValue() override
		{
			mLineEdit->setText(ToQString(ValueAs<String>()));
		}
		void UpdatePropertyValue() override
		{
			if (mLineEdit->text().isEmpty())
				ValueAs<String>() = String();
			else
				ValueAs<String>() = ToString(mLineEdit->text());
		}
	};
	//////////////////////////////////////////////////////////////////////////
	class PBNameProp : public PBBaseProp
	{
		QLineEdit* mLineEdit;
	public:
		PBNameProp(const PBBaseProp::Param& param, QWidget* parent) : PBBaseProp(param, parent)
		{
			mLineEdit = new QLineEdit(this);
			layout()->addWidget(mLineEdit);

			mLineEdit->setClearButtonEnabled(true);

			connect(mLineEdit, &QLineEdit::textChanged, this, [&](const QString& str) {
				WidgetValueChanged();
			});
		}

		void UpdateWidgetValue() override
		{
			mLineEdit->setText(ToQString(ValueAs<Name>()));
		}
		void UpdatePropertyValue() override
		{
			ValueAs<Name>() = ToName(mLineEdit->text());
		}
	};
	
	//////////////////////////////////////////////////////////////////////////
	//property for Vec2, Vec3, Vec4
	class PBVecProp : public PBBaseProp
	{
		static const int PRECISION_DECIMAL = 4;
		QDoubleSpinBox* mSpinBox[4];
		unsigned mLength;
	public:
		PBVecProp(const PBBaseProp::Param& param, QWidget* parent) : PBBaseProp(param, parent)
		{
			float minValue = FLT_MIN;
			float maxValue = FLT_MAX;
			Attrib attrRange;
			if (GetPropertyInfo()->GetAttributes().GetAttrib(EAT_Range, attrRange))
			{
				minValue = attrRange.GetValue0();
				maxValue = attrRange.GetValue1();
			}

			if (GetTypeInfo() == Vec2::GetClassInfoStatic()) mLength = 2;
			else if (GetTypeInfo() == Vec3::GetClassInfoStatic()) mLength = 3;
			else if (GetTypeInfo() == Vec4::GetClassInfoStatic()) mLength = 4;
			else UASSERT(false);

			for (unsigned i = 0; i < mLength; i++)
			{
				mSpinBox[i] = new QDoubleSpinBox(this);
				mSpinBox[i]->setMinimum(minValue);
				mSpinBox[i]->setMaximum(maxValue);
				mSpinBox[i]->setSingleStep(Min((maxValue - minValue) / 500.0f, 1.0f));
				mSpinBox[i]->setAccelerated(true);
				mSpinBox[i]->setDecimals(PRECISION_DECIMAL);

				layout()->addWidget(mSpinBox[i]);
			}

			UpdateWidgetValue();
			
			for (unsigned i = 0; i < mLength; i++)
			{
				connect(mSpinBox[i], (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [&](double value) {
					WidgetValueChanged();
				});
			}
		}
		void UpdateWidgetValue() override
		{
			struct F4 { float xyzw[4]; };
			for (unsigned i = 0; i < mLength; i++)
			{
				F4& v = ValueAs<F4>();
				if (Abs(mSpinBox[i]->value() - v.xyzw[i]) > PROPERTY_FLOAT_COMPARISON)
				{
					mSpinBox[i]->setValue(v.xyzw[i]);
				}
			}
		}
		void UpdatePropertyValue() override
		{
			struct F4 { float xyzw[4]; };
			for (unsigned i = 0; i < mLength; i++)
				ValueAs<F4>().xyzw[i] = mSpinBox[i]->value();
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//property for Color && Color32
	class PBColorProp : public PBBaseProp
	{
		QColor mPreColor;
		QColor mColor;
		bool mIs32BitColor;

	public:
		PBColorProp(const PBBaseProp::Param& param, QWidget* parent) : PBBaseProp(param, parent)
		{
			setAutoFillBackground(true);

			if (GetTypeInfo() == Color::GetClassInfoStatic()) mIs32BitColor = false;
			else if (GetTypeInfo() == Color32::GetClassInfoStatic()) mIs32BitColor = true;
			else UASSERT(false);

			mColor = mPreColor = QColor(0, 0, 0);

			UpdateWidgetValue();
		}
		void UpdatePropertyValue() override
		{
			if (mIs32BitColor)
				ValueAs<Color32>() = ToColor32(mColor);
			else
				ValueAs<Color>() = ToColor(mColor);
		}
		void UpdateWidgetValue() override
		{
			if (mIs32BitColor)
				SetWidgetQColor(ToQColor(ValueAs<Color32>()));
			else
				SetWidgetQColor(ToQColor(ValueAs<Color>()));
		}
		void SetWidgetQColor(QColor color)
		{
			if(mColor != color)
			{
				setStyleSheet("background-color:black;");
				QString style = QString::asprintf("background-color: rgba(%d, %d, %d, 255);", color.red(), color.green(), color.blue());
				setStyleSheet(style);
			}
			mColor = color;
		}
		virtual void mouseReleaseEvent(QMouseEvent * event) override
		{
			if (event->button() == Qt::MouseButton::LeftButton)
			{
				mPreColor = mColor;
				ColorSelectionDialog::OpenNow(mColor);

				ColorSelectionDialog::OnRejected(this, [&]() {
					SetWidgetQColor(mPreColor);
					WidgetValueChanged();
				});
				ColorSelectionDialog::OnAccepted(this, [&]() {
					
				});
				ColorSelectionDialog::OnColorChanged(this, [&](const QColor& color) {
					SetWidgetQColor(color);
					WidgetValueChanged();
				});
			}
		}

	};

	////////////////////////////////////////////////////////////////////////////
	////TODO bitflag for enum
	class PBEnumProp : public PBBaseProp
	{
		QComboBox* mElements = nullptr;
		QLineEdit* mLineEdit = nullptr; //show value of property if is not registered enum
		bool mIsBitFlag = false;

	public:
		PBEnumProp(const PBBaseProp::Param& param, QWidget* parent) : PBBaseProp(param, parent)
		{
			EnumInfo* enumInf = (EnumInfo*)GetTypeInfo();
			
			if (enumInf) // is a registered enum ?
			{
				mIsBitFlag = enumInf->IsBitFlag();

				mElements = new QComboBox(this);
				layout()->addWidget(mElements);

				//////////set comment as tooltip if exist
				if (enumInf->GetComment()) mElements->setToolTip(enumInf->GetComment());

				////////filling combobox
				QStringList elemsName;
				for (size_t i = 0; i < enumInf->NumElement(); i++)
				{
					elemsName << QString(enumInf->GetElement(i)->LegibleName().CStr());
				}
				mElements->addItems(elemsName);

				UpdateWidgetValue();

				connect(mElements, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
					WidgetValueChanged();
				});
			}
			else
			{
				mLineEdit = new QLineEdit(this);
				layout()->addWidget(mLineEdit);
				mLineEdit->setDisabled(true);
			}
		}
		void UpdatePropertyValue() override
		{
			EnumInfo* enumInf = (EnumInfo*)GetTypeInfo();
			if (enumInf)
			{
				if (auto elem = enumInf->GetElement(mElements->currentIndex()))
				{
					ValueAs<unsigned>() = elem->Value();
				}
				else
				{
					ULOG_FATAL("selected index is not in range of the enum's elements");
				}
			}
			else
			{

			}
		}
		void UpdateWidgetValue() override
		{
			EnumInfo* enumInf = (EnumInfo*)GetTypeInfo();
			if (enumInf) 
			{
				unsigned index = GetValuIndex();
				index = index != ~0 ? index : 0;
				mElements->setCurrentIndex(index);
			}
			else
			{
				mLineEdit->setText(QString::number(ValueAs<unsigned>()));
			}
		}
		//get corresponding index of property's value
		unsigned GetValuIndex()
		{
			EnumInfo* enumInf = (EnumInfo*)GetTypeInfo();
			return enumInf->FindElement(ValueAs<unsigned>());
		}
	};

	//////////////////////////////////////////////////////////////////////////
	class PBTArrayProp : public PBBaseProp
	{
		QPushButton* mBtnAdd;
		QPushButton* mBtnRemoveAll;
		bool mDoClear = false;
		bool mNumNewElement = 0;
		size_t mLength = 0;
		TArray<PBBaseProp*> mChildren;
	public:
		PBTArrayProp(const PBBaseProp::Param& param, QWidget* parent);

		void ReMakeElements();

		virtual void UpdatePropertyValue();
		virtual void UpdateWidgetValue();
	};


	//////////////////////////////////////////////////////////////////////////
	class PBTransformProp : public PBBaseProp
	{
	public:
		Ui_TransformProperty* ui;
		Transform	mTransform;

		PBTransformProp(const PBBaseProp::Param& param, QWidget* parent);

		virtual void UpdatePropertyValue() override;

		virtual void UpdateWidgetValue() override;

	private:
		void SetWidgetsValue(const Transform& trs);
	};
};
