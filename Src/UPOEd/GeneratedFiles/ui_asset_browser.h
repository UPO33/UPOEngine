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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AssetBrowser
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *filterLineEdit;
    QPushButton *btnSave;
    QTreeWidget *assetsTree;

    void setupUi(QWidget *AssetBrowser)
    {
        if (AssetBrowser->objectName().isEmpty())
            AssetBrowser->setObjectName(QStringLiteral("AssetBrowser"));
        AssetBrowser->resize(662, 549);
        verticalLayout = new QVBoxLayout(AssetBrowser);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        filterLineEdit = new QLineEdit(AssetBrowser);
        filterLineEdit->setObjectName(QStringLiteral("filterLineEdit"));
        filterLineEdit->setClearButtonEnabled(true);

        horizontalLayout->addWidget(filterLineEdit);

        btnSave = new QPushButton(AssetBrowser);
        btnSave->setObjectName(QStringLiteral("btnSave"));

        horizontalLayout->addWidget(btnSave);


        verticalLayout->addLayout(horizontalLayout);

        assetsTree = new QTreeWidget(AssetBrowser);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        assetsTree->setHeaderItem(__qtreewidgetitem);
        assetsTree->setObjectName(QStringLiteral("assetsTree"));
        assetsTree->setContextMenuPolicy(Qt::CustomContextMenu);
        assetsTree->setAcceptDrops(true);
        assetsTree->setEditTriggers(QAbstractItemView::EditKeyPressed);
        assetsTree->setDragEnabled(true);
        assetsTree->setDragDropOverwriteMode(false);
        assetsTree->setDragDropMode(QAbstractItemView::InternalMove);
        assetsTree->setDefaultDropAction(Qt::MoveAction);
        assetsTree->setAlternatingRowColors(true);
        assetsTree->setIndentation(18);
        assetsTree->header()->setVisible(false);

        verticalLayout->addWidget(assetsTree);


        retranslateUi(AssetBrowser);

        QMetaObject::connectSlotsByName(AssetBrowser);
    } // setupUi

    void retranslateUi(QWidget *AssetBrowser)
    {
        AssetBrowser->setWindowTitle(QApplication::translate("AssetBrowser", "Form", 0));
        filterLineEdit->setPlaceholderText(QApplication::translate("AssetBrowser", "Filter", 0));
#ifndef QT_NO_TOOLTIP
        btnSave->setToolTip(QApplication::translate("AssetBrowser", "save all assets", 0));
#endif // QT_NO_TOOLTIP
        btnSave->setText(QApplication::translate("AssetBrowser", "Save", 0));
    } // retranslateUi

};

namespace Ui {
    class AssetBrowser: public Ui_AssetBrowser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASSET_BROWSER_H
