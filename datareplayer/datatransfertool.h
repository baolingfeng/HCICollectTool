#ifndef DATATRANSFERTOOL_H
#define DATATRANSFERTOOL_H

#include <QMainWindow>
#include "ui_DataTransferTool.h"
#include <QTreeView>
#include <QGroupBox>
#include <QStandardItemModel>
#include <QVBoxLayout>

#include <vector>
#include "DBInterface.h"

/*
QT_BEGIN_NAMESPACE
class QAbstractItemModel;
class QCheckBox;
class QComboBox;
class QDateEdit;
class QGroupBox;
class QTreeView;
QT_END_NAMESPACE
*/

class DataTransferTool : public QMainWindow
{
	Q_OBJECT

public:
	DataTransferTool(QWidget *parent=0);
	~DataTransferTool();

	void displayMouseEvent();
	void displayKeyEvent();
	void displayCopyEvent();

public slots:
	void loadLogFile();
	void exportToDB();
	void filterEvents();
private:
	Ui::DataTransferWindow ui;

	QGroupBox *groupbox;
	QTreeView *logView;

	QString logDir;
	QString from_timestamp;
	QString to_timestamp;
	vector<LogEvent> mevents;
	vector<LogEvent> kevents;
	vector<CopyEvent> cevents;
	vector<LogEvent> originalMevents;
	vector<LogEvent> originalKevents;
	vector<CopyEvent> originalCevents;

	DBInterface db;
};

#endif // DATATRANSFERTOOL_H
