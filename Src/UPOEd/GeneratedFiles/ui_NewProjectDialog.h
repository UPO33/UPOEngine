/********************************************************************************
** Form generated from reading UI file 'NewProjectDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWPROJECTDIALOG_H
#define UI_NEWPROJECTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_NewProjectDialog
{
public:
    QLineEdit *projName;
    QLabel *label;
    QLineEdit *location;
    QLabel *label_2;
    QPushButton *locationBtn;
    QPushButton *createBtn;

    void setupUi(QDialog *NewProjectDialog)
    {
        if (NewProjectDialog->objectName().isEmpty())
            NewProjectDialog->setObjectName(QStringLiteral("NewProjectDialog"));
        NewProjectDialog->resize(468, 243);
        NewProjectDialog->setMinimumSize(QSize(468, 243));
        NewProjectDialog->setMaximumSize(QSize(468, 243));
        projName = new QLineEdit(NewProjectDialog);
        projName->setObjectName(QStringLiteral("projName"));
        projName->setGeometry(QRect(10, 50, 411, 31));
        QFont font;
        font.setPointSize(11);
        projName->setFont(font);
        projName->setText(QStringLiteral("NewProject"));
        projName->setMaxLength(256);
        projName->setAlignment(Qt::AlignCenter);
        projName->setClearButtonEnabled(true);
        label = new QLabel(NewProjectDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(11, 27, 121, 16));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setFont(font);
        location = new QLineEdit(NewProjectDialog);
        location->setObjectName(QStringLiteral("location"));
        location->setGeometry(QRect(10, 120, 411, 32));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(location->sizePolicy().hasHeightForWidth());
        location->setSizePolicy(sizePolicy1);
        location->setMinimumSize(QSize(0, 32));
        location->setMaximumSize(QSize(1111, 32));
        location->setBaseSize(QSize(0, 0));
        location->setFont(font);
        location->setAlignment(Qt::AlignCenter);
        location->setPlaceholderText(QStringLiteral(""));
        location->setClearButtonEnabled(true);
        label_2 = new QLabel(NewProjectDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 100, 121, 16));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setFont(font);
        locationBtn = new QPushButton(NewProjectDialog);
        locationBtn->setObjectName(QStringLiteral("locationBtn"));
        locationBtn->setGeometry(QRect(430, 120, 31, 31));
        createBtn = new QPushButton(NewProjectDialog);
        createBtn->setObjectName(QStringLiteral("createBtn"));
        createBtn->setGeometry(QRect(180, 180, 101, 41));
        QFont font1;
        font1.setPointSize(18);
        font1.setBold(true);
        font1.setWeight(75);
        createBtn->setFont(font1);

        retranslateUi(NewProjectDialog);

        QMetaObject::connectSlotsByName(NewProjectDialog);
    } // setupUi

    void retranslateUi(QDialog *NewProjectDialog)
    {
        NewProjectDialog->setWindowTitle(QApplication::translate("NewProjectDialog", "Dialog", 0));
        projName->setPlaceholderText(QString());
        label->setText(QApplication::translate("NewProjectDialog", "Project name", 0));
        label_2->setText(QApplication::translate("NewProjectDialog", "Location", 0));
        locationBtn->setText(QApplication::translate("NewProjectDialog", "...", 0));
        createBtn->setText(QApplication::translate("NewProjectDialog", "Create", 0));
    } // retranslateUi

};

namespace Ui {
    class NewProjectDialog: public Ui_NewProjectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWPROJECTDIALOG_H
