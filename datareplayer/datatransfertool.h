#ifndef DATATRANSFERTOOL_H
#define DATATRANSFERTOOL_H

#include <QMainWindow>
#include "ui_DataTransferTool.h"
#include <QTreeView>
#include <QGroupBox>
#include <QStandardItemModel>
#include <QVBoxLayout>

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

public slots:
	void loadLogFile();
	void exportToDB();
private:
	Ui::DataTransferWindow ui;

	QGroupBox *groupbox;
	QTreeView *logView;

	DBInterface db;
};

#endif // DATATRANSFERTOOL_H
