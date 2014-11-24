#include "playthread.h"
#include "qdebug.h"

PlayThread::PlayThread(QObject *parent)
	: QThread(parent)
{
	is_stop = false;
}

PlayThread::~PlayThread()
{

}

void PlayThread::stop()
{
	is_stop = true;
}

void PlayThread::run()
{
	while(!is_stop && current_index<events_duration.size())
	{
		double interval = events_duration[current_index];
		qDebug()<<interval<<" in thread"<<endl;
		
		sleep(interval);

		current_index++;
		emit nextEvent(current_index);

		
	}
}

void PlayThread::setInterval(double i)
{
	//interval = i;
}

void PlayThread::setTimes(LogManager& logMan)
{
	events_duration.clear();
	current_index = 0;
	for(int i=0; i<logMan.events.size() - 1; i++)
	{
		events_duration.push_back(logMan.events[i].duration);
	}
}