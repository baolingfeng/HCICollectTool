#pragma once
#include<iostream>
#include<vector>
#include<fstream>
#include<Windows.h>
#include<hash_map>
#include<set>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"

#include "LogEvent.h"
#include "LogReader.h"
#include "DBInterface.h"
using namespace std;


class LogManager
{
public:
	LogManager() {};
	LogManager(string folder);
	~LogManager(void);
	
public:
	void setEvents(vector<LogEvent>& logEvent);
	void calcStatistic();
	void groupAccAction();

	void genereateMarkovForProcess();

	double calcDuration(int from, int to);

	void clear();

	cv::Mat getScreenshot(string timestamp, string type);
	LogEvent getMouseEvent(string timestamp);
	string keysToString(vector<LogEvent>);
public:
	vector<vector<LogEvent>> keyEvents;
	vector<LogEvent> events;
	vector<CopyEvent> copy_events;
	vector<PasteEvent> paste_events;

	double totaltime;
	vector<int> screenshot_events;
	vector<int> accui_events;
	
	vector<EventProcess> processes;
	set<string> process_set;
	hash_map<string, double> process_stat;

	set<string> window_set;

	string name;
	DBInterface* db;
};

