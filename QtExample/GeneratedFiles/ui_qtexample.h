/********************************************************************************
** Form generated from reading UI file 'qtexample.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTEXAMPLE_H
#define UI_QTEXAMPLE_H

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

class Ui_QtExampleClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtExampleClass)
    {
        if (QtExampleClass->objectName().isEmpty())
            QtExampleClass->setObjectName(QStringLiteral("QtExampleClass"));
        QtExampleClass->resize(600, 400);
        menuBar = new QMenuBar(QtExampleClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        QtExampleClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtExampleClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QtExampleClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(QtExampleClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QtExampleClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(QtExampleClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QtExampleClass->setStatusBar(statusBar);

        retranslateUi(QtExampleClass);

        QMetaObject::connectSlotsByName(QtExampleClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtExampleClass)
    {
        QtExampleClass->setWindowTitle(QApplication::translate("QtExampleClass", "QtExample", 0));
    } // retranslateUi

};

namespace Ui {
    class QtExampleClass: public Ui_QtExampleClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTEXAMPLE_H
