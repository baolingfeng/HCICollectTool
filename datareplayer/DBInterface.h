#pragma once
#include <stdlib.h>
#include <iostream>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <vector>

#include "LogEvent.h"
#include "dao.h"
#include "util.h"

using namespace std;

class DBInterface
{
public:
	//static string DB_HOST;
	//static string DB_USER;
	//static string DB_PWD;
	//static string DB_SCHEMA;

	DBInterface(void);
	~DBInterface(void);

	bool initTable(string name);
	bool insertMouseEvent(string name, LogEvent e, string logDir);
	bool insertKeyEvent(string name, LogEvent e, string logDir);
	bool insertCopyEvent(string name, CopyEvent e, string logDir);
	bool isExistsLog(string name);

	bool delLog(string name);
	bool updateLogTime(string name, string from, string to);
	vector<LogRecord> getAllLogRecords();
	vector<LogEvent> getAllEvents(string name);
	LogEvent getMouseEventByTimestamp(string name, string timestamp);
	vector<CopyEvent> getAllCopyEvent(string name);
	AccElement getAccElement(string name, int id);
	cv::Mat getScreenshot(string name, string timestamp, string eventType);

	void close();

private:
	sql::Driver *driver;
	sql::Connection *conn;
};

