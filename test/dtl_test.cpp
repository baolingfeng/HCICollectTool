#include "stdafx.h"
#include <dtl\dtl.hpp>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;

void ReplaceAll(std::string& str,std::string from,std::string to)
{
	std::string::size_type start_pos = 0;
	while((start_pos = str.find(from, start_pos)) != std::string::npos) {
         str.replace(start_pos, from.length(), to);
         start_pos += to.length(); // ...
	}
}

int main()
{
	string s1;
	string s2;

	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *rs;

	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306", "root", "123456");
	/* Connect to the MySQL test database */
	con->setSchema("datacollect");

	string sql = "select * from mouse_event_action_blf_gui where id = 365";
	stmt = con->createStatement();
	rs = stmt->executeQuery(sql.c_str());
	if(rs->next())
	{
		s1 = string(rs->getString("action_value").c_str());
	}

	sql = "select * from mouse_event_action_blf_gui where id = 1089";
	stmt = con->createStatement();
	rs = stmt->executeQuery(sql.c_str());
	if(rs->next())
	{
		s2 = string(rs->getString("action_value").c_str());
	}

	delete stmt;
	delete rs;
	delete con;


	typedef char   elem;
    typedef string sequence;

	ReplaceAll(s1, "\\n", "\n");
	ReplaceAll(s1, "\\t", "\t");
	ReplaceAll(s2, "\\n", "\n");
	ReplaceAll(s2, "\\t", "\t");

    sequence A(s1);
    sequence B(s2);
    
    dtl::Diff< elem, sequence > d(A, B);
    //d.onOnlyEditDistance();
	d.onHuge();
    d.compose();
    
    // editDistance
    cout << "editDistance:" << d.getEditDistance() << endl;
    
    // Longest Common Subsequence
    vector< elem > lcs_v = d.getLcsVec();
    sequence       lcs_s(lcs_v.begin(), lcs_v.end());
    cout << "LCS:" << lcs_s << endl;
    
	
	dtl::Ses<elem> ses_v = d.getSes();
	vector<elem> t = ses_v.getSequence();
	//vector<char> seq = ses_v.getSequence();
	
    // Shortest Edit Script
    cout << "SES" << endl;
    d.printSES();

	return 0;
}