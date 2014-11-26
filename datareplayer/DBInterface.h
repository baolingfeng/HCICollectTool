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
	bool insertMouseEvent(string name, LogEvent e);
	void close();

private:
	sql::Driver *driver;
	sql::Connection *conn;
};

