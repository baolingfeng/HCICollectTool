/********************************************************************************
** Form generated from reading UI file 'datareplayer.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATAREPLAYER_H
#define UI_DATAREPLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DataReplayerClass
{
public:
    QPushButton *loadbtn;
    QTabWidget *tabWidget;
    QWidget *tab;
    QTableView *tableView;
    QWidget *tab_2;
    QTreeView *acctree;
    QPushButton *playbtn;
    QLabel *imagelabel;

    void setupUi(QWidget *DataReplayerClass)
    {
        if (DataReplayerClass->objectName().isEmpty())
            DataReplayerClass->setObjectName(QStringLiteral("DataReplayerClass"));
        DataReplayerClass->resize(1280, 900);
        loadbtn = new QPushButton(DataReplayerClass);
        loadbtn->setObjectName(QStringLiteral("loadbtn"));
        loadbtn->setGeometry(QRect(540, 830, 75, 23));
        tabWidget = new QTabWidget(DataReplayerClass);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(870, 0, 411, 831));
        tabWidget->setTabPosition(QTabWidget::East);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        tableView = new QTableView(tab);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setGeometry(QRect(0, 0, 251, 201));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        acctree = new QTreeView(tab_2);
        acctree->setObjectName(QStringLiteral("acctree"));
        acctree->setGeometry(QRect(10, 10, 371, 801));
        tabWidget->addTab(tab_2, QString());
        playbtn = new QPushButton(DataReplayerClass);
        playbtn->setObjectName(QStringLiteral("playbtn"));
        playbtn->setGeometry(QRect(440, 830, 75, 23));
        imagelabel = new QLabel(DataReplayerClass);
        imagelabel->setObjectName(QStringLiteral("imagelabel"));
        imagelabel->setGeometry(QRect(0, 10, 851, 721));
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        font.setWeight(75);
        imagelabel->setFont(font);
        imagelabel->setAlignment(Qt::AlignCenter);

        retranslateUi(DataReplayerClass);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(DataReplayerClass);
    } // setupUi

    void retranslateUi(QWidget *DataReplayerClass)
    {
        DataReplayerClass->setWindowTitle(QApplication::translate("DataReplayerClass", "DataReplayer", 0));
        loadbtn->setText(QApplication::translate("DataReplayerClass", "Load", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("DataReplayerClass", "Overview", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("DataReplayerClass", "Click Events", 0));
        playbtn->setText(QApplication::translate("DataReplayerClass", "Play", 0));
        imagelabel->setText(QApplication::translate("DataReplayerClass", "Image Area", 0));
    } // retranslateUi

};

namespace Ui {
    class DataReplayerClass: public Ui_DataReplayerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATAREPLAYER_H
