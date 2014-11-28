#include "fullscreendisplaywindow.h"
#include "qlayout.h"
#include <QDesktopWidget>
#include <qdebug.h>
#include <Windows.h>
#include <qtimer.h>


FullScreenDisplayWindow::FullScreenDisplayWindow(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);
	
	QDesktopWidget widget;
	QRect mainScreenSize = widget.availableGeometry(widget.primaryScreen()); 
	int width = mainScreenSize.width();
	int height = mainScreenSize.height()-60;

	this->setGeometry(mainScreenSize.x(), mainScreenSize.y(), width, height);
	this->setFixedSize(width, height);
	
	QWidget* bar = new QWidget(this);
	barui.setupUi(bar);
	bar->setGeometry(0, height-90, width, 90);

	imageLabel = new QLabel(this);
	imageLabel->setGeometry(QRect(0,0, width, height-100));
	
	timer = new QTimer;
	
	this->showMaximized();

	connect(barui.playbtn, SIGNAL(clicked()), this , SLOT(play()));
	connect(timer, SIGNAL(timeout()), this, SLOT(changeImage()));
	
}

FullScreenDisplayWindow::~FullScreenDisplayWindow()
{

}

void FullScreenDisplayWindow::play()
{
	QString btnName = barui.playbtn->text();
	if(btnName == "Play")
	{
		double interval = logMan.events[index].duration;
		timer->setInterval(interval*1000);

		timer->start();	
		barui.playbtn->setText("Stop");
	}
	else if(btnName == "Stop")
	{
		barui.playbtn->setText("Play");
		timer->stop();
	}
	
}



void FullScreenDisplayWindow::keyPressEvent(QKeyEvent *key)
{
	
	switch ( key->key() ) 
	{
	case Qt::Key_Escape:
		this->close(); 
		break;
		//this->hide();
	case Qt::Key_F1:
		//pthread->run(); 
		break;
	default:
		return;
	}
}

void FullScreenDisplayWindow::init(LogManager& logMan, LogReader& logReader)
{
	index = seq = 0;
	time = 0;
	this->logMan = logMan;
	this->logReader = logReader;
	
	this->show();
	this->setFixedSize(this->width(), this->height());

	setImage();
	barui.s1->setText(QString::number(logMan.totaltime));
	barui.s2->setText(QString::number(logMan.events.size()));
	barui.indexspin->setMaximum(logMan.events.size()-1);
}

void FullScreenDisplayWindow::setImage()
{
	LogEvent e = logMan.events[index];
	cv::Point pt(e.pt.x, e.pt.y);

	cv::Mat screenshot = logReader.readScreenshotByCV(logMan.getScreenshotTimestamp(seq));

	cv::Mat m = addImageOnFront(cv::Point(e.pt.x, e.pt.y), screenshot, MOUSEICON, MOUSEMASK);

	QImage qimg = Mat2QImage(m);

	setImage(qimg);
}

void FullScreenDisplayWindow::setImage(QImage img)
{

	int width = imageLabel->size().width();
	int height = imageLabel->size().height();

	QImage dispImg = img.scaled(width,height,Qt::KeepAspectRatio);

	imageLabel->setPixmap(QPixmap::fromImage(dispImg));
	imageLabel->repaint();
	imageLabel->update();
	
}

void FullScreenDisplayWindow::changeImage()
{
	index++;
	if(index>=logMan.events.size())
	{
		return;
	}

	qDebug()<<index;

	if(index>0)
	{
		time += logMan.events[index-1].duration;
	}

	double interval = logMan.events[index].duration;
	timer->setInterval(interval*1000);

	if(index > logMan.screenshot_events[seq] && seq < logMan.screenshot_events.size()-1)
	{
		seq++;
	}

	barui.timestamplabel->setText(QString::number(time));
	barui.durationlabel->setText(QString::number(interval));
	barui.indexspin->setValue(index);

	setImage();

	timer->start();

}