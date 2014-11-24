#ifndef DATAREPLAYER_H
#define DATAREPLAYER_H

#include <QtWidgets/QWidget>
#include <iostream>
#include "ui_datareplayer.h"
#include "playthread.h"
#include "LogManager.h"
#include "util.h"
#include "eventsbar.h"
#include "LogReader.h"
#include "fullscreendisplaywindow.h"
#include "distributedwidget.h"

using namespace std;

class DataReplayer : public QWidget
{
	Q_OBJECT

public:
	DataReplayer(QWidget *parent = 0);
	~DataReplayer();

	void displayImage(cv::Mat cimg);
	void displayImage(LogEvent e, cv::Mat screenshot);

public slots:
	void displayImage(int seq);
	void loadLog();
	void play();
	void nextEvent();

	void accClicked(const QModelIndex& index);
signals:
	void sendDuration(double);

public:
	//static cv::Mat MOUSEICON;
	//static cv::Mat MOUSEMASK;

private:
	Ui::DataReplayerClass ui;

	int index;
	int seq;

	LogReader logReader;
	LogManager logMan;

	DistributedWidget* dw;
	EventsBar eventsBar;

	PlayThread pthread;
	FullScreenDisplayWindow* displayWindow;
	//FullScreenDisplayWindow displayWindow;
};

#endif // DATAREPLAYER_H
