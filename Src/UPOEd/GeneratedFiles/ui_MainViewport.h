/********************************************************************************
** Form generated from reading UI file 'MainViewport.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINVIEWPORT_H
#define UI_MAINVIEWPORT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "UMainViewport.h"
#include "UPropertyBrowser.h"

QT_BEGIN_NAMESPACE

class Ui_MainViewport
{
public:
    QGridLayout *gridLayout;
    QWidget *mToolsWidget;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    UPOEd::PropertyBrowserWidget *mOptionsWidget;
    QVBoxLayout *verticalLayout;
    UPOEd::RenderViewportWidget *mViewport;

    void setupUi(QWidget *MainViewport)
    {
        if (MainViewport->objectName().isEmpty())
            MainViewport->setObjectName(QStringLiteral("MainViewport"));
        MainViewport->resize(603, 446);
        gridLayout = new QGridLayout(MainViewport);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        mToolsWidget = new QWidget(MainViewport);
        mToolsWidget->setObjectName(QStringLiteral("mToolsWidget"));
        mToolsWidget->setMaximumSize(QSize(16777215, 32));
        horizontalLayout_2 = new QHBoxLayout(mToolsWidget);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);

        gridLayout->addWidget(mToolsWidget, 0, 0, 1, 1);

        widget = new QWidget(MainViewport);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 2);
        mOptionsWidget = new UPOEd::PropertyBrowserWidget(widget);
        mOptionsWidget->setObjectName(QStringLiteral("mOptionsWidget"));
        mOptionsWidget->setMaximumSize(QSize(248, 16777215));
        verticalLayout = new QVBoxLayout(mOptionsWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

        horizontalLayout->addWidget(mOptionsWidget);

        mViewport = new UPOEd::RenderViewportWidget(widget);
        mViewport->setObjectName(QStringLiteral("mViewport"));

        horizontalLayout->addWidget(mViewport);


        gridLayout->addWidget(widget, 1, 0, 1, 1);


        retranslateUi(MainViewport);

        QMetaObject::connectSlotsByName(MainViewport);
    } // setupUi

    void retranslateUi(QWidget *MainViewport)
    {
        MainViewport->setWindowTitle(QApplication::translate("MainViewport", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class MainViewport: public Ui_MainViewport {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINVIEWPORT_H
