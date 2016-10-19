/********************************************************************************
** Form generated from reading UI file 'asset_browser.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ASSET_BROWSER_H
#define UI_ASSET_BROWSER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AssetBrowser
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *mFilter;
    QListWidget *mList;

    void setupUi(QWidget *AssetBrowser)
    {
        if (AssetBrowser->objectName().isEmpty())
            AssetBrowser->setObjectName(QStringLiteral("AssetBrowser"));
        AssetBrowser->resize(567, 486);
        verticalLayout = new QVBoxLayout(AssetBrowser);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        mFilter = new QLineEdit(AssetBrowser);
        mFilter->setObjectName(QStringLiteral("mFilter"));
        mFilter->setClearButtonEnabled(true);

        verticalLayout->addWidget(mFilter);

        mList = new QListWidget(AssetBrowser);
        mList->setObjectName(QStringLiteral("mList"));
        mList->setContextMenuPolicy(Qt::ActionsContextMenu);

        verticalLayout->addWidget(mList);


        retranslateUi(AssetBrowser);

        QMetaObject::connectSlotsByName(AssetBrowser);
    } // setupUi

    void retranslateUi(QWidget *AssetBrowser)
    {
        AssetBrowser->setWindowTitle(QApplication::translate("AssetBrowser", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class AssetBrowser: public Ui_AssetBrowser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASSET_BROWSER_H
