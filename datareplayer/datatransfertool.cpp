#include <QMainWindow>
#include "datatransfertool.h"
#include <QFileDialog>
#include <QDebug>
#include <QProgressDialog>
#include <QInputDialog>
#include <QMessageBox>
#include "LogEvent.h"
#include "LogReader.h"
#include "util.h"
#include "DBInterface.h"

DataTransferTool::DataTransferTool(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	/*
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
	*/

	connect(ui.actionImport, SIGNAL(triggered()), this, SLOT(loadLogFile()));
	connect(ui.actionExport_to_Database, SIGNAL(triggered()), this, SLOT(exportToDB()));
	connect(ui.filterBtn, SIGNAL(clicked()), this, SLOT(filterEvents()));
	//test
	//DBInterface db;
	//db.insertLogData("test");
}

DataTransferTool::~DataTransferTool()
{

}

bool validateTimeFormat(QString timestamp)
{
	vector<string> arr = split(timestamp.toStdString(), "-");
	if(arr.size() != 7)
	{
		qDebug() << "Timestamp is not right!";
		return false;
	}

	return true;
	/*
	string year = arr[0];
	if(year.length() != 4 && stringToNumber<int>(year) <= 0)
	{
		msg = "the length of Year must be 4";
		return false;
	}
	*/
}

void DataTransferTool::filterEvents()
{
	from_timestamp =  ui.fromEdit->text();
	to_timestamp = ui.toEdit->text();

	if(!validateTimeFormat(from_timestamp) || !validateTimeFormat(to_timestamp))
	{
		QMessageBox::information(this, tr("Timestamp format error"),
                                tr("Please input the right timestamp!"),
								   QMessageBox::Ok );
		return;
	}

	mevents.clear();
	for(int i=0; i<originalMevents.size(); i++)
	{
		LogEvent e = originalMevents[i];
		double interval1 = GetTimeDifference(toSystemTime(from_timestamp.toStdString()), toSystemTime(e.timestamp));
		double interval2 = GetTimeDifference(toSystemTime(e.timestamp), toSystemTime(to_timestamp.toStdString()));
		
		if(interval1<0) continue;

		if(interval2<0) break;

		mevents.push_back(e);
	}

	kevents.clear();
	for(int i=0; i<originalKevents.size(); i++)
	{
		LogEvent e = originalKevents[i];
		double interval1 = GetTimeDifference(toSystemTime(from_timestamp.toStdString()), toSystemTime(e.timestamp));
		double interval2 = GetTimeDifference(toSystemTime(e.timestamp), toSystemTime(to_timestamp.toStdString()));
		
		if(interval1<0) continue;

		if(interval2<0) break;

		kevents.push_back(e);
	}

	if(mevents.size() > 0)
	{
		double interval = GetTimeDifference(toSystemTime(mevents[0].timestamp), toSystemTime(mevents[mevents.size()-1].timestamp));
		ui.totalEdit->setText(QString::number(interval));
	}

	displayMouseEvent();
	displayKeyEvent();
	displayCopyEvent();
	//for(int )
}

void DataTransferTool::displayMouseEvent()
{
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

	QStandardItemModel* actionModel = new QStandardItemModel(0, 7, this);
	actionModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Timestamp"));
	actionModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Window Name"));
	actionModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Process Name"));
	actionModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Action Name"));
	actionModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Action Type"));
	actionModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Action Value"));
	actionModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Bounding"));
	ui.actionView->setModel(actionModel);

	//QAbstractItemModel* mouseModel = ui.mouseView->model();	
	//QAbstractItemModel* actionModel = ui.actionView->model();
	
	int actionIndex = 0;
	for(int i=0; i<mevents.size(); i++)
	{
		mouseModel->insertRow(i);
		mouseModel->setData(mouseModel->index(i, 0), QString::fromStdString(mevents[i].timestamp));
		mouseModel->setData(mouseModel->index(i, 1), QString::fromStdString(mevents[i].name));
		mouseModel->setData(mouseModel->index(i, 2), "(" + QString::number(mevents[i].pt.x) + " , " + QString::number(mevents[i].pt.y) + ")" );
		mouseModel->setData(mouseModel->index(i, 3), QString::fromLocal8Bit(mevents[i].windowName.c_str()));
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
			actionModel->setData(actionModel->index(actionIndex, 1), QString::fromLocal8Bit(mevents[i].windowName.c_str()));
			actionModel->setData(actionModel->index(actionIndex, 2), QString::fromStdString(mevents[i].processName));
			actionModel->setData(actionModel->index(actionIndex, 3), QString::fromLocal8Bit(mevents[i].acc.name.c_str()));
			actionModel->setData(actionModel->index(actionIndex, 4), QString::fromStdString(mevents[i].acc.type));
			actionModel->setData(actionModel->index(actionIndex, 5), QString::fromStdString(mevents[i].acc.value));
			actionModel->setData(actionModel->index(actionIndex, 6), "(" + QString::number(mevents[i].acc.bounding.left)  + " , " + QString::number(mevents[i].acc.bounding.top)  
			+ " , " + QString::number(mevents[i].acc.bounding.right)  + " , " + QString::number(mevents[i].acc.bounding.bottom)  + ")");
				
			actionIndex++;
		}

	}
}

void DataTransferTool::displayKeyEvent()
{
	QStandardItemModel* keyModel = new QStandardItemModel(0, 7, this);
	keyModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Timestamp"));
	keyModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Key Input"));
	keyModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Position"));
	keyModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Window Name"));
	keyModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Process Name"));
	keyModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Parent Window"));
	keyModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Has Screenshot"));
	ui.keyView->setModel(keyModel);

	for(int i=0; i<kevents.size(); i++)
	{
		keyModel->insertRow(i);
		keyModel->setData(keyModel->index(i, 0), QString::fromStdString(kevents[i].timestamp));
		keyModel->setData(keyModel->index(i, 1), QString::fromStdString(kevents[i].name));
		keyModel->setData(keyModel->index(i, 2), "(" + QString::number(kevents[i].pt.x) + " , " + QString::number(kevents[i].pt.y) + ")" );
		keyModel->setData(keyModel->index(i, 3), QString::fromLocal8Bit(kevents[i].windowName.c_str()));
		keyModel->setData(keyModel->index(i, 4), QString::fromStdString(kevents[i].processName));
		keyModel->setData(keyModel->index(i, 5), QString::fromStdString(kevents[i].parentWindowName));
		//keyModel->setData(keyModel->index(i, 6), "(" + QString::number(mevents[i].winRect.left)  + " , " + QString::number(mevents[i].winRect.top)  
		//	+ " , " + QString::number(mevents[i].winRect.right)  + " , " + QString::number(mevents[i].winRect.bottom)  + ")");
		keyModel->setData(keyModel->index(i, 7), kevents[i].isHasSrceenshot);
			
	}
}

void DataTransferTool::displayCopyEvent()
{
	QStandardItemModel* copyModel = new QStandardItemModel(0, 5, this);
	copyModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Timestamp"));
	copyModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Window Name"));
	copyModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Process Name"));
	copyModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Parent Window"));
	copyModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Text"));
	ui.copyView->setModel(copyModel);
	
	for(int i=0; i<cevents.size(); i++)
	{
		copyModel->insertRow(i);
		copyModel->setData(copyModel->index(i, 0), QString::fromStdString(cevents[i].timestamp));
		copyModel->setData(copyModel->index(i, 1), QString::fromLocal8Bit(cevents[i].windowName.c_str()));	
		copyModel->setData(copyModel->index(i, 2), QString::fromStdString(cevents[i].processName));
		copyModel->setData(copyModel->index(i, 3), QString::fromStdString(cevents[i].parentWindowName));
		copyModel->setData(copyModel->index(i, 4), QString::fromStdString(cevents[i].text));		
	}
}

void DataTransferTool::loadLogFile()
{
	QFileDialog *fileDialog = new QFileDialog(this, "Choose a Log File Directory", QDir::currentPath());
	fileDialog->setFileMode(QFileDialog::Directory);
	if(fileDialog->exec() == QDialog::Accepted) 
	{
		logDir = fileDialog->selectedFiles()[0] + "/";
		LogReader logReader(logDir.toStdString());

		mevents = logReader.readMouseEvents();
		kevents = logReader.readKeyInputEvents();
		cevents = logReader.readCopyEvents();

		originalMevents = mevents;
		originalKevents = kevents;
		originalCevents = cevents;

		if(mevents.size() > 0)
		{
			ui.fromEdit->setText(QString::fromStdString(mevents[0].timestamp));
			ui.toEdit->setText(QString::fromStdString(mevents[mevents.size()-1].timestamp));
			
			ui.fromEdit->setToolTip("Time Format: YYYY-MM-DD-hh-mm-ss-mis");

			double totalTime = GetTimeDifference(toSystemTime(mevents[0].timestamp),toSystemTime(mevents[mevents.size()-1].timestamp));
			ui.totalEdit->setText(QString::number(totalTime));
		}

		displayMouseEvent();
		displayKeyEvent();
		displayCopyEvent();
	}
}

string encodeString(string str)
{
	return QString::fromLocal8Bit(str.c_str()).toStdString();
}

void encodeLogEvent(LogEvent& e)
{
	e.windowName = encodeString(e.windowName.c_str());
	e.acc.name = encodeString(e.acc.name.c_str());
	e.acc.value = encodeString(e.acc.value);
}

void encodeCopyEvent(CopyEvent& e)
{
	e.windowName = encodeString(e.windowName.c_str());
	e.text = encodeString(e.text.c_str());
}

void DataTransferTool::exportToDB()
{
	bool ok;
    QString logName = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                        tr("Please input a log name:"), QLineEdit::Normal,
				                         "", &ok);

	if (!ok || logName.isEmpty()) return;
    
	string name = logName.toStdString();
	if(db.isExistsLog(name))
	{
		QMessageBox::information(this, tr("name has existed in database"),
                                tr("Please try again!"),
								   QMessageBox::Ok );
		return;
	}


	QAbstractItemModel* mouseModel = ui.mouseView->model();

	int row = mouseModel->rowCount();

	if(!db.initTable(name))
	{
		qDebug()<<"init fail";
		return;
	}
	
	QProgressDialog *progressDlg=new QProgressDialog(this);  
    progressDlg->setWindowModality(Qt::WindowModal);  
    progressDlg->setMinimumDuration(5);  
    progressDlg->setWindowTitle(tr("please wait"));  
	
	progressDlg->setLabelText(tr("inserting mouse record into database...."));  
    progressDlg->setRange(0,mevents.size()-1);  

	for(int i=0; i<mevents.size(); i++)
	{
		progressDlg->setValue(i);
		/*
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
		*/

		encodeLogEvent(mevents[i]);
		db.insertMouseEvent(name, mevents[i], logDir.toStdString());
	}

	progressDlg->setValue(0);
	progressDlg->setLabelText(tr("inserting keyinput record into database...."));  
	progressDlg->setRange(0,kevents.size()-1); 
	for(int i=0; i<kevents.size(); i++)
	{
		progressDlg->setValue(i);
		encodeLogEvent(kevents[i]);
		db.insertKeyEvent(name, kevents[i], logDir.toStdString());
	}

	progressDlg->setValue(0);
	progressDlg->setLabelText(tr("inserting copy record into database...."));  
	progressDlg->setRange(0,cevents.size()-1); 
	for(int i=0; i<cevents.size(); i++)
	{
		encodeCopyEvent(cevents[i]);
		db.insertCopyEvent(name, cevents[i], logDir.toStdString());
	}

	progressDlg->close();
}