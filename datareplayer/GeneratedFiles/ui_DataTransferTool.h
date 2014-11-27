/********************************************************************************
** Form generated from reading UI file 'DataTransferTool.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATATRANSFERTOOL_H
#define UI_DATATRANSFERTOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DataTransferWindow
{
public:
    QAction *actionImport;
    QAction *actionExport_to_Database;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *TabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QTreeView *mouseView;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QTreeView *keyView;
    QWidget *tab_3;
    QHBoxLayout *horizontalLayout;
    QTreeView *actionView;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_4;
    QTreeView *copyView;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QPushButton *filterBtn;
    QLabel *label;
    QLineEdit *toEdit;
    QLabel *label_2;
    QLineEdit *fromEdit;
    QLabel *label_3;
    QLineEdit *totalEdit;
    QMenuBar *menubar;
    QMenu *menuTool;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *DataTransferWindow)
    {
        if (DataTransferWindow->objectName().isEmpty())
            DataTransferWindow->setObjectName(QStringLiteral("DataTransferWindow"));
        DataTransferWindow->resize(1628, 756);
        actionImport = new QAction(DataTransferWindow);
        actionImport->setObjectName(QStringLiteral("actionImport"));
        actionExport_to_Database = new QAction(DataTransferWindow);
        actionExport_to_Database->setObjectName(QStringLiteral("actionExport_to_Database"));
        centralwidget = new QWidget(DataTransferWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        TabWidget = new QTabWidget(centralwidget);
        TabWidget->setObjectName(QStringLiteral("TabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        mouseView = new QTreeView(tab);
        mouseView->setObjectName(QStringLiteral("mouseView"));

        verticalLayout_2->addWidget(mouseView);

        TabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        keyView = new QTreeView(tab_2);
        keyView->setObjectName(QStringLiteral("keyView"));

        verticalLayout_3->addWidget(keyView);

        TabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        horizontalLayout = new QHBoxLayout(tab_3);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        actionView = new QTreeView(tab_3);
        actionView->setObjectName(QStringLiteral("actionView"));

        horizontalLayout->addWidget(actionView);

        TabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        verticalLayout_4 = new QVBoxLayout(tab_4);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        copyView = new QTreeView(tab_4);
        copyView->setObjectName(QStringLiteral("copyView"));

        verticalLayout_4->addWidget(copyView);

        TabWidget->addTab(tab_4, QString());

        verticalLayout->addWidget(TabWidget);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        filterBtn = new QPushButton(groupBox);
        filterBtn->setObjectName(QStringLiteral("filterBtn"));

        gridLayout->addWidget(filterBtn, 3, 0, 1, 2);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        toEdit = new QLineEdit(groupBox);
        toEdit->setObjectName(QStringLiteral("toEdit"));

        gridLayout->addWidget(toEdit, 1, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        fromEdit = new QLineEdit(groupBox);
        fromEdit->setObjectName(QStringLiteral("fromEdit"));

        gridLayout->addWidget(fromEdit, 0, 1, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        totalEdit = new QLineEdit(groupBox);
        totalEdit->setObjectName(QStringLiteral("totalEdit"));
        totalEdit->setEnabled(false);
        totalEdit->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(totalEdit, 1, 2, 1, 1);


        verticalLayout->addWidget(groupBox);

        DataTransferWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(DataTransferWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1628, 21));
        menuTool = new QMenu(menubar);
        menuTool->setObjectName(QStringLiteral("menuTool"));
        DataTransferWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(DataTransferWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        DataTransferWindow->setStatusBar(statusbar);

        menubar->addAction(menuTool->menuAction());
        menuTool->addAction(actionImport);
        menuTool->addAction(actionExport_to_Database);

        retranslateUi(DataTransferWindow);

        TabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(DataTransferWindow);
    } // setupUi

    void retranslateUi(QMainWindow *DataTransferWindow)
    {
        DataTransferWindow->setWindowTitle(QApplication::translate("DataTransferWindow", "Data Transfer&Clear Tool", 0));
        actionImport->setText(QApplication::translate("DataTransferWindow", "Load from Log File", 0));
        actionExport_to_Database->setText(QApplication::translate("DataTransferWindow", "Export to Database", 0));
        TabWidget->setTabText(TabWidget->indexOf(tab), QApplication::translate("DataTransferWindow", "Mouse Event", 0));
        TabWidget->setTabText(TabWidget->indexOf(tab_2), QApplication::translate("DataTransferWindow", "Keyinput Event", 0));
        TabWidget->setTabText(TabWidget->indexOf(tab_3), QApplication::translate("DataTransferWindow", "Click Action", 0));
        TabWidget->setTabText(TabWidget->indexOf(tab_4), QApplication::translate("DataTransferWindow", "Copy & Paste", 0));
        groupBox->setTitle(QApplication::translate("DataTransferWindow", "Filter", 0));
        filterBtn->setText(QApplication::translate("DataTransferWindow", "Filter", 0));
        label->setText(QApplication::translate("DataTransferWindow", "From", 0));
        label_2->setText(QApplication::translate("DataTransferWindow", "To", 0));
        label_3->setText(QApplication::translate("DataTransferWindow", "Total Time:", 0));
        menuTool->setTitle(QApplication::translate("DataTransferWindow", "Tool", 0));
    } // retranslateUi

};

namespace Ui {
    class DataTransferWindow: public Ui_DataTransferWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATATRANSFERTOOL_H
