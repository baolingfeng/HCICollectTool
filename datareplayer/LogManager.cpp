#include "LogManager.h"
#include "util.h"

LogManager::LogManager(string name)
{
	this->name = name;
	db = new DBInterface;

	events = db->getAllEvents(name);
	copy_events = db->getAllCopyEvent(name);

	calcStatistic();
}


LogManager::~LogManager(void)
{
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
	//groupAccAction();
}

void LogManager::calcStatistic()
{
	int len = events.size();
	totaltime = GetTimeDifference(toSystemTime(events[0].timestamp),toSystemTime(events[len-1].timestamp));
	
	EventProcess p(events[0].processName); p.from = 0;
	vector<LogEvent> keys;
	bool hasPushCtrl = false;
	for(int i=0; i<len-1; i++)
	{
		//LogEvent e = events[i];
		
		events[i].duration = GetTimeDifference(toSystemTime(events[i].timestamp),toSystemTime(events[i+1].timestamp));
		if(events[i].isHasAcc) 
		{
			accui_events.push_back(i);

			string acc_name = events[i].acc.name;
			std::transform(acc_name.begin(), acc_name.end(), acc_name.begin(), ::tolower);
			int acc_index = acc_name.find("paste");
			if(acc_index>=0 && events[i].acc.type == "menu item")
			{
				PasteEvent pe;
				pe.timestamp = events[i].timestamp;
				pe.windowName = events[i].windowName;
				pe.processName = events[i].processName;
				pe.parentWindowName = events[i].parentWindowName;
				pe.method = "Menu -> Paste";
				paste_events.push_back(pe);
			}
		}

		if(events[i].isHasSrceenshot)
		{
			screenshot_events.push_back(i);
		}

		process_set.insert(events[i].processName);
		window_set.insert(events[i].windowName);
		//events_map[events[i].timestamp] = i;
		
		if(events[i].eventType == "keyinput")
		{
			if(hasPushCtrl)
			{
				keys.push_back(events[i]);
				if(events[i].name != "Ctrl" && events[i].name != "Shift" && events[i].name != "Alt")
				{
					hasPushCtrl = false;
					keyEvents.push_back(keys);

					string keystr = this->keysToString(keys);
					if(keystr == "Ctrl+V")
					{
						PasteEvent pe;
						pe.timestamp = keys[0].timestamp;
						pe.windowName = keys[0].windowName;
						pe.processName = keys[0].processName;
						pe.parentWindowName = keys[0].parentWindowName;
						pe.method = keystr;
						paste_events.push_back(pe);
					}
					keys = vector<LogEvent>();
				}
			}
			else
			{
				if(events[i].name == "Ctrl")
				{
					hasPushCtrl = true;
					keys.push_back(events[i]);
				}
				else if(events[i].name.size()>1 && events[i].name.substr(0,1) == "F")
				{
					keys.push_back(events[i]);
					keyEvents.push_back(keys);
					keys = vector<LogEvent>();
				}
				else
				{
					if(keys.size()>0)
					{
						keyEvents.push_back(keys);
						keys = vector<LogEvent>();
					}
				}
			}
			
		}

		if(i>0)
		{
			if(events[i].processName != p.name)
			{
				p.to = i - 1;

				if(process_stat.find(p.name) != process_stat.end())
				{
					process_stat[p.name] += calcDuration(p.from, p.to);
				}
				else
				{
					process_stat[p.name] = calcDuration(p.from, p.to);
				}

				processes.push_back(p);
				p = EventProcess(events[i].processName);
				p.from = i;
			}
			
		}
	}

	p.to = len - 1;
	processes.push_back(p);
}

void LogManager::groupAccAction()
{
	vector<vector<int>> groups;
	vector<int> hasgroup;
	for(int i=0; i<accui_events.size(); i++)
	{
		if(std::find(hasgroup.begin(), hasgroup.end(), i) != hasgroup.end()) continue;

		int ei = accui_events[i];

		vector<int> g;
		g.push_back(ei);
		hasgroup.push_back(i);
		for(int j=i+1; j<accui_events.size(); j++)
		{
			int ej = accui_events[j];

			AccElement a1 = events[ei].acc;
			AccElement a2 = events[ej].acc;
			if(a1.name == a2.name && a1.type == a2.type && a1.value == a2.value)
			{
				g.push_back(ej);
				hasgroup.push_back(j);
			}
		}
		groups.push_back(g);
	}
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

void LogManager::genereateMarkovForProcess()
{
	
}

string LogManager::keysToString(vector<LogEvent> kevents)
{
	vector<string> keys;
	string pre_name = "";
	for(int j=0; j<kevents.size(); j++)
	{
		if(kevents[j].name != pre_name)
		{
			keys.push_back(kevents[j].name);
		}

		pre_name =kevents[j].name;
	}
	return join(keys, "+");
}

cv::Mat LogManager::getScreenshot(string timestamp, string type)
{
	return db->getScreenshot(name, timestamp, type);
}

LogEvent LogManager::getMouseEvent(string timestamp)
{
	return db->getMouseEventByTimestamp(name, timestamp);
}