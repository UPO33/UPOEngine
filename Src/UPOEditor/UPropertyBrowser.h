#pragma once

#include "UCommon.h"
#include "../UPOEngine/Meta/UMeta.h"
#include "UColorSelectionDialog.h"

namespace UPOEd
{
	class PBBaseProp
	{
	protected:
		void* mValuePtr;
		const PropertyInfo& mPropertyInfo;
		const ClassInfo* mRootClass;
		

	public:
		PBBaseProp(const PropertyInfo& pi, void* valuePtr)
			: mPropertyInfo(pi), mValuePtr(valuePtr)
		{

		}
		void Test()
		{

		}
		template <typename T> T& ValueAs() { return *((T*)mValuePtr); }
	};

	class PBBoolProp : public QCheckBox, PBBaseProp
	{
	public:
		PBBoolProp(const PropertyInfo& pi, void* vauePtr, QWidget* parent = nullptr) : QCheckBox(parent), PBBaseProp(pi, vauePtr)
		{
			setText("");
			setChecked(ValueAs<bool>());
			setCheckable(true);
			connect(this, &QCheckBox::stateChanged, this, [&](int state)
			{
				ValueAs<bool>() = state == Qt::CheckState::Checked;
			});
		}
	};
	class PBNumberProp : public QDoubleSpinBox, PBBaseProp
	{
		static const int PRECISION_DECIMAL_FLOAT = 4;
		static const int PRECISION_DECIMAL_DOUBLE = 5;

	public:
		PBNumberProp(const PropertyInfo& pi, void* vauePtr, QWidget* parent = nullptr) : QDoubleSpinBox(parent), PBBaseProp(pi, vauePtr)
		{
			float typeMin = 0;
			float typeMax = 0;
			int decimals = 0;
			switch (mPropertyInfo.GetType())
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
			if (mPropertyInfo.GetAttributes().GetAttrib(EAT_Range, rangeAttr))
			{
				min = Clamp(rangeAttr.GetValue0(), typeMin, typeMax);
				max = Clamp(rangeAttr.GetValue1(), typeMin, typeMax);
			}
			setMinimum(min);
			setMaximum(max);
			setDecimals(decimals);
			setSingleStep(1);
			InitValue();
			setAccelerated(true);

			connect(this, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [&](double value) {
				switch (mPropertyInfo.GetType())
				{
				case EPT_uint8: ValueAs<uint8>() = value; return;
				case EPT_uint16: ValueAs<uint16>() = value; return;
				case EPT_uint32: ValueAs<uint32>() = value; return;
				case EPT_uint64: ValueAs<uint64>() = value; return;

				case EPT_int8: ValueAs<int8>() = value; return;
				case EPT_int16: ValueAs<int64>() = value; return;
				case EPT_int32: ValueAs<int64>() = value; return;
				case EPT_int64: ValueAs<int64>() = value; return;

				case EPT_float: ValueAs<float>() = value; return;
				case EPT_double: ValueAs<double>() = value; return;
				}
			});
		}

		void InitValue()
		{
			double value = 0;
			switch (mPropertyInfo.GetType())
			{
			case EPT_uint8: value = (double)ValueAs<uint8>(); break;
			case EPT_uint16: value = (double)ValueAs<uint16>(); break;
			case EPT_uint32: value = (double)ValueAs<uint32>(); break;
			case EPT_uint64: value = (double)ValueAs<uint64>(); break;

			case EPT_int8: value = (double)ValueAs<int8>(); break;
			case EPT_int16: value = (double)ValueAs<int16>(); break;
			case EPT_int32: value = (double)ValueAs<int32>(); break;
			case EPT_int64: value = (double)ValueAs<int64>(); break;

			case EPT_float: value = (double)ValueAs<float>(); break;
			case EPT_double: value = (double)ValueAs<double>(); break;
			}

			setValue(value);
		}
	};
	class PBStringProp : public QLineEdit, PBBaseProp
	{
	public:
		PBStringProp(const PropertyInfo& pi, void* vauePtr, QWidget* parent = nullptr) : QLineEdit(parent), PBBaseProp(pi, vauePtr)
		{
			UPO::String* pStr = ((UPO::String*)mValuePtr);
			ULOG_MESSAGE("pStr %p", pStr);
			ULOG_MESSAGE("mStr %p", pStr->mStr);


			const char* stt = ValueAs<String>().CStr();
			ULOG_MESSAGE("stt %s", stt);
			if(stt)
				setText(QString(stt));
			setClearButtonEnabled(true);
			
			connect(this, &QLineEdit::textChanged, this, [&](const QString& str) {
				if (str.isEmpty())
					ValueAs<String>() = String();
				else
					ValueAs<String>() = String(str.toStdString().c_str());
			});
			
		}
	};
	class PBNameProp : public QLineEdit, PBBaseProp
	{
	public:
		PBNameProp(const PropertyInfo& pi, void* vauePtr, QWidget* parent = nullptr) : QLineEdit(parent), PBBaseProp(pi, vauePtr)
		{
			setText(QString(ValueAs<Name>().CStr()));
			setClearButtonEnabled(true);

			connect(this, &QLineEdit::textChanged, this, [&](const QString& str) {
				if (str.isEmpty())
					ValueAs<Name>() = Name();
				else
					ValueAs<Name>() = Name(str.toStdString().c_str());
			});
		}
	};
	class PBVec2Prop : public QWidget, PBBaseProp
	{
		static const int PRECISION_DECIMAL = 4;
		QDoubleSpinBox* mSpinBoxX;
		QDoubleSpinBox* mSpinBoxY;
	public:
		PBVec2Prop(const PropertyInfo& pi, void* vauePtr, QWidget* parent = nullptr) : QWidget(parent), PBBaseProp(pi, vauePtr)
		{
			float minValue = FLT_MIN;
			float maxValue = FLT_MAX;
			Attrib attrRange;
			if (mPropertyInfo.GetAttributes().GetAttrib(EAT_Range, attrRange))
			{
				minValue = attrRange.GetValue0();
				maxValue = attrRange.GetValue1();
			}
			setLayout(new QHBoxLayout());
			layout()->setMargin(0);
			layout()->setSpacing(0);
			
			mSpinBoxX = new QDoubleSpinBox(this);
			mSpinBoxX->setMinimum(minValue);
			mSpinBoxX->setMaximum(maxValue);
			mSpinBoxY = new QDoubleSpinBox(this);
			mSpinBoxY->setMinimum(minValue);
			mSpinBoxY->setMaximum(maxValue);
			layout()->addWidget(mSpinBoxX);
			layout()->addWidget(mSpinBoxY);
			mSpinBoxX->setValue(ValueAs<Vec2>().mX);
			mSpinBoxY->setValue(ValueAs<Vec2>().mY);
			connect(mSpinBoxX, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [&](double value) {
				ValueAs<Vec2>().mX = (float)value;
			});
			connect(mSpinBoxY, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [&](double value) {
				ValueAs<Vec2>().mY = (float)value;
			});
		}
	};

	class PBVec3Prop : public QWidget, PBBaseProp
	{
		static const int PRECISION_DECIMAL = 4;
		QDoubleSpinBox* mSpinBoxX;
		QDoubleSpinBox* mSpinBoxY;
		QDoubleSpinBox* mSpinBoxZ;
	public:
		PBVec3Prop(const PropertyInfo& pi, void* vauePtr, QWidget* parent = nullptr) : QWidget(parent), PBBaseProp(pi, vauePtr)
		{
			float minValue = FLT_MIN;
			float maxValue = FLT_MAX;
			Attrib attrRange;
			if (mPropertyInfo.GetAttributes().GetAttrib(EAT_Range, attrRange))
			{
				minValue = attrRange.GetValue0();
				maxValue = attrRange.GetValue1();
			}
			setLayout(new QHBoxLayout());
			layout()->setMargin(0);
			layout()->setSpacing(0);

			mSpinBoxX = new QDoubleSpinBox(this);
			mSpinBoxX->setMinimum(minValue);
			mSpinBoxX->setMaximum(maxValue);
			mSpinBoxY = new QDoubleSpinBox(this);
			mSpinBoxY->setMinimum(minValue);
			mSpinBoxY->setMaximum(maxValue);
			mSpinBoxZ = new QDoubleSpinBox(this);
			mSpinBoxZ->setMinimum(minValue);
			mSpinBoxZ->setMaximum(maxValue);

			layout()->addWidget(mSpinBoxX);
			layout()->addWidget(mSpinBoxY);
			layout()->addWidget(mSpinBoxZ);

			mSpinBoxX->setValue(ValueAs<Vec3>().mX);
			mSpinBoxY->setValue(ValueAs<Vec3>().mY);
			mSpinBoxZ->setValue(ValueAs<Vec3>().mZ);

			connect(mSpinBoxX, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [&](double value) {
				ValueAs<Vec3>().mX = (float)value;
			});
			connect(mSpinBoxY, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [&](double value) {
				ValueAs<Vec3>().mY = (float)value;
			});
			connect(mSpinBoxZ, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [&](double value) {
				ValueAs<Vec3>().mZ = (float)value;
			});
		}
	};


	class PBVec4Prop : public QWidget, PBBaseProp
	{
		static const int PRECISION_DECIMAL = 4;
		QDoubleSpinBox* mSpinBoxX;
		QDoubleSpinBox* mSpinBoxY;
		QDoubleSpinBox* mSpinBoxZ;
		QDoubleSpinBox* mSpinBoxW;
	public:
		PBVec4Prop(const PropertyInfo& pi, void* vauePtr, QWidget* parent = nullptr) : QWidget(parent), PBBaseProp(pi, vauePtr)
		{
			float minValue = FLT_MIN;
			float maxValue = FLT_MAX;
			Attrib attrRange;
			if (mPropertyInfo.GetAttributes().GetAttrib(EAT_Range, attrRange))
			{
				minValue = attrRange.GetValue0();
				maxValue = attrRange.GetValue1();
			}
			setLayout(new QHBoxLayout());
			layout()->setMargin(0);
			layout()->setSpacing(0);

			mSpinBoxX = new QDoubleSpinBox(this);
			mSpinBoxX->setMinimum(minValue);
			mSpinBoxX->setMaximum(maxValue);
			mSpinBoxY = new QDoubleSpinBox(this);
			mSpinBoxY->setMinimum(minValue);
			mSpinBoxY->setMaximum(maxValue);
			mSpinBoxZ = new QDoubleSpinBox(this);
			mSpinBoxZ->setMinimum(minValue);
			mSpinBoxZ->setMaximum(maxValue);
			mSpinBoxW = new QDoubleSpinBox(this);
			mSpinBoxW->setMinimum(minValue);
			mSpinBoxW->setMaximum(maxValue);

			layout()->addWidget(mSpinBoxX);
			layout()->addWidget(mSpinBoxY);
			layout()->addWidget(mSpinBoxZ);
			layout()->addWidget(mSpinBoxW);

			mSpinBoxX->setValue(ValueAs<Vec4>().mX);
			mSpinBoxY->setValue(ValueAs<Vec4>().mY);
			mSpinBoxZ->setValue(ValueAs<Vec4>().mZ);
			mSpinBoxW->setValue(ValueAs<Vec4>().mW);

			connect(mSpinBoxX, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [&](double value) {
				ValueAs<Vec4>().mX = (float)value;
			});
			connect(mSpinBoxY, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [&](double value) {
				ValueAs<Vec4>().mY = (float)value;
			});
			connect(mSpinBoxZ, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [&](double value) {
				ValueAs<Vec4>().mZ = (float)value;
			});
			connect(mSpinBoxW, (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged, this, [&](double value) {
				ValueAs<Vec4>().mW = (float)value;
			});
		}
	};

	//////////////////////////////////////////////////////////////////////////
	class PBColorProp : public QWidget, PBBaseProp
	{
		QColor mPreColor;

	public:
		PBColorProp(const PropertyInfo& pi, void* vauePtr, QWidget* parent = nullptr) : QWidget(parent), PBBaseProp(pi, vauePtr)
		{
			setAutoFillBackground(true);
			SetWidgetQColor(GetValueQColor());
		}
		QColor GetValueQColor()
		{
			Color color = Color(0);
			if (mPropertyInfo.GetTypeInfo() == Color::GetClassInfoStatic())
				color = Color(ValueAs<Color>());
			else if (mPropertyInfo.GetTypeInfo() == Color32::GetClassInfoStatic())
				color = Color(ValueAs<Color32>());
			else
				UASSERT(false);

			return QColor::fromRgbF(color.mR, color.mG, color.mB, color.mA);
		}
		void SetValueQColor(QColor qc)
		{
			if (mPropertyInfo.GetTypeInfo() == Color::GetClassInfoStatic())
			{
				Color color(qc.redF(), qc.greenF(), qc.blueF(), qc.alphaF());
				ValueAs<Color>() = color;
			}
			else
			{
				Color32 color = Color32(qc.red(), qc.green(), qc.blue(), qc.alpha());
				ValueAs<Color32>() = color;
			}
		}
		void SetWidgetQColor(QColor color)
		{
// 			QPalette palette;
// 			palette.setColor(Qt::active ,this->backgroundRole(), Qt::black);
// 			this->setPalette(palette);
// 			this->update();
			setStyleSheet("background-color:black;");
			QString style = QString::asprintf("background-color: rgba(%d, %d, %d, 255);", color.red(), color.green(), color.blue());
			setStyleSheet(style);
		}
// 		void paintEvent(QPaintEvent *) override
// 		{
// 			QStyleOption opt;
// 			opt.init(this);
// 			QPainter p(this);
// 			style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
// 		}
		virtual void mouseReleaseEvent(QMouseEvent * event) override
		{
			if (event->button() == Qt::MouseButton::LeftButton)
			{
				mPreColor = GetValueQColor();
				ColorSelectionDialog::OpenNow(GetValueQColor());
				
				ColorSelectionDialog::OnRejected(this, [&]() {
					ULOG_MESSAGE("rejected");
					SetValueQColor(mPreColor);
					SetWidgetQColor(mPreColor);
				});
				ColorSelectionDialog::OnAccepted(this, [&]() {
					ULOG_MESSAGE("accepted");
				});
				ColorSelectionDialog::OnColorChanged(this, [&](const QColor& color) {
					ULOG_MESSAGE("cc %f", color.alphaF());
					SetWidgetQColor(color);
					SetValueQColor(color);
				});



			}
		}

	};
	//////////////////////////////////////////////////////////////////////////
	class PropertyBrowserWidget : public QWidget
	{
		const ClassInfo*	mClass;
		void*				mObject;

		QLineEdit*		mSearchBox;
		QTreeWidget*	mTree;

	public:
		PropertyBrowserWidget(const ClassInfo* ci, void* object, QWidget* parent = nullptr) : QWidget(parent)
		{
			mClass = ci;
			mObject = object;

			setMinimumSize(QSize(300, 500));

			setLayout(new QVBoxLayout);
			mSearchBox = new QLineEdit(this);
			layout()->addWidget(mSearchBox);

			mTree = new QTreeWidget(this);
			layout()->addWidget(mTree);
			mTree->setColumnCount(2);
			mTree->setAlternatingRowColors(true);
			
// 			mTree->setHeaderHidden(true);

			FillTree();
		}
		QWidget* GetWidgetForProp(const PropertyInfo& prp, void* object)
		{
			void* valuePtr = prp.Map(object);

			switch (prp.GetType())
			{
			case UPO::EPT_Unknown:
				break;
			case UPO::EPT_bool:
				return new PBBoolProp(prp, valuePtr, mTree);
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
				return new PBNumberProp(prp, valuePtr, mTree);
			case UPO::EPT_enum:
				break;
			case UPO::EPT_TArray:
				break;
			case UPO::EPT_TObjectPtr:
				break;
			case UPO::EPT_ObjectPoniter:
				break;
			case UPO::EPT_MetaClass:
			{
				const ClassInfo* ci = prp.GetTypeInfo()->Cast<ClassInfo>();

				if (ci == Vec2::GetClassInfoStatic())	return new PBVec2Prop(prp, valuePtr, mTree);
				if (ci == Vec3::GetClassInfoStatic())	return new PBVec3Prop(prp, valuePtr, mTree);
				if (ci == Vec4::GetClassInfoStatic())	return new PBVec4Prop(prp, valuePtr, mTree);
				if (ci == Color::GetClassInfoStatic())	return new PBColorProp(prp, valuePtr, mTree);
				if (ci == Color32::GetClassInfoStatic()) return new PBColorProp(prp, valuePtr, mTree);
				if (ci == String::GetClassInfoStatic()) return new PBStringProp(prp, valuePtr, mTree);
				if (ci == Name::GetClassInfoStatic())	return new PBNameProp(prp, valuePtr, mTree);
			}
				break;
			default:
				break;
			}
			return nullptr;
		}
		bool MetaClassNeedSubProperties(const ClassInfo* ci)
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
		//////////////////////////////////////////////////////////////////////////
		void AddTreeItem(void* object, QTreeWidgetItem* parent, const PropertyInfo& prp)
		{
			QTreeWidgetItem* newItem = new QTreeWidgetItem(parent);
			newItem->setText(0, GetPropertyName(prp));
			newItem->setToolTip(1, prp.GetTypeName().CStr());
			Attrib attrComment;
			if (prp.GetAttributes().GetAttrib(EAtrribID::EAT_Comment, attrComment))
			{
				newItem->setToolTip(0, QString(attrComment.GetString().CStr()));
			}
			
			mTree->addTopLevelItem(newItem);
			QWidget* widget = GetWidgetForProp(prp, object);
			if (widget)
				mTree->setItemWidget(newItem, 1, widget);

			switch (prp.GetType())
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
				break;
			case UPO::EPT_ObjectPoniter:
				break;
			case UPO::EPT_MetaClass:
			{
				const ClassInfo* ci = prp.GetTypeInfo()->Cast<ClassInfo>();
				if(MetaClassNeedSubProperties(ci))
				{
					if (ClassInfo* classInfo = prp.GetTypeInfo()->Cast<ClassInfo>())
					{
						for (size_t i = 0; i < classInfo->NumProperty(); i++)
						{
							AddTreeItem(prp.Map(object), newItem, *classInfo->GetProperty(i));
						}
					}
				}
			}
				break;
			default:
				break;
			}
		}
		void FillTree()
		{
			mTree->clear();

			for (size_t i = 0; i < mClass->NumProperty(); i++)
			{
				AddTreeItem(mObject, nullptr, *mClass->GetProperty(i));
			}

		}

	};

};
