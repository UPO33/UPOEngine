/********************************************************************************
** Form generated from reading UI file 'ab_asset_item.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AB_ASSET_ITEM_H
#define UI_AB_ASSET_ITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AssetBrowserAssetItem
{
public:
    QHBoxLayout *horizontalLayout;
    QListWidget *listWidget;

    void setupUi(QWidget *AssetBrowserAssetItem)
    {
        if (AssetBrowserAssetItem->objectName().isEmpty())
            AssetBrowserAssetItem->setObjectName(QStringLiteral("AssetBrowserAssetItem"));
        AssetBrowserAssetItem->resize(375, 217);
        horizontalLayout = new QHBoxLayout(AssetBrowserAssetItem);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        listWidget = new QListWidget(AssetBrowserAssetItem);
        new QListWidgetItem(listWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(listWidget);


        retranslateUi(AssetBrowserAssetItem);

        QMetaObject::connectSlotsByName(AssetBrowserAssetItem);
    } // setupUi

    void retranslateUi(QWidget *AssetBrowserAssetItem)
    {
        AssetBrowserAssetItem->setWindowTitle(QApplication::translate("AssetBrowserAssetItem", "Form", 0));

        const bool __sortingEnabled = listWidget->isSortingEnabled();
        listWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listWidget->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("AssetBrowserAssetItem", "New Item", 0));
        listWidget->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class AssetBrowserAssetItem: public Ui_AssetBrowserAssetItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AB_ASSET_ITEM_H
