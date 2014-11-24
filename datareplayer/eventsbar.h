#ifndef EVENTSBAR_H
#define EVENTSBAR_H

#include <QWidget>
#include "LogManager.h"
//#include "datareplayer.h"

class EventsBar : public QWidget
{
	Q_OBJECT

public:
	EventsBar(QWidget *parent);
	~EventsBar();

	void paintEvent(QPaintEvent *event); 

	void setLogMan(LogManager& logMan) {this->logMan = logMan;}
	void DisplayProcessBar(bool isDisplay=true) {this->isDisplayProcess = isDisplay;}
private:
	LogManager logMan;
	bool isDisplayProcess;
};

#endif // EVENTSBAR_H
