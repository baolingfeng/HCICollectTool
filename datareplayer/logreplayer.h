#ifndef LOGREPLAYER_H
#define LOGREPLAYER_H

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QLayout>
#include <QPushButton>
#include <QFrame>
#include <QEvent>
#include <QWindowStateChangeEvent>
#include <QTimer>

#include "DBInterface.h"
#include "util.h"

class LogReplayer : public QWidget
{
	Q_OBJECT

public:
	LogReplayer(QWidget *parent, string name, DBInterface *db);
	~LogReplayer();

	void displayImage();
	double getInterval();

	void changeEvent(QEvent* event);
public slots:
	void playEvents();
private:
	string logName;
	DBInterface *db;

	vector<LogEvent> events;
	cv::Mat curImage;
	int curIndex;
	int curScreenIndex;
	double totalTime;

	QTimer *timer;


	QVBoxLayout *layout;
	QLabel *imgLabel;

	QFrame *sliderFrame;
	QHBoxLayout *sliderLayout;
	QLabel *startLabel;
	QLabel *endLabel;
	QSlider *slider;

	QFrame *bottomFrame;
	QHBoxLayout *bottomLayout;
	QLabel *curIndexLabel;
	QLabel *curIndexValue;
	QLabel *curTimestampLabel;
	QLabel *curTimestampValue;
	QPushButton *playBtn;
};

#endif // LOGREPLAYER_H
