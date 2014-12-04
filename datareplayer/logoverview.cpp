#include "logoverview.h"

LogOverview::LogOverview(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	dttool = new DataTransferTool(this);
	//replayer = new LogReplayer(this);

	db = new DBInterface();

	loadLogs();

	ui.logList->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(ui.actionImport, SIGNAL(triggered()), this, SLOT(showImportTool()));
	connect(ui.logList , SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomMenuRequested(const QPoint &)));
	connect(ui.actionReplay, SIGNAL(triggered()), this, SLOT(replay()));
}

LogOverview::~LogOverview()
{
	delete db;
}

void LogOverview::loadLogs()
{
	vector<LogRecord> records =  db->getAllLogRecords();

	QStandardItemModel* model = new QStandardItemModel(0, 2, this);
	model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
	model->setHeaderData(1, Qt::Horizontal, QObject::tr("Create Time"));
	ui.logList->setModel(model);

	for(int i=0; i<records.size(); i++)
	{
		model->insertRow(i);
		model->setData(model->index(i, 0), QString::fromStdString(records[i].name));
		model->setData(model->index(i, 1), QString::fromStdString(records[i].date));
	}
}

void LogOverview::showImportTool()
{
	dttool->show();
}

void LogOverview::onCustomMenuRequested(const QPoint &pos)
{
    QMenu *menu=new QMenu(this);
	QAction *refresh = new QAction("Refresh", this);
	QAction *viewDetail = new QAction("View Detail Information", this);
	QAction *del = new QAction("Delete the Log", this);
    menu->addAction(refresh);
	menu->addAction(viewDetail);
	menu->addAction(del);
    
    menu->popup(ui.logList->viewport()->mapToGlobal(pos));

	connect(refresh, SIGNAL(triggered()), this, SLOT(refresh()));
	connect(viewDetail, SIGNAL(triggered()), this, SLOT(veiwDetail()));
	connect(del, SIGNAL(triggered()), this, SLOT(delLog()));
}

void LogOverview::refresh()
{
	loadLogs();
}

void LogOverview::replay()
{
	QModelIndex index = ui.logList->currentIndex();
	QAbstractItemModel* model = ui.logList->model();

	QString name = model->data(model->index(index.row(), 0), Qt::DisplayRole).toString();
	
	replayer = new LogReplayer(0, name.toStdString(), db);
	replayer->showMaximized();
}

void LogOverview::veiwDetail()
{
	QModelIndex index = ui.logList->currentIndex();
	if(!index.isValid()) return;

	QAbstractItemModel* model = ui.logList->model();
	QString name = model->data(model->index(index.row(), 0), Qt::DisplayRole).toString();

	LogDetailWidget* w = new LogDetailWidget(NULL, name);
	w->show();
	w->setWindowModality(Qt::WindowModality::ApplicationModal);
}

void LogOverview::delLog()
{
	QModelIndex index = ui.logList->currentIndex();
	if(!index.isValid()) return;
	
	QAbstractItemModel* model = ui.logList->model();
	QString name = model->data(model->index(index.row(), 0), Qt::DisplayRole).toString();
	
	db->delLog(name.toStdString());

	refresh();
}