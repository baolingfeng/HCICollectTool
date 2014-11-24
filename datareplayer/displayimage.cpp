#include "displayimage.h"
#include <QDebug>
#include <qlayout.h>

DisplayImage::DisplayImage(QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout* layout = new QVBoxLayout;
	

	imgLabel = new QLabel;
	layout->addWidget(imgLabel);

	slider = new QSlider;
	slider->setOrientation(Qt::Orientation::Horizontal);
	layout->addWidget(slider);

	this->setLayout(layout);
}

DisplayImage::~DisplayImage()
{

}

void DisplayImage::setImage(QImage img)
{
	imgLabel->setPixmap(QPixmap::fromImage(img));
	imgLabel->repaint();
	imgLabel->update();
}

void DisplayImage::nextEvent(int index)
{
	qDebug()<<index;
	if(index >=  logMan.events.size() - 1) 
	{
		thread()->terminate();
		return;
	}

	LogEvent curEvent = logMan.events[index];

	if(index > logMan.screenshot_events[seq])
	{
		seq++;
	}
	
	QImage qimg = logReader.readScreenshotByQT(logMan.getScreenshotTimestamp(seq));
	setImage(qimg);
}

void DisplayImage::init(LogManager& logMan, LogReader& logReader)
{
	this->logMan = logMan;
	this->logReader = logReader;
}

void DisplayImage::display()
{
	while(current_index<logMan.events.size())
	{
		double interval = logMan.events[current_index].duration;
		qDebug()<<interval<<" in thread"<<endl;
		
		
		QThread::sleep(interval);

		current_index++;
		emit nextEvent(current_index);

		
	}
}