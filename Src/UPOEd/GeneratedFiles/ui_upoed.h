/********************************************************************************
** Form generated from reading UI file 'upoed.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UPOED_H
#define UI_UPOED_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UPOEdClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *UPOEdClass)
    {
        if (UPOEdClass->objectName().isEmpty())
            UPOEdClass->setObjectName(QStringLiteral("UPOEdClass"));
        UPOEdClass->resize(600, 400);
        menuBar = new QMenuBar(UPOEdClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        UPOEdClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(UPOEdClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        UPOEdClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(UPOEdClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        UPOEdClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(UPOEdClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        UPOEdClass->setStatusBar(statusBar);

        retranslateUi(UPOEdClass);

        QMetaObject::connectSlotsByName(UPOEdClass);
    } // setupUi

    void retranslateUi(QMainWindow *UPOEdClass)
    {
        UPOEdClass->setWindowTitle(QApplication::translate("UPOEdClass", "UPOEd", 0));
    } // retranslateUi

};

namespace Ui {
    class UPOEdClass: public Ui_UPOEdClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPOED_H
