/********************************************************************************
** Form generated from reading UI file 'StartupWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STARTUPWINDOW_H
#define UI_STARTUPWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StartupWindow
{
public:
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *tab1;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QListWidget *recentProjects;
    QPushButton *btnBrowse;
    QWidget *tab_2;
    QLineEdit *location;
    QLabel *label_2;
    QPushButton *btnCreate;
    QLineEdit *projName;
    QPushButton *btnLocation;
    QLabel *label;

    void setupUi(QWidget *StartupWindow)
    {
        if (StartupWindow->objectName().isEmpty())
            StartupWindow->setObjectName(QStringLiteral("StartupWindow"));
        StartupWindow->resize(582, 329);
        StartupWindow->setMinimumSize(QSize(582, 329));
        StartupWindow->setMaximumSize(QSize(582, 329));
        horizontalLayout = new QHBoxLayout(StartupWindow);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        tabWidget = new QTabWidget(StartupWindow);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab1 = new QWidget();
        tab1->setObjectName(QStringLiteral("tab1"));
        verticalLayout = new QVBoxLayout(tab1);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_3 = new QLabel(tab1);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout->addWidget(label_3);

        recentProjects = new QListWidget(tab1);
        recentProjects->setObjectName(QStringLiteral("recentProjects"));
        recentProjects->setEditTriggers(QAbstractItemView::NoEditTriggers);
        recentProjects->setAlternatingRowColors(true);

        verticalLayout->addWidget(recentProjects);

        btnBrowse = new QPushButton(tab1);
        btnBrowse->setObjectName(QStringLiteral("btnBrowse"));
        btnBrowse->setMinimumSize(QSize(120, 0));
        btnBrowse->setMaximumSize(QSize(60, 16777215));
        btnBrowse->setLayoutDirection(Qt::LeftToRight);

        verticalLayout->addWidget(btnBrowse, 0, Qt::AlignHCenter);

        tabWidget->addTab(tab1, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        location = new QLineEdit(tab_2);
        location->setObjectName(QStringLiteral("location"));
        location->setGeometry(QRect(60, 150, 411, 32));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(location->sizePolicy().hasHeightForWidth());
        location->setSizePolicy(sizePolicy);
        location->setMinimumSize(QSize(0, 32));
        location->setMaximumSize(QSize(1111, 32));
        location->setBaseSize(QSize(0, 0));
        QFont font;
        font.setPointSize(11);
        location->setFont(font);
        location->setAlignment(Qt::AlignCenter);
        location->setPlaceholderText(QStringLiteral(""));
        location->setClearButtonEnabled(true);
        label_2 = new QLabel(tab_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(60, 130, 121, 16));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);
        label_2->setFont(font);
        btnCreate = new QPushButton(tab_2);
        btnCreate->setObjectName(QStringLiteral("btnCreate"));
        btnCreate->setGeometry(QRect(230, 210, 101, 41));
        QFont font1;
        font1.setPointSize(14);
        font1.setBold(false);
        font1.setWeight(50);
        btnCreate->setFont(font1);
        projName = new QLineEdit(tab_2);
        projName->setObjectName(QStringLiteral("projName"));
        projName->setGeometry(QRect(60, 80, 411, 31));
        projName->setFont(font);
        projName->setText(QStringLiteral("NewProject"));
        projName->setMaxLength(256);
        projName->setAlignment(Qt::AlignCenter);
        projName->setClearButtonEnabled(true);
        btnLocation = new QPushButton(tab_2);
        btnLocation->setObjectName(QStringLiteral("btnLocation"));
        btnLocation->setGeometry(QRect(480, 150, 31, 31));
        label = new QLabel(tab_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(61, 57, 121, 16));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setFont(font);
        tabWidget->addTab(tab_2, QString());

        horizontalLayout->addWidget(tabWidget);


        retranslateUi(StartupWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(StartupWindow);
    } // setupUi

    void retranslateUi(QWidget *StartupWindow)
    {
        StartupWindow->setWindowTitle(QApplication::translate("StartupWindow", "Form", 0));
        label_3->setText(QApplication::translate("StartupWindow", "Recent Projecs:", 0));
        btnBrowse->setText(QApplication::translate("StartupWindow", "Browser", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab1), QApplication::translate("StartupWindow", "Open Project", 0));
        label_2->setText(QApplication::translate("StartupWindow", "Location", 0));
        btnCreate->setText(QApplication::translate("StartupWindow", "Create", 0));
        projName->setPlaceholderText(QString());
        btnLocation->setText(QApplication::translate("StartupWindow", "...", 0));
        label->setText(QApplication::translate("StartupWindow", "Project name", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("StartupWindow", "New Project", 0));
    } // retranslateUi

};

namespace Ui {
    class StartupWindow: public Ui_StartupWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTUPWINDOW_H
