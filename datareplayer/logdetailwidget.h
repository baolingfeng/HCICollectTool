#ifndef LOGDETAILWIDGET_H
#define LOGDETAILWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QLayout>
#include <QMenu>
#include <QTextEdit>
#include <QScrollArea>
#include "ui_logdetail.h"
#include "LogManager.h"
#include "DBInterface.h"
#include "pieview.h"
#include "util.h"
#include "distributedwidget.h"
#include "diff_match_patch.h"

class LogDetailWidget : public QWidget
{
	Q_OBJECT

public:
	LogDetailWidget(QWidget *parent, QString name);
	~LogDetailWidget();

	void setupModel();
	void classifyAction();
	void generateMarkov();
	void funcKeyInput();
	void copyPaste();

public slots:
	void onCustomMenuRequested(const QPoint &);
	void showScreenshot();
	void showEditHistory();
private:
	Ui::LogDetail ui;
	QStandardItemModel *model;
	QVBoxLayout *pieLayout;
	PieView *pieChart;
	QVBoxLayout *markovLayout;
	QVBoxLayout *seqLayout;
	DistributedWidget* seqChart;

	DBInterface* db;
	LogManager logMan;
	QString name;
};

#endif // LOGDETAILWIDGET_H
