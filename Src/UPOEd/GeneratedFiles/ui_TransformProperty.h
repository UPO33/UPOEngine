/********************************************************************************
** Form generated from reading UI file 'TransformProperty.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSFORMPROPERTY_H
#define UI_TRANSFORMPROPERTY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TransformProperty
{
public:
    QGridLayout *gridLayout;
    QDoubleSpinBox *mRY;
    QDoubleSpinBox *mSX;
    QDoubleSpinBox *mRZ;
    QDoubleSpinBox *mSY;
    QDoubleSpinBox *mSZ;
    QDoubleSpinBox *mRX;
    QDoubleSpinBox *mPX;
    QSpacerItem *verticalSpacer;
    QDoubleSpinBox *mPY;
    QDoubleSpinBox *mPZ;

    void setupUi(QWidget *TransformProperty)
    {
        if (TransformProperty->objectName().isEmpty())
            TransformProperty->setObjectName(QStringLiteral("TransformProperty"));
        TransformProperty->resize(464, 163);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TransformProperty->sizePolicy().hasHeightForWidth());
        TransformProperty->setSizePolicy(sizePolicy);
        TransformProperty->setMinimumSize(QSize(1, 0));
        gridLayout = new QGridLayout(TransformProperty);
        gridLayout->setSpacing(2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(2, 2, 2, 2);
        mRY = new QDoubleSpinBox(TransformProperty);
        mRY->setObjectName(QStringLiteral("mRY"));
        mRY->setAccelerated(true);

        gridLayout->addWidget(mRY, 1, 1, 1, 1);

        mSX = new QDoubleSpinBox(TransformProperty);
        mSX->setObjectName(QStringLiteral("mSX"));

        gridLayout->addWidget(mSX, 2, 0, 1, 1);

        mRZ = new QDoubleSpinBox(TransformProperty);
        mRZ->setObjectName(QStringLiteral("mRZ"));
        mRZ->setAccelerated(true);

        gridLayout->addWidget(mRZ, 1, 2, 1, 1);

        mSY = new QDoubleSpinBox(TransformProperty);
        mSY->setObjectName(QStringLiteral("mSY"));

        gridLayout->addWidget(mSY, 2, 1, 1, 1);

        mSZ = new QDoubleSpinBox(TransformProperty);
        mSZ->setObjectName(QStringLiteral("mSZ"));

        gridLayout->addWidget(mSZ, 2, 2, 1, 1);

        mRX = new QDoubleSpinBox(TransformProperty);
        mRX->setObjectName(QStringLiteral("mRX"));
        mRX->setAccelerated(true);
        mRX->setProperty("showGroupSeparator", QVariant(false));
        mRX->setDecimals(1);
        mRX->setMinimum(-99999);
        mRX->setMaximum(99999);

        gridLayout->addWidget(mRX, 1, 0, 1, 1);

        mPX = new QDoubleSpinBox(TransformProperty);
        mPX->setObjectName(QStringLiteral("mPX"));
        mPX->setFrame(true);
        mPX->setAccelerated(true);
        mPX->setDecimals(4);
        mPX->setMinimum(-1e+8);
        mPX->setMaximum(1e+9);

        gridLayout->addWidget(mPX, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 3, 1, 1, 1);

        mPY = new QDoubleSpinBox(TransformProperty);
        mPY->setObjectName(QStringLiteral("mPY"));
        mPY->setFrame(true);
        mPY->setAccelerated(true);
        mPY->setDecimals(4);
        mPY->setMinimum(-1e+8);
        mPY->setMaximum(1e+9);

        gridLayout->addWidget(mPY, 0, 1, 1, 1);

        mPZ = new QDoubleSpinBox(TransformProperty);
        mPZ->setObjectName(QStringLiteral("mPZ"));
        mPZ->setFrame(true);
        mPZ->setAccelerated(true);
        mPZ->setDecimals(4);
        mPZ->setMinimum(-1e+8);
        mPZ->setMaximum(1e+9);

        gridLayout->addWidget(mPZ, 0, 2, 1, 1);


        retranslateUi(TransformProperty);

        QMetaObject::connectSlotsByName(TransformProperty);
    } // setupUi

    void retranslateUi(QWidget *TransformProperty)
    {
        TransformProperty->setWindowTitle(QApplication::translate("TransformProperty", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class TransformProperty: public Ui_TransformProperty {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSFORMPROPERTY_H
