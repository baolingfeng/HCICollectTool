#include <Windows.h>
#include "DBInterface.h"
#include <QDebug>
#include <fstream>
#include "util.h"
#include <QString>
#include <QDebug>

void printException(sql::SQLException &e)
{
	qDebug() << "# ERR: SQLException in " << __FILE__
			<< "(" << __FUNCTION__ << ") on line " 
			<< __LINE__ << endl;
	qDebug() << "# ERR: " << e.what();
	qDebug() << " (MySQL error code: " << e.getErrorCode();
	qDebug() << ", SQLState: " << QString::fromStdString(e.getSQLState()) << " )" << endl;
}

DBInterface::DBInterface(void)
{
	try
	{
		sql::SQLString DB_HOST("tcp://127.0.0.1:3306");
		sql::SQLString DB_USER("root");
		sql::SQLString DB_PWD("123456");
		
		driver = get_driver_instance();
		//conn = driver->connect("tcp://127.0.0.1:3306", "root", "123456");
		conn = driver->connect(DB_HOST, DB_USER, DB_PWD);
		conn->setSchema("datacollect");
		
	}
	catch(sql::SQLException &e)
	{
		printException(e);
	}
	//conn->setSchema(DB_SCHEMA);
}


DBInterface::~DBInterface(void)
{
	if(conn != NULL)
	{
		delete conn;
	}
}

void DBInterface::close()
{
	conn->close();
	delete conn;
}

bool DBInterface::initTable(string name)
{
	try {
		sql::PreparedStatement *prep_stmt;

		prep_stmt = conn->prepareStatement("call add_log(?)");
		prep_stmt->setString(1, sql::SQLString(name.c_str()));
		prep_stmt->execute();

		/*
		//this part is executed in database by stored procedure
		prep_stmt = conn->prepareStatement("INSERT INTO logs(log_name, create_time) VALUES (?, now())");
		prep_stmt->setString(1, sql::SQLString(name.c_str()));
		prep_stmt->execute();
		
		delete prep_stmt;

		sql::Statement *stmt;
		stmt = conn->createStatement();
		//table: mouse_event_$name
		stmt->execute(sql::SQLString(("drop table if exists mouse_event_" + name).c_str()));
		string sql = "CREATE TABLE `mouse_event_" + name + "` ( \
			`id` INT NOT NULL AUTO_INCREMENT, \
			`timestamp` VARCHAR(30) NOT NULL, \
			`event_name` VARCHAR(45) NOT NULL, \
			`pos_x` INT NULL, \
			`pos_y` INT NULL, \
			`window_name` VARCHAR(300) NULL, \
			`process_name` VARCHAR(100) NULL, \
			`parent_window` VARCHAR(300) NULL, \
			`win_rect_left` INT NULL, \
			`win_rect_top` INT NULL, \
			`win_rect_right` INT NULL, \
			`win_rect_bottom` INT NULL, \
			`has_screenshot` INT NULL, \
			`has_acc` INT NULL, \
			PRIMARY KEY (`id`));";

		stmt->execute(sql::SQLString(sql.c_str()));

		//table: mouse_event_screenshot_$name
		sql = "drop table if exists mouse_event_screenshot_" + name;
		stmt->execute(sql.c_str());
		sql = "CREATE TABLE `mouse_event_screenshot_" + name + "` ( \
			  `id` INT NOT NULL, \
			  `screenshot` MEDIUMBLOB NULL, \
			  PRIMARY KEY (`id`));";
		stmt->execute(sql.c_str());

		//table: mouse_event_action_$name
		sql = "drop table if exists mouse_event_action_" + name;
		stmt->execute(sql.c_str());
		sql = "CREATE TABLE `mouse_event_action_" + name + "` ( \
			  `id` INT NOT NULL, \
			  `action_name` MEDIUMTEXT NULL, \
			  `action_type` VARCHAR(45) NULL, \
			  `action_value` MEDIUMTEXT NULL, \
			  `bound_left` INT NULL, \
			  `bound_top` INT NULL, \
			  `bound_right` INT NULL, \
			  `bound_bottom` INT NULL, \
			  PRIMARY KEY (`id`));";
		stmt->execute(sql.c_str());

		//table: key_event_$name
		sql = "drop table if exists key_event_" + name;
		stmt->execute(sql.c_str());
		sql = "CREATE TABLE `key_event_" + name + "` ( \
			  `id` INT NOT NULL AUTO_INCREMENT, \
			  `timestamp` VARCHAR(30) NULL, \
			  `key_input` VARCHAR(10) NULL, \
			  `window_name` VARCHAR(300) NULL, \
			  `process_name` VARCHAR(100) NULL, \
			  `parent_window` VARCHAR(300) NULL, \
			  `pos_x` INT NULL, \
			  `pos_y` INT NULL, \
			  `has_screenshot` INT NULL, \
			  PRIMARY KEY (`id`));";
		stmt->execute(sql.c_str());

		//table: key_event_screenshot_$name
		sql = "drop table if exists key_event_screenshot_" + name;
		stmt->execute(sql.c_str());
		sql = "CREATE TABLE `key_event_screenshot_" + name + "` ( \
			  `id` INT NOT NULL, \
			  `screenshot` MEDIUMBLOB NULL, \
			  PRIMARY KEY (`id`));";
		stmt->execute(sql.c_str());

		//table: copy_event_$name
		sql = "drop table if exists copy_event_" + name;
		stmt->execute(sql.c_str());
		sql = "CREATE TABLE `copy_event_" + name + "` ( \
			  `id` INT NOT NULL AUTO_INCREMENT, \
			  `timestamp` VARCHAR(30) NULL, \
			  `window_name` VARCHAR(300) NULL, \
			  `process_name` VARCHAR(100) NULL, \
			  `parent_window` VARCHAR(300) NULL, \
			  `text` MEDIUMTEXT NULL, \
			  PRIMARY KEY (`id`));";
		stmt->execute(sql.c_str());

		delete stmt;
		*/

		return true;
	}
	catch(sql::SQLException &e) 
	{
		printException(e);
		return false;
	}
}

bool DBInterface::insertMouseEvent(string name, LogEvent e, string logDir)
{
	try
	{
		sql::PreparedStatement *prep_stmt;
		sql::Statement *stmt;
		sql::ResultSet *rs;

		string sql = "INSERT INTO mouse_event_" + name + "(timestamp, event_name, pos_x, pos_y, window_name, process_name, parent_window, \
			win_rect_left, win_rect_top, win_rect_right, win_rect_bottom, has_screenshot, has_acc) VALUES (?, ?,?,?, ?, ?,?,?, ?, ?,?,?, ?)";
		prep_stmt = conn->prepareStatement(sql.c_str());
		prep_stmt->setString(1, e.timestamp.c_str());
		prep_stmt->setString(2, e.name.c_str());
		prep_stmt->setInt(3, e.pt.x);
		prep_stmt->setInt(4, e.pt.y);
		//string encodeStr = QString::fromLocal8Bit(e.windowName.c_str()).toStdString();
		//qDebug()<<QString::fromLocal8Bit(e.windowName.c_str());

		prep_stmt->setString(5, e.windowName.c_str());
		prep_stmt->setString(6, e.processName.c_str());
		prep_stmt->setString(7, e.parentWindowName.c_str());
		prep_stmt->setInt(8, e.winRect.left);
		prep_stmt->setInt(9, e.winRect.top);
		prep_stmt->setInt(10, e.winRect.right);
		prep_stmt->setInt(11, e.winRect.bottom);
		prep_stmt->setInt(12, e.isHasSrceenshot);
		prep_stmt->setInt(13, e.isHasAcc);

		prep_stmt->execute();
		delete prep_stmt;

		sql = "select last_insert_id() as auto_id";
		stmt = conn->createStatement();
		rs = stmt->executeQuery(sql.c_str());
		int autoId = -1;
		if(rs->next())
		{
			autoId = rs->getInt(1);
		}

		if(e.isHasSrceenshot)
		{
			sql = "INSERT INTO mouse_event_screenshot_" + name + " (id, screenshot) VALUES (?, ?)";
			prep_stmt = conn->prepareStatement(sql.c_str());
			
			ifstream imgstream(logDir + "screen/" + e.timestamp + ".png", std::ios_base::binary);
			prep_stmt->setInt(1, autoId);
			prep_stmt->setBlob(2, &imgstream);
			prep_stmt->execute();
			delete prep_stmt;
		}


		if(e.isHasAcc)
		{
			sql = "INSERT INTO mouse_event_action_" + name + " (id, action_name, action_type, action_value, action_parent, action_parent_type, bound_left, bound_top, bound_right, bound_bottom) \
				VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
			prep_stmt = conn->prepareStatement(sql.c_str());
			prep_stmt->setInt(1, autoId);
			prep_stmt->setString(2, e.acc.name.c_str());
			prep_stmt->setString(3, e.acc.type.c_str());
			prep_stmt->setString(4, e.acc.value.c_str());
			prep_stmt->setString(5, e.acc.parent_name.c_str());
			prep_stmt->setString(6, e.acc.parent_type.c_str());
			prep_stmt->setInt(7, e.acc.bounding.left);
			prep_stmt->setInt(8, e.acc.bounding.top);
			prep_stmt->setInt(9, e.acc.bounding.right);
			prep_stmt->setInt(10, e.acc.bounding.bottom);
			prep_stmt->execute();
			delete prep_stmt;
		}

		delete rs;
		delete stmt;
	}
	catch(sql::SQLException &exception)
	{
		printException(exception);
		return false;
	}

	return true;
}

bool DBInterface::insertKeyEvent(string name, LogEvent e, string logDir)
{
	try
	{
		sql::PreparedStatement *prep_stmt;
		sql::Statement *stmt;
		sql::ResultSet *rs;

		string sql = "INSERT INTO key_event_" + name + " (timestamp, key_input, window_name, process_name, parent_window, pos_x, pos_y, has_screenshot) \
													   VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
		prep_stmt = conn->prepareStatement(sql.c_str());
		prep_stmt->setString(1, e.timestamp.c_str());
		prep_stmt->setString(2, e.name.c_str());
		prep_stmt->setString(3, e.windowName.c_str());
		prep_stmt->setString(4, e.processName.c_str());
		prep_stmt->setString(5, e.parentWindowName.c_str());
		prep_stmt->setInt(6, e.pt.x);
		prep_stmt->setInt(7, e.pt.y);
		prep_stmt->setInt(8, e.isHasSrceenshot);
		prep_stmt->execute();

		delete prep_stmt;

		sql = "select last_insert_id() as auto_id";
		stmt = conn->createStatement();
		rs = stmt->executeQuery(sql.c_str());
		int autoId = -1;
		if(rs->next())
		{
			autoId = rs->getInt(1);
		}

		if(e.isHasSrceenshot)
		{
			sql = "INSERT INTO key_event_screenshot_" + name + " (id, screenshot) VALUES (?, ?)";
			prep_stmt = conn->prepareStatement(sql.c_str());
			
			ifstream imgstream(logDir + "screen/" + e.timestamp + ".png", std::ios_base::binary);
			prep_stmt->setInt(1, autoId);
			prep_stmt->setBlob(2, &imgstream);
			prep_stmt->execute();
			delete prep_stmt;
		}

		return true;
	}
	catch(sql::SQLException &exception)
	{
		printException(exception);
		return false;
	}
}

bool DBInterface::insertCopyEvent(string name, CopyEvent e, string logDir)
{
	try
	{
		sql::PreparedStatement *prep_stmt;
		
		string sql = "INSERT INTO copy_event_" + name + " (timestamp, window_name, process_name, parent_window, text) \
													   VALUES (?, ?, ?, ?, ?)";
		prep_stmt = conn->prepareStatement(sql.c_str());
		prep_stmt->setString(1, e.timestamp.c_str());
		prep_stmt->setString(2, e.windowName.c_str());
		prep_stmt->setString(3, e.processName.c_str());
		prep_stmt->setString(4, e.parentWindowName.c_str());
		prep_stmt->setString(5, e.text.c_str());

		prep_stmt->execute();

		delete prep_stmt;
	}
	catch(sql::SQLException &exception)
	{
		printException(exception);
		return false;
	}
	return true;
}

bool DBInterface::isExistsLog(string name)
{
	try
	{
		sql::PreparedStatement *prep_stmt;
		sql::ResultSet *rs;

		string sql = "select * from logs where log_name = ?";
		prep_stmt = conn->prepareStatement(sql.c_str());
		prep_stmt->setString(1, name.c_str());

		rs = prep_stmt->executeQuery();
		return rs->next();
	}
	catch(sql::SQLException &exception)
	{
		printException(exception);
		//return false;
	}

	return true;
}

bool DBInterface::updateLogTime(string name, string from, string to)
{
	try
	{
		sql::PreparedStatement *prep_stmt;
		//sql::ResultSet *rs;

		//string sql = "select id from logs where log_name = ?";
		//prep_stmt = conn->prepareStatement(sql.c_str());
		//prep_stmt->setString(1, name.c_str());
		//rs = prep_stmt->executeQuery(); 

		//if(rs->next())
		//{
		//int id = rs->getInt(1);
		string sql = "update logs set from_time=?, to_time=? where log_name = ?";
		prep_stmt = conn->prepareStatement(sql.c_str());
		prep_stmt->setString(1, from.c_str());
		prep_stmt->setString(2, to.c_str());
		prep_stmt->setString(3, name.c_str());
		prep_stmt->execute();	
		//}
		
		//delete rs;
		delete prep_stmt;
	}
	catch(sql::SQLException &exception)
	{
		printException(exception);
		return false;
	}
	return true;
}

vector<LogRecord> DBInterface::getAllLogRecords()
{
	vector<LogRecord> records;
	try
	{
		sql::Statement *stmt;
		sql::ResultSet *rs;

		string sql = "select id, log_name, create_time from logs";
		stmt = conn->createStatement();
		rs = stmt->executeQuery(sql.c_str());
		while(rs->next())
		{
			LogRecord rd;
			rd.id = rs->getInt("id");
			rd.name = string(rs->getString(2).c_str());
			rd.date = string(rs->getString(3).c_str());
			records.push_back(rd);
		}

		delete rs;
		delete stmt;
	}
	catch(sql::SQLException &exception)
	{
		printException(exception);
	}

	return records;
}

AccElement DBInterface::getAccElement(string name, int id)
{
	AccElement acc;
	try
	{
		sql::PreparedStatement *prep_stmt;
		sql::ResultSet *rs;

		string sql = "select * from mouse_event_action_" + name + " where id = ?";
		prep_stmt = conn->prepareStatement(sql.c_str());
		prep_stmt->setInt(1, id);

		rs = prep_stmt->executeQuery();
		if(rs->next())
		{
			acc.name = string(rs->getString("action_name").c_str());
			acc.type = string(rs->getString("action_type").c_str());
			acc.value = string(rs->getString("action_value").c_str());
			acc.parent_name = string(rs->getString("action_parent").c_str());
			acc.parent_type = string(rs->getString("action_parent_type").c_str());
			acc.bounding.left = rs->getInt("bound_left");
			acc.bounding.top = rs->getInt("bound_top");
			acc.bounding.right = rs->getInt("bound_right");
			acc.bounding.bottom = rs->getInt("bound_bottom");
		}

		delete rs;
		delete prep_stmt;
	}
	catch(sql::SQLException &exception)
	{
		printException(exception);
	}

	return acc;
}

vector<LogEvent> DBInterface::getAllEvents(string name)
{
	vector<LogEvent> events;
	try
	{
		sql::Statement *stmt;
		sql::ResultSet *rs;

		string sql = "select * from v_log_events_" + name;	
		stmt = conn->createStatement();
		rs = stmt->executeQuery(sql.c_str());

		while(rs->next())
		{
			LogEvent e;
			int id = rs->getInt("id");
			e.timestamp = string(rs->getString("timestamp").c_str());
			e.name = string(rs->getString("event_name").c_str());
			e.pt.x = rs->getInt("pos_x");
			e.pt.y = rs->getInt("pos_y");
			e.windowName = string(rs->getString("window_name").c_str());
			e.processName = string(rs->getString("process_name").c_str());
			e.parentWindowName = string(rs->getString("parent_window").c_str());
			e.winRect.left = rs->getInt("win_rect_left");
			e.winRect.top = rs->getInt("win_rect_top");
			e.winRect.right = rs->getInt("win_rect_right");
			e.winRect.bottom = rs->getInt("win_rect_bottom");
			e.isHasSrceenshot = rs->getInt("has_screenshot");
			e.isHasAcc = rs->getInt("has_acc");
			e.eventType = string(rs->getString("event_type").c_str());
			
			if(e.isHasAcc)
			{
				e.acc = getAccElement(name, id);
			}
			events.push_back(e);
		}

		delete rs;
		delete stmt;
	}
	catch(sql::SQLException &exception)
	{
		printException(exception);
	}

	return events;
}

cv::Mat DBInterface::getScreenshot(string name, string timestamp, string eventType)
{
	cv::Mat img;
	try
	{
		string table;
		if(eventType == "mouse")
		{
			table = "mouse";
		}
		else
		{
			table = "key";
		}

		string sql = "select screenshot from " + table + "_event_" + name + " a, " + table + "_event_screenshot_" + name + " b where a.id = b.id and a.timestamp = '" + timestamp + "'";
		sql::Statement *stmt;
		stmt = conn->createStatement();

		sql::ResultSet *rs = stmt->executeQuery(sql.c_str());
		if(rs->next())
		{
			istream* is = rs->getBlob(1);
			is->seekg(0, is->end);
			int len = is->tellg();
			char* temp = new char[len];
			is->seekg(0, is->beg);
			is->read(temp, len);

			vector<char> buffer(temp, temp+len);
			cv::Mat m(buffer);

			img = cv::imdecode(m, 1);
		}

		delete rs;
		delete stmt;
	}
	catch(sql::SQLException &exception)
	{
		printException(exception);
	}

	return img;
}

bool DBInterface::delLog(string name)
{
	try
	{
		sql::PreparedStatement *prep_stmt;

		prep_stmt = conn->prepareStatement("call del_log(?)");
		prep_stmt->setString(1, sql::SQLString(name.c_str()));
		prep_stmt->execute();

		delete prep_stmt;
	}
	catch(sql::SQLException &exception)
	{
		printException(exception);
		return false;
	}

	return true;
}

vector<CopyEvent> DBInterface::getAllCopyEvent(string name)
{
	vector<CopyEvent> cpevents;
	try
	{
		sql::PreparedStatement *prep_stmt;
		sql::Statement *stmt;
		sql::ResultSet *rs;
		string sql = "select * from copy_event_" + name;

		stmt = conn->createStatement();
		rs = stmt->executeQuery(sql.c_str());

		while(rs->next())
		{
			CopyEvent e;
			e.timestamp = string(rs->getString("timestamp").c_str());
			if(e.timestamp == "") continue;

			e.text = string(rs->getString("text").c_str());
			e.windowName = string(rs->getString("window_name").c_str());
			e.processName = string(rs->getString("process_name").c_str());
			e.parentWindowName = string(rs->getString("parent_window").c_str());
			cpevents.push_back(e);
		}

		delete stmt;
		delete rs;
	}
	catch(sql::SQLException &exception)
	{
		printException(exception);
	}
	return cpevents;
}

LogEvent DBInterface::getMouseEventByTimestamp(string name, string timestamp)
{
	LogEvent e;
	try
	{
		sql::PreparedStatement *prep_stmt;
		sql::Statement *stmt;
		sql::ResultSet *rs;

		string sql = "select * from mouse_event_" + name + " where timestamp = ?";
		prep_stmt = conn->prepareStatement(sql.c_str());
		prep_stmt->setString(1, timestamp.c_str());
		rs = prep_stmt->executeQuery();
		if(rs->next())
		{
			int id = rs->getInt("id");
			e.timestamp = string(rs->getString("timestamp").c_str());
			e.name = string(rs->getString("event_name").c_str());
			e.pt.x = rs->getInt("pos_x");
			e.pt.y = rs->getInt("pos_y");
			e.windowName = string(rs->getString("window_name").c_str());
			e.processName = string(rs->getString("process_name").c_str());
			e.parentWindowName = string(rs->getString("parent_window").c_str());
			e.winRect.left = rs->getInt("win_rect_left");
			e.winRect.top = rs->getInt("win_rect_top");
			e.winRect.right = rs->getInt("win_rect_right");
			e.winRect.bottom = rs->getInt("win_rect_bottom");
			e.isHasSrceenshot = rs->getInt("has_screenshot");
			e.isHasAcc = rs->getInt("has_acc");
			e.eventType = "mouse";
			
			if(e.isHasAcc)
			{
				e.acc = getAccElement(name, id);
			}
		}

		delete prep_stmt;
		delete rs;
	}
	catch(sql::SQLException &exception)
	{
		printException(exception);
	}
	return e;
}