#ifndef LOGOVERVIEW_H
#define LOGOVERVIEW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "ui_LogOverview.h"
#include "DBInterface.h"
#include "dao.h"
#include "datatransfertool.h"
#include "logreplayer.h"

class LogOverview : public QMainWindow
{
	Q_OBJECT

public:
	LogOverview(QWidget *parent=0);
	~LogOverview();

	void loadLogs();

public slots:
	void showImportTool();
	void replay();
	void onCustomMenuRequested(const QPoint &pos);
	void refresh();

private:

	DBInterface* db;
	Ui::LogOverview ui;

	DataTransferTool* dttool;
	LogReplayer* replayer;
};

#endif // LOGOVERVIEW_H
