#include "eventsbar.h"
#include <qpainter.h>
#include "datareplayer.h"
#include <cstdlib>

EventsBar::EventsBar(QWidget *parent)
	: QWidget(parent)
{
	isDisplayProcess = false;
}

EventsBar::~EventsBar()
{

}

void EventsBar::paintEvent(QPaintEvent *event)
{
	 QPainter painter(this);
	 QBrush brush;

	 if(isDisplayProcess)
	 {
		 hash_map<string, QColor> pcolors;
		 set<string> process_set = logMan.process_set;
	 
		 for(set<string>::iterator it= process_set.begin(); it != process_set.end(); it++)
		 {
			 int r = rand() % 255 + 1;
			 int g = rand() % 255 + 1;
			 int b = rand() % 255 + 1;
			 
			 pcolors[*it] = QColor::fromRgb(r,g,b);
			 
		 }
	
		 vector<EventProcess> processes =logMan.processes;
		 int x = 0;
		 for(int i=0; i<processes.size(); i++)
		 {
			 double time = logMan.calcDuration(processes[i].from, processes[i].to);
			 int y = rect().y();
			 int w = time / logMan.totaltime * rect().width();
			 int h = rect().height();

			 painter.setBrush(pcolors[processes[i].name]);
			 painter.setPen(pcolors[processes[i].name]);

			 painter.drawRect(QRect(x,y,w,h));
			 x = x + w;
		 }
	 }
	
}