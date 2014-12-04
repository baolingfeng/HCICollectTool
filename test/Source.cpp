#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp" 


using namespace std;

int main_source()
{
	string str1 = "中文";
	string str2 = "中文";
	bool f = (str1 == str2);

	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;

	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306", "root", "123456");
	/* Connect to the MySQL test database */
	con->setSchema("datacollect");

	/*
	//ifstream with binary model
	std::ifstream ifs("D:/baolingfeng/GitHub/HCICollectDll/hcicollectapp/log/screen/2014-11-25-09-17-32-539.png", std::ios_base::binary);
	
	sql::PreparedStatement *prep_stmt;
	prep_stmt = con->prepareStatement("INSERT INTO testimage(timestamp, image) VALUES (?, ?)");
	prep_stmt->setString(1, "test2");
	prep_stmt->setBlob(2, &ifs);
	
	prep_stmt->execute();
	*/
		
	sql::PreparedStatement *prep_stmt;
	prep_stmt = con->prepareStatement("select id, image from testimage where id=3");
	res = prep_stmt->executeQuery();
	if(res->next())
	{
		istream* is = res->getBlob(2);
		is->seekg(0, is->end);
		int len = is->tellg();
		char* temp = new char[len];
		is->seekg(0, is->beg);
		is->read(temp, len);

		vector<char> buffer(temp, temp+len);
		cv::Mat m(buffer);

		cv::Mat img = cv::imdecode(m, 1);

		cv::imshow("test", img);
		cv::waitKey();

		//ofstream os("test.png", std::ofstream::binary);
		//os.write(temp, len);
		//os.close();
		delete temp;
	}
	
	//stmt = con->createStatement();
	//stmt->execute("USE " "test");
	//stmt->execute("DROP TABLE IF EXISTS test");
	//stmt->execute("CREATE TABLE test(id INT, label CHAR(1))");
	//stmt->execute("INSERT INTO test(id, label) VALUES (1, 'a')");

	
	

	//delete stmt;
	delete con;

	return 0;
}