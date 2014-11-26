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

	string getScreenshotTimestamp(int seq);
	//string getScreenImagePath(int seq=0);
	//cv::Mat readScreenImageInCV(int seq=0);

	LogEvent getEvent(string timestamp);
	LogEvent getEvent(int index);
	AccElement getAccElement(string timestamp);
	AccElement getAccElement(int index);

	double calcDuration(int from, int to);

	void setLogDir(string folder);
	void clear();

public:
	LogReader reader;
	vector<LogEvent> events;
	
	//statistic
	double totaltime;
	vector<int> screenshot_events;
	vector<int> accui_events;
	//vector<AccElement> elements;
	
	hash_map<string, int> events_map;
	set<string> process_set;
	set<string> window_set;
	vector<EventProcess> processes;

	string logDir;
};

