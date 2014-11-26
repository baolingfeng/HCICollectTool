#include "distributedwidget.h"
#include <QPainter>

DistributedWidget::DistributedWidget(QWidget *parent)
	: QWidget(parent)
{
	isInit = false;
}

DistributedWidget::~DistributedWidget()
{

}

QColor DistributedWidget::USE_COLORS[] = {QColor::fromRgb(255,0,0), QColor::fromRgb(0,255,0), QColor::fromRgb(0,0,255), QColor::fromRgb(255,0,255), QColor::fromRgb(255,255,0), QColor::fromRgb(0,255,255), QColor::fromRgb(255,255,255)};

void DistributedWidget::paintEvent(QPaintEvent* event)
{
	if(!isInit) return ;

	QPainter painter(this);
	QBrush brush;

	int x = 0;
	for(int i=0; i<items.size(); i++)
	{
		//double time = logMan.calcDuration(processes[i].from, processes[i].to);
		int y = rect().y();
		int w = items[i].getValue() * rect().width();
		int h = rect().height()/2 - 5;

		painter.setBrush(color_map[items[i].getName()]);
		painter.setPen(color_map[items[i].getName()]);
		painter.drawRect(QRect(x,y,w,h));
		x = x + w;
	}

	painter.setPen("black");

	int avgW = rect().width() / category.size();
	x = 0;
	int y = rect().y() + rect().height()/2;
	for(set<string>::iterator it= category.begin(); it != category.end(); it++)
	{
		painter.setBrush(color_map[*it]);
		painter.drawRect(QRect(x,y, 16, 16));

		x = x + 20;
		painter.drawText(QRect(x,y,100,16), QString::fromStdString(*it));
		x = x + 105;
	}
}

void DistributedWidget::initForProcess(LogManager& logMan)
{
	this->category = logMan.process_set;
	
	for(int i=0; i<logMan.processes.size(); i++)
	{
		EventProcess process = logMan.processes[i];
		double interval = logMan.calcDuration(process.from, process.to);

		category.insert(process.name);
		DistributedItem item(process.name, interval/logMan.totaltime);

		this->items.push_back(item);
	}

	int i = 0;
	for(set<string>::iterator it= category.begin(); it != category.end(); it++)
	{
		color_map[*it] = USE_COLORS[i];
		i++;
	}

	isInit = true;
	this->update();
	this->repaint();
}