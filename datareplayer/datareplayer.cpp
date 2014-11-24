#include "datareplayer.h"
#include "qdebug.h"
#include "qstandarditemmodel.h"
#include <sstream>

DataReplayer::DataReplayer(QWidget *parent): QWidget(parent), pthread(this), eventsBar(this)
{
	ui.setupUi(this);
	connect(ui.loadbtn, SIGNAL(clicked()), this, SLOT(loadLog()));
	connect(ui.playbtn, SIGNAL(clicked()), this, SLOT(play()));
	//connect(&pthread, SIGNAL(nextEvent()), this, SLOT(nextEvent()));
	connect(this, SIGNAL(sendDuration(double)), &pthread, SLOT(setInterval(double)));
	connect(ui.acctree, SIGNAL(clicked(const QModelIndex&)), this, SLOT(accClicked(const QModelIndex&)));
		
	//dw->initForProcess();
	//eventsBar.setGeometry(10,860, 800,30);
	
	QStandardItemModel* model = new QStandardItemModel;
	ui.acctree->setModel(model);
	index = 0;
	seq = 0;

	logReader.setLogDir("../HCICollectApp/log/");
	logMan.setEvents(logReader.read());

	dw = new DistributedWidget(this);
	dw->setGeometry(10,860, 800,60);

	//eventsBar.setLogMan(logMan);
	
	displayWindow = new FullScreenDisplayWindow;
	displayWindow->hide();
}

DataReplayer::~DataReplayer()
{

}

//cv::Mat DataReplayer::MOUSEICON = cv::imread("../icons/1.png");
//cv::Mat DataReplayer::MOUSEMASK = cv::imread("../icons/1_msk.png", 0);

void DataReplayer::displayImage(LogEvent e, cv::Mat screenshot)
{
	cv::Point pt(e.pt.x, e.pt.y);

	addImageOnFront(cv::Point(e.pt.x, e.pt.y), screenshot, MOUSEICON, MOUSEMASK);

	displayImage(screenshot);
}

void DataReplayer::displayImage(cv::Mat cimg)
{
	QImage qimg = Mat2QImage(cimg);

	int width = ui.imagelabel->size().width();
	int height = ui.imagelabel->size().height();

	QImage dispImg = qimg.scaled(width,height,Qt::KeepAspectRatio);
	ui.imagelabel->setPixmap(QPixmap::fromImage(dispImg));
	ui.imagelabel->repaint();
	ui.imagelabel->update();
}

void DataReplayer::displayImage(int seq)
{
	//string path = logMan.getScreenImagePath(seq);
	//qDebug()<<QString::fromStdString(path)<<endl;
	//QImage qimg(path.c_str());
	string timestamp = logMan.getScreenshotTimestamp(seq);

	QImage qimg = logReader.readScreenshotByQT(timestamp);
	int width = ui.imagelabel->size().width();
	int height = ui.imagelabel->size().height();

	QImage dispImg = qimg.scaled(width,height,Qt::KeepAspectRatio);
	ui.imagelabel->setPixmap(QPixmap::fromImage(dispImg));
	ui.imagelabel->repaint();
	ui.imagelabel->update();
}

void DataReplayer::loadLog()
{
	//logMan.readEventLog();
	displayImage(seq);
	dw->initForProcess(logMan);
	//eventsBar.DisplayProcessBar();
	//eventsBar.update();

	QStandardItemModel* model = (QStandardItemModel*)ui.acctree->model();
	model->clear();
	QStandardItem* root = model->invisibleRootItem();
	
	for(int i=0; i<logMan.accui_events.size(); i++)
	{
		LogEvent logEvent = logMan.getEvent(logMan.accui_events[i]);
		string time = logEvent.timestamp;
		AccElement ele = logMan.getAccElement(time);
		
		stringstream ss;
		ss<<"("<<ele.bounding.left<<" "<<ele.bounding.top<<" "
			<<ele.bounding.right<<" "<<ele.bounding.bottom<<")";
		
		string process =  logMan.events[logMan.accui_events[i]].processName;
		string window =  logMan.events[logMan.accui_events[i]].windowName;

	
		QList<QStandardItem*> firstRow;
		QStandardItem *timeItem = new QStandardItem(QString::fromStdString(time));
		QStandardItem *nameItem = new QStandardItem(QString::fromUtf8(ele.name.c_str()));
		QStandardItem *typeItem = new QStandardItem(QString::fromStdString(ele.type));
		QStandardItem *boundItem = new QStandardItem(QString::fromStdString(ss.str()));
		QStandardItem *valueItem = new QStandardItem(QString::fromStdString(ele.value));
		//QStandardItem *processItem = new QStandardItem(QString::fromStdString(process));
		//QStandardItem *windowItem = new QStandardItem(QString::fromStdString(window));

		//firstRow.push_back(timeItem);
		//firstRow.push_back(nameItem);
		root->appendRow(timeItem);

		timeItem->appendRow(nameItem);
		timeItem->appendRow(typeItem);
		timeItem->appendRow(boundItem);
		timeItem->appendRow(valueItem);
	}
	
}

void DataReplayer::play()
{
	displayWindow->init(logMan, logReader);
	/*
	seq = 0;
	index = 0;

	string timestamp = logMan.getScreenshotTimestamp(seq);
	QImage qimg = logReader.readScreenshotByQT(timestamp);

	displayWindow->showFullScreen();
	displayWindow->setImage(qimg);

	LogEvent curEvent = logMan.events[index];
	LogEvent nextEvent =  logMan.events[index+1];
	double interval = GetTimeDifference(toSystemTime(curEvent.timestamp),toSystemTime(nextEvent.timestamp));
	
	emit sendDuration(interval);
	*/
	//pthread.run();
}

void DataReplayer::nextEvent()
{
	index++;
	if(index >=  logMan.events.size() - 1) 
	{
		pthread.stop();
		return;
	}

	LogEvent curEvent = logMan.events[index];
	LogEvent nextEvent =  logMan.events[index+1];

	double interval = GetTimeDifference(toSystemTime(curEvent.timestamp),toSystemTime(nextEvent.timestamp));
	qDebug()<<"Main thread: "<<interval<<endl;

	if(index > logMan.screenshot_events[seq])
	{
		seq++;
		//displayImage(seq);
	}
	
	QImage qimg = logReader.readScreenshotByQT(logMan.getScreenshotTimestamp(seq));
	displayWindow->setImage(qimg);
	//displayImage(curEvent, logReader.readScreenshotByCV(logMan.getScreenshotTimestamp(seq)));

	emit sendDuration(interval);

}

void DataReplayer::accClicked(const QModelIndex& index)
{
	string text = index.data(Qt::DisplayRole).toString().toStdString();
	qDebug()<<QString::fromStdString(text)<<endl;
	if(!index.parent().isValid())
	{
		cv::Mat img = logReader.readScreenshotByCV(text);
		LogEvent e = logMan.getEvent(text);
		
		cv::Rect rect = fromWindowRECT(e.acc.bounding);
		cv::rectangle(img, rect, cv::Scalar(0,0,255), 2);

		addImageOnFront(cv::Point(e.pt.x, e.pt.y), img, MOUSEICON, MOUSEMASK);

		cv::circle(img, cv::Point(e.pt.x, e.pt.y), 3, cv::Scalar(0,255,0), 1);

		displayImage(img);
	}
	
}