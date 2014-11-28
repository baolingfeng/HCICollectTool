/********************************************************************************
** Form generated from reading UI file 'LogOverview.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGOVERVIEW_H
#define UI_LOGOVERVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LogOverview
{
public:
    QAction *actionImport;
    QAction *actionReplay;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QTreeView *logList;
    QGroupBox *groupBox_2;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *lineEdit_2;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QMenuBar *menubar;
    QMenu *menuLogs;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *LogOverview)
    {
        if (LogOverview->objectName().isEmpty())
            LogOverview->setObjectName(QStringLiteral("LogOverview"));
        LogOverview->resize(841, 600);
        actionImport = new QAction(LogOverview);
        actionImport->setObjectName(QStringLiteral("actionImport"));
        actionReplay = new QAction(LogOverview);
        actionReplay->setObjectName(QStringLiteral("actionReplay"));
        centralwidget = new QWidget(LogOverview);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        logList = new QTreeView(groupBox);
        logList->setObjectName(QStringLiteral("logList"));

        verticalLayout->addWidget(logList);

        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        formLayout = new QFormLayout(groupBox_2);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        lineEdit_2 = new QLineEdit(groupBox_2);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setEnabled(false);

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit_2);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        lineEdit = new QLineEdit(groupBox_2);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setEnabled(false);

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEdit);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        lineEdit_3 = new QLineEdit(groupBox_2);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setEnabled(false);

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEdit_3);


        verticalLayout->addWidget(groupBox_2);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        LogOverview->setCentralWidget(centralwidget);
        menubar = new QMenuBar(LogOverview);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 841, 21));
        menuLogs = new QMenu(menubar);
        menuLogs->setObjectName(QStringLiteral("menuLogs"));
        LogOverview->setMenuBar(menubar);
        statusbar = new QStatusBar(LogOverview);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        LogOverview->setStatusBar(statusbar);

        menubar->addAction(menuLogs->menuAction());
        menuLogs->addAction(actionImport);
        menuLogs->addAction(actionReplay);

        retranslateUi(LogOverview);

        QMetaObject::connectSlotsByName(LogOverview);
    } // setupUi

    void retranslateUi(QMainWindow *LogOverview)
    {
        LogOverview->setWindowTitle(QApplication::translate("LogOverview", "Log Overview", 0));
        actionImport->setText(QApplication::translate("LogOverview", "import", 0));
        actionReplay->setText(QApplication::translate("LogOverview", "replay", 0));
        groupBox->setTitle(QApplication::translate("LogOverview", "Records in Dababase", 0));
        groupBox_2->setTitle(QApplication::translate("LogOverview", "Overview", 0));
        label->setText(QApplication::translate("LogOverview", "From", 0));
        label_2->setText(QApplication::translate("LogOverview", "to", 0));
        label_3->setText(QApplication::translate("LogOverview", "Total Time", 0));
        menuLogs->setTitle(QApplication::translate("LogOverview", "Logs", 0));
    } // retranslateUi

};

namespace Ui {
    class LogOverview: public Ui_LogOverview {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGOVERVIEW_H
