#pragma once
#include <stdlib.h>
#include <iostream>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "LogEvent.h"

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
	void close();

private:
	sql::Driver *driver;
	sql::Connection *conn;
};

