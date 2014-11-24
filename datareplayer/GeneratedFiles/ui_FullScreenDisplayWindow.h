/********************************************************************************
** Form generated from reading UI file 'FullScreenDisplayWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FULLSCREENDISPLAYWINDOW_H
#define UI_FULLSCREENDISPLAYWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_fullscreenwidget
{
public:
    QGridLayout *gridLayout;

    void setupUi(QWidget *fullscreenwidget)
    {
        if (fullscreenwidget->objectName().isEmpty())
            fullscreenwidget->setObjectName(QStringLiteral("fullscreenwidget"));
        fullscreenwidget->resize(517, 448);
        gridLayout = new QGridLayout(fullscreenwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));

        retranslateUi(fullscreenwidget);

        QMetaObject::connectSlotsByName(fullscreenwidget);
    } // setupUi

    void retranslateUi(QWidget *fullscreenwidget)
    {
        fullscreenwidget->setWindowTitle(QApplication::translate("fullscreenwidget", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class fullscreenwidget: public Ui_fullscreenwidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FULLSCREENDISPLAYWINDOW_H
