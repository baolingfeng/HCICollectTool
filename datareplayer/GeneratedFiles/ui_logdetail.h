/********************************************************************************
** Form generated from reading UI file 'logdetail.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGDETAIL_H
#define UI_LOGDETAIL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LogDetail
{
public:
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tabwidget;
    QWidget *proc_tab;
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTableView *include_proc_view;
    QLabel *label_2;
    QListView *excluded_proc_view;
    QTabWidget *tabWidget_proc;
    QWidget *pie_tab;
    QWidget *seq_tab;
    QWidget *markov_tab;
    QWidget *action_tab;
    QVBoxLayout *verticalLayout_2;
    QTreeView *aaView;
    QWidget *key_tab;
    QVBoxLayout *verticalLayout_4;
    QTreeView *funcKeyView;
    QWidget *cp_tab;
    QVBoxLayout *verticalLayout_5;
    QTreeView *cpView;
    QWidget *tab;

    void setupUi(QWidget *LogDetail)
    {
        if (LogDetail->objectName().isEmpty())
            LogDetail->setObjectName(QStringLiteral("LogDetail"));
        LogDetail->resize(1204, 648);
        verticalLayout_3 = new QVBoxLayout(LogDetail);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        tabwidget = new QTabWidget(LogDetail);
        tabwidget->setObjectName(QStringLiteral("tabwidget"));
        proc_tab = new QWidget();
        proc_tab->setObjectName(QStringLiteral("proc_tab"));
        horizontalLayout = new QHBoxLayout(proc_tab);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        frame = new QFrame(proc_tab);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        include_proc_view = new QTableView(frame);
        include_proc_view->setObjectName(QStringLiteral("include_proc_view"));

        verticalLayout->addWidget(include_proc_view);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);

        excluded_proc_view = new QListView(frame);
        excluded_proc_view->setObjectName(QStringLiteral("excluded_proc_view"));

        verticalLayout->addWidget(excluded_proc_view);


        horizontalLayout->addWidget(frame);

        tabWidget_proc = new QTabWidget(proc_tab);
        tabWidget_proc->setObjectName(QStringLiteral("tabWidget_proc"));
        tabWidget_proc->setTabPosition(QTabWidget::West);
        pie_tab = new QWidget();
        pie_tab->setObjectName(QStringLiteral("pie_tab"));
        tabWidget_proc->addTab(pie_tab, QString());
        seq_tab = new QWidget();
        seq_tab->setObjectName(QStringLiteral("seq_tab"));
        tabWidget_proc->addTab(seq_tab, QString());
        markov_tab = new QWidget();
        markov_tab->setObjectName(QStringLiteral("markov_tab"));
        tabWidget_proc->addTab(markov_tab, QString());

        horizontalLayout->addWidget(tabWidget_proc);

        tabwidget->addTab(proc_tab, QString());
        action_tab = new QWidget();
        action_tab->setObjectName(QStringLiteral("action_tab"));
        verticalLayout_2 = new QVBoxLayout(action_tab);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        aaView = new QTreeView(action_tab);
        aaView->setObjectName(QStringLiteral("aaView"));

        verticalLayout_2->addWidget(aaView);

        tabwidget->addTab(action_tab, QString());
        key_tab = new QWidget();
        key_tab->setObjectName(QStringLiteral("key_tab"));
        verticalLayout_4 = new QVBoxLayout(key_tab);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        funcKeyView = new QTreeView(key_tab);
        funcKeyView->setObjectName(QStringLiteral("funcKeyView"));

        verticalLayout_4->addWidget(funcKeyView);

        tabwidget->addTab(key_tab, QString());
        cp_tab = new QWidget();
        cp_tab->setObjectName(QStringLiteral("cp_tab"));
        verticalLayout_5 = new QVBoxLayout(cp_tab);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        cpView = new QTreeView(cp_tab);
        cpView->setObjectName(QStringLiteral("cpView"));

        verticalLayout_5->addWidget(cpView);

        tabwidget->addTab(cp_tab, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        tabwidget->addTab(tab, QString());

        verticalLayout_3->addWidget(tabwidget);


        retranslateUi(LogDetail);

        tabwidget->setCurrentIndex(0);
        tabWidget_proc->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(LogDetail);
    } // setupUi

    void retranslateUi(QWidget *LogDetail)
    {
        LogDetail->setWindowTitle(QApplication::translate("LogDetail", "Log Information", 0));
        label->setText(QApplication::translate("LogDetail", "Included Process", 0));
        label_2->setText(QApplication::translate("LogDetail", "Excluded Process", 0));
        tabWidget_proc->setTabText(tabWidget_proc->indexOf(pie_tab), QApplication::translate("LogDetail", "Pie Chart", 0));
        tabWidget_proc->setTabText(tabWidget_proc->indexOf(seq_tab), QApplication::translate("LogDetail", "Sequence Chart", 0));
        tabWidget_proc->setTabText(tabWidget_proc->indexOf(markov_tab), QApplication::translate("LogDetail", "Markov Chart", 0));
        tabwidget->setTabText(tabwidget->indexOf(proc_tab), QApplication::translate("LogDetail", "Overview", 0));
        tabwidget->setTabText(tabwidget->indexOf(action_tab), QApplication::translate("LogDetail", "Abstract Actions", 0));
        tabwidget->setTabText(tabwidget->indexOf(key_tab), QApplication::translate("LogDetail", "Function Key Input", 0));
        tabwidget->setTabText(tabwidget->indexOf(cp_tab), QApplication::translate("LogDetail", "Copy&&Paste", 0));
        tabwidget->setTabText(tabwidget->indexOf(tab), QApplication::translate("LogDetail", "Key Word using Search", 0));
    } // retranslateUi

};

namespace Ui {
    class LogDetail: public Ui_LogDetail {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGDETAIL_H
