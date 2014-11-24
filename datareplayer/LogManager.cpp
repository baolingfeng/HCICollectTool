#include "LogManager.h"
#include "util.h"

LogManager::LogManager(string folder)
{
	 logDir = folder;
}


LogManager::~LogManager(void)
{
}

void LogManager::setLogDir(string folder)
{
	logDir = folder;
	//clear
	clear();
}

void LogManager::clear()
{
	events.clear();
	screenshot_events.clear();
	accui_events.clear();
	//elements.clear();
}

void LogManager::setEvents(vector<LogEvent>& events)
{
	this->events = events;
	calcStatistic();
}

void LogManager::calcStatistic()
{
	int len = events.size();
	totaltime = GetTimeDifference(toSystemTime(events[0].timestamp),toSystemTime(events[len-1].timestamp));
	
	EventProcess p(events[0].processName); p.from = 0;
	for(int i=0; i<len-1; i++)
	{
		//LogEvent e = events[i];
		events[i].duration = GetTimeDifference(toSystemTime(events[i].timestamp),toSystemTime(events[i+1].timestamp));
		if(events[i].isHasAcc) 
		{
			accui_events.push_back(i);
		}

		if(events[i].isHasSrceenshot)
		{
			screenshot_events.push_back(i);
		}

		process_set.insert(events[i].processName);
		window_set.insert(events[i].windowName);
		events_map[events[i].timestamp] = i;

		if(i>0)
		{
			if(events[i].processName != p.name)
			{
				p.to = i - 1;
				processes.push_back(p);
				p = EventProcess(events[i].processName);
				p.from = i;
			}
			
		}
	}

	p.to = len - 1;
	processes.push_back(p);
}

double LogManager::calcDuration(int from, int to)
{
	if(from >= events.size()-1) return 0;

	string fromtime = events[from].timestamp;
	int temp = to + 1;
	if(temp >= events.size())
	{
		temp = to;
	}
	string totime = events[temp].timestamp;

	return GetTimeDifference(toSystemTime(fromtime),toSystemTime(totime));
}

string  LogManager::getScreenshotTimestamp(int seq)
{
	int index = screenshot_events[seq];
	LogEvent e = events[index];
	return e.timestamp;
}

LogEvent LogManager::getEvent(string timestamp)
{
	
	if(events_map.find(timestamp)== events_map.end())
	{
		cout<<"Error: No event on timestamp: "<<timestamp<<endl;
	}
	return getEvent(events_map[timestamp]);
}

LogEvent LogManager::getEvent(int index)
{
	return events[index];
}

AccElement LogManager::getAccElement(int index)
{
	return getEvent(index).acc;
}

AccElement LogManager::getAccElement(string timestamp)
{
	return getEvent(timestamp).acc;
}