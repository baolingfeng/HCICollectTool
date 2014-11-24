/********************************************************************************
** Form generated from reading UI file 'playbar.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYBAR_H
#define UI_PLAYBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_playbar
{
public:
    QPushButton *playbtn;
    QPushButton *gobtn;
    QSpinBox *indexspin;
    QLabel *timestamplabel;
    QLabel *label;
    QLabel *label_3;
    QLabel *durationlabel;
    QFrame *line;
    QGroupBox *groupBox;
    QLabel *label_2;
    QLabel *label_5;
    QLabel *s1;
    QLabel *s2;

    void setupUi(QWidget *playbar)
    {
        if (playbar->objectName().isEmpty())
            playbar->setObjectName(QStringLiteral("playbar"));
        playbar->resize(671, 84);
        playbtn = new QPushButton(playbar);
        playbtn->setObjectName(QStringLiteral("playbtn"));
        playbtn->setGeometry(QRect(570, 12, 91, 61));
        gobtn = new QPushButton(playbar);
        gobtn->setObjectName(QStringLiteral("gobtn"));
        gobtn->setGeometry(QRect(490, 10, 51, 23));
        indexspin = new QSpinBox(playbar);
        indexspin->setObjectName(QStringLiteral("indexspin"));
        indexspin->setGeometry(QRect(390, 10, 91, 22));
        timestamplabel = new QLabel(playbar);
        timestamplabel->setObjectName(QStringLiteral("timestamplabel"));
        timestamplabel->setGeometry(QRect(450, 40, 71, 20));
        label = new QLabel(playbar);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(390, 40, 61, 20));
        label_3 = new QLabel(playbar);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(390, 60, 46, 20));
        durationlabel = new QLabel(playbar);
        durationlabel->setObjectName(QStringLiteral("durationlabel"));
        durationlabel->setGeometry(QRect(450, 60, 46, 20));
        line = new QFrame(playbar);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(360, 0, 20, 91));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        groupBox = new QGroupBox(playbar);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(0, 0, 121, 81));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        groupBox->setFont(font);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 20, 61, 16));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 40, 91, 16));
        s1 = new QLabel(groupBox);
        s1->setObjectName(QStringLiteral("s1"));
        s1->setGeometry(QRect(80, 20, 46, 16));
        s2 = new QLabel(groupBox);
        s2->setObjectName(QStringLiteral("s2"));
        s2->setGeometry(QRect(80, 40, 46, 16));

        retranslateUi(playbar);

        QMetaObject::connectSlotsByName(playbar);
    } // setupUi

    void retranslateUi(QWidget *playbar)
    {
        playbar->setWindowTitle(QApplication::translate("playbar", "Form", 0));
        playbtn->setText(QApplication::translate("playbar", "Play", 0));
        gobtn->setText(QApplication::translate("playbar", "Go", 0));
        timestamplabel->setText(QApplication::translate("playbar", "00:00:00:000", 0));
        label->setText(QApplication::translate("playbar", "Timestamp:", 0));
        label_3->setText(QApplication::translate("playbar", "Duration:", 0));
        durationlabel->setText(QApplication::translate("playbar", "0", 0));
        groupBox->setTitle(QApplication::translate("playbar", "Overview", 0));
        label_2->setText(QApplication::translate("playbar", "Duration:", 0));
        label_5->setText(QApplication::translate("playbar", "Number:", 0));
        s1->setText(QApplication::translate("playbar", "0", 0));
        s2->setText(QApplication::translate("playbar", "0", 0));
    } // retranslateUi

};

namespace Ui {
    class playbar: public Ui_playbar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYBAR_H
