#include <QMainWindow>
#include "datatransfertool.h"
#include <QFileDialog>
#include <QDebug>
#include "LogEvent.h"
#include "LogReader.h"
#include "util.h"
#include "DBInterface.h"

DataTransferTool::DataTransferTool(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	QStandardItemModel* mouseModel = new QStandardItemModel(0, 10, this);
	mouseModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Timestamp"));
	mouseModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Event Name"));
	mouseModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Position"));
	mouseModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Window Name"));
	mouseModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Process Name"));
	mouseModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Parent Window"));
	mouseModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Window Rectangle"));
	mouseModel->setHeaderData(7, Qt::Horizontal, QObject::tr("Has Screenshot"));
	mouseModel->setHeaderData(8, Qt::Horizontal, QObject::tr("Has Accessibility API Action"));
	mouseModel->setHeaderData(9, Qt::Horizontal, QObject::tr("Duration"));
	ui.mouseView->setModel(mouseModel);

	QStandardItemModel* keyinputModel = new QStandardItemModel(0, 7, this);
	keyinputModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Timestamp"));
	keyinputModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Key Input"));
	keyinputModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Position"));
	keyinputModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Window Name"));
	keyinputModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Process Name"));
	keyinputModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Parent Window"));
	keyinputModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Has Screenshot"));
	ui.keyView->setModel(keyinputModel);

	QStandardItemModel* actionModel = new QStandardItemModel(0, 7, this);
	actionModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Timestamp"));
	actionModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Window Name"));
	actionModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Process Name"));
	actionModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Action Name"));
	actionModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Action Type"));
	actionModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Action Value"));
	actionModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Bounding"));
	ui.actionView->setModel(actionModel);

	connect(ui.actionImport, SIGNAL(triggered()), this, SLOT(loadLogFile()));
	connect(ui.actionExport_to_Database, SIGNAL(triggered()), this, SLOT(exportToDB()));
	//test
	//DBInterface db;
	//db.insertLogData("test");
}

DataTransferTool::~DataTransferTool()
{

}

void DataTransferTool::loadLogFile()
{
	QFileDialog *fileDialog = new QFileDialog(this, "Choose a Log File Directory", QDir::currentPath());
	fileDialog->setFileMode(QFileDialog::Directory);
	if(fileDialog->exec() == QDialog::Accepted) 
	{
		QString path = fileDialog->selectedFiles()[0] + "/";
		LogReader logReader(path.toStdString());

		vector<LogEvent> mevents = logReader.readMouseEvents();
		vector<LogEvent> kevents = logReader.readKeyInputEvents();

		QAbstractItemModel* mouseModel = ui.mouseView->model();	
		QAbstractItemModel* actionModel = ui.actionView->model();
		int actionIndex = 0;
		for(int i=0; i<mevents.size(); i++)
		{
			mouseModel->insertRow(i);
			mouseModel->setData(mouseModel->index(i, 0), QString::fromStdString(mevents[i].timestamp));
			mouseModel->setData(mouseModel->index(i, 1), QString::fromStdString(mevents[i].name));
			mouseModel->setData(mouseModel->index(i, 2), "(" + QString::number(mevents[i].pt.x) + " , " + QString::number(mevents[i].pt.y) + ")" );
			mouseModel->setData(mouseModel->index(i, 3), QString::fromStdString(mevents[i].windowName));
			mouseModel->setData(mouseModel->index(i, 4), QString::fromStdString(mevents[i].processName));
			mouseModel->setData(mouseModel->index(i, 5), QString::fromStdString(mevents[i].parentWindowName));
			mouseModel->setData(mouseModel->index(i, 6), "(" + QString::number(mevents[i].winRect.left)  + " , " + QString::number(mevents[i].winRect.top)  
				+ " , " + QString::number(mevents[i].winRect.right)  + " , " + QString::number(mevents[i].winRect.bottom)  + ")");
			mouseModel->setData(mouseModel->index(i, 7), mevents[i].isHasSrceenshot);
			mouseModel->setData(mouseModel->index(i, 8), mevents[i].isHasAcc);

			double duration = 0;
			if(i < mevents.size() - 1)
			{
				duration = GetTimeDifference(toSystemTime(mevents[i].timestamp), toSystemTime(mevents[i+1].timestamp));
			}
			mouseModel->setData(mouseModel->index(i, 9), duration);

			if(mevents[i].isHasAcc)
			{
				actionModel->insertRow(actionIndex);
				actionModel->setData(actionModel->index(actionIndex, 0), QString::fromStdString(mevents[i].timestamp));
				actionModel->setData(actionModel->index(actionIndex, 1), QString::fromStdString(mevents[i].windowName));
				actionModel->setData(actionModel->index(actionIndex, 2), QString::fromStdString(mevents[i].processName));
				actionModel->setData(actionModel->index(actionIndex, 3), QString::fromStdString(mevents[i].acc.name));
				actionModel->setData(actionModel->index(actionIndex, 4), QString::fromStdString(mevents[i].acc.type));
				actionModel->setData(actionModel->index(actionIndex, 5), QString::fromStdString(mevents[i].acc.value));
				actionModel->setData(actionModel->index(actionIndex, 6), "(" + QString::number(mevents[i].acc.bounding.left)  + " , " + QString::number(mevents[i].acc.bounding.top)  
				+ " , " + QString::number(mevents[i].acc.bounding.right)  + " , " + QString::number(mevents[i].acc.bounding.bottom)  + ")");
				
				actionIndex++;
			}

		}
		
		QAbstractItemModel* keyModel = ui.keyView->model();
		for(int i=0; i<kevents.size(); i++)
		{
			keyModel->insertRow(i);
			keyModel->setData(keyModel->index(i, 0), QString::fromStdString(kevents[i].timestamp));
			keyModel->setData(keyModel->index(i, 1), QString::fromStdString(kevents[i].name));
			keyModel->setData(keyModel->index(i, 2), "(" + QString::number(kevents[i].pt.x) + " , " + QString::number(kevents[i].pt.y) + ")" );
			keyModel->setData(keyModel->index(i, 3), QString::fromStdString(kevents[i].windowName));
			keyModel->setData(keyModel->index(i, 4), QString::fromStdString(kevents[i].processName));
			keyModel->setData(keyModel->index(i, 5), QString::fromStdString(kevents[i].parentWindowName));
			//keyModel->setData(keyModel->index(i, 6), "(" + QString::number(mevents[i].winRect.left)  + " , " + QString::number(mevents[i].winRect.top)  
			//	+ " , " + QString::number(mevents[i].winRect.right)  + " , " + QString::number(mevents[i].winRect.bottom)  + ")");
			keyModel->setData(keyModel->index(i, 7), kevents[i].isHasSrceenshot);
			
		}
	}
}

void DataTransferTool::exportToDB()
{
	QAbstractItemModel* mouseModel = ui.mouseView->model();

	int row = mouseModel->rowCount();

	if(!db.initTable("test"))
	{
		qDebug()<<"init fail";
		return;
	}

	for(int i=0; i<row; i++)
	{
		QString timestamp =    mouseModel->data(mouseModel->index(i,0)).toString();
		QString name =		   mouseModel->data(mouseModel->index(i,1)).toString();
		QString position =	   mouseModel->data(mouseModel->index(i,2)).toString();
		QString windowName =   mouseModel->data(mouseModel->index(i,3)).toString();
		QString processName =  mouseModel->data(mouseModel->index(i,4)).toString();
		QString parentWindow = mouseModel->data(mouseModel->index(i,5)).toString();
		QString winRect =	   mouseModel->data(mouseModel->index(i,6)).toString();
		bool hasScreenshot =   mouseModel->data(mouseModel->index(i,7)).toBool();
		bool hasAcc =		   mouseModel->data(mouseModel->index(i,8)).toBool();

		LogEvent e;
		e.timestamp = timestamp.toStdString();
		e.name = name.toStdString();
		int index = position.indexOf(",");
		e.pt.x = position.mid(1, index-1).toInt();
		e.pt.y = position.mid(index+2, position.length()-index-3).toInt();

		e.windowName = windowName.toStdString();
		e.processName = processName.toStdString();
		e.parentWindowName = parentWindow.toStdString();
		index = winRect.indexOf(",");
		e.winRect.left = winRect.mid(1, index-1).toInt();
		int index2 = winRect.indexOf(",", index+1);
		e.winRect.top = winRect.mid(index+2, index2-index-2).toInt();
		index = index2;
		index2 = winRect.indexOf(",", index+1);
		e.winRect.right = winRect.mid(index+2, index2-index-2).toInt();
		QString temp =  winRect.mid(index2+2, winRect.length()-index2-3);
		e.winRect.bottom = winRect.mid(index2+2, winRect.length()-index2-3).toInt();

		e.isHasSrceenshot = hasScreenshot;
		e.isHasAcc = hasAcc;

		db.insertMouseEvent("test", e);
	}

}