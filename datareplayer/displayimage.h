#ifndef DISPLAYIMAGE_H
#define DISPLAYIMAGE_H

#include <QLabel>
#include <QImage>
#include <QSlider>
#include <QThread>
#include "LogManager.h"
#include "LogReader.h"

class DisplayImage : public QWidget
{
	Q_OBJECT

public:
	DisplayImage(QWidget *parent=0);
	~DisplayImage();

	void init(LogManager& logMan, LogReader& logReader);

public slots:
	void setImage(QImage img);
	void nextEvent(int);
	void display();

private:
	int current_index;
	int seq;
	QImage img;
	LogManager logMan;
	LogReader logReader;

	QLabel* imgLabel;
	QSlider* slider;
};

#endif // DISPLAYIMAGE_H
