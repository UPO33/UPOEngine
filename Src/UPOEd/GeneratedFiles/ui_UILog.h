/********************************************************************************
** Form generated from reading UI file 'UILog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UILOG_H
#define UI_UILOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QHBoxLayout *horizontalLayout;
    QTableWidget *tableWidget;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(692, 368);
        horizontalLayout = new QHBoxLayout(Form);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        tableWidget = new QTableWidget(Form);
        if (tableWidget->columnCount() < 5)
            tableWidget->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        if (tableWidget->rowCount() < 4)
            tableWidget->setRowCount(4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(2, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(3, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget->setItem(0, 0, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget->setItem(0, 1, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget->setItem(0, 2, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget->setItem(0, 3, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget->setItem(0, 4, __qtablewidgetitem13);
        QBrush brush(QColor(255, 0, 0, 255));
        brush.setStyle(Qt::NoBrush);
        QFont font;
        font.setKerning(false);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        __qtablewidgetitem14->setFont(font);
        __qtablewidgetitem14->setForeground(brush);
        __qtablewidgetitem14->setFlags(Qt::NoItemFlags);
        tableWidget->setItem(1, 0, __qtablewidgetitem14);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setFrameShape(QFrame::NoFrame);
        tableWidget->setFrameShadow(QFrame::Plain);
        tableWidget->setTabKeyNavigation(false);
        tableWidget->setProperty("showDropIndicator", QVariant(true));
        tableWidget->setDragEnabled(true);
        tableWidget->setDragDropMode(QAbstractItemView::DragDrop);
        tableWidget->setDefaultDropAction(Qt::MoveAction);
        tableWidget->setAlternatingRowColors(true);
        tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setTextElideMode(Qt::ElideNone);
        tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableWidget->setShowGrid(false);
        tableWidget->setGridStyle(Qt::DashDotLine);
        tableWidget->setSortingEnabled(false);
        tableWidget->setWordWrap(false);
        tableWidget->setCornerButtonEnabled(false);
        tableWidget->horizontalHeader()->setVisible(true);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget->horizontalHeader()->setDefaultSectionSize(70);
        tableWidget->horizontalHeader()->setHighlightSections(false);
        tableWidget->horizontalHeader()->setMinimumSectionSize(60);
        tableWidget->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        tableWidget->verticalHeader()->setVisible(false);
        tableWidget->verticalHeader()->setHighlightSections(false);

        horizontalLayout->addWidget(tableWidget);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("Form", "New Row", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->verticalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("Form", "New Row", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->verticalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("Form", "New Row", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->verticalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("Form", "New Row", 0));

        const bool __sortingEnabled = tableWidget->isSortingEnabled();
        tableWidget->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->item(0, 0);
        ___qtablewidgetitem4->setText(QApplication::translate("Form", "main.cpp", 0));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->item(0, 1);
        ___qtablewidgetitem5->setText(QApplication::translate("Form", "main", 0));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->item(0, 2);
        ___qtablewidgetitem6->setText(QApplication::translate("Form", "23", 0));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->item(0, 3);
        ___qtablewidgetitem7->setText(QApplication::translate("Form", "0x33", 0));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->item(0, 4);
        ___qtablewidgetitem8->setText(QApplication::translate("Form", "a new log created hahahahaahhaaaaaaaaaaa", 0));
        tableWidget->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UILOG_H
