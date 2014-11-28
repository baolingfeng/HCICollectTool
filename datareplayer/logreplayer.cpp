#include "logreplayer.h"
#include <QDesktopWidget>
#include <QDebug>

string getTimeLabel(double interval)
{
	int hour = interval / 3600;
	int minute = (interval - hour * 3600) / 60;
	int second = interval - hour * 3600 - minute * 60;

	char temp[20];
	sprintf(temp, "%02d:%02d:%02d",hour, minute, second);

	return string(temp);
}

LogReplayer::LogReplayer(QWidget *parent, string name, DBInterface *db)
	: QWidget(parent)
{
	this->logName = name;
	this->db = db;
	curIndex = 0;

	events = db->getAllEvents(name);
	for(int i=0; i<events.size(); i++)
	{
		if(events[i].isHasSrceenshot)
		{
			curImage = db->getScreenshot(name, events[i].timestamp, events[i].eventType);
			curScreenIndex = i;
			break;
		}
	}

	layout = new QVBoxLayout;
	this->setLayout(layout);

	imgLabel = new QLabel;
	layout->addWidget(imgLabel);
	
	sliderFrame = new QFrame;
	sliderLayout = new QHBoxLayout;
	sliderFrame->setLayout(sliderLayout);

	startLabel = new QLabel;
	startLabel->setText("00:00");
	sliderLayout->addWidget(startLabel);

	slider = new QSlider;
	slider->setOrientation(Qt::Orientation::Horizontal);
	sliderLayout->addWidget(slider);
	slider->setEnabled(false);
	slider->setMaximum(99);

	totalTime = GetTimeDifference(toSystemTime(events[0].timestamp), toSystemTime(events[events.size()-1].timestamp));

	endLabel = new QLabel;
	endLabel->setText(QString::fromStdString(getTimeLabel(totalTime)));
	sliderLayout->addWidget(endLabel);
	
	layout->addWidget(sliderFrame);


	bottomFrame = new QFrame;
	bottomLayout = new QHBoxLayout;
	bottomFrame->setLayout(bottomLayout);

	curIndexLabel = new QLabel;
	curIndexLabel->setText("Current Index");
	bottomLayout->addWidget(curIndexLabel);

	curIndexValue = new QLabel;
	curIndexValue->setText("0");
	bottomLayout->addWidget(curIndexValue);

	curTimestampLabel = new QLabel;
	curTimestampLabel->setText("Current Timestamp");
	bottomLayout->addWidget(curTimestampLabel);

	curTimestampValue = new QLabel;
	curTimestampValue->setText("2000-10-11-00-00-00-000");
	bottomLayout->addWidget(curTimestampValue);

	playBtn = new QPushButton;
	playBtn->setText("Pause");
	bottomLayout->addWidget(playBtn);

	layout->addWidget(bottomFrame);

	QDesktopWidget widget;
	QRect mainScreenSize = widget.availableGeometry(widget.primaryScreen()); 
	int width = mainScreenSize.width();
	int height = mainScreenSize.height()-90;
	
	imgLabel->resize(1898 , 906);
	displayImage();

	timer = new QTimer;
	connect(timer, SIGNAL(timeout()), this, SLOT(playEvents()));
	
	timer->setInterval(getInterval() * 1000);
	timer->start();
	
}

LogReplayer::~LogReplayer()
{

}

double LogReplayer::getInterval()
{
	if(curIndex >= events.size() - 1) return 0;

	return GetTimeDifference(toSystemTime(events[curIndex].timestamp), toSystemTime(events[curIndex+1].timestamp));
}

void LogReplayer::playEvents()
{
	curIndex++;

	if(curIndex > events.size()-1) return;

	timer->setInterval(getInterval() * 1000);

	if(curIndex > curScreenIndex && curScreenIndex< events.size() - 1)
	{
		for(int i=curScreenIndex+1; i<events.size(); i++)
		{
			if(events[i].isHasSrceenshot)
			{
				curImage = db->getScreenshot(logName, events[i].timestamp, events[i].eventType);
				curScreenIndex = i;
				break;
			}
		}
	}

	curIndexValue->setText(QString::number(curIndex));
	curTimestampValue->setText(QString::fromStdString(events[curIndex].timestamp));

	double interval = GetTimeDifference(toSystemTime(events[0].timestamp), toSystemTime(events[curIndex].timestamp));
	int k = interval / totalTime * 100;
	slider->setValue(k);


	displayImage();
	timer->start();
}

void LogReplayer::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::WindowStateChange)
	{
		if (static_cast<QWindowStateChangeEvent*>(event)->oldState() == windowState())
		{
			return;
		}

		if (isMaximized())
		{
			//displayImage();
		}
		
	}
	QWidget::changeEvent(event);
}

void LogReplayer::displayImage()
{
	int width = imgLabel->width();
	int height = imgLabel->height();

	//qDebug()<<width<<height;
	if(events[curIndex].eventType == "keyinput") return;

	cv::Mat m = addImageOnFront(cv::Point(events[curIndex].pt.x, events[curIndex].pt.y), curImage, MOUSEICON, MOUSEMASK);

	QImage qimg = Mat2QImage(m);

	QImage dispImg = qimg.scaled(width,height);

	imgLabel->setPixmap(QPixmap::fromImage(dispImg));
	imgLabel->repaint();
	imgLabel->update();
}