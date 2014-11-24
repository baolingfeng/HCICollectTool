#pragma once
#include "LogEvent.h"
#include<vector>
#include<fstream>
#include "util.h"
#include <qimage.h>

class LogReader
{
public:
	LogReader() {isKeepUnknown = true;};
	LogReader(string folder, bool isKeepUnknown=true);
	~LogReader(void);

	//LogEvent getOneMouseEvent(ifstream& mouseFile);
	//LogEvent getOneKeyEvent(ifstream& keyFile);
	
	bool getOneMouseEvent(ifstream& mouseFile, LogEvent& logEvent);
	bool getOneKeyEvent(ifstream& keyFile, LogEvent& logEvent);

	bool getAccElement(string timestamp, AccElement& e);
	bool isScreenshotExists(string timestamp);
	cv::Mat readScreenshotByCV(string timestamp);
	QImage readScreenshotByQT(string timestamp);

	vector<LogEvent> read(bool isKeepUnknown=true);

	void setLogDir(string folder) {logDir = folder;}
	string getLogDir() {return logDir;}
private:
	string logDir;
	bool isKeepUnknown;
};

